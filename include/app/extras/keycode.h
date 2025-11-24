#ifndef KEYCODE_H
#define KEYCODE_H

// tipo de tecla clicada
typedef enum {
    KC_OTHER = 0,
    KC_UP,
    KC_DOWN,
    KC_LEFT,
    KC_RIGHT,
    KC_ENTER,
    KC_ESC,
    RESIZE_EVENT
} KeyCode;

#endif