// Cryptopals Set 1 - Challenge 2
// XOR two equal length hex numbers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to convert hex string to byte arrays
unsigned char *convertHexStringToByteArray(char *hexString, size_t num_bytes) {
  // Allocate buffer for the byte array
  unsigned char *byte_arr = malloc(num_bytes);
  if (!byte_arr) {
    return NULL;
  }

  // Iterate through hex string and populate byte array
  char *pos = hexString;
  for (size_t count = 0; count < num_bytes; count++) {
    sscanf(pos, "%2hhx", &byte_arr[count]);
    // Move by 2 hex characters (1 byte)
    pos += 2 * sizeof(char);
  }

  return byte_arr;
}

void logMallocError(char* reason) {
  fprintf(stderr, "Failed to allocate memory for %s!\n", reason);
}

int main(int argc, char *argv[]) {
  // Validate that the user entered two input values
  if (argc != 3) {
    printf("Usage: ./main <hex1> <hex2>");
    return 1;
  }

  // Store input args
  char *hex1 = argv[1];
  char *hex2 = argv[2];

  // Validate that both hex values are equal in length
  if (strlen(hex1) != strlen(hex2)) {
    printf("Hex numbers must be the same size!");
    return 1;
  }

  // These size values will end up being for both input values as well as the output value
  size_t hex_len = strlen(hex1);
  size_t num_bytes = hex_len / 2;

  // Convert both hex strings into byte arrays
  unsigned char *byte_arr_1 = convertHexStringToByteArray(hex1, num_bytes);
  if (!byte_arr_1) {
    logMallocError("hex1");
    return 1;
  }

  unsigned char *byte_arr_2 = convertHexStringToByteArray(hex2, num_bytes);
  if (!byte_arr_2) {
    logMallocError("hex2");
    free(byte_arr_1);
    return 1;
  }
  
  // Allocate an equal length buffer for the output string
  unsigned char *output_string = malloc(num_bytes + 1); // Enough for output + null terminator
  if (!output_string) {
    logMallocError("output buffer");
    free(byte_arr_1);
    free(byte_arr_2);
    return 1;
  }

  // Initialize output index
  size_t cur_output_index = 0;

  // Loop through both input hex strings simultaneously and calculate XOR'd values
  for (size_t count = 0; count < num_bytes; count++) {
    // Store current byte from each hex string
    int byte1 = (int) byte_arr_1[count];
    int byte2 = (int) byte_arr_2[count];
    // XOR the bytes
    int xor_result = byte1 ^ byte2;

    // I originally wrote this using an intermediary buffer, but some learning using ChatGPT showed this trick:
    char hex_characters[] = "0123456789abcdef";
    // Convert the high nibble (calculated using bit shift) to a hex character
    output_string[cur_output_index++] = hex_characters[xor_result >> 4];
    // Convert the low nibble (calculated by logical AND against 0x0F) to a hex character
    output_string[cur_output_index++] = hex_characters[xor_result & 0x0F];
  }

  // Add null terminator
  output_string[cur_output_index] = '\0';

  // Display hex output
  printf("%s", output_string);

  // Free all allocated memory
  free(byte_arr_1);
  free(byte_arr_2);
  free(output_string);

  return 0;
}
