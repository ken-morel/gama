# Creating and managing systems

See [system.h reference](/reference/system_8h.html)

If you want to add physics processing to your game, 
you can use gama built in physics.

## Creating a system

You create a system using [`gm_system_create`](/reference/system_8h.html#r_a12674760bc8132b8a9c64afe03ac7106).
```c
gmSystem world = gm_system_create();
```
This creates a new structure of type `gmSystem` which you can configure,
for properties, you can see [gm_system reference](/reference/structgm__system.html)

And for more information about systems, you can view the [system.h reference](/reference/system_8h.html).

## Creating bodies

A body is an entity in the system, it is like a physics body with it's own mass, shape
and forces acting on it.
Gama currently supports only two shapes of bodies, circles and rectangles, and
provide differenc functions for creating objects of those different shapes:
- [`gm_rectangle_body`](/reference/body_8h.html#a1cabf192f8e65f00161d2a9344884505)
- [`gm_circle_body`](/reference/body_8h.html#aa4fb765e44193dec7646c814ebd526c2)

[body.h reference](/reference/body_8h.html)

Bodies have properties you can modify to change their behavious
- `mass`: holds the mass of the body which is used in collisions,
  a body with a mass of zero is considered having infinite mass,
  and for example won't move during collisions but can still have
  a velocity.
- `is_active`: set this to zero and gama will completely ignore the body.
- `is_static`: marks the body as non-moving
- `position`, `velocity`, `acceleration`: they are all structs of type
  `gmPos`, having `.x` and `.y`.
- ...

You can view other properties in the [`gmBody` reference](/reference/structgm_body.html).

## Adding the bodies to the system

The last step is to add the bodies into the system, to do this, you are going
to pass a reference to you body object to the system.
you can use the `gm_system_push` and other `gm_system_pushN` functions where N
is the number of shapes you want to add less than six.

## Updating the system

You can update the system with `gm_system_update` function.

## Example

```c
#include <gama.h>

int main() {
  gmSystem world = gm_system_create();
  world.acceleration.y = -0.9; // add gravity
  world.damping = 0.1;         // add damping(... air resistance?)

  gmBody ground_body = gm_rectangle_body(0, 0, -1, 2, 0.1);
  ground_body.is_static = 1;     // ground never moves with system
  ground_body.restitution = 0.6; // somehow ellastic, maybe there is grass on it
  gm_system_push(&world, &ground_body);

  gmBody ball_body = gm_circle_body(1, 0, 0, 1); // unit circle of mass 1
  gmBody player_body = gm_rectangle_body(34, 0, 0, 0.3, 0.3);
  gm_system_push2(&world, &ball_body, &player_body);

  gmBody other_bodies[57] = ...;
  gm_system_push_array(&world, 57, other_bodies);

  do {
    gm_system_update(&world); // update the system

    gm_draw_rect_body(&ground_body, GM_GREEN);
    // .. draw other things
  } while (gm_yield());
  gm_system_destroy(&world);
  return 0;
}
```

## Getting collisions

When two bodies collide, gama creates a new [`gmCollision`](/reference/structgm_collision.html)
object which holds information about the collision,
you can get and test for a collision with [`gm_collision_detect`](/reference/physics_8h.html#a249b7c9b7732fd0cb0708994d9aec3f1)

```c
gmCollision myCollision;

if(gm_collision_detect(&myCollision,&mySystem, &bodyA, &bodyB) 
    // process the collision
 
 
// OR if you don't need the collision

if(gm_collision_detect(NULL, &mySystem, &bodyA, &bodyB))
    // process the collision
```

## References

- [physics.h reference](/reference/physics_8h.html)
- [collision.h reference](/reference/collision_8h.html)
- [system.h reference](/reference/system_8h.html)
- [body.h reference](/reference/body_8h.html)
