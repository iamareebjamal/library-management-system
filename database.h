#ifndef DB_H
#define DB_H

#include <string.h>
#include "textutils.h"

//#define PATH "/sdcard/AppProjects/Algorithm/jni/db.bin"
#define PATH "db.bin"

struct Book {
	char title[40];
	char author[40];
	char publisher[40];
	int  id;
	int  stock;
};

struct Library {
	struct Book books[1000];
	int keys[1000];
	int book_count;
};

struct Transactions {
	struct Issues {
		char fac_no[6];
		int date;
		int book_id;
	} issues[1000];

	struct Returns {
		char fac_no[6];
		int date;
		int book_id;
	} returns[1000];

	int issue_count;
	int return_count;
};

typedef struct DB {
	struct Transactions transactions;
	struct Library library;
	char password[15];
} DB;

int save(DB*);
int init(DB*);
int load(DB*);


void print_book(struct Book *b) {
	printf("%-20s%-20s%-20s\t%d\n", b->title, b->author, b->publisher, b->id);
}


/* Setters */

int set_password(DB *db, const char* password) {
	if (verify_pass(password)) {
		strcpy(db->password, password);
		save(db);
		return 1;
	}
	return 0;
}

int delete(int value, int* array, int *size) {
	int i = 0, shift = 0;
	for (i = 0; i < *size; i++) {
		if (array[i] == value) {
			shift = 1;
		}

		if (shift){
			if (i != *size - 1)
				array[i] = array[i + 1];
			else
				break;
		}
	}

	if (shift) {
		(*size)--;
		return 1;
	}
	return 0;
}

int delete_book(DB *db, struct Book *book){
	if(book == NULL){
		printf("Null Pointer Error\n");
		return 0;
	}
	book->title[0] = '\0';
	book->author[0] = '\0';
	book->publisher[0] = '\0';
	return (delete(book->id, db->library.keys, &db->library.book_count) && save(db));
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

int init(DB* db) {
	printf("Creating new Database...\n");
	char* pass = "0000";
	printf("Values Initialised...\n");
	db->transactions.issue_count = 0;
	db->transactions.return_count = 0;
	db->library.book_count = 0;
	strcpy(db->password, pass);
	return save(db);
}

int load(DB *di) {
	FILE *fi;
	fi = fopen(PATH, "rb");
	if (fi == NULL) {
		if (init(di) == 0) {
			return 0;
		}
		return 1;
	}
	printf("Reading old Database...\n");
	fread(di, sizeof(DB), 1, fi);
	fclose(fi);
	return 1;
}

#endif
