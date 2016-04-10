#ifndef DB_H
#define DB_H

#include "textutils.h"
#include <time.h>

#define PATH "db.bin" // Relative path to database
struct Book {
    char title[50];
    char author[50];
    char publisher[50];
    int id;
    int stock;
};

struct Library {
    struct Book books[1000];
    int keys[1000];
    int book_count;
};

struct Transactions {
    char fac_no[9];  // 1 character more for null
    time_t date;
    int book_id;
};

struct Manager {
    struct Transactions issues[1000];
    struct Transactions returns[1000];
    int issue_count;
    int return_count;
};

typedef struct DB {
    struct Manager manager;
    struct Library library;
    char password[15];
} DB;

/* Setters */
int set_password(DB *db, char *password) {
    if (verify_pass(password)) {
        strcpy(db->password, password);
        return save(db);
    }
    return 0;
}

/* File Operations */
int save(DB *db) {
    FILE *fo;
    fo = fopen(PATH, "wb");
    if (fo == NULL) {
        printf("File Write Error!\n");
        return 0;
    }
    fwrite(db, sizeof(DB), 1, fo);
    fclose(fo);
    return 1;
}

int init(DB *db) {
    // Initialise values
    char pass[5] = "0000";
    db->manager.issue_count = 0;
    db->manager.return_count = 0;
    db->library.book_count = 0;
    strcpy(db->password, pass);
    return save(db);
}

int load(DB *di) {
    FILE *fi;
    fi = fopen(PATH, "rb");
    if (fi == NULL) {
        if (init(di) == 0)
            return 0;
        return 1;
    }
    fread(di, sizeof(DB), 1, fi);
    fclose(fi);
    return 1;
}

#endif
