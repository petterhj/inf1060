/*
    INF1060 - Oblig 1 - Oppgave 2
    Encoding: ISO 8859-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


unsigned char* buffer = NULL;   // Using character types as numbers
long fileSize = NULL;


// Read file
int read_file(char *file_name) {
    // Open file
    FILE *f;

    f = fopen(file_name, "r");
    
    if (f == NULL) {
        printf("[ERROR] Could not open file for reading (errno %d: %s)!\n", errno, strerror(errno));
        return 0;
    }
    else {
        // Get size of file
        fseek(f, 0, SEEK_END);
        fileSize = ftell(f);
        rewind(f);

        // Allocate memory
        buffer = malloc(fileSize);

        // Read file to buffer
        fread(buffer, fileSize, 1, f);
    
        // Close file
        fclose(f);

        return 1;
    }
}

// [1] Print input file
void print_file() {
    // Simply print buffer
    printf("%s", buffer);
}

// [2] Encode file
int encode_file(char *output_file) {
    // Open file for writing
    FILE *f;

    f = fopen(output_file, "wb");
    
    if (f == NULL) {
        printf("[ERROR] Could not open file for reading (errno %d: %s)!\n", errno, strerror(errno));
        return 0;
    }
    else {
        // Encode file
        printf(" Encoding to %s........ ", output_file);

        int i;
        char ch = 0;
        int bit = 0;

        // For each char in buffer
        for(i = 0; i < strlen(buffer); i++) {
            char hex = 0;
        
            if (buffer[i] == ' ')       // ' ' (blank)      = 00 = 0x0
                hex = 0x0;
            if (buffer[i] == ':')       // ':'              = 01 = 0x1
                hex = 0x1;
            if (buffer[i] == '@')       // '@'              = 10 = 0x2
                hex = 0x2;
            if (buffer[i] == '\n')      // '\n' (newline)   = 11 = 0x3
                hex = 0x3;
            
            // Bit shift left
            bit = i % 4 + 1;            // Every fourth

            if (bit == 1)
                ch |= hex << 6;
            if (bit == 2)
                ch |= hex << 4;
            if (bit == 3)
                ch |= hex << 2;
            if (bit == 4) {
                ch |= hex;

                putc(ch, f);            // Write "generated" char

                ch = 0;
            }
        }

        putc('\0', f);

        printf("Done!\n");
    }
}

// [3] Decode file
void decode_file() {
    char chars[] = {' ', ':', '@', '\n'};

    int i;
    int bit;

    for(i = 0; i < fileSize; i++) {
        // Count to four
        for(bit = 1; bit < 5; bit++) {
            if (bit == 1)
                printf("%c", chars[buffer[i] >> 6]);

            if (bit == 2)
                printf("%c", chars[(buffer[i] >> 4) & 3]);

            if (bit == 3)
                printf("%c", chars[(buffer[i] >> 2) & 3]);

            if (bit == 4)
                printf("%c", chars[buffer[i] & 3]);
        }
    }
}

// Print usage
void print_usage(char *name) {
    printf("USAGE: %s <command> <input_file> <output_file>\n\n", name);
    printf("where <command> is one of the following:\n\n");
    printf(" p\tprint input_file\n");
    printf("\t(output_file is ignored if specified)\n");
    printf(" e\tencode/compress input_file to output_file\n");
    printf(" d\tdecode/uncompress and print input_file\n");
    printf("\t(output_file is ignored if specified)\n");
}

// Main
int main(int argc, char *argv[]) {
    // Header
    printf("\n Oblig 1 - Oppgave 2\n");
    printf("============================================================\n\n");

    // Check argument count
    if (argc < 3) {
        // Usage
        print_usage(argv[0]);
    }
    else {
        // Print
        if (strcmp(argv[1], "p") == 0) {
            if(read_file(argv[2]))
                print_file();
        }
        // Encode/Compress
        else if (strcmp(argv[1], "e") == 0) {
            if (argc < 4) {
                printf("[ERROR] No output file specified!\n\n");
            }
            else {
                if(read_file(argv[2]))
                    encode_file(argv[3]);
            }
        }
        // Decode/Uncompress
        else if (strcmp(argv[1], "d") == 0) {
            if(read_file(argv[2]))
                decode_file();
        }
        else {
            printf("[ERROR] Invalid command!\n\n");
        }
    }

    // Free allocated buffer memory
    free(buffer);

    // Footer
    printf("\n\n");
}