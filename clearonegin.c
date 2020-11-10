#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <locale.h>
#include "clearonegin.h"

void set_ptrs(char* buf, line* ptrs, int size, int nn);
void printext(FILE *writehere, line* ptrs, int nn);
void print_original(FILE *writehere, line* ptrs, int nn);
void blankfix(line* str);
void insert_slash_nulls(char* buffer, char* newbuffer);
void sort_and_print(line* text, int nlines, FILE *output, const char* path);
int countlines(char* buffer, int size);
void ptr_clean(line* text, int nlines);
int text_sorter(const char* path_in, const char* path_out);

const int SMTH_BAD = 666;

int main() {
    printf("Starting text_sorter...\n");
    if (!text_sorter("onegin.txt", "onegin_out.txt"))
        printf("Sorted successfully.\n");
    else
        printf("Error. Unable to sort.\n");
}

int text_sorter(const char* path_in, const char* path_out) {
    char* encoding = setlocale (LC_ALL, "ru_RU.CP1251");
    printf("encoding = %s\n", encoding); 

    FILE *input = NULL, *output = NULL;
    struct stat info;
    int nlines = SMTH_BAD;

    if ((input = fopen(path_in, "r")) == NULL) {
        printf("No such file or directory.\n");
        return 1;
    }
    
    stat(path_in, &info);
    printf("info.st_size = %ld\n", info.st_size);

    char* buffer = (char*)calloc(info.st_size, sizeof(char));
    fread(buffer, info.st_size, sizeof(char), input);
    fclose(input);

    nlines = countlines(buffer, info.st_size);
    printf("nlines = %d\n", nlines);
    
    line* text = (line*)calloc(nlines, sizeof(line));
    ptr_clean(text, nlines);
   
    char* newbuffer = (char*)calloc(info.st_size + nlines, sizeof(char));
    insert_slash_nulls(buffer, newbuffer);
    free(buffer);

    set_ptrs(newbuffer, text, info.st_size, nlines);
    sort_and_print(text, nlines, output, path_out);

    free(newbuffer);
    free(text);
    return 0;
}

void set_ptrs(char* buf, line* ptrs, int size, int nn) {
    ptrs[0].start = buf;
    for (int i = 0, j = 1; i < size + nn - 1; ++i)
        if (buf[i] == '\0' && buf[i + 1] != '\0') {
            ptrs[j].start   = &buf[i + 1];
            ptrs[j - 1].end = &buf[i - 1];
            ++j;
        }
    if (buf[size + nn - 2] == '\0') ptrs[nn - 1].end = &buf[size + nn - 3];
    else                            ptrs[nn - 1].end = &buf[size + nn - 2];
}

void printext(FILE *writehere, line* ptrs, int nn) {
    for (int i = 0; i < nn; ++i)
        if (!noLetters(ptrs[i].start))
            fprintf(writehere, "%s", ptrs[i].start);
}

void print_original(FILE *writehere, line* ptrs, int nn) {
    for (int i = 0; i < nn; ++i)
        fprintf(writehere, "%s", ptrs[i].start);
}

void blankfix(line* str) {
    int shift = 0;
    for (shift = 0; shift < str->end - str->start; ++shift)
        if (str->start[shift] != ' ' && str->start[shift] != '\t')
            break;
    str->start = &str->start[shift];
}

void insert_slash_nulls(char* buffer, char* newbuffer) { 
    for (int old = 0, new = 0; buffer[old] != '\0'; ++old, ++new) {
        newbuffer[new] = buffer[old];
        if (buffer[old] == '\n') {
            ++new;
            newbuffer[new] = '\0';
        }
    }
}

void sort_and_print(line* text, int nlines, FILE *output, const char* path) {
    line* text2 = (line*)calloc(nlines, sizeof(line));  
    for (int i = 0; i < nlines; ++i)
        text2[i] = text[i];

    for (int i = 0; i < nlines; ++i)
        blankfix(&text[i]);

    output = fopen(path, "w");

    ney_qsort(text, 0, nlines - 1, my_strcmp, nlines);
    fprintf(output, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "                                                                  <<<ALPHABET SORTED>>>"
                    "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printext(output, text, nlines);

    qsort(text, nlines, sizeof(line), rhyme_strcmp);
    fprintf(output, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "                                                                    <<<RHYME SORTED>>>"
                    "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    
    printext(output, text, nlines);

    fprintf(output, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "                                                                      <<<ORIGINAL>>>"
                    "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    print_original(output, text2, nlines);

    fclose(output);
    free(text2);
}

int countlines(char* buffer, int size) {
    int nlines = 1;
    for (int i = 0; buffer[i] != '\0'; ++i)
        if (buffer[i] == '\n')
            if (i + 1 < size && buffer[i + 1] != '\0')
                ++nlines;
    return nlines;
}

void ptr_clean(line* text, int nlines) {
    for (int i = 0; i < nlines; ++i) {
        text[i].start = NULL;
        text[i].end   = NULL;
    }
}
