# Switch

The switch widget is a toggleable control that allows users to switch between two states (on/off). The configuration is at `gmwSwitch`, see [`gmwSwitchTheme`](/reference/structgmw_switch_theme.html).

You draw a switch with [`gmw_switch`](/reference/switch_8h.html#a6855e5d8c6b6e7abc6115d3e59d69e78). The function returns 1 when the switch is toggled to the "on" position and 0 when it's in the "off" position. Like other widgets, you can also use the animated version [`gmw_switch_anim`](/reference/switch_8h.html#a03f7c01171977b669306b5b94301167c) for smooth visual transitions.

## Example

```c
#include <gama.h>

int light_on = 0; // Variable to store switch state

int main() {
    gm_init(500, 500, "Switch Example");
    
    do {
        // Draw the switch and update its state
        light_on = gmw_switch(0, 0.3, 0.2, 0.1);
        
        // Draw based on switch state
        if(light_on) {
            gm_draw_text(-0.9, 0.8, "Light: ON", "", 0.1, GM_WHITE);
            gm_draw_circle(0, -0.2, 0.1, GM_YELLOW); // Light bulb on
        } else {
            gm_draw_text(-0.9, 0.8, "Light: OFF", "", 0.1, GM_GRAY);
            gm_draw_circle(0, -0.2, 0.1, GM_DARK_GRAY); // Light bulb off
        }
        
    } while(gm_yield());
    
    return 0;
}
```

## Animated Example

```c
#include <gama.h>

int power_on = 0; // Variable to store switch state
double power_on_anim = 0; // Animation state variable

int main() {
    gm_init(500, 500, "Animated Switch Example");
    
    do {
        // Draw the animated switch and update its state
        power_on = gmw_switch_anim(0, 0.3, 0.2, 0.1, &power_on_anim);
        
        // Draw based on animated switch state
        if(power_on_anim) {
            gm_draw_text(-0.9, 0.8, "Power: ON", "", 0.1, GM_WHITE);
            gm_draw_circle(0, -0.2, 0.1, GM_YELLOW); // Animated power indicator
        } else {
            gm_draw_text(-0.9, 0.8, "Power: OFF", "", 0.1, GM_GRAY);
            gm_draw_circle(0, -0.2, 0.1, GM_DARK_GRAY); // Animated power indicator
        }
        
    } while(gm_yield());
    
    return 0;
}
```

## Configuration

The switch appearance and behavior can be customized through the `gmwSwitch` global variable:

```c
// Change the switch colors when off
gmwSwitch.off.background = GM_RED;
gmwSwitch.off.border = GM_DARK_RED;

// Change the switch colors when on
gmwSwitch.on.background = GM_GREEN;
gmwSwitch.on.border = GM_DARK_GREEN;

// Adjust scale
gmwSwitch.scale = 1.2;
```

## Tips

- The switch returns its current state (1 for on, 0 for off)
- Use the animated version for smoother visual transitions
- The fourth parameter controls the size of the switch area
- The switch automatically toggles when clicked

## Reference

[switch.h reference](/reference/switch_8h.html)
