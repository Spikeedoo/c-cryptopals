// Cryptopals Set 1 - Challenge 1
// Convert Hex to Base 64
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char base64CharacterMap[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
  'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
  'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
  's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
  '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

int main(int argc, char *argv[]) {
  // Validate that user passed input argument
  if (argc != 2) {
    printf("Usage: ./main <hex>");
    return 1;
  }

  // Store the hex string from the input arg
  char *hex = argv[1];
  
  // Get the number of hex characters
  size_t hex_len = strlen(hex);

  // Make sure hex string has even num of chars
  if (hex_len % 2 != 0) {
    printf("Hex string length must be even!");
    return 1;
  }

  // Store total number of bytes in hex string
  size_t num_bytes = hex_len / 2;

  // Dynamically allocate byte array based on previous calculations
  unsigned char *byte_arr = malloc(num_bytes);
  if (!byte_arr) {
    perror("Failed to allocate memory for hex bytes.");
    return 1;
  }

  const char *pos = hex;
  // Parse the hex string into the byte array
  for (size_t count = 0; count < num_bytes; count++) {
    sscanf(pos, "%2hhx", &byte_arr[count]);
    // Move by 2 hex characters (1 byte)
    pos += 2 * sizeof(char);
  }

  size_t output_num_bytes = (size_t) ceil(4.0 * ((double) num_bytes / 3.0));
  size_t cur_output_index = 0;
  unsigned char *output_string = malloc(output_num_bytes + 2); // Allocate space for base 64 string + padding if needed

  if (!output_string) {
    perror("Failed to allocate memory for output string.");
    return 1;
  }

  /*
    The main procedure is to 
    1. Ingest 3 bytes (24 bits) of data
    2. Split up the bytes into 4 6-bit chunks
    3. Use the decimal number that those new 6-bit chunks represent to index into the base 64 character map
    4. Add that character to the result string
  */
  for (size_t count = 0; count < num_bytes; count += 3) {
    // Capture next three bytes if possible
    // Store the first byte
    char first_byte = byte_arr[count];
    // Use a bitmask to determine which bits are going to be moved to combine & make another 6-bit part
    char first_dropped_bits = first_byte & 0b00000011;
    // Drop the bits
    char first_part = (int) first_byte >> 2;

    if (((count + 3) - num_bytes) == 2) {
      // Edge case: only one byte left in the last triple
      // This means we need to expand the second output part & add two padding characters
      char expanded_part = first_dropped_bits << 4; // Pad with 4 zeros

      output_string[cur_output_index++] = base64CharacterMap[first_part];
      output_string[cur_output_index++] = base64CharacterMap[expanded_part];
      output_string[cur_output_index++] = '=';
      output_string[cur_output_index] = '=';

      break;
    }

    // Store the second byte
    char second_byte = byte_arr[count+1];
    // Use a bitmask to determine which bits are going to be moved to combine & make another 6-bit part
    char second_dropped_bits = second_byte & 0b00001111;
    // Drop the bits and use logical OR to add the bits dropped from the first byte
    char second_part = (int) (first_dropped_bits << 4) | (second_byte >> 4);

    if (((count + 3) - num_bytes) == 1) {
      // Edge case: only two bytes left in the last triple
      // This means we need to expand the third output part & add one padding character
      char expanded_part = second_dropped_bits << 2; // Pad with two zeros
      
      output_string[cur_output_index++] = base64CharacterMap[first_part];
      output_string[cur_output_index++] = base64CharacterMap[second_part];
      output_string[cur_output_index++] = base64CharacterMap[expanded_part];
      output_string[cur_output_index] = '=';

      break;
    }

    // Store the third byte
    char third_byte = byte_arr[count+2];
    // Use a bitmask to determine which bits are going to be moved to combine & make another 6-bit part
    char third_dropped_bits = third_byte & 0b00111111;

    // Drop the bits and use logical OR to add the bits dropped from the second byte
    char third_part = (int) (second_dropped_bits << 2) | (third_byte >> 6);

    // The fourth part is just the remaining dropped bits from the third byte
    char fourth_part = (int) third_dropped_bits; 

    // Add these characters to our base 64 string
    output_string[cur_output_index++] = base64CharacterMap[first_part];
    output_string[cur_output_index++] = base64CharacterMap[second_part];
    output_string[cur_output_index++] = base64CharacterMap[third_part];
    output_string[cur_output_index++] = base64CharacterMap[fourth_part];
  }

  // Display base64 output
  printf("%s", output_string);

  // Free the mallocs
  free(byte_arr);
  free(output_string);

  return 0;
}
