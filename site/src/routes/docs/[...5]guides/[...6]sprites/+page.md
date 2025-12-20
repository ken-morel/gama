# Sprites

A sprite acts like an animated image, we can use
it to represent players or game objects.

## Designing a sprite image

Before inserting a sprite, you will need to manually design it,
with a tool like microsoft paint, gimp, etc.
In the end you should have an image like this:

![Phaser.js](https://cdn.phaser.io/tutorials/making-your-first-phaser-3-game/dude.png)
> example image from phaser.js tutorial

## loading the image

You can load the sprite image as you open a normal image:
See [image guid](/docs/guides/images).

```c
gmImage img = gm_image_open("assets/sprites/player.png");
```
if `assets/sprites/player.png` is the path to your player image
in assets folder.

## Creating the sprite

```c
gmSprite player = gm_sprite_create(img, 9);
```
[`gm_sprite_create`] creates a sprite using img and with the specified
number of slices(sub images).

You may also create a sprite from an image file directly with
<a href="/reference/sprite_8h.html#a7342953abee5e846d950d246c1c8607a">`gm_sprite_open`</a>

## animating the sprite

For the sprite to move and switch between it's images you are going
to create `gmSpriteAnim` objects,
See <a hRef="/Reference/structgm_sprite_anim.html">`gmspriteanim` reference</a>,

you can create an animation using
<a href="/reference/sprite_8h.html#a54b463efc50065a0c4569d2e5fbf93b7">`gm_sprite_anim_create`</a>.


## Example

```c
// put the path to the image sprite
gmSprite player = gm_sprite_open("assets/sprites/player.png", 9);

gmSpriteAnim
    player_walk_anim = gm_sprite_anim_create(0.2, "abcdcb"),
    player_jump_anim = gm_sprite_anim_create(0.2, "defgh");

player.animation = player_walk_anim;

gm_sprite_draw(&player, 0, 0, 0.1, 0.1);
```

## Reference

For more information on sprites, see <a href="/reference/sprite_8h.html">sprite.h reference</a>
