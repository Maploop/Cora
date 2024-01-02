#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "game_types.h"

extern b8 create_game(game* out_game);

/**
 * The main entry point
*/
int main(void) {
    game game_inst;
    if (!create_game(&game_inst)) {
        CFATAL("Could not create game!");
        return -1;
    }

    if (!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
        CFATAL("The game's functions are not set properly!");
        return -1;
    }

    // Initialization
    if (!application_create(&game_inst)) {
        CINFO("Application failed to create!");
        return 1;
    }

    // Begin the game loop
    if (!application_run()) {
        CINFO("The application did not shutdown gracefully!");
        return 2;
    }

    return 0;
}