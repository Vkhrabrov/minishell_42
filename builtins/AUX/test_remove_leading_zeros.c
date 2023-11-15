/*#include <stdio.h>

void removeLeadingZeros(char *str) {
    if (str == NULL) {
        return; // Handle NULL strings
    }

    char *nonZeroPtr = str;

    // Find the first non-zero character
    while (*nonZeroPtr == '0' && *(nonZeroPtr + 1) != '\0') {
        nonZeroPtr++;
    }
    // Shift the remaining characters to the beginning
    int i = 0;
    while (*nonZeroPtr != '\0') {
        str[i++] = *nonZeroPtr++;
    }
    str[i] = '\0'; // Null-terminate the modified string
}

int main() {
    char str1[] = "0000000015";
    char str2[] = "0000000000";

    printf("Before: %s\n", str1);
    removeLeadingZeros(str1);
    printf("After : %s\n", str1);

    printf("Before: %s\n", str2);
    removeLeadingZeros(str2);
    printf("After : %s\n", str2);

    return 0;
}*/
#include <stdio.h>

void removeLeadingZeros(char *str) {
    if (str == NULL) {
        return; // Handle NULL strings
    }

    char *nonZeroPtr = str;

    // Find the first non-zero character
    while (*nonZeroPtr == '0' && *(nonZeroPtr + 1) != '\0') {
        nonZeroPtr++;
    }

    // At this point, nonZeroPtr points to the first non-zero character
    //printf("String after removing leading zeros: %s\n", nonZeroPtr);
}

int main() {
    char str1[] = "0000000015";
    char str2[] = "0000000000";

    printf("Before: %s\n", str1);
    removeLeadingZeros(str1);
	printf("After: %s\n", str1);

    printf("Before: %s\n", str2);
    removeLeadingZeros(str2);
	printf("After: %s\n", str2);

    return 0;
}

