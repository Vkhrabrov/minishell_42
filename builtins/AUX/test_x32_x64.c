#include <stdio.h>

int main() {
#ifdef __x86_64__
    printf("This is a 64-bit system.\n");
#elif defined(__i386__)
    printf("This is a 32-bit system.\n");
#else
    printf("Unable to determine the system's bitness.\n");
#endif

    return 0;
}

