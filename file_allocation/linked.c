#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 32
#define DIRECTORY_SIZE 1

// Global array: Disk and directory info
char disk[DISK_SIZE];
struct directory_t *dir;

// File type
struct file_t {
    char name;
    int* link_arr;
    int length;
    char mode[4];
};

// Directory type
struct directory_t {
    int size;
    struct file_t *arr;
};

// Command type
enum command_t { READ, WRITE, REMOVE, CHMOD, LS, DF, UNKNOWN };

// Commnadline type
struct commandline_t {
    enum command_t command;
    int argc;
    char argv[2][10];
};

// Initializing the disk array with given txt file 
void InitializeDisk()
{
    FILE *fp = fopen("a.txt", "r");
    char character = '\0';
    for (int i = 0; i < DISK_SIZE; i++) {
        disk[i] = fgetc(fp);
    }
    fclose(fp);
}

// Initializing the directory with given info
struct directory_t *MakeDirectory() {
    struct directory_t *dir;
    dir = (struct directory_t*)malloc(sizeof(struct directory_t));
    dir->size = DIRECTORY_SIZE;
    dir->arr = (struct file_t *)malloc(sizeof(struct file_t) * dir->size);
    
    dir->arr[0].name = 'j';
    dir->arr[0].length = 5;
    dir->arr[0].link_arr = (int*)malloc(sizeof(int) * (dir->arr[0].length + 2));
    dir->arr[0].link_arr[0] = 9;
    dir->arr[0].link_arr[1] = 16;
    dir->arr[0].link_arr[2] = 1;
    dir->arr[0].link_arr[3] = 10;
    dir->arr[0].link_arr[4] = 25;
    dir->arr[0].link_arr[5] = -1;
    dir->arr[0].link_arr[6] = -1;
    dir->arr[0].link_arr[7] = -1;
    
    // copy '664' to array
    strncpy(dir->arr[0].mode, "664", 3);

    return dir;
}

// Print the commandline
void PrintCommandline(struct commandline_t *commandline)
{
    printf("%d\n",commandline->command);
    printf("%d\n",commandline->argc);
    printf("%s\n",commandline->argv[0]);
    printf("%s\n",commandline->argv[1]);
}

// Parsing the buffer
struct commandline_t ParseBuffer(char buf[])
{
    int i = 0;
    struct commandline_t commandline;
    char operation[10];
    sscanf(buf, "%s", operation);
    if (strcmp(operation, "read") == 0) {
        commandline.command = READ;
        commandline.argc = 1;
        sscanf(buf, "%*s %s", commandline.argv[0]);
    } else if (strcmp(operation, "write") == 0) {
        commandline.command = WRITE;
        commandline.argc = 2;
        sscanf(buf, "%*s %s %s", 
                commandline.argv[0], commandline.argv[1]);
    } else if (strcmp(operation, "remove") == 0) {
        commandline.command = REMOVE;
        commandline.argc = 1;
        sscanf(buf, "%*s %s", commandline.argv[0]);
    } else if (strcmp(operation, "chmod") == 0) {
        commandline.command = CHMOD;
        commandline.argc = 2;
        sscanf(buf, "%*s %s %s", 
                commandline.argv[0], commandline.argv[1]);
    } else if (strcmp(operation, "ls") == 0) {
        commandline.command = LS;
        commandline.argc = 0;
    } else if (strcmp(operation, "df") == 0) {
        commandline.command = DF;
        commandline.argc = 0;
    } else {
        commandline.command = UNKNOWN;
    }
    
    return commandline;
}

/* Read file mode
 * Get file name and read the data from disk */
void Read(struct commandline_t *commandline)
{
    char name = commandline->argv[0][0];
    
    if (dir->arr[0].link_arr != NULL) {
        for (int i = 0; i < dir->arr[0].length; i++) {
            printf("%c ", disk[dir->arr[0].link_arr[i]]);
        }
        printf("\n");
    } else {
        printf("READ ERROR: file %c doesn't exist\n", name);
    }
}

int IsAlphabet(char character)
{
    return character > 57;
}

