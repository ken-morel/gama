switch (key) {
// Arrow Keys
case GLFW_KEY_LEFT:
  event->key = KeyLeft;
  break;
case GLFW_KEY_RIGHT:
  event->key = KeyRight;
  break;
case GLFW_KEY_UP:
  event->key = KeyUp;
  break;
case GLFW_KEY_DOWN:
  event->key = KeyDown;
  break;

// Action Keys
case GLFW_KEY_ENTER:
  event->key = KeyEnter;
  break;
case GLFW_KEY_SPACE:
  event->key = KeySpace;
  break;
case GLFW_KEY_ESCAPE:
  event->key = KeyEscape;
  break;
case GLFW_KEY_TAB:
  event->key = KeyTab;
  break;
case GLFW_KEY_BACKSPACE:
  event->key = KeyBackspace;
  break;
case GLFW_KEY_DELETE:
  event->key = KeyDelete;
  break;
case GLFW_KEY_INSERT:
  event->key = KeyInsert;
  break;

// Navigation Keys
case GLFW_KEY_HOME:
  event->key = KeyHome;
  break;
case GLFW_KEY_END:
  event->key = KeyEnd;
  break;
case GLFW_KEY_PAGE_UP:
  event->key = KeyPageUp;
  break;
case GLFW_KEY_PAGE_DOWN:
  event->key = KeyPageDown;
  break;

// Modifier Keys. [1, 2]
case GLFW_KEY_LEFT_SHIFT:
  event->key = KeyLeftShift;
  break;
case GLFW_KEY_RIGHT_SHIFT:
  event->key = KeyRightShift;
  break;
case GLFW_KEY_LEFT_CONTROL:
  event->key = KeyLeftControl;
  break;
case GLFW_KEY_RIGHT_CONTROL:
  event->key = KeyRightControl;
  break;
case GLFW_KEY_LEFT_ALT:
  event->key = KeyLeftAlt;
  break;
case GLFW_KEY_RIGHT_ALT:
  event->key = KeyRightAlt;
  break;
case GLFW_KEY_LEFT_SUPER:
  event->key = KeyLeftSuper;
  break;
case GLFW_KEY_RIGHT_SUPER:
  event->key = KeyRightSuper;
  break;

// Lock Keys
case GLFW_KEY_CAPS_LOCK:
  event->key = KeyCapsLock;
  break;
case GLFW_KEY_NUM_LOCK:
  event->key = KeyNumLock;
  break;
case GLFW_KEY_SCROLL_LOCK:
  event->key = KeyScrollLock;
  break;

// Function Keys
case GLFW_KEY_F1:
  event->key = KeyF1;
  break;
case GLFW_KEY_F2:
  event->key = KeyF2;
  break;
case GLFW_KEY_F3:
  event->key = KeyF3;
  break;
case GLFW_KEY_F4:
  event->key = KeyF4;
  break;
case GLFW_KEY_F5:
  event->key = KeyF5;
  break;
case GLFW_KEY_F6:
  event->key = KeyF6;
  break;
case GLFW_KEY_F7:
  event->key = KeyF7;
  break;
case GLFW_KEY_F8:
  event->key = KeyF8;
  break;
case GLFW_KEY_F9:
  event->key = KeyF9;
  break;
case GLFW_KEY_F10:
  event->key = KeyF10;
  break;
case GLFW_KEY_F11:
  event->key = KeyF11;
  break;
case GLFW_KEY_F12:
  event->key = KeyF12;
  break;

// Alphabet Keys
case GLFW_KEY_A:
  event->key = KeyA;
  break;
case GLFW_KEY_B:
  event->key = KeyB;
  break;
case GLFW_KEY_C:
  event->key = KeyC;
  break;
case GLFW_KEY_D:
  event->key = KeyD;
  break;
case GLFW_KEY_E:
  event->key = KeyE;
  break;
case GLFW_KEY_F:
  event->key = KeyF;
  break;
case GLFW_KEY_G:
  event->key = KeyG;
  break;
case GLFW_KEY_H:
  event->key = KeyH;
  break;
case GLFW_KEY_I:
  event->key = KeyI;
  break;
case GLFW_KEY_J:
  event->key = KeyJ;
  break;
case GLFW_KEY_K:
  event->key = KeyK;
  break;
case GLFW_KEY_L:
  event->key = KeyL;
  break;
case GLFW_KEY_M:
  event->key = KeyM;
  break;
case GLFW_KEY_N:
  event->key = KeyN;
  break;
case GLFW_KEY_O:
  event->key = KeyO;
  break;
case GLFW_KEY_P:
  event->key = KeyP;
  break;
case GLFW_KEY_Q:
  event->key = KeyQ;
  break;
case GLFW_KEY_R:
  event->key = KeyR;
  break;
case GLFW_KEY_S:
  event->key = KeyS;
  break;
case GLFW_KEY_T:
  event->key = KeyT;
  break;
case GLFW_KEY_U:
  event->key = KeyU;
  break;
case GLFW_KEY_V:
  event->key = KeyV;
  break;
case GLFW_KEY_W:
  event->key = KeyW;
  break;
case GLFW_KEY_X:
  event->key = KeyX;
  break;
case GLFW_KEY_Y:
  event->key = KeyY;
  break;
case GLFW_KEY_Z:
  event->key = KeyZ;
  break;

// Number Keys
case GLFW_KEY_0:
  event->key = Key0;
  break;
case GLFW_KEY_1:
  event->key = Key1;
  break;
case GLFW_KEY_2:
  event->key = Key2;
  break;
case GLFW_KEY_3:
  event->key = Key3;
  break;
case GLFW_KEY_4:
  event->key = Key4;
  break;
case GLFW_KEY_5:
  event->key = Key5;
  break;
case GLFW_KEY_6:
  event->key = Key6;
  break;
case GLFW_KEY_7:
  event->key = Key7;
  break;
case GLFW_KEY_8:
  event->key = Key8;
  break;
case GLFW_KEY_9:
  event->key = Key9;
  break;

// Keypad Keys. [2]
case GLFW_KEY_KP_0:
  event->key = KeyPad0;
  break;
case GLFW_KEY_KP_1:
  event->key = KeyPad1;
  break;
case GLFW_KEY_KP_2:
  event->key = KeyPad2;
  break;
case GLFW_KEY_KP_3:
  event->key = KeyPad3;
  break;
case GLFW_KEY_KP_4:
  event->key = KeyPad4;
  break;
case GLFW_KEY_KP_5:
  event->key = KeyPad5;
  break;
case GLFW_KEY_KP_6:
  event->key = KeyPad6;
  break;
case GLFW_KEY_KP_7:
  event->key = KeyPad7;
  break;
case GLFW_KEY_KP_8:
  event->key = KeyPad8;
  break;
case GLFW_KEY_KP_9:
  event->key = KeyPad9;
  break;
case GLFW_KEY_KP_DECIMAL:
  event->key = KeyPadDecimal;
  break;
case GLFW_KEY_KP_DIVIDE:
  event->key = KeyPadDivide;
  break;
case GLFW_KEY_KP_MULTIPLY:
  event->key = KeyPadMultiply;
  break;
case GLFW_KEY_KP_SUBTRACT:
  event->key = KeyPadSubtract;
  break;
case GLFW_KEY_KP_ADD:
  event->key = KeyPadAdd;
  break;
case GLFW_KEY_KP_ENTER:
  event->key = KeyPadEnter;
  break;
case GLFW_KEY_KP_EQUAL:
  event->key = KeyPadEqual;
  break;

// Symbol Keys
case GLFW_KEY_GRAVE_ACCENT:
  event->key = KeyGraveAccent;
  break;
case GLFW_KEY_APOSTROPHE:
  event->key = KeyApostrophe;
  break;
case GLFW_KEY_COMMA:
  event->key = KeyComma;
  break;
case GLFW_KEY_MINUS:
  event->key = KeyMinus;
  break;
case GLFW_KEY_PERIOD:
  event->key = KeyPeriod;
  break;
case GLFW_KEY_SLASH:
  event->key = KeySlash;
  break;
case GLFW_KEY_SEMICOLON:
  event->key = KeySemicolon;
  break;
case GLFW_KEY_EQUAL:
  event->key = KeyEqual;
  break;
case GLFW_KEY_LEFT_BRACKET:
  event->key = KeyLeftBracket;
  break;
case GLFW_KEY_BACKSLASH:
  event->key = KeyBackslash;
  break;
case GLFW_KEY_RIGHT_BRACKET:
  event->key = KeyRightBracket;
  break;

default:
  free(event); // Free the allocated memory if the key is not handled
  return;
}
