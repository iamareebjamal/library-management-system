#include <stdio.h>
#include "hash.h"
#include "transact.h"

DB  db;

void ask_pass() {
	char password[15];
	printf("Please enter new password : ");
	fgets(password, 15, stdin);
	while (!set_password(&db, password)) {
		printf("\nPassword is of invalid format."\
		       "\nPlease enter a valid password of length"\
		       " between 8 and 15 characters with no spaces"\
		       "\nYou can type \'^cancel\' to cancel the password modification.\n\n");
		fgets(password, 15, stdin);
		if (strcmp(password, "^cancel") == 0)return;
	}
}

void test_upper(char *s, char *z) {

	char *u = to_upper(s);
	strcpy(z, u);
	printf("%s: %s\n", u, z);
	free (u);
}

void list_books() {
	int *list = search_books(&db, "Ha", TITLE);
	int size = *list;
	int i;
	printf("%d\n", size);
	for (i = 1; i <= size; i++) {
		struct Book *b = find_by_id(&db, list[i]);
		print_book(b);
	}
	free(list);
}

void add_b() {
	struct Book b;
	char title[40], author[40], pub[40];
	int count;
	printf("enter title\n");
	scanf("%s", title);
	printf("enter author\n");
	scanf("%s", author);
	printf("enter pub\n");
	scanf("%s", pub);

	printf("enter stock\n");
	scanf("%d", &count);

	strcpy(b.title , title);
	strcpy(b.author, author);
	strcpy(b.publisher, pub);
	b.stock = count;
	add_book(&db, &b);

}

int main() {
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int length = 9;
	int i;
	char pass[15];
	if (load(&db) == 0) {
		printf("Database load failed. Terminating");
		return 0;
	}

	printf("%s\n", db.password);
	char* fn = "14peb049";
	char* f = "12peb049";

	/*if(strcmp(db.password, "0000")==0){
		ask_pass();
	}*/

	print_books(&db);
	printf("\n\n\n");
	list_books();


	//add_b();
	//struct Book book = {"ASasdwaaw", "George RR Martin", "U/K", 0, 40};
	//printf("%d\n", add_book(&db, &book));
	printf("%d\n", db.library.book_count);



	//issue_book(&db, 509, fn);
	

	printf("\nstock :%d\n", db.library.books[509].stock);
	
	printf("\nissue_count: %d\n", db.manager.issue_count);

	print_issued_books(&db);



	/*delete_from_issues(&db, 509, fn);

	
	printf("\nISSUED BOOKS:\n\n");
	print_issued_books(&db);*/

	int *j = get_issued_fac(&(db.manager), fn);
	printf("\nALL BOOKS ISSUED To FAC\n");
	for(i=1; i<=*j; i++){
		print_transaction(&db, &(db.manager.issues[j[i]]));
	}
	

	

	//i  = add_to_return(&db, &(db.manager.issues[j[1]]));
	//printf("\nadding successful or not : %d\n",i);
	printf("Return count: %d\n",db.manager.return_count);
	i = return_book(&db, &(db.manager.issues[j[2]]));
	printf("\nreturn successful or not : %d\n",i);
	printf("Return count: %d\n",db.manager.return_count);
	//delete_from_returns(&db, 509, fn);
	
	printf("%d\n",db.manager.return_count);
	

	print_issued_books(&db);

	printf("%d\n",db.manager.issue_count );

	return 0;

}