/* Write file mode
 * Get file name and if the content is
 * - string: add data in file
 * - number: delete the values from the end */
void Write(struct commandline_t *commandline)
{
    char name = commandline->argv[0][0];
    for (int i = 0; i < dir->size; i++) {
        int length = dir->arr[i].length;
        if (name == dir->arr[i].name) {
            // if alphabet, then add the data
            if (IsAlphabet(commandline->argv[1][0])) {
                for (int j = 0; j < strlen(commandline->argv[1]); j++) {
                    // Assumed that there is only 'j' file in the directory
                    // Link another block
                    dir->arr[i].link_arr[length]
                        = dir->arr[i].link_arr[length - 1] + 1;
                    
                    // Write data in the new block
                    disk[dir->arr[i].link_arr[length]]
                        = commandline->argv[1][j];
                    
                    length++;

                }
                dir->arr[i].length += strlen(commandline->argv[1]);
            } else {
            // if number, then reduce the length    
                if (dir->arr[i].length > strlen(commandline->argv[1])) {
                    dir->arr[i].length -= atoi(commandline->argv[1]);
                } else {
                    printf("REMOVE ERROR: segmentation fault\n");
                }
            }
            break;
        }
    }
}

/* Remove file mode
 * Get file name and remove the file in directory */
void Remove(struct commandline_t *commandline)
{
    char name = commandline->argv[0][0];
    for (int i = 0; i < dir->size; i++) {
        if (name == dir->arr[i].name) {
            dir->arr[i].link_arr = NULL;
            dir->arr[i].length = -1;
            printf("%c removed successfully\n", name);
            break;
        }
    }
}

/* Change mode file mode
 * Get file name and authority
 * Change authority in file if needed */
void Chmod(struct commandline_t *commandline)
{
    char name = commandline->argv[1][0];
    for (int i = 0; i < dir->size; i++) {
        if (name == dir->arr[i].name) {
            strncpy(dir->arr[i].mode, commandline->argv[0], 3);
            printf("%c: %s mode changed successfully\n", name, dir->arr[i].mode);
            break;
        }
    }
}

/* ls file mode
 * Print all file info in the directory
 * [file_name], [authority], [size] */
void Ls(struct commandline_t *commandline)
{
    for (int i = 0; i < DIRECTORY_SIZE; i++) {
        if (dir->arr[i].link_arr != NULL) {
            printf("%c %s %d\n", 
                    dir->arr[i].name,
                    dir->arr[i].mode,
                    dir->arr[i].length);
        }
    }
}

/* df file mode
 * Print disk information
 * [Total size], [Occupied Size], [Available Size] */
void Df(struct commandline_t *commandline)
{
    int occupied = 0;
    for (int i = 0; i < DIRECTORY_SIZE; i++) {
        if (dir->arr[i].link_arr != NULL) {
            occupied += dir->arr[i].length;
        }
    }

    printf("total: %d occupied: %d available: %d\n",
            DISK_SIZE, occupied, DISK_SIZE - occupied);
}

int main(int argc, const char *argv[])
{
    InitializeDisk();
    dir = MakeDirectory();
 
    printf("**************************\n");
    printf("1. read 'file_name'\n2. write 'file_name' 'content'\n3. remove 'file_name'\n4. chmod 'authority' 'file_name'\n5. ls\n6. df\n");
    printf("**************************\n\n");
    
    char buf[128];
    struct commandline_t commandline;
    while (1) {
        printf("> ");
        fgets(buf, sizeof(buf), stdin);
        commandline = ParseBuffer(buf);
        switch (commandline.command) {
            case READ:
                Read(&commandline);
                break;
            case WRITE:
                Write(&commandline);
                break;
            case REMOVE:
                Remove(&commandline);
                break;
            case CHMOD:
                Chmod(&commandline);
                break;
            case LS:
                Ls(&commandline);
                break;
            case DF:
                Df(&commandline);
                break;
            default:
                printf("COMMAND ERROR: Unknown command\n");
        }
    }
    
    return 0;
}
