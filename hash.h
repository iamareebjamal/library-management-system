#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "textutils.h"
#include "database.h"

char* gen_key(char* string){
	char *key = to_upper(string);
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

int insert_in_hash( DB *db, struct Book *book, unsigned long hash){
	int offset = hash%997;
	printf("%lu\n", hash);
	//Check if the address is pre-occupied
	if(key_in_table(offset, &db->library)==-1){
		book->id = offset;
		struct Library *lib = &db->library;
		int *index = &(lib->book_count);
		lib->keys[*index] = offset;
		lib->books[offset] = *book;
		(*index)++;
		save(db);
		printf("Book entered\n");
		printf("Offset : %d\n", offset);
		return offset;
	} else {
		//Either Collision has occurred or same ID book is being added again!
		//Let's find out what?
		if(strcmp(db->library.books[offset].title, book->title)==0){
			printf("Same Book entered\tOffset : %d\n", offset);
			return -1;
		} else {
			printf("Collision\n");
			insert_in_hash(db, book, hash+1);
		}
	}
}

int add_book(DB *db, struct Book *book){
	//Generate hash address for the book
	unsigned long hash = gen_hash(gen_key(book->title));
	int offset =  insert_in_hash(db, book, hash);
	printf("Returned offset : %d\n", offset);
	return offset;
}

struct Book* find_book(DB *db, char* title){
	int key = gen_hash(gen_key(title))%997;
	if(key_in_table(key, &db->library)!=-1){
		return &db->library.books[key];
	} else {
		printf("Book not found\n");
		return NULL;
	}
}

#endif
