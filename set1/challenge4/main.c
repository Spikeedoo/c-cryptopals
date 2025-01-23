// Cryptopals Set 1 - Challenge 4
// Detect which string in file was single-character XOR'd
// Encrypted string: 7b5a4215415d544115415d5015455447414c155c46155f4058455c5b523f (Line 3214 of output.txt)
// XOR key: 5
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
  FILE *fp = fopen("input.txt", "r");
  FILE *of = fopen("output.txt", "w");
  char s[1024];

  while (fgets(s, sizeof s, fp) != NULL) {
    // printf("******%s*****\n", s);
    fprintf(of, "*****%s*****\n", s);
    size_t num_bytes = strlen(s) / 2;
    unsigned char *encrypted_byte_arr = convertHexStringToByteArray(s, num_bytes);
    if (!encrypted_byte_arr) {
      fprintf(stderr, "Failed to allocate memory!");
      return 1;
    }

    for (int i = 32; i < 128; i++) {
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

      float e_freq = ((float) e_frequency_counter / strlen(s)) * 100;
      float t_freq = ((float) t_frequency_counter / strlen(s)) * 100;
      float a_freq = ((float) a_frequency_counter / strlen(s)) * 100;

      if (e_freq == 0 || a_freq == 0) {
        continue;
      }

      // printf("For key: %c, e-freq: %.1fpct, a-freq: %.1fpct, decoded: %s \n", i, e_freq, a_freq, decryption_test);
      fprintf(of, "For key: %c, e-freq: %.1fpct, a-freq: %.1fpct, decoded: %s \n", i, e_freq, a_freq, decryption_test);
      // fprintf(of, "%s\n", decryption_test);
    }

    free(encrypted_byte_arr);
  }

  fclose(fp);
  fclose(of);
}
