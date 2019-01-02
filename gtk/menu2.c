#include <gtk/gtk.h>

static void
action_print(GSimpleAction *action,
                  GVariant      *parameter,
				  gpointer user_data)
{
	g_print("hello\n");
}
static GActionEntry entries[] = {
	{"p", action_print},
};
const gchar *mui =
	"<interface>"
	"<menu id='mb'>"
	"  <submenu>"
	"    <attribute name='label'>P</attribute>"
	"    <section>"
	"      <item>"
	"        <attribute name='label'>_Pt</attribute>"
	"        <attribute name='action'>test.p</attribute>"
	"      </item>"
	"    </section>"
	"  </submenu>"
	"  <submenu>"
	"    <attribute name='label'>T</attribute>"
	"    <section>"
	"      <item>"
	"        <attribute name='label'>_Ps</attribute>"
	"        <attribute name='action'>test.p</attribute>"
	"      </item>"
	"    </section>"
	"  </submenu>"
	"</menu>"
	"</interface>";

int main(int argc, char **argv)
{
	GtkWidget *mwin;
	GtkWidget *mbar;
	GtkBuilder *builder;
	GSimpleActionGroup *actions;
	GMenuModel *menu;
	GtkWidget *box;

	gtk_init(&argc, &argv);

	mwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(mwin), 200, 200);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(mwin), box);

	actions = g_simple_action_group_new();
	g_action_map_add_action_entries(G_ACTION_MAP(actions), entries, G_N_ELEMENTS(entries), mwin);

	builder = gtk_builder_new();
	gtk_builder_add_from_string(builder, mui, -1, NULL);
	menu = G_MENU_MODEL(gtk_builder_get_object(builder, "mb"));
	mbar = gtk_menu_bar_new_from_model(menu);
	gtk_widget_set_size_request(mbar, 20, 20);
	gtk_widget_insert_action_group(mbar, "test", G_ACTION_GROUP(actions));
	gtk_box_pack_start(GTK_BOX(box), mbar, 0, 0, 0);

	gtk_widget_show_all(mwin);
	gtk_main();
	return 0;
}
