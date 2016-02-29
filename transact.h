#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"


// Non POSIX. To be removed.
void clear_screen(){
	system ("tput clear");
}

// Non essential to transaction. Is UI element
// Either move to main or create other UI header
// containing all UI related functions like 
// ask password, serach books, etc
int user_input(){
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

struct Book* get_book(int id){
	
}

#endif