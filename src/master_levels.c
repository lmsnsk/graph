#include "database.h"

void printing_levels(FILE *db, int id) {
    int count = get_records_count_in_file_lev(db);
    if (id) {
        if (id <= count) count = id;
    }
    for (int i = 0; i < count; i++) {
        int offset = i * sizeof(Levels);
        fseek(db, offset, SEEK_SET);
        Levels rec;
        fread(&rec, sizeof(Levels), 1, db);
        rewind(db);
        printf("%d %d %d\n", rec.memory_level_num, rec.cells_count, rec.flag_def);
    }
}

void insert_levels(FILE *db) {
    int count = get_records_count_in_file_lev(db);
    Levels *str = (Levels *)malloc(255 * sizeof(Levels));
    char c1;

    if (scanf("%d %d %d%c", &str->memory_level_num, &str->cells_count, &str->flag_def, &c1) == 4 &&
        c1 == '\n') {
        write_record_in_file_lev(db, str, count);
    }
    free(str);
}

int get_records_count_in_file_lev(FILE *pfile) { return get_file_size_in_bytes_lev(pfile) / sizeof(Levels); }

int get_file_size_in_bytes_lev(FILE *pfile) {
    int size = 0;
    fseek(pfile, 0, SEEK_END);
    size = ftell(pfile);
    rewind(pfile);
    return size;
}

void write_record_in_file_lev(FILE *pfile, const Levels *record_to_write, int index) {
    int offset = index * sizeof(Levels);
    fseek(pfile, offset, SEEK_SET);
    fwrite(record_to_write, sizeof(Levels), 1, pfile);
    fflush(pfile);
    rewind(pfile);
}

void update_levels(FILE *db) {
    Levels *str = (Levels *)malloc(255 * sizeof(Levels));
    char c1;
    int count = get_records_count_in_file_lev(db);
    printf("Insert new data: \n");
    if (scanf("%d %d %d%c", &str->memory_level_num, &str->cells_count, &str->flag_def, &c1) == 4 &&
        c1 == '\n') {
        if (str->memory_level_num > count) {
            printf("The number of records is less than the entered ID!\n");
            free(str);
            return;
        }
        for (int i = 0; i < count; i++) {
            int offset = i * sizeof(Levels);
            fseek(db, offset, SEEK_SET);
            Levels rec;
            fread(&rec, sizeof(Levels), 1, db);
            rewind(db);
            if (rec.memory_level_num == str->memory_level_num) write_record_in_file_lev(db, str, i);
        }
    } else {
        printf("Incorrect record format!\n");
        getchar();
    }

    free(str);
}

void delete_levels(FILE *db) {
    int item;
    char c1;
    int count = get_records_count_in_file_lev(db);
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
            int offset = i * sizeof(Levels);
            fseek(db, offset, SEEK_SET);
            Levels rec;
            fread(&rec, sizeof(Levels), 1, db);
            rewind(db);
            if (rec.memory_level_num != item) {
                write_record_in_file_lev(file_w, &rec, n);
                n++;
            }
        }
        fclose(file_w);
        fclose(db);

        FILE *file_r;
        file_r = fopen("../materials/master_levels.db", "wb");
        fclose(file_r);

        remove("../materials/master_levels.db");
        rename("../materials/database.tmp", "../materials/master_levels.db");

    } else {
        printf("Incorrect ID!\n");
        getchar();
    }
}

void set_protaction_flag() {
    FILE *db;
    db = fopen("../materials/master_levels.db", "r+b");
    int count = get_records_count_in_file_lev(db);
    char c1;
    int mem_lev, prot_fl;
    printf("Enter memory level:\n");
    if (scanf("%d%c", &mem_lev, &c1) == 2 && c1 == '\n' && mem_lev < 5 && mem_lev > 0) {
        printf("Enter protection flag:\n");
        if (scanf("%d%c", &prot_fl, &c1) == 2 && c1 == '\n' && prot_fl < 2 && prot_fl > 0) {
            for (int i = 0; i < count; i++) {
                int offset = i * sizeof(Levels);
                fseek(db, offset, SEEK_SET);
                Levels rec;
                fread(&rec, sizeof(Levels), 1, db);
                rewind(db);
                if (rec.memory_level_num == mem_lev) {
                    rec.flag_def = prot_fl;
                    write_record_in_file_lev(db, &rec, i);
                }
            }
        } else
            printf("Incorrect protection flag!\n");
    } else
        printf("Incorrect memory level!\n");
    fclose(db);
}
