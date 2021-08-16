#include <stdbool.h>
#include "config.h"

// for debug
bool verbose = true;

// conneciton details
const config_t config = 
{
    .game = {
        .host = "0.0.0.0",
        .port = 7777,
        .window_width = 1024,
        .window_height = 768,
        .window_x = 888,
        .window_y = 0
    },

    .app = {
        .width = 800,
        .height = 500
    }
};

