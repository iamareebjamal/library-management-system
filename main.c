#include "ui.h"

void admin_panel();

int main() {
    int select;
    if (!construct())
        return !prompt("");

    do {
        clear();
        printf("Welcome to Library Management System\n\nMain Menu:\n"
                       "1. Issue a Book\n2. Return a Book\n"
                       "3. Admin Panel\n4. Exit\n"
                       "Please select your choice : ");
        scanf("%d", &select);
        flush();

        switch (select) {
            case 1:
                while (!issue());
                break;
            case 2:
                while (!request_return());
                break;
            case 3:
                admin_panel();
                break;
            case 4:
                break;
            default:
                prompt("Please enter correct choice.");
                break;
        }
    } while (select != 4);

    prompt("\nProgram exiting...\n");
    return 0;
}

void admin_panel() {
    int select;

    if (!match_pass())
        return;

    do {
        clear();
        printf("Admin Tools\n"
                       "1. See Issued Books\n2. Approve Returns\n"
                       "3. Manage Stock\n4. Change Password\n"
                       "5. Exit\nPlease select your choice : ");
        scanf("%d", &select);
        flush();

        switch (select) {
            case 1:
                printf("\n\nIssued Books : \n");
                print_issued_books(getDB());
                prompt("\n\n");
                break;
            case 2:
                while (!approve_returns());
                break;
            case 3:
                while (!manage_stock());
                break;
            case 4:
                change_pass();
                break;
            case 5:
                break;
            default:
                prompt("Wrong choice entered. Try again.");
                break;
        }
    } while (select != 5);

}
