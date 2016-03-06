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
		if (strcmp(db->manager.issues[i].fac_no, fac_no) == 0 && (db->manager.issues[i].book_id == id)) {
			free(fac_no);
			return i;
		}
	}
	free(fac_no);
	return -1;
}




int is_book_avail(struct Book *book) {
	return book->stock > 0;
}




int issue_book(DB *db, int id, char* fac) {

	if (is_already_issued(db, id, fac) != -1) {
		printf("\nBook already issued to this faculty number :\t%s\n", fac);
		return 0;
	}

	struct Book *b = find_by_id(db, id);
	if (b != NULL && is_book_avail(b)) {
		int *issue_count = &(db->manager.issue_count);
		struct Transactions *transact = &(db->manager.issues[(*issue_count)++]);
		char* fac_no = to_upper(fac);
		strcpy(transact->fac_no , fac_no);
		//printf("%s\n",transact->fac_no);
		transact->book_id = id;
		transact->date = get_current_date();
		b->stock--;
		save(db);
		printf("Book Issued to : %s\n", fac_no);
		free(fac_no);
		return 1;
	}

	return 0;

}




void print_issued_books(DB *db) {

	int i;
	struct Manager *m = &db->manager;
	i = m->issue_count - 1;
	for (i; i >= 0; i--) {
		printf("%s\t", m->issues[i].fac_no);
		print_date(m->issues[i].date);
		struct Book *b = find_by_id(db, m->issues[i].book_id);
		print_book(b);
	}
}



int delete_index(struct Transactions *array, int index, int *length) {
	struct Transactions item ;
	item = array[index];
	int i;
	for (i = index; i < *length; i++) {
		array[i] = array[i + 1];
	}
	*length = *length - 1;
	return item.book_id;
}




int delete_from_issues(DB *db, int id, char *fac) {

	char *f = to_upper(fac);
	int index;
	struct Manager *m = &(db->manager);
	index = is_already_issued(db, id, fac);
	if (index == -1) {
		printf("cant delete since not issued\n");
		free(f);
		return 0;
	}
	else {

		int book_id = delete_index(db->manager.issues, index, &(db->manager.issue_count));
		struct Book *b = find_by_id(db, id);
		save(db);
	}

	free(f);
	return 1;
}

//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//
//
//
//
////*****************************************************************RETURN MODULE*************************************************************************
//******************************************************************************************************************************************************



int* get_issued_fac(DB *db, char* fac) {
	char * fac_no = to_upper(fac);
	int i;
	int *id_array = (int*)calloc(1000, sizeof(int));

	for (i = 0; i < db->manager.issue_count; i++) {
		if (strcmp(db->manager.issues[i].fac_no, fac_no) == 0)
			id_array[++id_array[0]] = i;
	}
	free(fac_no);
	return id_array;
}




void print_transaction(DB *db, struct Transactions *t) {

	printf("%s\t", t->fac_no);

	print_book(find_by_id(db, t->book_id));
	print_date(t->date);
}




int is_in_returns(DB *db, int id, char *fac){
	int i;
	char *fac_no = to_upper(fac);
	for (i = 0; i < db->manager.return_count; i++) {
		if (strcmp(db->manager.returns[i].fac_no, fac_no) == 0 && (db->manager.returns[i].book_id == id)){
			free(fac_no);
			return i;
		}
	}
	free(fac_no);
	return -1;
}




int add_to_return(DB *db, int id, char *fac) {
	struct Transactions *t;
	int index = is_already_issued(db, id, fac);

	if ((index != -1) && (is_in_returns(db,id,fac) == -1)) {
		t = &(db->manager.issues[index]);
		db->manager.returns[db->manager.return_count++] = *t;
		return save(db);
	}else{
		printf("Error !");
		return 0;
	}
	
}




int delete_from_returns(DB *db, int id, char* fac) {
	int i;
	char *fac_no = to_upper(fac);
	if(is_in_returns(db,id,fac) == -1){
		printf("Can't delete ");
		return 0;
	}
	for (i = 0; i < db->manager.return_count; i++) {
		if (strcmp(db->manager.returns[i].fac_no, fac_no) == 0 && (db->manager.returns[i].book_id == id))
			break;
	}
	int book_id = delete_index(db->manager.returns, i, &(db->manager.return_count));
	struct Book *b = find_by_id(db, id);
	b->stock++;
	save(db);
	return 1;
}



#endif
