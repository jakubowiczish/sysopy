#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    DIR* directory;
    if (argc != 2) {
        printf("Call %s path", argv[0]);
        return 1;
    }
    struct dirent* position;

    /*
     * Otwórz katalog, w przypadku błędu otwarcia zwróć błąd funkcji
     otwierającej i wartość 1. Wyświetl zawartość katalogu katalog, pomijając
     "." i ".." Jeśli podczas tej operacji wartość errno zostanie ustawiona,
     zwróć błąd funkcji czytającej oraz wartość 2*/

    /* ADDED BY ME - FROM HERE */

    char* directory_path = argv[1];

    if ((directory = opendir(directory_path)) != NULL) {
        while ((position = readdir(directory)) != NULL) {
            if (strcmp(position->d_name, ".") != 0 &&
                strcmp(position->d_name, "..") != 0)
                printf("%s\n", position->d_name);
            if (errno != 0) {
                perror("READDIR");
                strerror(errno);
                return 2;
            }
        }
    } else {
        perror("Something went wrong");
        return 1;
    }

    /* TILL HERE */

    closedir(directory);
    return (0);
}