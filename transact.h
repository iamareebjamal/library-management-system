#ifndef TRANSACT_H
#define TRANSACT_H

#include <stdio.h>
#include "database.h"
#include "hash.h"

int is_already_issued(DB *db, int id, char *fac){
	int i, flag = 0;
	for(i=0; i<db->manager.issue_count; i++){
		if (strcmp(db->manager.issues[i].fac_no,fac) == 0 && (db->manager.issues[i].book_id == id)){
			printf("%d",i);
			flag = 1;
		}
			
	}
	if(flag)
		return 1;
	else{
		printf("Book not already Issued\n");
	}
	return 0;
}

void issue_book(DB *db,int id, char* fac){
	
	struct Book *b = find_by_id(db,id);
	char fno[8];
	strcpy(fno, fac);
	if(!is_already_issued(db, id, fac) && find_by_id(db, id)){
		//issue_count  =(int*) malloc (sizeof(int));
		int *issue_count = &(db->manager.issue_count);
		struct Transactions *transact= &(db->manager.issues[*issue_count]);
		
		strcpy(transact->fac_no ,fno);
		transact->book_id = id;
		transact->date =0000;	// change date 
		(*issue_count )++;
		b->stock -= 1;
	}else{
		printf("Error occured!");
	}

}
#endif
