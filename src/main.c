#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "db_ops.h"

// Инициализация базы: создание таблиц, если не существуют
void init_db(sqlite3 *db) {
    const char *bus_table_sql =
        "CREATE TABLE IF NOT EXISTS buses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "number TEXT NOT NULL,"
        "model TEXT NOT NULL,"
        "mileage REAL DEFAULT 0);";

    const char *route_table_sql =
        "CREATE TABLE IF NOT EXISTS routes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "start TEXT NOT NULL,"
        "end TEXT NOT NULL,"
        "distance REAL);";

    char *err_msg = NULL;

    if (sqlite3_exec(db, bus_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Ошибка при создании таблицы автобусов: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    if (sqlite3_exec(db, route_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Ошибка при создании таблицы маршрутов: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Меню на русском
void show_menu() {
    printf("\n=== Меню туристического бюро ===\n");
    printf("1. Добавить автобус\n");
    printf("2. Удалить автобус\n");
    printf("3. Обновить пробег автобуса\n");
    printf("4. Показать все автобусы\n");
    printf("5. Добавить маршрут\n");
    printf("6. Удалить маршрут\n");
    printf("7. Обновить расстояние маршрута\n");
    printf("8. Показать все маршруты\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

int main() {
    sqlite3 *db;
    if (sqlite3_open("database/tourism_bureau.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Не удалось открыть базу данных: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    init_db(db);

    int choice;
    do {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // очистка ввода
            continue;
        }

        switch (choice) {
            case 1: {
                char number[50], model[50];
                printf("Введите номер автобуса: ");
                scanf("%s", number);
                printf("Введите модель автобуса: ");
                scanf("%s", model);
                add_bus(db, number, model);
                break;
            }
            case 2: {
                int id;
                printf("Введите ID автобуса для удаления: ");
                scanf("%d", &id);
                delete_bus(db, id);
                break;
            }
            case 3: {
                int id;
                float mileage;
                printf("Введите ID автобуса: ");
                scanf("%d", &id);
                printf("Введите новый пробег: ");
                scanf("%f", &mileage);
                update_bus_mileage(db, id, mileage);
                break;
            }
            case 4:
                print_all_buses(db);
                break;

            case 5: {
                char name[50], start[50], end[50];
                float distance;
                printf("Введите название маршрута: ");
                scanf("%s", name);
                printf("Введите начальную точку: ");
                scanf("%s", start);
                printf("Введите конечную точку: ");
                scanf("%s", end);
                printf("Введите расстояние: ");
                scanf("%f", &distance);
                add_route(db, name, start, end, distance);
                break;
            }
            case 6: {
                int id;
                printf("Введите ID маршрута для удаления: ");
                scanf("%d", &id);
                delete_route(db, id);
                break;
            }
            case 7: {
                int id;
                float distance;
                printf("Введите ID маршрута: ");
                scanf("%d", &id);
                printf("Введите новое расстояние: ");
                scanf("%f", &distance);
                update_route_distance(db, id, distance);
                break;
            }
            case 8:
                print_all_routes(db);
                break;

            case 0:
                printf("Выход...\n");
                break;

            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }

    } while (choice != 0);

    sqlite3_close(db);
    return 0;
}

