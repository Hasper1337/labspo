#ifndef LAB4_SQLITE_H
#define LAB4_SQLITE_H
#include <gtk/gtk.h> 

#define DB_FILE "mydb.db"

void sqlite_get_data();
void sqlite_update(int ownerid, char *ownername, int automobileid, char *automobilemodel, char *automobilevin, int automobilehp, char *automobilecolor, int carbrandid, char *carbrandname); // Доделать!!!
int sqlite_get_next_owner_id(void);
int sqlite_get_next_auto_id(void);
int sqlite_get_brand_id_by_name(const char *brandname);
int sqlite_insert(int ownerid, const char *ownername,
                  int autoid, const char *automodel,
                  const char *autovin, int autohp,
                  const char *autocolor, int carbrandid,
                  const char *carbrandname);
int sqlite_delete(int ownerid, int autoid);
void sqlite_fill_brand_combo(GtkComboBoxText *combo);
void on_dialog_add_response(GtkDialog *dialog, gint response_id, gpointer user_data);

#endif
