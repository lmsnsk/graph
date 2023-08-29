#include <stdio.h>
#include <stdlib.h>

#define N 255

typedef struct data {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int status;
    int code;
} Data;

Data read_record_from_file(FILE *pfile, int index);
int get_records_count_in_file(FILE *pfile);
int get_file_size_in_bytes(FILE *pfile);
void write_record_in_file(FILE *pfile, const Data *record_to_write, int index);
void bubble_sort(FILE *file, int records_count);
void swap_records_in_file(FILE *pfile, int record_index1, int record_index2);
void output(int count, FILE *file);

int main() {
    FILE *file;
    char *filename = (char *)malloc(N * sizeof(char));
    char symb;
    for (int i = 0; i < N; i++) {
        scanf("%c", &symb);
        if (symb == '\n') break;
        filename[i] = symb;
    }

    if ((file = fopen(filename, "rb")) == NULL) {
        printf("n/a\n");
    } else {
        char c;
        int year1, month1, day1, year2, month2, day2;
        if (scanf("%d.%d.%d %d.%d.%d%c", &day1, &month1, &year1, &day2, &month2, &year2, &c) == 7 &&
            c == '\n') {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            if (size != 0) {
                int records_count;

                records_count = get_records_count_in_file(file);

                fclose(file);

                FILE *file_sort;
                file_sort = fopen(filename, "r+b");
                fclose(file_sort);

                FILE *file_w;
                file_w = fopen("../datasets/door_state.tmp", "wb");

                FILE *file_s;
                file_s = fopen(filename, "rb");

                Data record;
                int not_found = 1;
                int counter = 0;
                for (int i = 0; i < records_count; i++) {
                    record = read_record_from_file(file_s, i);
                    if ((record.year <= year1 && record.month <= month1 && record.day <= day1) ||
                        (record.year >= year2 && record.month >= month2 && record.day >= day2)) {
                        not_found = 0;
                        write_record_in_file(file_w, &record, counter);
                        counter++;
                    }
                }
                fclose(file_w);
                fclose(file_s);

                FILE *file_read;
                file_read = fopen("../datasets/door_state.tmp", "r+b");
                bubble_sort(file_read, counter);
                if (not_found) {
                    printf("n/a");
                } else {
                    output(counter, file_read);
                }
                fclose(file_read);

                FILE *file_rem;
                file_rem = fopen(filename, "wb");
                fclose(file_rem);
                remove(filename);

                rename("../datasets/door_state.tmp", filename);

            } else
                printf("n/a");
        } else
            printf("n/a");
        fclose(file);
    }
    free(filename);
    return 0;
}

Data read_record_from_file(FILE *pfile, int index) {
    int offset = index * sizeof(Data);
    fseek(pfile, offset, SEEK_SET);
    Data record;
    fread(&record, sizeof(Data), 1, pfile);
    rewind(pfile);
    return record;
}

int get_records_count_in_file(FILE *pfile) { return get_file_size_in_bytes(pfile) / sizeof(Data); }

int get_file_size_in_bytes(FILE *pfile) {
    int size = 0;
    fseek(pfile, 0, SEEK_END);
    size = ftell(pfile);
    rewind(pfile);
    return size;
}

void write_record_in_file(FILE *pfile, const Data *record_to_write, int index) {
    int offset = index * sizeof(Data);
    fseek(pfile, offset, SEEK_SET);
    fwrite(record_to_write, sizeof(Data), 1, pfile);
    fflush(pfile);
    rewind(pfile);
}

void bubble_sort(FILE *file, int records_count) {
    for (int step = 0; step < records_count - 1; ++step) {
        for (int i = 0; i < records_count - step - 1; ++i) {
            Data el1 = read_record_from_file(file, i);
            Data el2 = read_record_from_file(file, i + 1);

            if (el1.year > el2.year || (el1.year == el2.year && el1.month > el2.month) ||
                (el1.year == el2.year && el1.month == el2.month && el1.day > el2.day) ||
                (el1.year == el2.year && el1.month == el2.month && el1.day == el2.day &&
                 el1.hour > el2.hour) ||
                (el1.year == el2.year && el1.month == el2.month && el1.day == el2.day &&
                 el1.hour == el2.hour && el1.minute > el2.minute) ||
                (el1.year == el2.year && el1.month == el2.month && el1.day == el2.day &&
                 el1.hour == el2.hour && el1.minute == el2.minute && el1.second > el2.second)) {
                swap_records_in_file(file, i, (i + 1));
            }
        }
    }
}

void swap_records_in_file(FILE *pfile, int record_index1, int record_index2) {
    Data record1 = read_record_from_file(pfile, record_index1);
    Data record2 = read_record_from_file(pfile, record_index2);
    write_record_in_file(pfile, &record1, record_index2);
    write_record_in_file(pfile, &record2, record_index1);
}

void output(int count, FILE *file) {
    for (int i = 0; i < count; i++) {
        Data arr = read_record_from_file(file, i);
        printf("%d %d %d %d %d %d %d %d\n", arr.year, arr.month, arr.day, arr.hour, arr.minute, arr.second,
               arr.status, arr.code);
    }
}
