#pragma once

#include "gapi.h"

/**
 * @brief Checks if a key is currently pressed.
 * @param t The type of key event ('a' for arrow keys, 'm' for mouse events, 's' for special keys, 'c' for character keys).
 * @param k The specific key character or identifier.
 * @return 1 if the key is pressed, 0 otherwise.
 */
int gm_key_pressed(char t, char k) { return gapi_key_pressed(t, k); }

/**
 * @brief Converts a character to lowercase if it's an uppercase letter.
 * @param k The character to convert.
 * @return The lowercase version of the character, or the original if not uppercase.
 */
char gm_lower_case(char k) {
  int i = (int)k;
  return (i >= (int)'A' && i <= (int)'Z') ? (char)(i + (int)'a' - (int)'A') : k;
}

/**
 * @brief Converts a character to uppercase if it's a lowercase letter.
 * @param k The character to convert.
 * @return The uppercase version of the character, or the original if not lowercase.
 */
char gm_upper_case(char k) {
  int i = (int)k;
  return (i >= (int)'a' && i <= (int)'z') ? (char)(i + (int)'A' - (int)'a') : k;
}

/**
 * @brief Decodes a key shortcut into a type and key character.
 * @param key The shortcut key to decode.
 * @param t Pointer to store the decoded key type.
 * @param k Pointer to store the decoded key character.
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
 * @brief Encodes a type and key character into a shortcut key.
 * @param t The key type ('a' for arrow keys, 'm' for mouse events, 's' for special keys, 'c' for character keys).
 * @param k The key character.
 * @return The encoded shortcut key.
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

/**
 * @brief Checks if a key is currently pressed using a shortcut key.
 * @param key The shortcut key to check.
 * @return 1 if the key is pressed, 0 otherwise.
 */
int gm_key(char key) {
  char t, k;
  gm_decode_key_shortcut(key, &t, &k);
  return gm_key_pressed(t, k);
}
