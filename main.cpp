#include <stdio.h>
#include <textutils.h>
#include <database.h>
#include <hash.h>

DB  db;

void ask_pass(){
	char password[15] = "";
	printf("Please enter new password : ");
	scanf("%s", password);
	while(!set_password(&db, password)){
		printf("\nPassword is of invalid format."\
		"\nPlease enter a valid password of length"\
		" between 8 and 15 characters with no spaces"\
		"\nYou can type \'^cancel\' to cancel the password modification.\n\n");
		scanf("%s", password);
		if(strcmp(password, "^cancel")==0)break;
	}
	printf("Password updated successfully...\n\n");
}

int main(){
	db = load();
	printf("%s\n", db.password);
	char* fn = "14mEb049";
	
	if(strcmp(db.password, "0000")==0){
		ask_pass();
	}
	
	struct Book book = {"Harry Potter", "J.K. Rowling", "Bloomsbury", 0, 20};
	//printf("%d", add_book(&db, &book));
	printf("%d", db.library.book_count);
	printf("\n%d\n", verify_fn(fn));
	
	return 0;
}
