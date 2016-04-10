#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "database.h"

static const int TITLE = 0;
static const int AUTHOR = 1;
static const int PUBLISHER = 2;

/* Hash key generators */
char *gen_key(char *string) {
    char *key = to_upper(string);
    clean(key);
    return key;
}

unsigned long gen_hash(char *key) {
    unsigned long hash = 5381;
    char *iter = key;
    while (*++iter) {
        hash = ((hash << 5) ^ hash) + *iter;
    }
    free(key);
    return hash;
}

/* Book Printing */
void print_book(struct Book *b) {
    printf("%-30s%-20s%-20s%d\n", b->title, b->author, b->publisher, b->stock);
}

void print_books(DB *db) {
    //Print all the entered books
    int i;
    printf("\n%-33s%-20s%-19s%-20s\n\n", "Title", "Author", "Publisher", "Stock");
    for (i = 0; i < db->library.book_count; i++) {
        struct Book b = db->library.books[db->library.keys[i]];
        printf("\n%d. ", i + 1);
        print_book(&b);
    }
}

/* Checks if a key is present in hash table */
int key_in_table(int key, struct Library *library) {
    int len = library->book_count;
    int i;
    for (i = 0; i < len; i++) {
        if (key == library->keys[i])
            return i;
    }
    return -1;
}

/* Hash insertions  */
int insert_in_hash(DB *db, struct Book *book, unsigned long hash) {
    int offset = hash % 997;
    //Check if the address is pre-occupied
    if (key_in_table(offset, &db->library) == -1) {
        book->id = offset;
        struct Library *lib = &db->library;
        int *index = &(lib->book_count);
        lib->keys[(*index)++] = offset;
        lib->books[offset] = *book;
        printf("\nBook entered\tID : %d\n", offset);
        save(db);
        return offset;
    } else {
        // Either Collision has occurred or same ID book is being added again!
        // Clean, strip and capitalise the entries
        char *stored = to_upper(db->library.books[offset].title);
        char *current = to_upper(book->title);
        clean(stored);
        clean(current);

        int matched = strcmp(stored, current) == 0;
        free(stored);
        free(current);

        if (matched) {
            printf("\nBook is already in database\tID : %d\n", offset);
            book->id = offset;
            return -1;
        } else {
            // Collision has occurred. Linearly probe the hash
            insert_in_hash(db, book, hash + 1);
        }
    }

    return -1;
}

int add_book(DB *db, struct Book *book) {
    //Generate hash address for the book
    unsigned long hash = gen_hash(gen_key(book->title));
    int offset = insert_in_hash(db, book, hash);
    return offset;
}

/* Find book by its ID */
struct Book *find_by_id(DB *db, int id) {

    if (key_in_table(id, &db->library) == -1) {
        printf("Book not found...\n");
        return NULL;
    } else {
        return &db->library.books[id];
    }
}

/* Find a book by its exact title */
struct Book *find_book(DB *db, char *title) {
    unsigned long key = gen_hash(gen_key(title));

    int offset = key % 997;

    while (strcmp(db->library.books[offset].title, title) != 0) {
        offset = (++key) % 997;
        if (key_in_table(offset, &db->library) == -1) {
            break;
        }
    }

    if (strcmp(db->library.books[offset].title, title) == 0) {
        printf("\nBook found...\n");
        return &db->library.books[offset];
    } else {
        printf("\nBook not found by exact search...\n");
        return NULL;
    }

}

/**
 * Fuzzy Search of Books by title, author or publisher. Returns an integer
 *  array containing the ids of the found books
 */
int *search_books(DB *db, char *search, int mode) {
    int i;
    int *list = (int *) calloc(1000, sizeof(int));
    char *pattern = to_upper(search);
    clean(pattern);
    switch (mode) {
        case 0:
            for (i = 0; i < db->library.book_count; i++) {
                struct Book book = db->library.books[db->library.keys[i]];
                char *matcher = to_upper(book.title);
                clean(matcher);
                if (strstr(matcher, pattern) != NULL) {
                    list[++list[0]] = book.id;
                }
                free(matcher);
            }
            free(pattern);
            return list;
        case 1:
            for (i = 0; i < db->library.book_count; i++) {
                struct Book book = db->library.books[db->library.keys[i]];
                char *matcher = to_upper(book.author);
                clean(matcher);
                if (strstr((matcher), pattern) != NULL) {
                    list[++list[0]] = book.id;
                }
                free(matcher);
            }
            free(pattern);
            return list;
        case 2:
            for (i = 0; i < db->library.book_count; i++) {
                struct Book book = db->library.books[db->library.keys[i]];
                char *matcher = to_upper(book.publisher);
                clean(matcher);
                if (strstr((matcher), pattern) != NULL) {
                    list[++list[0]] = book.id;
                }
                free(matcher);
            }
            free(pattern);
            return list;
        default:
            printf("Invalid Mode. Range 0~2\n");
            return NULL;
    }
}

/* Updates the stock of the book */
int update_book(DB *db, int id, int stock) {
    struct Book *book = find_by_id(db, id);
    if (book) {
        book->stock = stock;
        return save(db);
    }
    return 0;
}

#endif
