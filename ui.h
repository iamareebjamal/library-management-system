#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include "transact.h"

// Define clear based on OS
#if defined(_WIN32) || defined(_WIN64)
    #define clear() system("cls")
#else
    #define clear() printf("\033[H\033[J")
#endif

DB db;

DB *getDB() {
    return &db;
}

/* Clean the input stream from trailing newlines */
void flush() {
    int c; // Because getchar returns int, not char
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Display the error message with continue confiramtion */
int prompt(const char *str) {
    printf("%s", str);
    printf(" Press any key to continue...");
    getchar();
    return 0;
}

/* Secure input in a string with a check for empty strings */
void input(char *str, size_t size) {
    do {
        fgets(str, (int) size, stdin);
        str[strcspn(str, "\n")] = '\0';
    } while (strcmp(str, "") == 0 && !prompt("Please Retry."));
}

/* Password Verification and Updation */
void ask_pass() {
    char password[15];
    printf("\n\nPlease enter new password : ");
    input(password, 15);
    while (!set_password(&db, password)) {
        printf("\nPassword is of invalid format."
                       "\nPlease enter a valid password of length"
                       " between 8 and 15 characters with no spaces"
                       "\nYou can type \'^cancel\' to cancel the password modification.\n\n");
        input(password, 15);
        if (strcmp(password, "^cancel") == 0)
            return (void) prompt("\nPassword modification cancelled.");
    }

    prompt("\nPassword stored successfully.");
}

int match_pass() {
    char password[15];
    printf("\n\nPlease enter current password : ");
    input(password, 15);
    while (strcmp(db.password, password) != 0) {
        printf("\nPassword doesn't match."
                       "\nPlease enter a valid password set by the administrator."
                       " If you forgot your password, only option is to delete the"
                       " entire database which will cause the library and password"
                       " to reset to default value."
                       "\nYou can type \'^cancel\' to cancel the password authentication.\n\n");
        input(password, 15);
        if (strcmp(password, "^cancel") == 0)
            return prompt("\nPassword authentication failed.");
    }

    return !prompt("\nPassword matched successfully.");
}

void change_pass() {
    if (!match_pass())
        return;

    ask_pass();
}

/* Handle loading of the database from disk */
int construct() {
    if (load(&db) == 0)
        return prompt("Database load failed. Terminating.");

    // Ask password if it's the first run
    if (strcmp(db.password, "0000") == 0)
        ask_pass();

    return 1;
}

/* Faculty Number Input */
void get_fac_no(char fac_no[9]) {
    printf("\n\nPlease enter your faculty number : ");
    input(fac_no, 9);

    while (!verify_fn(fac_no)) {
        printf("\nFaculty Number is invalid."
                       "\nPlease enter a valid faculty number of length"
                       " 8 characters with no spaces\nYou can type "
                       "\'^cancel\' to cancel the issuing of book.\n\n");
        input(fac_no, 9);
        if (strcmp(fac_no, "^cancel") == 0)
            return;
    }

}

/* Print an arbitrary list of books */
void print_list(int *list) {
    int size = *list;
    int i;
    printf("\n%-33s%-20s%-19s%s\n\n", "Title", "Author", "Publisher", "Stock");
    for (i = 1; i <= size; i++) {
        printf("\n%d. ", i);
        struct Book *b = find_by_id(&db, list[i]);
        print_book(b);
    }
}

/* Verify if input is within bounds */
int valid_selection(int upper) {
    int select;
    scanf("%d", &select);
    flush();

    if (select < 1 || select > upper)
        return prompt("\nInvalid selection.");

    return select;
}

/* Book input front-end */
int add_to_stock() {
    struct Book b;
    char title[50], author[50], publisher[50];
    int count = 0;

    printf("Title of the book : ");
    input(title, 50);
    printf("Author : ");
    input(author, 50);
    printf("Publisher : ");
    input(publisher, 50);
    printf("Stock : ");
    scanf("%d", &count);
    flush();

    if (count <= 0)
        return prompt("Book Stock should be greater than 0.");

    strcpy(b.title, title);
    strcpy(b.author, author);
    strcpy(b.publisher, publisher);
    b.stock = count;

    if (add_book(&db, &b) != -1)
        return !prompt("Book entered in database.");

    return !prompt("");
}

/* Library Stock Management */
int manage_stock() {
    int select, stock;

    clear();
    print_books(&db);
    printf("\n\nPlease enter 0 for adding a book to stock and choose "
                   "any book otherwise for modification :\n-> ");
    select = getchar() - '0';
    flush();
    // If scanf() is used, any non-numeric input returns 0, which collides with
    // actual 0 input

    if (select == 0) {
        while (!add_to_stock());
        return 1;
    }

    if (select > db.library.book_count || select < 1)
        return prompt("Invalid Selection.");

    printf("\n\nBook to be modified is : \n");
    print_book(find_by_id(&db, db.library.keys[select - 1]));

    printf("\nEnter new stock : ");
    scanf("%d", &stock);
    flush();

    if (stock <= 0)
        return prompt("Invalid entry for stock!");

    update_book(&db, db.library.keys[select - 1], stock);

    return !prompt("Stock Updated.");
}

/* Book Issue front-end */
int issue() {
    int select;
    char query[50], fac_no[9];
    struct Book *book = NULL;

    clear();
    if (db.library.book_count == 0)
        return !prompt("Sorry. The library is currently empty.");

    printf("Please select criteria for searching the book :"
                   "\n1. Search by Name"
                   "\n2. Search by Author"
                   "\n3. Search by Publisher\n"
                   "\n-> ");

    select = valid_selection(3);
    if (!select) return 0;

    printf("\nPlease enter the search query : ");
    input(query, 50);

    /* Search for the book */
    if (select == 1) {
        printf("\nAttempting exact search...\n");
        book = find_book(&db, query);
    }

    if (book == NULL) {
        printf("\nAttempting fuzzy search...\n");
        int *list = search_books(&db, query, select - 1);

        if (*list == 0)
            return prompt("\nNo book found.");

        printf("\nList of book(s) :");
        print_list(list);

        printf("\n\nEnter Book number to issue : ");

        select = valid_selection(*list);
        if (!select) return 0;

        book = find_by_id(&db, list[select]);
        free(list);
    }

    printf("\nBook Selected...\n");

    if (!is_book_avail(book))
        return !prompt("\n\nSorry, Book is out of stock...");


    get_fac_no(fac_no);

    if (strcmp(fac_no, "^cancel") == 0)
        prompt("\n\nIssuing of book cancelled.");
    else
        issue_book(&db, book->id, fac_no);

    flush();
    return !prompt("\n");
}

/* Book return request front-end */
int request_return() {
    int select, i;
    char fac_no[9];

    clear();
    get_fac_no(fac_no);
    if (strcmp(fac_no, "^cancel") == 0)
        return !prompt("\nReturn request of book cancelled.");

    flush();
    int *list = get_issued_fac(&(db.manager), fac_no);
    if (*list == 0)
        return !prompt("\nThere are no books issued to this faculty number.");

    printf("\nBooks Issued to you are : \n");
    for (i = 1; i <= *list; i++) {
        printf("%d. ", i);
        print_transaction(&db, &(db.manager.issues[list[i]]));
    }
    printf("%d: Exit\n", i);

    printf("Select the book to return:\t");
    select = valid_selection(*list + 1);

    if (!select) return 0;
    else if (select == *list + 1) return !prompt("\nReturn request cancelled.");

    if (add_to_return(&db, &(db.manager.issues[list[select]]))) {
        printf("\nYour request for returning :\n");
        print_book(find_by_id(&db, db.manager.issues[list[select]].book_id));
        free(list);
        return !prompt("is successful.");
    }

    return !prompt("\n");
}

/* Book return approval front-end */
int approve_returns() {
    int i;

    clear();
    if (db.manager.return_count == 0)
        return !prompt("No requests for returns.");

    for (i = 0; i < db.manager.return_count; i++) {
        printf("%d. ", i + 1);
        print_transaction(&db, &(db.manager.returns[i]));
    }
    printf("%d: Exit\n", i + 1);

    printf("Select the book to return:\t");
    i = valid_selection(db.manager.return_count + 2);
    if (!i) return 0;
    else if (i == db.manager.return_count + 1) return !prompt("\nReturn request cancelled.");

    if (return_book(&db, &(db.manager.returns[i - 1])) == 1)
        return !prompt("\nSuccessfully returned.");
    return 1;
}

#endif
