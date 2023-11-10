#include <stdio.h>

int main() {
    int maxFileHandles = _getmaxstdio();

    if (maxFileHandles == -1) {
        perror("_getmaxstdio");
        return 1;
    }

    printf("Maximum number of open file handles: %d\n", maxFileHandles);

    return 0;
}
