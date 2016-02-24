#include <stdio.h>
#include "hash.h"

DB  db;

void ask_pass(){
	char password[15];
	printf("Please enter new password : ");
	scanf("%s", password);
	while(!set_password(&db, password)){
		printf("\nPassword is of invalid format."\
		"\nPlease enter a valid password of length"\
		" between 8 and 15 characters with no spaces"\
		"\nYou can type \'^cancel\' to cancel the password modification.\n\n");
		fgets(password, 15, stdin);
		if(strcmp(password, "^cancel")==0)return;
	}
}

void print_books(){
	//Print all the entered books
	int i;
	for(i = 0; i < db.library.book_count; i++){
		struct Book b = db.library.books[db.library.keys[i]];
		print_book(&b);
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
	int *list = search_books(&db, "J.K", AUTHOR);
	int size = *list;
	printf("%d\n", size);
	int i;
	for(i=1;i<=size;i++){
		struct Book *b = find_by_id(&db, list[i]);
		print_book(b);
	}
	free(list);
}

int main(){
	int i = 0;
	if(load(&db)==0){
		printf("Database load failed. Terminating");
		return 0;
	}

	printf("%s\n", db.password);
	char* fn = "14peb049";
	
	/*if(strcmp(db.password, "0000")==0){
		ask_pass();
	}*/
	print_books();
	printf("\n\n\n");
	list_books();

	/*
	struct Book *found = find_book(&db, "MKAWER");
	if(found!=NULL)
		printf("id %s\n", found->title);*/
	struct Book book = {"Harry Potter", "J.K. Rowling", "Bloomsbury", 0, 20};
	//printf("%d\n", add_book(&db, &book));
	printf("%d\n", db.library.book_count);
	printf("\nPattern Match\t%d\n", verify_fn(fn));
	
	return 0;
}
