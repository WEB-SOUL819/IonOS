#ifndef IONOS_KEYBOARD_H
#define IONOS_KEYBOARD_H

#include <stdint.h>

namespace OnScreenKeyboard {

    // Edits the provided buffer in-place.
    // Returns true if user pressed SELECT to accept, false if BACK to cancel.
    bool editString(char *buffer, uint8_t max_len);

}

#endif
