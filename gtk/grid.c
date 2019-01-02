#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data)
{
	g_print("hello\n");
}

static void act(GtkApplication *app, gpointer user_data)
{
	GtkWidget *win;
	GtkWidget *grid;
	GtkWidget *button;

	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "window");
	gtk_container_set_border_width(GTK_CONTAINER(win), 10);

	grid = gtk_grid_new();

	gtk_container_add(GTK_CONTAINER(win), grid);

	button = gtk_button_new_with_label("but1");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

	button = gtk_button_new_with_label("but2");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
	gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

	button = gtk_button_new_with_label("quit");
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), win);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

	gtk_widget_show_all(win);
}

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("org.gtk.grid", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(act), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
