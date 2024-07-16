#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 
Convert a decimal number to a base64-encoded string.*
*
*/
char* decimal_to_base64(int num) {
    /* Define the characters used for base64 encoding. */
    char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char first, second;
    /* Allocate memory for the result string. */
    char* result = (char*)malloc(2 * sizeof(char) + 1);

    /* Initialize variables to hold the buffer, buffer length, and result index. */
    int buffer = 0;
    int buffer_length = 0;
    int result_index = 0;

    /* Fill the buffer with bits from the input number. */
    buffer |= num << buffer_length;
    buffer_length += 12;

    /* Convert the buffer to base64 characters. */
    while (buffer_length >= 6) {
        int index = buffer & 0x3F; /* Extract the lowest 6 bits. */
        result[result_index++] = base64_chars[index];
        buffer >>= 6; /* Shift the buffer to the right by 6 bits. */
        buffer_length -= 6;
    }

    /* If there are remaining bits in the buffer, convert them to a base64 character. */
    if (buffer_length > 0) {
        int index = buffer & 0x3F; /* Extract the remaining bits. */
        result[result_index++] = base64_chars[index];
    }

    /* Swap the positions of the first and second characters in the result. */
    first = result[0];
    second = result[1];
    result[0] = second;
    result[1] = first;

    /* Null-terminate the result string. */
    result[result_index] = '\0';

    return result;
}