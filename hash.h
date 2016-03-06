#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "database.h"

static const int TITLE     = 0;
static const int AUTHOR    = 1;
static const int PUBLISHER = 2;

char* gen_key(char* string) {
	char *key = to_upper(string);
	clean(key);
	return key;
}

unsigned long gen_hash(char* key) {
	unsigned long hash = 5381;
	char* iter = key;
	while (*++iter) {
		hash = ( (hash << 5) ^ hash ) + *iter;
	}
	free(key);
	return hash;
}

int key_in_table(int key, struct Library *library) {
	int len = library->book_count;
	int i;
	for (i = 0; i < len; i++) {
		if (key == library->keys[i])
			return i;
	}
	return -1;
}

int insert_in_hash( DB *db, struct Book *book, unsigned long hash) {
	int offset = hash % 997;
	//Check if the address is pre-occupied
	if (key_in_table(offset, &db->library) == -1) {
		book->id = offset;
		struct Library *lib = &db->library;
		int *index = &(lib->book_count);
		lib->keys[(*index)++] = offset;
		lib->books[offset] = *book;
		printf("Book entered\n");
		printf("Offset : %d\n", offset);
		save(db);
		return offset;
	} else {
		//Either Collision has occurred or same ID book is being added again!
		//Let's find out what?
		if (strcmp(db->library.books[offset].title, book->title) == 0) {
			printf("Same Book entered\tOffset : %d\n", offset);
			book->id = offset;
			return -1;
		} else {
			printf("Collision\n");
			insert_in_hash(db, book, hash + 1);
		}
	}
}

int add_book(DB *db, struct Book *book) {
	//Generate hash address for the book
	unsigned long hash = gen_hash(gen_key(book->title));
	int offset =  insert_in_hash(db, book, hash);
	printf("Returned offset : %d\n", offset);
	return offset;
}

struct Book* find_by_id(DB *db, int id) {

	if (key_in_table(id, &db->library) == -1) {
		printf("Book not found...\n");
		return NULL;
	} else {
		return &db->library.books[id];
	}
}

int update_book(DB *db, int id, int stock){
	struct Book *book = find_by_id(db, id);
	if(book){
		book->stock = stock;
		return save(db);
	}
	return 0;
}

/* Find a book by its exact title */
// To Do: match by clean
struct Book* find_book(DB *db, char* title) {
	unsigned long key = gen_hash(gen_key(title));

	int offset = key % 997;

	while (strcmp(db->library.books[offset].title, title) != 0) {
		offset = (++key) % 997;
		if (key_in_table(offset, &db->library) == -1) {
			break;
		}
	}

	if (strcmp(db->library.books[offset].title, title) == 0) {
		return &db->library.books[offset];
	} else {
		printf("Book not found...\n");
		return NULL;
	}

}

//Fuzzy Search of Books by title, author or publisher. Returns an integer array
//containing the id of books
//To Do: Reorganize Similar Code
int* search_books(DB *db, char* search, int mode) {
	int i;
	int *list = (int*) calloc(1000, sizeof(int));
	char* pattern = to_upper(search);
	clean(pattern);
	switch (mode) {
	case 0:
		for (i = 0; i < db->library.book_count; i++) {
			struct Book book = db->library.books[db->library.keys[i]];
			char* matcher = to_upper(book.title);
			clean(matcher);
			if (strstr(matcher, pattern) != NULL) {
				list[++list[0]] = book.id;
			}
			free(matcher);
		}
		free(pattern);
		return list;
		break;
	case 1:
		for (i = 0; i < db->library.book_count; i++) {
			struct Book book = db->library.books[db->library.keys[i]];
			char* matcher = to_upper(book.author);
			clean(matcher);
			if (strstr((matcher), pattern) != NULL) {
				list[++list[0]] = book.id;
			}
			free(matcher);
		}
		free(pattern);
		return list;
		break;
	case 2:
		for (i = 0; i < db->library.book_count; i++) {
			struct Book book = db->library.books[db->library.keys[i]];
			char* matcher = to_upper(book.publisher);
			clean(matcher);
			if (strstr((matcher), pattern) != NULL) {
				list[++list[0]] = book.id;
			}
			free(matcher);
		}
		free(pattern);
		return list;
		break;
	default:
		printf("Invalid Mode. Range 0~2\n");
		return NULL;
	}
}

#endif
