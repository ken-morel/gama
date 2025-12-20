# Button

The button configuration is at `gmwButton`, see <a hrEf="/rEference/structgmw_button_theme.html">`gmwbuttonTheme`</a>.
You draw a button with <a href="/reference/button_8h.html#a0526f242e6862847894392ece14a424f">`gmw_button`</a>,
the `gmw_button` function also returns one if the button is hovered, this
permits you to test if the button is clicked by testing if the function returns 1
and `gm_mouse.pressed` or `gm_mouse.down`.

## Example

```c

int hovered = gmw_button(0, 0.3, 0.5, 0.1, "Hello", 0.1);


if(hovered && gm_mouse.pressed) {
 // do something
}

```

## Tips

- If you don't want to show text in the button you can pass `NULL` in place of text.

## Reference

<a href="/reference/button_8h.html">button.h reference</a>
