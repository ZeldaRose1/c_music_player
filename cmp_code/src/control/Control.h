#pragma once

// Standard library imports
#include <iostream>

// Third party imports
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Zelda written code imports
#include "../database/Database.h"


using namespace std;

// Forward declaration of database classes
class Database;
class Track;

class Control{
    private:
        Database *d;
        // TODO: verify that track_list is not needed and remove it from UML diagram if not.
        // vector<Track> track_list; // I don't think I need this.
        bool shuffle; // Maybe rethink where this should be stored.
        Mix_Music* currently_playing; // Keep track of currently playing track
    public:
        Control(Database &); // Constructor that sets the Database pointer
        ~Control(); // Destructor that frees audio resources
        void play(Track); // Starts playing audio
        void stop(); // Stops playing audio
};
