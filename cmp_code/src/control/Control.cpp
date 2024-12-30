#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>

#include "../control/Control.h"

// Constructor to set the database
Control::Control(Database &db)
{
    // Initialize pointer to Database object
    d = &db;

    // Initialize pause_int value
    pause_int = 1;

    // Initialize currently playing object to nullptr
    currently_playing = nullptr;

    // Initialize the SDL2 audio object
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        // Print error message
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
    }

    // Check SDL mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
        {
            // Print error message
            cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
            // Close audio file
            Mix_CloseAudio();
            // Exit and free resources.
            SDL_Quit();
        }

}

// Control Destructor
Control::~Control(){
    // Close any audio that's playing
    Mix_CloseAudio();
    // Free SDL resources
    SDL_Quit();
}


// Play music
void Control::play(Track T)
{
    // Check if music is currently playing and stop it
    if (currently_playing != nullptr)
    {
        // Release resources
        Mix_FreeMusic(currently_playing);
        currently_playing = nullptr;
    }

    // Pull track location
    string file_name = T.GetTrackLocation().c_str();
    // const char* file_name = "/data/Music/divine_lollia.mp3";

    // Initialize music file
    Mix_Music* music = Mix_LoadMUS(file_name.c_str());
    // Check if opening file succeeded.
    if (!music)
    {
        // Print error message
        std::cerr << "Failed to load audio: " << Mix_GetError() << endl;
        // Close file
        Mix_CloseAudio();
        // Free resources
        SDL_Quit();
    }

    // Assign the music to the currently playing file
    currently_playing = music;
    // Play music once
    Mix_PlayMusic(music, 1);
}

// Stop track
void Control::stop(){
    // Stops music if it is playing
    
    // Ensure there is music to stop
    if (currently_playing == nullptr)
        return;

    // Stop the track and free it of resources
    Mix_FreeMusic(currently_playing);

    // Reset pointer
    currently_playing = nullptr;
}

// Toggle track pause
void Control::pause(){
    // Toggle pause tracker
    pause_int = pause_int == 0 ? 1 : 0;
    
    if (pause_int)
        Mix_ResumeMusic();
    else
        Mix_PauseMusic();
}