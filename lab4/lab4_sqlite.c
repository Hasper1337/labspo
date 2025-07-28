#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "lab4_sqlite.h"

int callback(void *, int, char **, char **);
void sqlite_get_data(){
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql = "SELECT \
                    owner.owner_id, \
                    owner.full_name, \
                    automobile.automobile_id, \
                    automobile.model, \
                    automobile.vin, \
                    automobile.hp, \
                    automobile.color, \
                    car_brand.car_brand_id, \
                    car_brand.name \
                FROM \
                    automobile \
                INNER JOIN car_brand ON automobile.car_brand_id = car_brand.car_brand_id \
                INNER JOIN owner ON automobile.owner_id = owner.owner_id;";
    rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}


// Дописать для owner и для carbrand функцию UPDATE
void sqlite_update(int ownerid, char *ownerfullname, int automobileid, char *automobilemodel,
                   char *automobilevin, int automobilehp, char *automobilecolor, 
                   int carbrandid, char *carbrandname) {
    
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql_automobile_temp = "UPDATE automobile\
                                    SET \
                                    model = '%s', \
                                    vin = '%s', \
                                    hp = %d, \
                                    color = '%s' \
                                WHERE \
                                    automobile_id = %d;";

    
    
    char sql_automobile[355];
    sprintf(sql_automobile, sql_automobile_temp, automobilemodel, 
    automobilevin, automobilehp, automobilecolor, automobileid);
    rc = sqlite3_exec(db, sql_automobile, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data in components\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    // Обновление owner
    char *sql_owner_temp = "UPDATE owner SET full_name = '%s' WHERE owner_id = %d;";
    char sql_owner[255];
    sprintf(sql_owner, sql_owner_temp, ownerfullname, ownerid);
    rc = sqlite3_exec(db, sql_owner, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to update owner: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // Обновление car_brand
    char *sql_carbrand_temp = "UPDATE car_brand SET name = '%s' WHERE car_brand_id = %d;";
    char sql_carbrand[255];
    sprintf(sql_carbrand, sql_carbrand_temp, carbrandname, carbrandid);
    rc = sqlite3_exec(db, sql_carbrand, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to update car_brand: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_close(db);
}

int sqlite_get_next_owner_id(void) {
    sqlite3 *db;
    int next_id = 1;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return next_id;
    }

    const char *sql = "SELECT COALESCE(MAX(owner_id),0)+1 FROM owner;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            next_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return next_id;
}

// Аналогично для automobile_id
int sqlite_get_next_auto_id(void) {
    sqlite3 *db;
    int next_id = 1;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return next_id;
    }

    const char *sql = "SELECT COALESCE(MAX(automobile_id),0)+1 FROM automobile;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            next_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return next_id;
}

// Получает carbrand_id по имени
int sqlite_get_brand_id_by_name(const char *brandname) {
    sqlite3 *db;
    int cid = 0;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return cid;
    }

    const char *sql = "SELECT car_brand_id FROM car_brand WHERE name = ? LIMIT 1;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, brandname, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            cid = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return cid;
}

// Вставка новой записи сразу в обе таблицы owner & automobile
int sqlite_insert(int ownerid, const char *ownername,
                  int autoid, const char *automodel,
                  const char *autovin, int autohp,
                  const char *autocolor, int carbrandid,
                  const char *carbrandname)
{
    sqlite3 *db;
    char *err_msg = NULL;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Начинаем транзакцию
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) goto rollback;

    // 1) Вставка OWNER (если не существует)
    {
        sqlite3_stmt *st;
        const char *sql1 = 
            "INSERT OR IGNORE INTO owner(owner_id, full_name) VALUES(?,?);";
        sqlite3_prepare_v2(db, sql1, -1, &st, NULL);
        sqlite3_bind_int(st,    1, ownerid);
        sqlite3_bind_text(st,   2, ownername, -1, SQLITE_STATIC);
        sqlite3_step(st);
        sqlite3_finalize(st);
    }

    // 2) Вставка AUTOMOBILE
    {
        sqlite3_stmt *st2;
        const char *sql2 =
            "INSERT INTO automobile(automobile_id, owner_id, model, vin, hp, color, car_brand_id) "
            "VALUES(?,?,?,?,?,?,?);";
        sqlite3_prepare_v2(db, sql2, -1, &st2, NULL);
        sqlite3_bind_int(st2,   1, autoid);
        sqlite3_bind_int(st2,   2, ownerid);
        sqlite3_bind_text(st2,  3, automodel, -1, SQLITE_STATIC);
        sqlite3_bind_text(st2,  4, autovin,   -1, SQLITE_STATIC);
        sqlite3_bind_int(st2,   5, autohp);
        sqlite3_bind_text(st2,  6, autocolor, -1, SQLITE_STATIC);
        sqlite3_bind_int(st2,   7, carbrandid);
        sqlite3_step(st2);
        sqlite3_finalize(st2);
    }

    // Подтверждаем транзакцию
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, &err_msg);
    if (rc == SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

rollback:
    sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    fprintf(stderr, "SQLite insert error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 0;
}

// Удаление записи автомобиля (owner не трогаем)
int sqlite_delete(int ownerid, int autoid)
{
    sqlite3 *db;
    char *err_msg = NULL;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    
    char sql[256];
    snprintf(sql, sizeof(sql),
             "DELETE FROM automobile WHERE owner_id = %d AND automobile_id = %d;",
             ownerid, autoid);

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQLite delete error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}

void sqlite_fill_brand_combo(GtkComboBoxText *combo) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT car_brand_id, name FROM car_brand ORDER BY name;";
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        g_warning("Cannot open DB for brands: %s", sqlite3_errmsg(db));
        return;
    }
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Сначала очищаем старые элементы
        gtk_combo_box_text_remove_all(combo);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char *name = (const char*)sqlite3_column_text(stmt, 1);
            // Первый аргумент — идентификатор элемента (строка), здесь int→строка
            char id_str[16];
            snprintf(id_str, sizeof(id_str), "%d", id);
            gtk_combo_box_text_append(combo, id_str, name);
        }
        sqlite3_finalize(stmt);
    } else {
        g_warning("Failed to prepare brand query: %s", sqlite3_errmsg(db));
    }
    sqlite3_close(db);
}


