#include "auth.h"
#include <gtest/gtest.h>
#include <sqlite3.h>

extern "C" {  // Добавляем обёртку для C-функций
    void setup_test_db(sqlite3 *db);
}

void setup_test_db(sqlite3 *db) {
    char *err_msg = nullptr;

    const char *sql = 
        "CREATE TABLE crews ("
        "id INTEGER PRIMARY KEY,"
        "personnel_number TEXT UNIQUE NOT NULL,"
        "role INTEGER NOT NULL"
        ");"
        "INSERT INTO crews (personnel_number, role) VALUES ('CRW001', 1);";

    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
}

TEST(AuthTest, ValidLogin) {
    sqlite3 *db;
    ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
    setup_test_db(db);

    Role role;
    EXPECT_TRUE(authenticate(db, "CRW001", &role));
    EXPECT_EQ(role, CREW);

    sqlite3_close(db);
}

TEST(AuthTest, InvalidLogin) {
    sqlite3 *db;
    ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
    setup_test_db(db);

    Role role;
    EXPECT_FALSE(authenticate(db, "INVALID_USER", &role));

    sqlite3_close(db);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
