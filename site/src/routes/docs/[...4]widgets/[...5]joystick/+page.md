# Joystick

The joystick widget is interactive controller that allows users to input directional movement, commonly used in games for character or camera control. The configuration is at `gmwJoystick`, see <a hrEf="/refErence/structgmw_joystick_theme.html">`gmwjoystickTheme`</a>.

You draw a joystick with <a href="/reference/joystick_8h.html#a4129148e0bdd6a651900a45d62ceed5e). the funcTion returns 1 when the joystick is active (being manipulated">`gmw_joystick`</a> and provides the X and Y position values of the joystick knob.

For animated joysticks with smooth transitions, you can use <a href="/reference/joystick_8h.html#ae8e0e95e6da39118def943187c819dfb">`gmw_joystick_anim`</a> which takes additional parameters for animation state.

## Example

```c
#include <gama.h>

double joy_x = 0, joy_y = 0; // Variables to store joystick position
double joy_x_anim, joy_y_anim; // Animation state variables

int main() {
    gm_init(500, 500, "Joystick Example");

    do {
        // Draw the joystick and get its position
        int active = gmw_joystick(0, 0, 0.3, &joy_x, &joy_y);

        // Or use the animated version for smoother transitions
        // int active = gmw_joystick_anim(0, 0, 0.3, &joy_x, &joy_y, &joy_x_anim, &joy_y_anim);

        // Use the joystick values
        if(active) {
            // Joystick is being held/used
            gm_draw_text(-0.9, 0.8, "Joystick Active!", "", 0.1, GM_WHITE);
        }

        // Display current values
        gm_draw_text(-0.9, 0.6, "X: %.2f", "", 0.08, GM_WHITE);
        gm_draw_text(-0.9, 0.5, "Y: %.2f", "", 0.08, GM_WHITE);

        // Visualize the animated joystick position (use joy_x_anim, joy_y_anim if using animated version)
        gm_draw_circle(joy_x * 0.5, joy_y * 0.5, 0.05, GM_YELLOW);

    } while(gm_yield());

    return 0;
}
```

## Configuration

The joystick appearance and behavior can be customized through the `gmwJoystick` global variable:

```c
// Change the default background color
gmwJoystick.background = GM_DARK_GRAY;

// Change the knob color
gmwJoystick.knob = GM_BLUE;

// Adjust scale
gmwJoystick.scale = 1.2;
```

## Tips

- The joystick position values range from -1 to 1 in both X and Y directions
- Use the return value to determine if the joystick is currently being manipulated
- The third parameter of `gmw_joystick` controls the size of the joystick area

## Reference

<a href="/reference/joystick_8h.html">joystick.h reference</a>