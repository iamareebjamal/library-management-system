#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"
#include "hash.h"
#include <time.h>

//*****************************************************************ISSUE MODULE*************************************************************************
//******************************************************************************************************************************************************
void print_date(time_t t) {

	printf("%s", ctime(&t));
}

time_t get_current_date() {
	time_t t = time(0);
	struct tm *tm = localtime(&t);
	t = mktime(tm);
	return t;
}

int is_already_issued(DB *db, int id, char *fac) {
	int i;
	char * fac_no = to_upper(fac);
	for (i = 0; i < db->manager.issue_count; i++) {
		printf("%s: %s\n", db->manager.issues[i].fac_no,fac_no);
		if (strcmp(db->manager.issues[i].fac_no, fac_no) == 0 && (db->manager.issues[i].book_id == id)) {
			printf("%s: %s\n", db->manager.issues[i].fac_no,fac_no);
			printf("\nBOOK already issued to this faculty number :\t%s\n", fac);
			free(fac_no);
			return 1;
		}
	}
	free(fac_no);
	return 0;
}

int is_book_avail(struct Book *book){
	return book->stock > 0;
}

int issue_book(DB *db, int id, char* fac) {
	
	//add verify fac function
	//donot issue if book stock is 0 ; delete key from library also decrease book count
	if (is_already_issued(db, id, fac)){
		printf("Book Already Issued\n");
		return -1;
	}
	struct Book *b = find_by_id(db, id);
	if (b!=NULL && is_book_avail(b)) {
		int *issue_count = &(db->manager.issue_count);
		struct Transactions *transact = &(db->manager.issues[(*issue_count)++]);
		char* fac_no = to_upper(fac);
		strcpy(transact->fac_no , fac_no);
		printf("%s\n",transact->fac_no);
		transact->book_id = id;
		transact->date = get_current_date();
		b->stock--;
		save(db);
		printf("Book Issued to : %s\n", fac_no);
		free(fac_no);
		return 1;
	}
	printf("Book Out of Stock\n");
	return 0;

}

void print_issue_books(DB *db){

	int i;
	struct Manager *m = &db->manager;
	i = m->issue_count;
	for (i; i>=0; i--){
		printf("%-30s",m->issues[i].fac_no);
		print_date(m->issues[i].date);
		struct Book *b = find_by_id(db, m->issues[i].book_id);
		//print_book(b);
	}
}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
#endif
