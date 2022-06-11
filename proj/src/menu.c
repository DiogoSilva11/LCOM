#include <lcom/lcf.h>

#include <stdint.h>

#include "menu.h"

#include "game.h"

// ----------------------------------------------------------------------------------------

int (main_menu)() {
    if (init()) 
        return 1;

    int process = 1;
    while (process) {
        if (displayMenu())
            return 1;
        
        int r, ipc_status;
        message msg;

        uint8_t key_code = 0;
        int key_loop = 1;
        while (key_loop) {
            /* Get a request message. */
            if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                printf("driver_receive failed with: %d", r);
                continue;
            }
            if (is_ipc_notify(ipc_status)) { // received notification
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE: // hardware interrupt notification
                        if (msg.m_notify.interrupts & kbd_irq_set) { // subscribed interrupt
                            kbc_ih();
                            if (!kbd_inc_code()) {
                                key_code = get_key();
                                key_loop = 0;
                            }
                        }
                        break;
                    default:
                        break; // no other notifications expected: do nothing
                }
            }
            else { // received a standard message, not a notification
                // no standard messages expected: do nothing
            }
        }

        switch (key_code) {
            case KEY_1:
                if (gameplay())
                    return 1;
                break;
            case KEY_0:
                process = 0;
                break;
            default:
                break;
        }
    }

    if (leave())
        return 1;

    return 0;
}

// ----------------------------------------------------------------------------------------

int (displayMenu)() {
    vg_clear_screen();

    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        for (int j = 0; j < CONSOLE_HEIGHT; j++) {
            if (vg_generate_pixel(i, j, 0x07)) {
                if (leave())
                    return 1;
                return 1;
            }
        }
    }

    vg_draw_content();

    return 0;
}

// ----------------------------------------------------------------------------------------
