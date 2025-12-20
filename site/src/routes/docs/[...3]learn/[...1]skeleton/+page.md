# The skeleton

Let's start with a simple gama application.

```c
#include <gama.h>

int main() {
  gm_init(700, 500, "Skeleton gama application");
  // 1. loading & creation & initialization
  do {
  
  } while(gm_yield());
}
```

## Initialization

The first part of your program lifecycle would be initializaiton,
initialization is when you do things like initial object creation,
startup checks, loading configurations or other things your app needs
to do before running.

The first part of your initialization will need to be calling `gm_init`,
this call initializes gama, creates the windows and initializes the
graphics system, and for that to happen you can specify a window
title and dimensions.

```c
int width = 700, height = 500;
const char title[] = "My gama application";

gm_init(width, height, title);
```

After and only after you did the initialization you can start using gama.

### Mainloop

Gama gameloop looks like this:

```c
do {

} while(gm_yield());
```

The gameloop usually runs a simple series of steps continously:
- getting and processing events.
- updating systems, bodies and other kind of calculations.
- drawing to the application.
- calling `gm_yield` to finalize everything and prepare for the next loop.

It is important to do drawing at the end if possible, this permits
gama to your applicaiton to update faster.

### Advanced


When building applications you will usually face two scenarios:

- **The event-driven applications**: These kind of applications draw all the
  window contents, and then don't do anything, until the user causes an
  event(with a click or keypress) or another event 
  where it responds to the event and redraws the application.
  As such the drawing is done only when necessary and it saves cpu power.

  This one is more suitable for normal applications.
- **Gameloop application**: or realtime applications,
  these applications continuously run in a loop, it is better for 
  applications which will continously update or receive events, this
  is what gama uses.

## A litle recap

```c
#include <gama.h>

int main() {
  gm_init(width, height, title);
  // define variables
  // create shapes
  // build systems
  do {
    // get and process events
    // update shapes and systems
 
    // draw widgets

    // draw shapes and things 
  
  } while(gm_yield());
  // destroy variables, shapes and systems
  // say bye
}
```

## Advanced

Your C code and the window/gama updating code run in two
different threads, your `c thread` where our c code runs and the
other thread we will call the `v thread`, those threads run in
parallell(at the same time) and without blocking each other, 
permitting your `c thread` to do calculation and updates 
at the same time the `v thread` is interacting with the operating 
system and processing events, saving time which would have been
wasted if they run in sequence. When you call `gm` functions for 
drawing or updating, you send a message to the `v thread` to tell 
it it should do something and you force your program to wait 
for the `v thread` has finished all what it had to do to start drawing
and thus wasting time, that's why you should do those only at the end.
