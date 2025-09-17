switch (wParam) {
// Arrow Keys
case VK_LEFT:
  event.key = KeyLeft;
  break;
case VK_RIGHT:
  event.key = KeyRight;
  break;
case VK_UP:
  event.key = KeyUp;
  break;
case VK_DOWN:
  event.key = KeyDown;
  break;

// Action Keys
case VK_RETURN:
  event.key = KeyEnter;
  break;
case VK_SPACE:
  event.key = KeySpace;
  break;
case VK_ESCAPE:
  PostQuitMessage(0);
  break; // Special case to close
case VK_TAB:
  event.key = KeyTab;
  break;
case VK_BACK:
  event.key = KeyBackspace;
  break;
case VK_DELETE:
  event.key = KeyDelete;
  break;
case VK_INSERT:
  event.key = KeyInsert;
  break;

// Navigation Keys
case VK_HOME:
  event.key = KeyHome;
  break;
case VK_END:
  event.key = KeyEnd;
  break;
case VK_PRIOR:
  event.key = KeyPageUp;
  break;
case VK_NEXT:
  event.key = KeyPageDown;
  break;

// Modifier Keys
case VK_LSHIFT:
  event.key = KeyLeftShift;
  break;
case VK_RSHIFT:
  event.key = KeyRightShift;
  break;
case VK_LCONTROL:
  event.key = KeyLeftControl;
  break;
case VK_RCONTROL:
  event.key = KeyRightControl;
  break;
case VK_LMENU:
  event.key = KeyLeftAlt;
  break;
case VK_RMENU:
  event.key = KeyRightAlt;
  break;

// Lock Keys
case VK_CAPITAL:
  event.key = KeyCapsLock;
  break;
case VK_NUMLOCK:
  event.key = KeyNumLock;
  break;
case VK_SCROLL:
  event.key = KeyScrollLock;
  break;

// Function Keys
case VK_F1:
  event.key = KeyF1;
  break;
case VK_F2:
  event.key = KeyF2;
  break;
case VK_F3:
  event.key = KeyF3;
  break;
case VK_F4:
  event.key = KeyF4;
  break;
case VK_F5:
  event.key = KeyF5;
  break;
case VK_F6:
  event.key = KeyF6;
  break;
case VK_F7:
  event.key = KeyF7;
  break;
case VK_F8:
  event.key = KeyF8;
  break;
case VK_F9:
  event.key = KeyF9;
  break;
case VK_F10:
  event.key = KeyF10;
  break;
case VK_F11:
  event.key = KeyF11;
  break;
case VK_F12:
  event.key = KeyF12;
  break;

// Alphabet Keys (ASCII 'A' through 'Z')
case 'A':
  event.key = KeyA;
  break;
case 'B':
  event.key = KeyB;
  break;
case 'C':
  event.key = KeyC;
  break;
case 'D':
  event.key = KeyD;
  break;
case 'E':
  event.key = KeyE;
  break;
case 'F':
  event.key = KeyF;
  break;
case 'G':
  event.key = KeyG;
  break;
case 'H':
  event.key = KeyH;
  break;
case 'I':
  event.key = KeyI;
  break;
case 'J':
  event.key = KeyJ;
  break;
case 'K':
  event.key = KeyK;
  break;
case 'L':
  event.key = KeyL;
  break;
case 'M':
  event.key = KeyM;
  break;
case 'N':
  event.key = KeyN;
  break;
case 'O':
  event.key = KeyO;
  break;
case 'P':
  event.key = KeyP;
  break;
case 'Q':
  event.key = KeyQ;
  break;
case 'R':
  event.key = KeyR;
  break;
case 'S':
  event.key = KeyS;
  break;
case 'T':
  event.key = KeyT;
  break;
case 'U':
  event.key = KeyU;
  break;
case 'V':
  event.key = KeyV;
  break;
case 'W':
  event.key = KeyW;
  break;
case 'X':
  event.key = KeyX;
  break;
case 'Y':
  event.key = KeyY;
  break;
case 'Z':
  event.key = KeyZ;
  break;

// Number Keys (ASCII '0' through '9')
case '0':
  event.key = Key0;
  break;
case '1':
  event.key = Key1;
  break;
case '2':
  event.key = Key2;
  break;
case '3':
  event.key = Key3;
  break;
case '4':
  event.key = Key4;
  break;
case '5':
  event.key = Key5;
  break;
case '6':
  event.key = Key6;
  break;
case '7':
  event.key = Key7;
  break;
case '8':
  event.key = Key8;
  break;
case '9':
  event.key = Key9;
  break;

// Numpad Keys
case VK_NUMPAD0:
  event.key = KeyPad0;
  break;
case VK_NUMPAD1:
  event.key = KeyPad1;
  break;
case VK_NUMPAD2:
  event.key = KeyPad2;
  break;
case VK_NUMPAD3:
  event.key = KeyPad3;
  break;
case VK_NUMPAD4:
  event.key = KeyPad4;
  break;
case VK_NUMPAD5:
  event.key = KeyPad5;
  break;
case VK_NUMPAD6:
  event.key = KeyPad6;
  break;
case VK_NUMPAD7:
  event.key = KeyPad7;
  break;
case VK_NUMPAD8:
  event.key = KeyPad8;
  break;
case VK_NUMPAD9:
  event.key = KeyPad9;
  break;
case VK_DECIMAL:
  event.key = KeyPadDecimal;
  break;
case VK_DIVIDE:
  event.key = KeyPadDivide;
  break;
case VK_MULTIPLY:
  event.key = KeyPadMultiply;
  break;
case VK_SUBTRACT:
  event.key = KeyPadSubtract;
  break;
case VK_ADD:
  event.key = KeyPadAdd;
  break;

default:
  keyHandled = 0; // The key was not one we are handling
  break;
}
