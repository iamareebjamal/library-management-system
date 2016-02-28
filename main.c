#include <stdio.h>
#include "hash.h"
#include "transact.h"

DB  db;

void ask_pass(){
	char password[15];
	printf("Please enter new password : ");
	fgets(password, 15, stdin);
	while(!set_password(&db, password)){
		printf("\nPassword is of invalid format."\
		"\nPlease enter a valid password of length"\
		" between 8 and 15 characters with no spaces"\
		"\nYou can type \'^cancel\' to cancel the password modification.\n\n");
		fgets(password, 15, stdin);
		if(strcmp(password, "^cancel")==0)return;
	}
}

void print_test(int* a, int size){
	int i = 0;
	for(i = 0; i < size; i++){
		printf("%d  ", a[i]);
	}
	printf("\n");
}

void list_books(){
	int *list = search_books(&db, "Ha", TITLE);
	int size = *list;
	int i;
	printf("%d\n", size);
	for(i=1;i<=size;i++){
		struct Book *b = find_by_id(&db, list[i]);
		print_book(b);
	}
	free(list);
}

int main(){
	if(load(&db)==0){
		printf("Database load failed. Terminating");
		return 0;
	}

	printf("%s\n", db.password);
	char* fn = "14peb049";
	char *s = "14";
	
	/*if(strcmp(db.password, "0000")==0){
		ask_pass();
	}*/
	print_books(&db);
	printf("\n\n\n");
	list_books();

	/*
	struct Book *found = find_book(&db, "MKAWER");
	if(found!=NULL)
		printf("id %s\n", found->title);*/
	struct Book book = {"Introduction to Chemistry", "DC Pandey", "Bloomsbury", 0, 20};
	printf("%d\n", add_book(&db, &book));
	printf("%d\n", db.library.book_count);
	//printf("\nPattern Match\t%d\n", verify_fn(fn));
	printf("%d",is_fac_year(s));
	//int i = user_input();
	return 0;
}
