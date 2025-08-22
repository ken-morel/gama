#include <gama.h>

// Load files, fonts, images related to your app
// while the loading screen shows before app starts
int load() { return 0; }

// Create shapes, and ther objects
// just after the app started
int init() { return 0; }

// update the shapes and objects
// theta is the time since last update
int update(float theta) { return 0; }

// render shape objects(draw them on the screen)
int render() { return 0; }

// run on app close, to free pointeers, 🤣 I'm joking
int shutdown() { return 0; }
