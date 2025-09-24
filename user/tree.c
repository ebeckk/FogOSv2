#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// This func is used to add the indentation prefix for each level of the directory structure
char* strcat(char *dest, const char *src) {
    char *rdest = dest; // Save the starting address of the destination string
    while (*dest)
        dest++; // Find the end of the destination string
    while ((*dest++ = *src++)); // Copy the source string to the end of the destination
    return rdest; // Return the ptr to the beginning of the modified string
}

void tree(char *path, char *prefix, int *dir_count, int *file_count) {
    char buf[512], *p;
    int fd;
    struct dirent de; // Entry in a dir
    struct stat st;   // Holds the metadata of a file

    if ((fd = open(path, O_RDONLY)) < 0) { // Checking if the file can be opened
        fprintf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) { // ~ if file metadata can be read
        fprintf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) { // ~ if the path is a dir
        fprintf(2, "tree: %s is not a directory\n", path);
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    // Ensure the path ends with a slash
    if (p > buf && *(p - 1) != '/') {
        *p++ = '/';
    }

	// This while loops through the directory, reading one entry at a time with the read system call.
	// Each entry is stored in a dirent structure, which contains the file/directory name and its inode number
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue; // Skip the current dir and the parent dir entries to avoid infinite loop

        strcpy(p, de.name); // Holds the name of the current item it has found, copies the item's name into the buffer

        if (stat(buf, &st) < 0) {
            printf("tree: cannot stat %s\n", buf); // Error checking for stat to see if there is a file system error
            continue;
        }
        
        printf("%s|-- %s\n", prefix, de.name);

        if (st.type == T_DIR) { 	// If the entry is a dir
            (*dir_count)++;			// increace the dir counter
            char new_prefix[512];
            strcpy(new_prefix, prefix);
            strcat(new_prefix, "|   "); // Call the strcat to add identation
            tree(buf, new_prefix, dir_count, file_count); // Creates a new, deeper prefix and calls itself for subdir
        } else {
            (*file_count)++;		// If the entry is a file, increase the file counter
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    int dir_count = 0;
    int file_count = 0; // These counters will be used to keep a running total of the dirs and files found
    char *start_path = "."; // This sets a default value. If the user doesn't provide any path, the program will use the current dir.

    if (argc > 1) { // Check the argument, if user provided a path, then use this instead of the default path
        start_path = argv[1];
    }
    
    printf("%s\n", start_path);
    
    // This if statement ensures that the initial directory you ask tree to list is included in the final summary,
    // unless it's the special cases of . or /
    if(strcmp(start_path, ".") != 0 && strcmp(start_path, "/") != 0) {
        struct stat st;
        if(stat(start_path, &st) == 0 && st.type == T_DIR) {
            dir_count++;
        }
    }

    tree(start_path, "", &dir_count, &file_count); // call to the tree func and passing the path, an empty prefix, and the counters

    printf("\n%d directories, %d files\n", dir_count, file_count); // prints the final counts for dir and files
    exit(0);
}
