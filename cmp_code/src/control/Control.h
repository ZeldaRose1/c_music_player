#pragma once

// Standard library imports
#include <iostream>

// Third party imports
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>

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
        vector<Track> track_list; // Keep track of current track for autoplay
        int pause_int; // integer value to control pausing behavior
        int cur_track; // Maintain number of currently playing track
        // SDL_AudioDeviceID devID;
        static void EOSHook(); // End of song hook
    public:
        static Control* instance;
        Control(Database &); // Constructor that sets the Database pointer
        ~Control(); // Destructor that frees audio resources
        void setTrackList(vector<Track>); // Set track list
        
        void play(int); // Starts playing audio; start_index
        void stop(); // Stops playing audio
        void pause(); // Toggle pause / play on current track
        int nextTrack(); // Skip to next track in playlist
        int prevTrack(); // Return to previously played track
};
