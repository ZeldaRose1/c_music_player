#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../control/Control.h"

// Initialize static pointer to fix compile time error checking
Control *Control::instance = nullptr;

void (Control::*fncPoint)() = NULL;
// typedef void(Control::*fncPoint)(void);
// Constructor to set the database
Control::Control(Database &db)
{
    // Initialize pointer to Database object
    d = &db;

    // Initialize pause_int value
    pause_int = 1;

    // Initialize currently playing object to nullptr
    currently_playing = nullptr;

    // Initialize cur_track int
    cur_track = -1;

    // Set global pointer to this
    instance = this; // This still needs to be corrected in play()
        // Unsure why this correction is needed.

    // Initialize the SDL2 audio object
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        // Print error message
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
    }

    // Check SDL mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        // Print error message
        cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
        // Close audio file
        Mix_CloseAudio();
        // Exit and free resources.
        SDL_Quit();
    }

    // Set callback hook for autoplay
    // Mix_HookMusicFinished(sMusicFinishedCallback);
    // Mix_HookMusicFinished(Control::EOSHook);
    // cout << "hook set" << endl;
}

// Control Destructor
Control::~Control()
{
    // Close any audio that's playing
    Mix_CloseAudio();
    // Free SDL resources
    SDL_Quit();
    // Remove global pointer
    instance = nullptr;
}

void Control::setTrackList(vector<Track> T)
{
    // Sets control objects track_list
    // track_list = T.copy();
    // Clear track list
    if (track_list.size() > 0)
        track_list.clear();
    
    // Copy tracks from input to Control::track_list
    copy(T.begin(), T.end(), back_inserter(track_list));
    // reset cur_track index.
    cur_track = -1;
}

// Play music
void Control::play(int s_int)
{
    // Ensure the pointer for the hook matches the control object
    if (Control::instance != this)
    {
        // cout << "instance pointer does not match control object" << endl;
        Control::instance = this;
    }


    // for (int i = 0; i < track_list.size(); i++)
    // cout << track_list[i].GetTitle() << endl;
    
    // Check if music is currently playing and stop it
    if (currently_playing != nullptr)
    {
        // Release resources
        Mix_FreeMusic(currently_playing);
        currently_playing = nullptr;
    }

    // Set track_list pointer
    cur_track = s_int;
    // cout << "s_int in play() function is:  " << s_int << endl;
    // cout << "cur_track in play() function is:  " << cur_track << endl;
    // cout << "track_list in play() function has size() of " << track_list.size() << endl;
    if (cur_track >= track_list.size())
    {
        cur_track = -1;
        stop();
        cout << "Play early return bc cur_track >= track_list" << endl;
        return;
    }


    // Pull track location
    string file_name = track_list[cur_track].GetTrackLocation().c_str();

    // Initialize music file
    Mix_Music *music = Mix_LoadMUS(file_name.c_str());
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

    // Set play hook
    Mix_HookMusicFinished(EOSHook);

    // Assign the music to the currently playing file
    currently_playing = music;
    // Play music once
    Mix_PlayMusic(music, 1);
    
}

// Stop track
void Control::stop()
{
    // Stops music if it is playing

    // Ensure there is music to stop
    if (currently_playing == nullptr)
        return;

    // Stop the track and free it of resources
    Mix_FreeMusic(currently_playing);

    // Reset pointers
    currently_playing = nullptr;
}

// Toggle track pause
void Control::pause()
{
    // Toggle pause tracker
    pause_int = pause_int == 0 ? 1 : 0;

    if (pause_int)
        Mix_ResumeMusic();
    else
        Mix_PauseMusic();
}

// Skip to next track
int Control::nextTrack()
{
    // Check if there is track to continue to.
    if (cur_track < track_list.size())
        cur_track++;
    else
    {
        stop();
        return 0;
    }

    play(cur_track);
    // If the command was successful return 1
    return 1;
}

// Skip to next track
int Control::prevTrack()
{
    // Check if there is a previous track in the current playlist
    if (cur_track > 0)
        cur_track--;
    else
    {
        stop();
        return 0;
    }

    play(cur_track);
    return 1;
}

void Control::EOSHook()
{
    // Ensure there is no music playing
    if (Mix_PlayingMusic() == 0)
    {
        if (instance)
        {
            instance->nextTrack();
        }
    } else
        cerr << "Music should not be playing while hook is called" << endl;

    cout << Mix_GetError() << endl;
}