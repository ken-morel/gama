# Frame


A Frame widget can serve as a container for other widgets to place themselves in,
you can draw a frame with <a href="/reference/frame_8h.html#ac7eb9487dca540d1bfd9adce50dc636c">`gmw_frame`</a> function
it returns 1 if the cursor is currently on the frame(i.e the frame is hovered upon).


Frame configuration is located in `gmwFrame`
see <a hrEf="/Reference/structgmw_frame_theme.html">`gmwframetheme` reference</a>.

```c
gmwFrame.border = GM_RED;
int hovered = gm_frame(0, 0, 1, 1);
```

## Reference

<a href="/reference/frame_8h.html">frame.h reference</a>.
