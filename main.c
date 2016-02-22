#include <stdio.h>
#include "textutils.h"
#include "database.h"
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

int main(){
	if(load(&db)==0){
		printf("Database load failed. Terminating");
		return;
	}

	printf("%s\n", db.password);
	char* fn = "14peb049";
	
	if(strcmp(db.password, "0000")==0){
		ask_pass();
	}
	
	struct Book book = {"Harry Potter", "J.K. Rowling", "Bloomsbury", 0, 20};

	printf("%d\n", add_book(&db, &book));
	printf("%d\n", db.library.book_count);
	printf("\n%d\n", verify_fn(fn));
	
	return 0;
}
