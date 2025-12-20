# Frame


A Frame widget can serve as a container for other widgets to place themselves in,
you can draw a frame with [`gmw_frame`](/reference/frame_8h.html#ac7eb9487dca540d1bfd9adce50dc636c) function
it returns 1 if the cursor is currently on the frame(i.e the frame is hovered upon).


Frame configuration is located in `gmwFrame`
see [`gmwFrameTheme` reference](/reference/structgmw_frame_theme.html).

```c
gmwFrame.border = GM_RED;
int hovered = gm_frame(0, 0, 1, 1);
```

## Reference

[frame.h reference](/reference/frame_8h.html).
