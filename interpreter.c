#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 10000
#define DEBUG 0

int ARRAY[ARRAY_SIZE] = {0};
int MEMORY_PTR = 0;

char* read_bf(char* file)
{
    char valid_symbol[] = {'<', '>', '+', '-', ',', '.', '[', ']'};
    int length = 1;
    char* result = (char*)malloc(sizeof(char) * length);
    result[0] = '\0';
    char temp;

    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Not a valid file\n");
        return 0;
    }
    while ((temp = fgetc(fp)) != EOF) {
        for (int i = 0; i < 8; ++i) {
            if (temp == valid_symbol[i]) {
                length++;
                /** printf("%c\n", temp); */

                result = realloc(result, sizeof(char*) * length);
                strncat(result, &temp, 1);
            }
        }
    }
    /** printf("%s\n", result); */

    return result;
}

void dump_memory_file(char* file)
{
    char* filename;
    filename = (char*)malloc(sizeof(char) * (strlen(file) + 10));
    strcpy(filename, "");
    strcat(filename, file);
    strcat(filename, "_mem_dump");
    FILE* fp = fopen(filename, "w");
    fprintf(fp, "MEMORY_PTR: %d\n\n", MEMORY_PTR);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        fprintf(fp, "%d", ARRAY[i]);
        ((i + 1) % 30 == 0) ? fputs("\n", fp) : fputs(" ", fp);
    }
    fputs("\n", fp);
    fclose(fp);
    free(filename);
}

void dump_memory()
{
    FILE* fp = fopen("mem_dump", "w");
    fprintf(fp, "MEMORY_PTR: %d\n\n", MEMORY_PTR);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        fprintf(fp, "%d", ARRAY[i]);
        ((i + 1) % 30 == 0) ? fputs("\n", fp) : fputs(" ", fp);
    }
    fputs("\n", fp);
    fclose(fp);
}

void single_instr(char c)
{
    char ch;
#if DEBUG
    printf("Currently on MEMORY_PTR: %d\tExecuting Instruction: %c\n",
           MEMORY_PTR, c);

#endif
    switch (c) {
        case '>':
            MEMORY_PTR++;
            break;
        case '<':
            MEMORY_PTR--;
            break;
        case '+':
            ARRAY[MEMORY_PTR]++;
            break;
        case '-':
            ARRAY[MEMORY_PTR]--;
            break;
        case '.':
            putc(ARRAY[MEMORY_PTR], stdout);
            break;
        case ',':
            ch = getchar();
            ARRAY[MEMORY_PTR] = ch;
            break;
        default:
            printf("Instruction not found :%c\n", c);
            exit(0);
    }
    dump_memory();
}

int interpret_code(char* code)
{
    if (strlen(code) == 0) return 0;
#if DEBUG
    printf("Got code: %s\n", code);
#endif
    int length = 0;
    while (code[length] != '\0') {
        if (code[length] == '[') {
            // HANDLE WHILE LOOP
            int start = length + 1;
            int end = length + 1;
            int total_open = 1;
            while (total_open > 0 && code[end] != '\0') {
                if (code[end] == '[') {
                    total_open++;
                }
                if (code[end] == ']') {
                    total_open--;
                }
                end++;
            }
            if (total_open != 0) {
                printf("No matching ']' found\n");
                exit(0);
            }
            else if (end == start + 1) {
                length++;
            }
            else {
                char new_code[end - start];
                for (int i = 0; i < end - start - 1; ++i) {
                    new_code[i] = code[start + i];
                }
                new_code[end - start - 1] = '\0';
#if DEBUG
                printf("The new_code is: %s\n", new_code);
#endif
                while (ARRAY[MEMORY_PTR] > 0) {
                    interpret_code(new_code);
                }
                length += end - start;
            }
        }
        else {
            single_instr(code[length]);
        }
#if DEBUG
        printf("Currently on index: %d of code: %s\n", length, code);
#endif
        length++;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    /** for (int i = 0; i < ARRAY_SIZE; ++i) ARRAY[i] = 1; */
    if (argc != 2) {
        printf("Usage: ./interpreter file.bf\n");
        exit(0);
    }

    char* file = argv[1];
    int length = strlen(file);
    char* exten = (char*)malloc(sizeof(char) * 3);

    exten[0] = file[length - 2];
    exten[1] = file[length - 1];
    exten[2] = '\0';
    if (strcmp(exten, "bf") != 0) {
        printf("File must end in .bf\n");
        exit(0);
    }

    printf("\n");
    printf("\n");
    char* code = read_bf(file);
    interpret_code(code);

    printf("\n");
    dump_memory();
    free(code);
    free(exten);
    return 0;
}
