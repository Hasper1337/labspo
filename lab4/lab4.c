#include "lab4_sqlite.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define UI_FILE "lab4.glade" 

enum
{
    OWNERID = 0,
    OWNERNAME,
    AUTOID,
    AUTOMODEL,
    AUTOVIN,
    AUTOHP,
    AUTOCOLOR,
    CARBRANDID,
    CARBRANDNAME
};

struct mainWindowObjects{
    GtkWindow *main_window;
    GtkTreeView *treeview;
    GtkListStore *liststore;
    GtkTreeViewColumn *column_owner_id;
    GtkTreeViewColumn *column_owner_name;
    GtkTreeViewColumn *column_automobile_id;
    GtkTreeViewColumn *column_automobile_model;
    GtkTreeViewColumn *column_automobile_vin;
    GtkTreeViewColumn *column_automobile_hp;
    GtkTreeViewColumn *column_automobile_color;
    GtkTreeViewColumn *column_carbrand_id;
    GtkTreeViewColumn *column_carbrand_name;
    GtkDialog        *dialog_add;
    GtkEntry         *entryOwnerNameDlg;
    GtkEntry         *entryAutoModelDlg;
    GtkEntry         *entryAutoVINDlg;
    GtkEntry         *entryAutoHPDlg;
    GtkEntry         *entryAutoColorDlg;
    GtkComboBoxText  *comboCarBrandDlg;
} mainWindowObjects;


int callback(void *not_used, int argc, char **argv, char **col_names)
{
    GtkTreeIter iter;
    if (argc == 9)
    {
        gtk_list_store_append(GTK_LIST_STORE(mainWindowObjects.liststore), &iter);
        gtk_list_store_set(GTK_LIST_STORE(mainWindowObjects.liststore), &iter, OWNERID, 
                            atoi(argv[OWNERID]), OWNERNAME, argv[OWNERNAME], AUTOID, 
                            atoi(argv[AUTOID]), AUTOMODEL, argv[AUTOMODEL], AUTOVIN, 
                            argv[AUTOVIN],
                           AUTOHP, atoi(argv[AUTOHP]), AUTOCOLOR, argv[AUTOCOLOR],
                           CARBRANDID, atoi(argv[CARBRANDID]), CARBRANDNAME, argv[CARBRANDNAME], -1);
    }
    return 0;
}

int main(int argc, char **argv){
    GtkBuilder *builder;
    GError *error = NULL;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
    {
        g_warning("%s\n", error->message);
        g_free(error);
        return (1);
    }

    mainWindowObjects.main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    mainWindowObjects.treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree_owner"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(mainWindowObjects.treeview);
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
    mainWindowObjects.liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore_all"));

    mainWindowObjects.column_owner_id = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_owner_id"));
    mainWindowObjects.column_owner_name = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_owner_name"));
    mainWindowObjects.column_automobile_id = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_auto_id"));
    mainWindowObjects.column_automobile_model = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_auto_model"));
    mainWindowObjects.column_automobile_vin = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_auto_vin"));
    mainWindowObjects.column_automobile_hp = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_auto_hp"));
    mainWindowObjects.column_automobile_color = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_auto_color"));
    mainWindowObjects.column_carbrand_id = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_carbrand_id"));
    mainWindowObjects.column_carbrand_name = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_carbrand_name"));
mainWindowObjects.dialog_add = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_add"));
mainWindowObjects.entryOwnerNameDlg = GTK_ENTRY(gtk_builder_get_object(builder, "entryOwnerNameDlg"));
mainWindowObjects.entryAutoModelDlg = GTK_ENTRY(gtk_builder_get_object(builder, "entryAutoModelDlg"));
mainWindowObjects.entryAutoVINDlg = GTK_ENTRY(gtk_builder_get_object(builder, "entryAutoVINDlg"));
mainWindowObjects.entryAutoHPDlg = GTK_ENTRY(gtk_builder_get_object(builder, "entryAutoHPDlg"));
mainWindowObjects.entryAutoColorDlg = GTK_ENTRY(gtk_builder_get_object(builder, "entryAutoColorDlg"));
mainWindowObjects.comboCarBrandDlg = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "comboCarBrandDlg"));

    sqlite_fill_brand_combo(mainWindowObjects.comboCarBrandDlg);
    
    gtk_builder_connect_signals(builder, &mainWindowObjects);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.main_window));

    sqlite_get_data();
    gtk_main();
}

