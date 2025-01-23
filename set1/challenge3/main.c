// Cryptopals Set 1 - Challenge 3
// Decrypt message XOR'd against a single character
/*
  Result after brute force:
  XOR encryption key: "X"
  Decrypted message: "Cooking MC's like a pound of bacon"
*/


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

int main(int argc, char *argv[]) {
  // Encrypted hex message (68 chars; 34 bytes)
  char *encrypted_message = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  size_t num_bytes = strlen(encrypted_message) / 2;
  unsigned char *encrypted_byte_arr = convertHexStringToByteArray(encrypted_message, num_bytes);
  if (!encrypted_byte_arr) {
    fprintf(stderr, "Failed to allocate memory!");
    return 1;
  }

  for (int i = 32; i < 256; i++) {
    int e_frequency_counter = 0;
    int t_frequency_counter = 0;
    int a_frequency_counter = 0;
    char decryption_test[34]; 

    for (size_t count = 0; count < num_bytes; count++) {
      int current_byte = (int) encrypted_byte_arr[count];
      int xor_result = current_byte ^ i;
      if (xor_result == 69 || xor_result == 101) {
        e_frequency_counter++;
      }

      if (xor_result == 84 || xor_result == 116) {
        t_frequency_counter++;
      }

      if (xor_result == 65 || xor_result == 97) {
        a_frequency_counter++;
      }

      decryption_test[count] = (char) xor_result;
    }

    float e_freq = ((float) e_frequency_counter / strlen(encrypted_message)) * 100;
    float t_freq = ((float) t_frequency_counter / strlen(encrypted_message)) * 100;
    float a_freq = ((float) a_frequency_counter / strlen(encrypted_message)) * 100;

    if (e_freq < 2 && a_freq < 2) {
      continue;
    }

    printf("For key: %c, e-freq: %.1fpct, a-freq: %.1fpct, decoded: %s \n", i, e_freq, a_freq, decryption_test);

  }

  free(encrypted_byte_arr);

  return 0;
}
