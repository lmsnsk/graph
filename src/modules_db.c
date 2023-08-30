#include <stdio.h>

#include "database.h"

int print_main_menu();
void table_choosing(int op);
void number_of_records(FILE *db, int kind_of_db, int op);

int main() {
    int first_item = print_main_menu();
    if (first_item < 5) table_choosing(first_item);
    if (first_item == 5) get_active_modules();
    if (first_item == 6) delete_modules_by_ids();
    if (first_item == 7) printf("Progress in work...\n");
    if (first_item == 8) printf("Progress in work...\n");
    if (first_item == 9) set_protaction_flag();

    return 0;
}

int print_main_menu() {
    printf("Please choose one operation:\n");
    printf("  1. SELECT\n  2. INSERT\n  3. UPDATE\n  4. DELETE\n");
    printf("  5. Get all active additional modules (last module status is 1)\n");
    printf("  6. Delete modules by ids\n");
    printf("  7. Set protected mode for module by id\n");
    printf("  8. Move module by id to specified memory level and cell\n");
    printf("  9. Set protection flag of the specified memory level\n");
    char c;
    int item;
    while (1) {
        if (scanf("%d%c", &item, &c) == 2 && c == '\n' && item <= 9 && item > 0) {
            break;
        } else {
            printf("Enter correct menu item (1-9)\n");
            getchar();
        }
    }
    return item;
}

void table_choosing(int op) {
    printf("Please choose a table:\n");
    printf("  1. Modules\n");
    printf("  2. Levels\n");
    printf("  3. Status events\n");
    FILE *pfile;
    char c;
    int item;
    while (1) {
        if (scanf("%d%c", &item, &c) == 2 && c == '\n' && item <= 3 && item > 0) {
            if (op == 1) printf("Insert the number of records or leave empty to output all of them: \n");
            if (op == 2) printf("Insert new data: \n");
            if (item == 1) {
                pfile = fopen("../materials/master_modules.db", "r+b");
                number_of_records(pfile, 1, op);
            } else if (item == 2) {
                pfile = fopen("../materials/master_levels.db", "r+b");
                number_of_records(pfile, 2, op);
            } else if (item == 3) {
                pfile = fopen("../materials/master_status_events.db", "r+b");
                number_of_records(pfile, 3, op);
            } else if (item == 4) {
                pfile = fopen("../materials/master_status_events.db", "r+b");
                number_of_records(pfile, 4, op);
            }
            break;
        } else {
            printf("Enter correct menu item (1-3)\n");
            getchar();
        }
    }
    fclose(pfile);
}

void number_of_records(FILE *db, int kind_of_db, int op) {
    if (op == 1) {
        int item;
        char c;
        if (scanf("%d%c", &item, &c) == 2 && c == '\n' && item >= 0) {
            select(db, item, kind_of_db);
        } else {
            printf("Enter correct record number\n");
            getchar();
        }
    }
    if (op == 2) insert(db, kind_of_db);
    if (op == 3) update(db, kind_of_db);
    if (op == 4) delete (db, kind_of_db);
}
