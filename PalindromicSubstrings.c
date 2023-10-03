#include <stdio.h>
#include <stdlib.h>

int compareString(char string[1000], int start, int end) {
    // Default value of 1 to represent the string being palindromic
    int result = 1;
    // This loop will end when the start and end point become next to each other
    while (start < end) {
        // If the string's current characters don't equal each other, then the string isn't palindromic and returns 0
        if (string[start] != string[end]) {
            result = 0;
            return result;
        }
        // When the current characters in the string equal each other, they will collapse down closer together and check again
        start++;
        end--;
    }
    // If the character pairs of the string equal each other all the way through, then it will return 1, counting as a palindromic substring
    return result;
}

int findSubstrings(char string[1000], int len) {
    // How many plaindromic substrings there are
    int num = 0;

    // 'i' is the starting location. It's where the later loop collapses onto
    for (int i = 0; i < len; i++) {
        // 'j' starts of as 'i' but gradually increases outwards to encapsulate more of the string
        for (int j = i; j < len; j++) {
            // Updates the 'num' variable by whether or not the substring is palindromic
            num += compareString(string, i, j);
        }
    }

    // Returns the final result
    return num;
}


int main (int argc, char **argv) {
    // Set max size of string
    char buffer[1000];
    // Determines the string length
    int strLength = 0;

    // Makes sure that a valid input is recieved
    if (argc < 2) {
        printf("Provide a valid string\n");
        return 0;
    }

    // For loop for copying the argument into the buffer and setting it as lowercase
    for (int i = 0; i < sizeof(buffer); i++) {
        // Checks for the null terminating character and sets one for 'buffer'
        if (argv[1][i] == '\0' || i == sizeof(buffer) - 1) {
            buffer[i] = '\0';
            break;
        } else {
            // Checks if the current character is uppercase
            if (argv[1][i] >= 'A' && argv[1][i] <= 'Z') {
                // Adding 32 to an uppercase character makes it lowercase, subtracting 32 does the reverse
                buffer[i] = argv[1][i] + 32;
            } else {
                // Copies the arguments character into the buffer
                buffer[i] = argv[1][i];
            }
        }
    }
    // Calculates the strings length
    while (buffer[strLength] != 0) {strLength++;};

    // Prints the final result
    printf("Number of Palindromic Substrings: %i\n",findSubstrings(buffer, strLength));

    return 0;
}