#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"

void print_books(DB *db){
	//Print all the entered books
	int i;
	for(i = 0; i < db->library.book_count; i++){
		struct Book b = db->library.books[db->library.keys[i]];
		print_book(&b);
	}
}

/*int user_input(){
	int i;
	printf("What u Want To do ?\n1: Issue\n2: Return");
	scanf("%d", &i);
	if (i == 1 || i == 2)
		return i;
	else {
		printf("Wrong Choice Choose Again:\n");
		user_input();
	}
}
int select_book(DB *db){
	print_books(&db)

}*/
#endif