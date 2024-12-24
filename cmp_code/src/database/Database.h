#pragma once

// Standard library imports
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// Third party imports
#include <sqlite3.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstringlist.h>
#include <taglib/tvariant.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

// Custom library imports
#include "../config/Config.h"
#include "../view/View.h"

using namespace std;

// Forward declaration of View
class View;

class Track{
    private:
        int track_id; // db specific unique_id for tracks
        int track_no; // Track's place in album
        string title; // Track title
        string genre; // Genre associated with track
        string album_name; // Album, single, or EP track is part of
        string artist_name; // Artist who performs in track
        int release_year; // Year track was released
        float duration; // length of track
        string trackLocation; // Absolute path to file location
        string cover_art; // path to cover art.
};


class Playlist{
    private:
        vector<Track> track_list; // List of all tracks in playlist
        string name; // Name of playlist
        float duration; // Full playtime of playlist
        int track_count; // Number of tracks in the playlist
};


class Tags{
    private:
        int track_id;
        string tag;
};


class Database{
    private:
        sqlite3 *db;
        Config conf;
        vector<Playlist> playlist_list;
        vector<Track> track_list;
        vector<View> view_list;
        void checkDirectory(string); // Ensure database folder exists
        void createTables(); // Private method to create main tables
    public:
        Database(); // Forward declaration of constructor
        ~Database(); // Destructor
        // Database(string, Config); // Forward declaration of constructor
        void refresh_database(); // Refresh SQL database
        void subscribe(View); // Add view to the list of views
        void unsubscribe(View); // Remove view from view_list
        void scanMusicFolder(string); // Scan through database
        void notify();
        void addTrack(string); // Add track at file location to database
        sqlite3 * getDatabase();
        
        // Update database functions
        void updateTrackNo(int, int); // TODO; update UML with the update functions
        void updateTitle(int, string);
        void updateArtist(int, string);
        void updateAlbum(int, string);
        void updateReleaseYear(int, int);
        // void updateDuration(int, float); // TODO: Implement automated method
        void updateCoverArt(int, string);

        // bool checkTables(); // Verifies that the tables exist
};


