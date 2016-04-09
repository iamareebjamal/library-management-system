#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"
#include "hash.h"
#include <time.h>

//*****************************************************************ISSUE MODULE*************************************************************************
//******************************************************************************************************************************************************



time_t get_current_date() {
	time_t t = time(0);
	struct tm *tm = localtime(&t);
	t = mktime(tm);
	return t;
}

int is_book_avail(struct Book *book) {
	return book->stock > 0;
}

int is_already_issued(struct Manager *manager, int id, char *fac) {
	int i;
	char * fac_no = to_upper(fac);
	for (i = 0; i < manager->issue_count; i++) {
		if (strcmp(manager->issues[i].fac_no, fac_no) == 0 && (manager->issues[i].book_id == id)) {
			free(fac_no);
			return i;
		}
	}
	free(fac_no);
	return -1;
}
/**
 * issue_book function :
 * @params = DB*, int id, char * faculty_number
 * @return = 0, when book is already issued
 *           1, when successful issue
 *          -1, when book not found or book stock is 0
 */

int issue_book(DB *db, int id, char* fac) {

	if (is_already_issued(&(db->manager), id, fac) != -1) {
		printf("\nBook already issued to this faculty number :\t%s\n", fac);
		return 0;
	}

	struct Book *b = find_by_id(db, id);

	if (b != NULL && is_book_avail(b)) {
		int *issue_count = &(db->manager.issue_count);
		struct Transactions *transact = &(db->manager.issues[(*issue_count)++]);
		char* fac_no = to_upper(fac);
		strcpy(transact->fac_no , fac_no);
		transact->book_id = id;
		transact->date = get_current_date();
		b->stock--;
		save(db);
		printf("Book Issued to : %s\n", fac_no);
		free(fac_no);
		return 1;
	}

	return -1;
}

void print_issued_books(DB *db) {
	int i;
	struct Manager *m = &db->manager;
	for (i = m->issue_count - 1; i >= 0; i--) {
		printf("%s\t", m->issues[i].fac_no);
		struct Book *b = find_by_id(db, m->issues[i].book_id);
		print_book(b);
		printf("%s", ctime(&(m->issues[i].date)));
	}
}

void delete_index(struct Transactions *array, int index, int *length) {
	int i;
	for (i = index; i < *length; i++) {
		array[i] = array[i + 1];
	}
	(*length)--;
}

//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//
//
//
//
////*****************************************************************RETURN MODULE*************************************************************************
//******************************************************************************************************************************************************


/**
 * [get_issued_fac gives list of indexes where it founds the issued books of particular faculty number
 * @param  manager [pointer to the manager that contains the issues and returns array also respective count]
 * @param  fac     [faculty number whose issued books to list]
 * @return         [int pointer to the indexes where in issue the match is found]
 */
int* get_issued_fac(struct Manager* manager, char fac[9]) {
	char *fac_no = to_upper(fac);
	int i;
	int *id_array = (int*)calloc(1000, sizeof(int));

	for (i = 0; i < manager->issue_count; i++) {
		if (strcmp(manager->issues[i].fac_no, fac_no) == 0)
			id_array[++id_array[0]] = i;
	}
	free(fac_no);
	return id_array;
}

void print_transaction(DB *db, struct Transactions *t) {

	printf("%s\t", t->fac_no);

	print_book(find_by_id(db, t->book_id));
	printf("%s", ctime(&(t->date)));
}

/**
 * @return      index if the book found in returns array
 *              -1 if book not found
 */
int is_in_returns(struct Manager *manager, int id, char *fac) {
	int i;
	char *fac_no = to_upper(fac);
	for (i = 0; i < manager->return_count; i++) {
		if (strcmp(manager->returns[i].fac_no, fac_no) == 0 && (manager->returns[i].book_id == id)) {
			free(fac_no);
			return i;
		}
	}
	free(fac_no);
	return -1;
}

int add_to_return(DB *db, struct Transactions* transact) {
	int index = is_already_issued(&(db->manager), transact->book_id, transact->fac_no);

	if ((index != -1) && (is_in_returns(&(db->manager), transact->book_id, transact->fac_no) == -1)) {
		db->manager.returns[db->manager.return_count++] = *transact;
		return save(db);
	} else {
		printf("Error ! Already queued for returns!");
		return 0;
	}

}

/*
	@return 0, when not found in returns array
		   -1, when not found in issues array
		    1, when successful transaction
 */

int return_book(DB *db, struct Transactions* transact) {
	int return_index = is_in_returns(&(db->manager), transact->book_id, transact->fac_no);
	int issue_index = is_already_issued(&(db->manager), transact->book_id, transact->fac_no);

	if (return_index == -1) {
		printf("\nNot queued for returns!");
		return 0;
	}

	if (issue_index == -1) {
		printf("\nNot found in issues!");
		return -1;
	}

	struct Book *b = find_by_id(db, transact->book_id);
	b->stock++;

	delete_index(db->manager.returns, return_index, &(db->manager.return_count));
	delete_index(db->manager.issues, issue_index, &(db->manager.issue_count));

	return save(db);
}


#endif
