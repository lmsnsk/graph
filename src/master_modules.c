#include "database.h"

void printing_modules(FILE *db, int id) {
    int count = get_records_count_in_file_mod(db);
    if (id) {
        if (id <= count) count = id;
    }
    for (int i = 0; i < count; i++) {
        int offset = i * sizeof(Modules);
        fseek(db, offset, SEEK_SET);
        Modules rec;
        fread(&rec, sizeof(Modules), 1, db);
        rewind(db);
        printf("%d %s %d %d %d\n", rec.id_module, rec.name, rec.memory_level_num, rec.cell_num, rec.flag_rem);
    }
}

void insert_modules(FILE *db) {
    int count = get_records_count_in_file_mod(db);
    Modules *str = (Modules *)malloc(255 * sizeof(Modules));
    char c1;

    if (scanf("%d %s %d %d %d%c", &str->id_module, str->name, &str->memory_level_num, &str->cell_num,
              &str->flag_rem, &c1) == 6 &&
        c1 == '\n') {
        write_record_in_file_mod(db, str, count);
    } else
        printf("Incorrect record format!\n");
    free(str);
}

int get_records_count_in_file_mod(FILE *pfile) { return get_file_size_in_bytes_mod(pfile) / sizeof(Modules); }

int get_file_size_in_bytes_mod(FILE *pfile) {
    int size = 0;
    fseek(pfile, 0, SEEK_END);
    size = ftell(pfile);
    rewind(pfile);
    return size;
}

void write_record_in_file_mod(FILE *pfile, const Modules *record_to_write, int index) {
    int offset = index * sizeof(Modules);
    fseek(pfile, offset, SEEK_SET);
    fwrite(record_to_write, sizeof(Modules), 1, pfile);
    fflush(pfile);
    rewind(pfile);
}

void update_modules(FILE *db) {
    Modules *str = (Modules *)malloc(255 * sizeof(Modules));
    char c1;
    int count = get_records_count_in_file_mod(db);
    printf("Insert new data: \n");
    if ((scanf("%d %s %d %d %d%c", &str->id_module, str->name, &str->memory_level_num, &str->cell_num,
               &str->flag_rem, &c1)) == 6 &&
        c1 == '\n') {
        if (str->id_module > count) {
            printf("The number of records is less than the entered ID!\n");
            free(str);
            return;
        }
        for (int i = 0; i < count; i++) {
            int offset = i * sizeof(Modules);
            fseek(db, offset, SEEK_SET);
            Modules rec;
            fread(&rec, sizeof(Modules), 1, db);
            rewind(db);
            if (rec.id_module == str->id_module) write_record_in_file_mod(db, str, i);
        }
    } else {
        printf("Incorrect record format!\n");
        getchar();
    }

    free(str);
}

void delete_modules(FILE *db) {
    int item;
    char c1;
    int count = get_records_count_in_file_mod(db);
    printf("Enter record ID: \n");
    if (scanf("%d%c", &item, &c1) == 2 && c1 == '\n') {
        if (item > count) {
            printf("The number of records is less than the entered ID!\n");
            return;
        }
        FILE *file_w;
        file_w = fopen("../materials/database.tmp", "wb");
        int n = 0;
        for (int i = 0; i < count; i++) {
            int offset = i * sizeof(Modules);
            fseek(db, offset, SEEK_SET);
            Modules rec;
            fread(&rec, sizeof(Modules), 1, db);
            rewind(db);
            if (rec.id_module != item) {
                write_record_in_file_mod(file_w, &rec, n);
                n++;
            }
        }
        fclose(file_w);
        fclose(db);

        FILE *file_r;
        file_r = fopen("../materials/master_modules.db", "wb");
        fclose(file_r);

        remove("../materials/master_modules.db");
        rename("../materials/database.tmp", "../materials/master_modules.db");

    } else {
        printf("Incorrect ID!\n");
        getchar();
    }
}

void get_active_modules() {
    FILE *db;
    db = fopen("../materials/master_modules.db", "r+b");
    int count = get_records_count_in_file_mod(db);
    int count1 = 0;

    for (int i = 0; i < count; i++) {
        int offset = i * sizeof(Modules);
        fseek(db, offset, SEEK_SET);
        Modules rec;
        fread(&rec, sizeof(Modules), 1, db);
        rewind(db);
        if (rec.flag_rem == 1) {
            printf("%d %s %d %d %d\n", rec.id_module, rec.name, rec.memory_level_num, rec.cell_num,
                   rec.flag_rem);
            count1++;
        }
    }
    if (count1 == 0) printf("No active additional modules...\n");
    fclose(db);
}

void delete_modules_by_ids() {
    FILE *db;
    db = fopen("../materials/master_modules.db", "r+b");
    char c1;
    int count = get_records_count_in_file_mod(db);
    int *nums = (int *)malloc(150 * sizeof(int));
    printf("Enter IDs:\n");
    int count2 = 0;
    for (int i = 0; i < 150; i++) {
        count2++;
        if (scanf("%d%c", &nums[i], &c1) != 2 || c1 == '\n') break;
    }

    FILE *file_w;
    file_w = fopen("../materials/database.tmp", "wb");
    for (int i = 0; i < count; i++) {
        int n = 0;
        int offset = i * sizeof(Modules);
        fseek(db, offset, SEEK_SET);
        Modules rec;
        fread(&rec, sizeof(Modules), 1, db);
        for (int j = 0; j < count2; j++) {
            if (rec.id_module == nums[j]) {
                n = 1;
            }
        }
        if (n == 0) write_record_in_file_mod(file_w, &rec, n);
        rewind(db);
    }
    fclose(file_w);
    fclose(db);

    FILE *file_r;
    file_r = fopen("../materials/master_modules.db", "wb");
    fclose(file_r);

    remove("../materials/master_modules.db");
    rename("../materials/database.tmp", "../materials/master_modules.db");

    free(nums);
}
