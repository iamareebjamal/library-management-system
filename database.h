#ifndef DB_H
#define DB_H

#include <string.h>


struct Book{
	char title[40];
	char author[40];
	char publisher[40];
	int  id;
	int  stock;
};

struct Library{
	struct Book books[1000];
	int keys[1000];
	int book_count;
};

struct Transactions{
	struct Issues{
		char fac_no[6];
		int date;
		int book_id;
	} issues[1000];
	
	struct Returns{
		char fac_no[6];
		int date;
		int book_id;
	} returns[1000];
	
	int issue_count;
	int return_count;
};

struct DB{
	struct Transactions transactions;
	struct Library library;
	char password[15];
};

void save(DB *db);
void init(DB* db);
DB load();

/* Setters */

int set_password(DB *db, const char* password){
	if(verify_pass(password)){
		strcpy(db->password, password);
		save(db);
		return 1;
	}
	return 0;
}



/* File Operations */

void save(DB *db){
	FILE *fo;
	fo = fopen("/sdcard/AppProjects/Algorithm/jni/db.bin", "wb");
	fwrite(db, sizeof(DB), 1, fo);
	fclose(fo);
}

void init(DB* db){
	printf("Creating new Database...\n");
	char* pass = "0000";
	printf("Values Initialised...\n");
	db->transactions.issue_count = 0;
	db->transactions.return_count = 0;
	db->library.book_count = 0;
	strcpy(db->password, pass);
	save(db);
}

DB load(){
	DB di;
	FILE *fi;
	fi = fopen("/sdcard/AppProjects/Algorithm/jni/db.bin", "rb");
	if(fi==NULL){
		init(&di);
		return di;
	}
	printf("Reading old Database...\n");
	fread(&di, sizeof(DB), 1, fi);
	fclose(fi);
	return di;
}

#endif
