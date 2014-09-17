/*
    INF1060 - Oblig 1 - Oppgave 1
    Encoding: ISO 8859-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


// Struct (node)
struct line {
    char* string;
    struct line* next;
};

struct line* first = NULL;
int lineCount = 0;
char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y', 'æ', 'ø', 'å'};

// Read file to linked list
int read_file(char *input_file) {
    // Open file
    FILE *f;

    f = fopen(input_file, "r");
    
    if (f == NULL) {
        printf("[ERROR] Could not open file for reading (errno %d: %s)!\n", errno, strerror(errno));
        return 0;
    }
    else {
        // Generate list
        struct line* current = first;

        char line_buffer[256];

        while (fgets(line_buffer, sizeof(line_buffer), f)) {
            // Increase line counter
            lineCount++;

            // Replace any newline with string terminator
            if (line_buffer[strlen(line_buffer) - 1] == '\n')           
                line_buffer[strlen(line_buffer) - 1] = '\0';

            // New line node
            struct line* newl = (struct line*) malloc(sizeof(struct line));
            newl->string = (char*) malloc (strlen(line_buffer) + 1);        // Allocate memory
            memcpy(newl->string, line_buffer, strlen(line_buffer) + 1);     // Copy string from buffer to node

            if (first == NULL)
                first = newl;
            else
                current->next = newl;

            // Set current to latest line
            current = newl;
        }

        // Make sure next of last is NULL
        current->next = NULL;

        // Close file
        fclose(f);

        return 1;
    }
}

// Free memory
void free_memory(void) {
    // Free memory
    struct line* current = first;
    struct line* next;

    while(current != NULL) {
        next = current->next;
        free(current->string);  // Free string data
        free(current);          // Free node
        current = next;         // Repeat until reaching tail
    }
}

// Check if character is vowel
int is_vowel(char c) {
    int i = 0;

    for (i = 0; i < sizeof(vowels); i++)
        if (vowels[i] == c)
            return 1;

    return 0;
}

// Remove char at index
void remove_char(char *src, int index) {
    int i;

    for (i = index; i < strlen(src); i++) {
        src[i] = src[(i + 1)];
    }
}

// [1] Print input file
void print_file(void) {
    printf("File contents:\n");

    // Loop list untill next of tail NULL
    struct line* fline = first;
    
    while(fline != NULL) {
        printf("\t%s\n", fline->string);
        fline = fline->next;
    }
}

// [2] Print random line
void print_random_line(void) {
    // Generate random number in range with time as seed, as per
    //  http://www.cplusplus.com/reference/cstdlib/rand/?kw=rand
    srand(time(NULL));
    int r = rand() % lineCount;

    printf("Random line (%d):\n", (r+1));
    
    int i = 0;
    
    struct line* fline = first;
    
    while(fline != NULL) {
        if (i == r)
            printf("\t%s\n", fline->string);
        
        fline = fline->next;
        i++;
    }
}

// [3] Replace vowels
void replace_vowels(void) {
    printf("Replace vowels ...\n");

    int i;

    // Loop through all vowels
    for (i = 0; i < sizeof(vowels); i++) {
        printf("\n... with vowel \'%c\'\n", vowels[i]);
        // Loop lines
        struct line* fline = first;
    
        while(fline != NULL) {
            int j;

            // Replace vowel
            for (j = 0; j < strlen(fline->string); j++)
                if (is_vowel(fline->string[j]))
                    fline->string[j] = vowels[i];

            // Print line
            printf("\t%s\n", fline->string);

            fline = fline->next;
        }
    }
}

// [4] Remove vowels
void remove_vowels(void) {
    printf("Remove vowels:\n");

    // Loop lines
    struct line* fline = first;

    while(fline != NULL) {
        int i;

        // Remove vowel
        for (i = 0; i < strlen(fline->string); i++)
            if (is_vowel(fline->string[i]))
                remove_char(fline->string, i);

        // Print line
        printf("\t%s\n", fline->string);

        fline = fline->next;
    }
}

// [5] Print number of characters
void print_character_count(void) {
    // Loop list and count each line
    int cc = 0;
    struct line* fline = first;
    
    while(fline != NULL) {
        cc += strlen(fline->string);

        fline = fline->next;
    }

    // Sans null chars, and depending on encoding of file
    printf("The text is %d characters long (sans control chars)\n", cc);
}

// Print usage
void print_usage(char *name) {
    printf("USAGE: %s <command> <input_file>\n\n", name);
    printf("where <command> is one of the following:\n\n");
    printf(" print\t\tprint input_file\n");
    printf(" random\t\tprint a random line\n");
    printf(" replace\treplace the vowels with all the other vowels\n");
    printf(" remove\t\tremove vowels\n");
    printf(" len\t\tprint the number of characters in the input_file\n");
}

// Main
int main(int argc, char *argv[]) {
    // Header
    printf("\n Oblig 1 - Oppgave 1\n");
    printf("============================================================\n\n");

    // Check argument count
    if (argc < 3) {
        // Usage
        print_usage(argv[0]);
    }
    else {
        // Options
        if (strcmp(argv[1], "print") == 0) {
            if(read_file(argv[2]))
                print_file();
        } 
        else if (strcmp(argv[1], "random") == 0) {
            if(read_file(argv[2]))
                print_random_line();
        }
        else if (strcmp(argv[1], "replace") == 0) {
            if(read_file(argv[2]))
                replace_vowels();
        }
        else if (strcmp(argv[1], "remove") == 0) {
            if(read_file(argv[2]))
                remove_vowels();
        }
        else if (strcmp(argv[1], "len") == 0) {
            if(read_file(argv[2]))
                print_character_count();
        }
        else {
            printf("[ERROR] Invalid command!\n\n");
            print_usage(argv[0]);
        }

        // Free allocated memory
        free_memory();
    }

    printf("\n");

    return 0;
}