// Кнопка "сохранить"
G_MODULE_EXPORT void on_saveButton_clicked(GtkWidget *button, gpointer data)
{
    GtkTreeIter iter;
    gboolean reader =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    while (reader)
    {
        gint ownerid;
        gchar *ownername;
        gint automobileid;
        gchar *automobilemodel;
        gchar *automobilevin;
        gint automobilehp;
        gchar *automobilecolor;
        gint carbrandid;
        gchar *carbrandname;
        gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter, OWNERID, &ownerid,
                           OWNERNAME, &ownername, AUTOID, &automobileid, AUTOMODEL, &automobilemodel,
                           AUTOVIN, &automobilevin, AUTOHP, &automobilehp, AUTOCOLOR, &automobilecolor, 
                           CARBRANDID, &carbrandid, CARBRANDNAME, &carbrandname, -1);
        sqlite_update(ownerid, ownername, automobileid, automobilemodel, automobilevin, automobilehp, 
        automobilecolor, carbrandid, carbrandname);
        reader = gtk_tree_model_iter_next(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    }
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();
}

G_MODULE_EXPORT void
on_dialog_add_response(GtkDialog *dialog, gint response_id, gpointer data)
{
    if (response_id == GTK_RESPONSE_OK) {
        // 1) Считаем введённые данные из полей диалога
        const char *ownername = gtk_entry_get_text(mainWindowObjects.entryOwnerNameDlg);
        const char *automodel = gtk_entry_get_text(mainWindowObjects.entryAutoModelDlg);
        const char *autovin   = gtk_entry_get_text(mainWindowObjects.entryAutoVINDlg);
        const char *hp_text   = gtk_entry_get_text(mainWindowObjects.entryAutoHPDlg);
        const char *autocolor = gtk_entry_get_text(mainWindowObjects.entryAutoColorDlg);
        const char *brandid_s = gtk_combo_box_get_active_id(GTK_COMBO_BOX(mainWindowObjects.comboCarBrandDlg));
        int         carbrandid = brandid_s ? atoi(brandid_s) : 0;
        int         autohp     = atoi(hp_text);

        // Проверка обязательных полей
        if (ownername[0] && automodel[0]) {
            int ownerid = sqlite_get_next_owner_id();
            int autoid  = sqlite_get_next_auto_id();

            // 2) Добавляем в список и БД
            GtkTreeIter iter;
            gtk_list_store_append(mainWindowObjects.liststore, &iter);
            gtk_list_store_set(mainWindowObjects.liststore, &iter,
                OWNERID, ownerid,
                OWNERNAME, ownername,
                AUTOID, autoid,
                AUTOMODEL, automodel,
                AUTOVIN, autovin,
                AUTOHP, autohp,
                AUTOCOLOR, autocolor,
                CARBRANDID, carbrandid,
                CARBRANDNAME, gtk_combo_box_text_get_active_text(mainWindowObjects.comboCarBrandDlg),
                -1);

            if (!sqlite_insert(ownerid, ownername, autoid, automodel,
                               autovin, autohp, autocolor,
                               carbrandid, NULL))
            {
                // при провале вставки откатим интерфейс
                gtk_list_store_remove(mainWindowObjects.liststore, &iter);
                // TODO: показать пользователю сообщение об ошибке
            }
        }
    }

    // В любом случае прячем диалог
    gtk_widget_hide(GTK_WIDGET(dialog));
}

//  Доделать события для редактировния строк в столбцах(для всех которые видит пользователь)
G_MODULE_EXPORT void on_clnrender_owner_name_edited(GtkCellRendererText *renderer, gchar *path,
                                                         gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, OWNERNAME, new_text, -1);
    }
}

G_MODULE_EXPORT void on_clnrender_automobile_model_edited(GtkCellRendererText *renderer, gchar *path,
                                                         gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, AUTOMODEL, new_text, -1);
    }
}

G_MODULE_EXPORT void on_clnrender_automobile_vin_edited(GtkCellRendererText *renderer, gchar *path,
                                                        gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, AUTOVIN, new_text, -1);    
    }
}

G_MODULE_EXPORT void on_clnrender_automobile_hp_edited(GtkCellRendererText *renderer, gchar *path,
                                                        gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {   
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, AUTOHP, new_text, -1);
    }   
}

G_MODULE_EXPORT void on_clnrender_automobile_color_edited(GtkCellRendererText *renderer, gchar *path,
                                                        gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, AUTOCOLOR, new_text, -1);
    }
}


G_MODULE_EXPORT void on_clnrender_carbrand_name_edited(GtkCellRendererText *renderer, gchar *path,
                                                        gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, CARBRANDNAME, new_text, -1);
    }   
}

