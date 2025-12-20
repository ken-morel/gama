# Receiving and processing events

Every time you call `gm_yield`, gama collects all the events which
have happened and stores them in a way you can easily access them.

## Mouse events

Events happening to the mouse are stored in a global
`gm_mouse` structure,

```c
struct _gmMouse {
  gmPos position;
  gmPos movement;

  int32_t pressed;
  int32_t down;
};
struct _gmMouse gm_mouse;
```
As such:
- you can get the mouse last position with `gm_mouse.position.x`
  and `gm_mouse.position.y`.
- You can get the mouse change in position with `gm_mouse.movement`.
- `gm_mouse.down` reports if the mouse is currently down, if the user
  holds the mouse down, this will contiuously be `.
- `gm_mouse.pressed` reports like `gm_mouse.down`, except that this
  is reported only once even if the user holds the mouse down,
  you can use this to tests for clicks.

See [gm_mouse reference](/reference/gapi_8h.html#ae348e1e764210f492ce421ed6460db40)

### Little example

```c
#include <gama.h>

int main() {
    gm_init(500, 500, "test");
    
    do {
        // green if the mouse is pressed else blue
        gmColor color = gm_mouse.down ? GM_GREEN : GM_BLUE;
        double mouse_x = gm_mouse.position.x, // <- a comma
               mouse_y = gm_mouse.position.y; // <- a semicolon
        // draw the circle with radius 0.5 at mouse position
        gm_draw_circle(mouse_x, mouse_y, 0.5, color);
    } while(gm_yield());
}
```


## Keyboard events

Processing keyboard events is a bit more interesting, the fact is that 
there are plenty of keys on the keyboard, you have a few functions to
test if a key is currently being pressed, for convenience I will talk
only of that will mostly use.

```c
int gm_key(char keycode);
```
The keycode is usually the key itself in lowercase, e.g
`'a'` for key _A_, `' '` for key _space_, `'.'` for key dot. 
But for special keys you will use upper case letters:
- `'U'` for key up
- `'D'` for key down
- `'L'` for key left
- `'R'` for key right
- `'E'` for key escape
- `'S'` for shift key
- `'C'` for control key
- `'A'` for alt key
- `'\0'` for unsupported or invalid keys

### Little example

```c
int key_a_pressed = gm_key('a');
int key_alt_pressed = gm_key('A');

if(gm_key('U'))
    go_up();
else if(gm_key('D'))
    go_down();
```
