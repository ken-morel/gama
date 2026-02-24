#ifndef GM_KEY_H_INCLUDED
#define GM_KEY_H_INCLUDED
/**
 * @brief Converts a character to lowercase if it's an uppercase letter.
 * @param k The character to convert.
 * @return The lowercase version of the character, or the original if not
 * uppercase.
 */
char gm_lower_case(char k) {
  int i = (int)k;
  return (i >= (int)'A' && i <= (int)'Z') ? (char)(i + (int)'a' - (int)'A') : k;
}

/**
 * @brief Converts a character to uppercase if it's a lowercase letter.
 * @param k The character to convert.
 * @return The uppercase version of the character, or the original if not
 * lowercase.
 */
char gm_upper_case(char k) {
  int i = (int)k;
  return (i >= (int)'a' && i <= (int)'z') ? (char)(i + (int)'A' - (int)'a') : k;
}

/**
 * @brief Decodes a single character shortcut into a key type and key code.
 *
 * This function translates a compact shortcut character into its corresponding
 * event type and key, used by `gm_key_down`.
 * - 'U', 'D', 'L', 'R' -> type 'a' (arrow)
 * - 'E' -> type 's' (special), key 'x'
 * - 'S', 'C', 'A' -> type 'm' (modifier: Shift, Ctrl, Alt)
 * - Other characters -> type 'c' (character)
 *
 * @param key The single character shortcut (e.g., 'U' for Up Arrow).
 * @param t Pointer to store the decoded key type.
 * @param k Pointer to store the decoded key code.
 */
void gm_decode_key_shortcut(char key, char *t, char *k) {
  switch (key) {
  case '\0':
    *t = ' ';
    *k = ' ';
    break;
  case 'U':
  case 'D':
  case 'L':
  case 'R':
    *t = 'a';
    *k = gm_lower_case(key);
    break;
  case 'E':
    *t = 's';
    *k = 'x';
    break;
  case 'S':
  case 'C':
  case 'A':
    *t = 'm';
    *k = gm_lower_case(key);
    break;
  default:
    *t = 'c';
    *k = key;
  }
}

/**
 * @brief Encodes a key type and code into a single character shortcut.
 * This is the reverse of `gm_decode_key_shortcut`.
 * @param t The key type ('a' for arrow, 'm' for modifier, 's' for special, 'c' for character).
 * @param k The key code.
 * @return The encoded single character shortcut.
 */
char gm_encode_key_shortcut(char t, char k) {
  switch (t) {
  case ' ':
    return '\0';
  case 'a':
  case 'm':
    return gm_upper_case(k);
  case 's':
    switch (k) {
    case 'x':
      return 'E';
    }
  case 'c':
    return k;
  default:
    return '\0';
  }
}

#ifndef GM_NO_GAPI
#include "gapi.h"

/**
 * @brief Checks if a key is currently pressed.
 *
 * This is the low-level input checking function.
 *
 * @param t The type of key: 'c' for character, 'a' for arrow, 's' for special.
 * @param k The key code to check (e.g., 'w', 'u' for up arrow).
 * @return 1 if the key is pressed, 0 otherwise.
 */
int gm_key_down(char t, char k) { return gapi_key_down(t, k); }

/**
 * @brief Checks if a key is currently pressed using a single character shortcut.
 * @param key The shortcut character to check (e.g., 'w', 'U' for up).
 * @return 1 if the key is pressed, 0 otherwise.
 * @see gm_decode_key_shortcut
 * @see gm_key_down
 */
int gm_key(char key) {
  char t, k;
  gm_decode_key_shortcut(key, &t, &k);
  return gm_key_down(t, k);
}

#endif

#endif // GM_KEY_H_INCLUDED