G_MODULE_EXPORT void on_clnrender_carbrand_id_edited(GtkCellRendererText *renderer, gchar *path,
                                                     gchar *new_text, gpointer data) {
    if (g_ascii_strcasecmp(new_text, "") != 0) {
        GtkTreeIter iter;
        GtkTreeModel *model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, CARBRANDID, atoi(new_text), -1);
        }
    }
}
G_MODULE_EXPORT void
on_addButton_clicked(GtkWidget *button, gpointer data)
{
    /* Сбрасываем старые вводы — чтобы при повторном открытии поля были пустыми */
    gtk_entry_set_text(mainWindowObjects.entryOwnerNameDlg, "");
    gtk_entry_set_text(mainWindowObjects.entryAutoModelDlg, "");
    gtk_entry_set_text(mainWindowObjects.entryAutoVINDlg,   "");
    gtk_entry_set_text(mainWindowObjects.entryAutoHPDlg,    "");
    gtk_entry_set_text(mainWindowObjects.entryAutoColorDlg, "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(mainWindowObjects.comboCarBrandDlg), -1);

    /* Показываем диалог и ждем ответа */
    gint response = gtk_dialog_run(mainWindowObjects.dialog_add);
    if (response == GTK_RESPONSE_OK) {
        /* Если нажали «Добавить» — выполним вставку */
        const char *ownername = gtk_entry_get_text(mainWindowObjects.entryOwnerNameDlg);
        const char *automodel = gtk_entry_get_text(mainWindowObjects.entryAutoModelDlg);
        const char *autovin = gtk_entry_get_text(mainWindowObjects.entryAutoVINDlg);
        const char *hp_text = gtk_entry_get_text(mainWindowObjects.entryAutoHPDlg);
        const char *autocolor = gtk_entry_get_text(mainWindowObjects.entryAutoColorDlg);
        const char *brandname = gtk_combo_box_text_get_active_text(mainWindowObjects.comboCarBrandDlg);

        if (ownername[0] && automodel[0]) {
            int autohp = atoi(hp_text);
            int ownerid = sqlite_get_next_owner_id();
            int autoid = sqlite_get_next_auto_id();
            int carbrandid = sqlite_get_brand_id_by_name(brandname);

            GtkTreeIter iter;
            gtk_list_store_append(mainWindowObjects.liststore, &iter);
            gtk_list_store_set(mainWindowObjects.liststore, &iter,
                OWNERID, ownerid,
                OWNERNAME, ownername,
                AUTOID, autoid,
                AUTOMODEL, automodel,
                AUTOVIN, autovin,
                AUTOHP, autohp,
                AUTOCOLOR, autocolor,
                CARBRANDID, carbrandid,
                CARBRANDNAME, brandname,
                -1);

            /* Сразу записываем в БД */
            if (!sqlite_insert(ownerid, ownername, autoid, automodel,
                               autovin, autohp, autocolor,
                               carbrandid, brandname)) {
                gtk_list_store_remove(mainWindowObjects.liststore, &iter);
                // TODO: показать пользователю сообщение об ошибке
            }
        }
    }
    /* Скрываем диалог (gtk_dialog_run сам не прячет его) */
    gtk_widget_hide(GTK_WIDGET(mainWindowObjects.dialog_add));
}


// Кнопка "сохранить"
G_MODULE_EXPORT void on_btnsave_clicked(GtkWidget *button, gpointer data)
{
    GtkTreeIter iter;
    gboolean reader =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    while (reader)
    {
        gint ownerid;
        gchar *ownername;
        gint autoid;
        gchar *automodel;
        gchar *autovin;
        gint autohp;
        gchar *autocolor;
        gint carbrandid;
        gchar *carbrandname;
        
        gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter,
                          OWNERID, &ownerid,
                          OWNERNAME, &ownername,
                          AUTOID, &autoid,
                          AUTOMODEL, &automodel,
                          AUTOVIN, &autovin,
                          AUTOHP, &autohp,
                          AUTOCOLOR, &autocolor,
                          CARBRANDID, &carbrandid,
                          CARBRANDNAME, &carbrandname,
                          -1);
                          
        sqlite_update(ownerid, ownername, autoid, automodel, autovin, autohp, autocolor, carbrandid, carbrandname);
        reader = gtk_tree_model_iter_next(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    }
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();
}

/* Кнопка "Удалить" */
G_MODULE_EXPORT void on_deleteButton_clicked(GtkWidget *button, gpointer data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(mainWindowObjects.treeview));
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int ownerid, autoid;
        gtk_tree_model_get(model, &iter, OWNERID, &ownerid, AUTOID, &autoid, -1);
        if (sqlite_delete(ownerid, autoid)) {
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        }
    }
}

G_MODULE_EXPORT void on_window_destroy(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}
