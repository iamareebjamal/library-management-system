#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <textutils.h>
#include <database.h>

char* gen_key(struct Book *book){
	char *key = to_upper(combine(book->title, book->author, book->publisher));
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
	int hash = gen_hash(gen_key(book))%997;
	book->id = hash;
	struct Library *lib = &db->library;
	int *index = &(lib->book_count);
	lib->keys[*index] = hash;
	lib->books[hash] = *book;
	(*index)++;
	save(db);
	return hash;
}

#endif
