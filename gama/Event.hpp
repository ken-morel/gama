#ifndef GAMA_EVENT_HPP_INCLUDED
#define GAMA_EVENT_HPP_INCLUDED
#include <windows.h>
enum class ArrowKey {LEFT=VK_LEFT, RIGHT=VK_RIGHT, UP=VK_UP, DOWN=VK_DOWN};
enum class Key {
    UP=VK_UP,
    DOWN=VK_DOWN,
    LEFT=VK_LEFT,
    RIGHT=VK_RIGHT
};
class KeyBoardEventHandler {
public:
    KeyBoardEventHandler() = default;
    virtual void on_keydown(Key key)
    {};
    virtual void on_arrowkeydown(ArrowKey key)
    {};

};
#endif
