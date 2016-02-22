#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "textutils.h"
#include "database.h"

char* gen_key(struct Book *book){
	char *key = to_upper(book->title);
	clean(key);
	return key;
}

unsigned long gen_hash(char* key){
	unsigned long hash = 5381;
	char* iter = key;
	while(*++iter){
		hash = ( (hash<<5) ^ hash ) + *iter;
	}
	free(key);
	return hash;
}

int key_in_table(int key, struct Library *library){
	int len = library->book_count;
	int i;
	for(i=0;i<len;i++){
		if(key==library->keys[i])
			return i;
	}
	return -1;
}

int add_book(DB *db, struct Book *book){
	//Generate hash address for the book
	int hash = gen_hash(gen_key(book))%997;
	//Check if the address is pre-occupied
	if(key_in_table(hash, &db->library)==-1){
		book->id = hash;
		struct Library *lib = &db->library;
		int *index = &(lib->book_count);
		lib->keys[*index] = hash;
		lib->books[hash] = *book;
		(*index)++;
		save(db);
		return hash;
	} else {
		//Either Collision has occurred or same ID book is being added again!
		//Let's find out what?

	}
	return 0;
}

#endif
