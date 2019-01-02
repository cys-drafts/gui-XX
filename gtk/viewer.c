#include <gtk/gtk.h>

struct ui {
	GtkWidget *win;
	GtkWidget *toolbar;
	GtkWidget *viewer;
	GtkTextTagTable *tagtab;
};

struct ui _ui, *ui = &_ui;


struct buffer {
	GtkTextBuffer *buf;
};

static void create_tags(void)
{
	GtkTextTagTable *tab;
	GtkTextTag *tag;
	GValue val = G_VALUE_INIT;
	
	tab = gtk_text_tag_table_new();
	
	tag = gtk_text_tag_new("keyword");
	g_value_init(&val, G_TYPE_STRING);
	g_value_set_static_string(&val, "red");
	g_object_set_property(G_OBJECT(tag), "foreground", &val);
	
	gtk_text_tag_table_add(tab, tag);
	ui->tagtab = tab;
}
#define is_good_char(c) (g_unichar_isalnum(c) || (c == '_'))
#define current_char(iter) (gtk_text_iter_get_char(iter))
static inline gboolean is_keyword(gchar *str)
{
	static char *keywords[] = {"static", "void", "return", "if", "else", "while", "do", "break", "goto", "inline", "struct"};
	int i;
	for (i = 0; i < sizeof(keywords); i++) {
		if (g_strcmp0(str, keywords[i]) == 0)
			return TRUE;
	}
	return FALSE;
}
static gint get_token(GtkTextIter *start, GtkTextIter *end)
{
	gunichar c;
	
	c = current_char(start);
	if (c == 0)
		return 0;
	
	*end = *start;
	if (is_good_char(c)) {
		do {
			gtk_text_iter_forward_char(end);
			c = current_char(end);
		} while (is_good_char(c));
		gchar *word = gtk_text_iter_get_text(start, end);
		if (is_keyword(word)) {
			gtk_text_buffer_apply_tag_by_name(gtk_text_iter_get_buffer(start), "keyword", start, end);
		}
	} else {
		gtk_text_iter_forward_char(end);
	}
	return 1;
}
static void highlight(GtkTextBuffer *buf)
{
	GtkTextIter start, end;
	
	gtk_text_buffer_get_start_iter(buf, &start);
	while (get_token(&start, &end)) {
		start = end;
	}
}
static void open_file(GFile *file)
{
	gchar *contents;
	gsize length;
	
	if (g_file_load_contents(file, NULL, &contents, &length, NULL, NULL))
	{
		GtkTextBuffer *buf;

		buf = gtk_text_buffer_new(ui->tagtab);
		gtk_text_buffer_set_text(buf, contents, length);
		gtk_text_view_set_buffer(GTK_TEXT_VIEW(ui->viewer), buf);
		highlight(buf);
		g_free(contents);
	}
	else
	{
		g_print("cannot load file\n");
	}
}
static void choose_file(GtkWidget *btn, gpointer data)
{
	GtkFileChooserNative *native;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	native = gtk_file_chooser_native_new ("Open File",
                                      GTK_WINDOW(ui->win),
                                      action,
                                      "_Open",
                                      "_Cancel");

	res = gtk_native_dialog_run (GTK_NATIVE_DIALOG (native));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		GFile *file;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
		file = gtk_file_chooser_get_file (chooser);
		open_file (file);
	}

	g_object_unref (native);
}

static GtkWidget *create_toolbar(void)
{
	GtkWidget *tbar;
	GtkToolItem *newbtn;
	
	tbar = gtk_toolbar_new();
	newbtn = gtk_tool_button_new(NULL, "New");
	g_signal_connect(newbtn, "clicked", G_CALLBACK(choose_file), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(tbar), GTK_TOOL_ITEM(newbtn), 1);

	ui->toolbar = tbar;

	return tbar;
}
static GtkWidget *create_text_viewer(void)
{
	GtkWidget *swin;
	GtkWidget *tview;
	GtkWidget *sbar;
	GtkTextBuffer *buf;
	
	swin = gtk_scrolled_window_new(NULL, NULL);

	gtk_widget_set_hexpand(swin, TRUE);
	gtk_widget_set_vexpand(swin, TRUE);
	tview = gtk_text_view_new();
	buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tview));
	gtk_text_buffer_set_text(buf, "Hello Text View!\n", -1);
	gtk_container_add(GTK_CONTAINER(swin), tview);

	ui->viewer = tview;

	return swin;
}
static void create_main_window(GtkApplication *app, gpointer data)
{
	GtkWidget *win;
	GtkWidget *button;
	GtkWidget *vbox;
	GtkWidget *viewer;
	GtkWidget *tbar;
	
	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "vbox");
	gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
	gtk_container_set_border_width(GTK_CONTAINER(win), 1);
	ui->win = win;
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); 
	gtk_container_add(GTK_CONTAINER(win), vbox);

	tbar = create_toolbar();
	gtk_box_pack_start(GTK_BOX(vbox), tbar, FALSE, FALSE, 0);

	viewer = create_text_viewer();
	gtk_box_pack_start(GTK_BOX(vbox), viewer, TRUE, TRUE, 0);

	gtk_widget_show_all(win);
	
	create_tags();
}

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;
	
	app = gtk_application_new("org.gtk.viewer", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(create_main_window), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
