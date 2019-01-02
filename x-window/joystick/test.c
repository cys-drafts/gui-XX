#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/joystick.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#ifndef likely
#define likely(cond)	__builtin_expect((cond), 1)
#endif
#ifndef unlikely
#define unlikely(cond)	__builtin_expect((cond), 0)
#endif

#define dbg(fmt, ...)	printf(fmt, ##__VA_ARGS__)

#define VALUE_MIN	(-32767)
#define VALUE_MAX	(32767)

Display *display;
int screen;
Window win;
unsigned int width, height;

struct joystick {
	int fd;
	int index;
	long naxes;
	long nbutton;
	char name[128];
};

static void setup_x(void)
{
	Window ret_win;
	unsigned int border_width, depth;
	int x, y;

	display = XOpenDisplay("");
	if (!display) {
		printf("cannot open display\n");
		exit(1);
	}

	screen = DefaultScreen(display);
	win = RootWindow(display, screen);
	XGetGeometry(display, win, &ret_win, &x, &y, &width, &height, &border_width, &depth);
}

static int get_js_info(struct joystick *js, int index)
{
	char file[32];
	int rc;

	assert(js);
	assert(index >= 0);

	memset(js, 0, sizeof(*js));

	snprintf(file, 32, "/dev/input/js%d", index);

	js->index = index;
	js->fd = open(file, O_RDONLY);
	if (unlikely(js->fd < 0)) {
		printf("cannot open %*s\n", 32, file);
		return -1;
	}
	rc = ioctl(js->fd, JSIOCGNAME(sizeof(js->name)), js->name);
	if (unlikely(rc < 0)) {
		strncpy(js->name, "unknown", sizeof(js->name));
	}
	rc = ioctl(js->fd, JSIOCGAXES, &js->naxes);
	if (unlikely(rc < 0)) {
	}
	rc = ioctl(js->fd, JSIOCGBUTTONS, &js->nbutton);
	if (unlikely(rc < 0)) {
	}

	return 0;
}

static void show_js_info(struct joystick *js)
{
	assert(js);
	dbg("joystick(%s) has %ld buttons and %ld axes\n",
			js->name, js->nbutton, js->naxes);
}

static int event_kbd_ctrl_w(void)
{
	KeySym ks;
	KeyCode kc_ctl, kc_w;

	dbg("Ctrl-W\n");

	ks = XStringToKeysym("Control_L");
	kc_ctl = XKeysymToKeycode(display, ks);
	XTestFakeKeyEvent(display, kc_ctl, 1, 0);
	ks = XStringToKeysym("w");
	kc_w = XKeysymToKeycode(display, ks);
	XTestFakeKeyEvent(display, kc_w, 1, 0);
	XTestFakeKeyEvent(display, kc_w, 0, 0);
	XTestFakeKeyEvent(display, kc_ctl, 0, 0);

	XSync(display, 1);

	return 0;
}

static void mouse_goto_r(int x, int y)
{
	XTestFakeRelativeMotionEvent(display, x, y, 0);
	XSync(display, 1);
}

static int event_mouse_motion(int direction)
{
	switch(direction) {
		case 0: //left
			mouse_goto_r(-1, 0);
			break;
		case 1: //right
			mouse_goto_r(1, 0);
			break;
		case 2: //up
			mouse_goto_r(0, -1);
			break;
		case 3: //down
			mouse_goto_r(0, 1);
			break;
		default:
			printf("mouse: where to go? %d\n", direction);
			break;
	}
	return 0;
}

static int js_button_handler(struct joystick *js, int number, int value)
{
	assert(js);

	if (number == 2 && value == 0) {
		event_kbd_ctrl_w();
	}
	return 0;
}

static int js_axes_handler(struct joystick *js, int number, int value)
{
	assert(js);

	if (number == 0) {
		if (value < 0) {
			event_mouse_motion(0);
		} else if (value > 0) {
			event_mouse_motion(1);
		}
	} else if (number == 1) {
		if (value < 0) {
			event_mouse_motion(2);
		} else if (value > 0) {
			event_mouse_motion(3);
		}
	}
	return 0;
}

static int do_one_event(struct joystick *js, struct js_event *e)
{
	assert(js);
	assert(e);

	if (e->type & JS_EVENT_INIT) {
		dbg("init\n");
		if (e->type & JS_EVENT_BUTTON) {
			dbg("button-%d init %s\n", e->number, e->value?"P":"R");
		}
		if (e->type & JS_EVENT_AXIS) {
			dbg("axes-%d init %d\n", e->number, e->value);
		}
		return 0;
	}

	switch(e->type) {
		case JS_EVENT_BUTTON:
			js_button_handler(js, e->number, e->value);
			dbg("button-%d %s\n", e->number, e->value?"pressed":"released");
			break;
		case JS_EVENT_AXIS:
			js_axes_handler(js, e->number, e->value);
			dbg("axes-%d %d\n", e->number, e->value);
			break;
		case JS_EVENT_INIT:
			dbg("cannot reach\n");
			break;
		default:
			printf("unknown event: %d\n", e->type);
			break;
	}
	return 0;
}

static int event_loop(struct joystick *js)
{
	struct js_event e;
	int rc;

	while (1) {
		rc = read(js->fd, &e, sizeof(e));
		if (rc == sizeof(e)) {
			do_one_event(js, &e);
		} else {
			printf("read returned %d\n", rc);
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	struct joystick js;
	int rc;

	rc = get_js_info(&js, 0);
	if (unlikely(rc != 0)) {
		printf("cannot get js-0 info\n");
	}
	show_js_info(&js);

	setup_x();

	event_loop(&js);

	return 0;
}
