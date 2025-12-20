# Creating and using images

You handle images similarly as you handle other shapes, except that
to use an image you have to load it before hand.

## Loading an image

You can use [`gm_image_open`](/reference/image_8h.html#a46d598a9f3591a28a247599313157375),
and passing the path to the image you want to open, you are adviced to use
png images since I ensure that at least them should be supported everywhere.

the function returns a [`gmImage` object](/reference/structgm_image.html), 
which has properties like `height` and `width` which are the actual 
image's dimensions.

## drawing the image

easy, use [`gm_image_draw`](/reference/image_8h.html#aec56e25f8f3aab9a80ac99ea14751cdb), passing image dimensins,
if you want to draw only part of an image(that's what [sprites](/docs/guides/sprites) do)
you can use [`gm_image_draw_part`](/reference/image_8h.html#a93b48efa6484ce4d2b7e6bec2ab52949).

## Example

```c
gmImage person = gm_image_open("assets/images/person.png");
int width = person.width;
    height = person.height;
gm_image_draw(person, 0, 0, 1, 1);
```

## Reference

[image.h reference](/reference/image_8h.html)
