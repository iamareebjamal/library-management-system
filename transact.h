#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"

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

#endif