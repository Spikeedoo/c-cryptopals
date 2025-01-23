// Cryptopals Set 1 - Challenge 5
// Implement repeating-key XOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* repeatingKeyXOR(char *input, char *key) {
  // Store size of input and key
  size_t in_out_bytes = strlen(input);
  size_t key_size = strlen(key);

  // Attempt memory allocation for output buffer
  unsigned char *output_string = malloc(in_out_bytes + 1);
  if (!output_string) {
    return NULL;
  }

  // Initialize output index
  size_t cur_output_index = 0;

  for (size_t count = 0; count < in_out_bytes; count++) {
    // Get the current input byte
    int selected_input_byte = (int) input[count];

    // Get the current key byte using modulus operator against key size
    int selected_key_byte = (int) key[count % key_size];

    // XOR the values
    int xor_result = selected_input_byte ^ selected_key_byte;

    // Convert resulting byte to hex characters
    char hex_characters[] = "0123456789abcdef";
    // Convert the high nibble (calculated using bit shift) to a hex character
    output_string[cur_output_index++] = hex_characters[xor_result >> 4];
    // Convert the low nibble (calculated by logical AND against 0x0F) to a hex character
    output_string[cur_output_index++] = hex_characters[xor_result & 0x0F];
  }

  // Add null terminator
  output_string[cur_output_index] = '\0';

  return output_string;
}

int main(int argc, char *argv[]) {
  // Do we need \n here?
  char *stanza = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  char *key = "ICE";

  unsigned char *result = repeatingKeyXOR(stanza, key);
  if (!result) {
    fprintf(stderr, "Unable to allocate memory for XOR output!");
    return 1;
  }

  printf("%s", result);

  return 0;
}
