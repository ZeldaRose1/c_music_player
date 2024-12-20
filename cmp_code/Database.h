#pragma once

// Standard library imports
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// Third party imports
#include <sqlite3.h>
// #include <taglib/tag_c.h>  // This is not needed the rest are imported from taglib/examples
#include "tpropertymap.h"
#include "tstringlist.h"
#include "tvariant.h"
#include "fileref.h"
#include "tag.h"

// Custom library imports
#include "Config.h"
#include "View.h"

using namespace std;


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
        void subscribe(View v) {view_list.push_back(v);}; // Add view to the list of views
        void unsubscribe(View); // Remove view from view_list
        void scanMusicFolder(string); // Scan through database
        void notify();
        void addTrack(string); // Add track at file location to database
};


Database::Database(){
    // Set configuration to the passed config
    conf = Config();
    
    // Initialize sqlite database
    // sqlite3* db;
    
    // Check if folder exists
    checkDirectory(conf.getDatabaseLocation());

    // Initialize variable to store error message
    int exit = 0;
    // Open database and save error flag to exit
    exit = sqlite3_open(conf.getDatabaseLocation().c_str(), &db);
    cout << "database location:\t" << conf.getDatabaseLocation() << endl;

    if (exit){
        cerr << "Error opening sqlite database:\n" << sqlite3_errmsg(db) << endl;
    }

    // Create tables if they do not exist
    createTables();

};

void Database::unsubscribe(View v)
{
    for (int i = 0; i < view_list.size(); i++)
    {
        // Check for equality in view vector
        if (view_list[i] == v)
        {
            // Pull all views one space earlier to overwrite space i
            for (int j = i + 1; j < view_list.size(); j++)
            {
                view_list[j - 1] = view_list[j];
            }
            
            // Delete the last element that is now duplicated
            view_list.pop_back();
            
            // Exist loop to ensure only first view match is deleted
            break;
        }
    }
};



void Database::checkDirectory(string path="working/database/cmp.db"){
    // Checks for existing directory
    // Create directory if no directory exists
    // Assumes database name is included in path and
    // db name is "cmp.db"
    string fol_path = path.substr(0, path.size() - 6);
    
    // Count number of subfolders in path
    int sf_count = 0;
    for (int i = 0; i < fol_path.size(); i++) {
        if (fol_path[i] == '/')
            sf_count++;
    }
    
    // String for storing temporary directory
    string cur_sf = "";
    // Loop over subfolders to check and create each
    int cur_sf_index = 0;
    for (int i = 0; i < sf_count; i++) {
        // Pull substring up to the folder marker
        cur_sf += fol_path.substr(cur_sf_index, fol_path.find("/") + 1);
        // Increase starting index to match first subfolder marker
        cur_sf_index = fol_path.find("/") + 1;

        // Check if current iteration folder exists
        if (!(std::filesystem::exists(cur_sf) && std::filesystem::is_directory(cur_sf)))
            // cout << "folder " << cur_sf << " already exists" << endl;
        // else 
        {
            // Create Folder
            std::filesystem::create_directory(cur_sf.c_str());
            cout << "folder " << cur_sf << " created" << endl;
        }
    }
};


Database::~Database(){
    // Teardown database
    sqlite3_close(db);
}


void Database::createTables(){
    // Create tracks, playlists, and tags tables if they do not already exist
    
    // Save output variable
    int exit = 0;
    // Save query to variable
    string sql_tasks = "CREATE TABLE IF NOT EXISTS tracks(\
        track_id INTEGER PRIMARY KEY AUTOINCREMENT,\
        track_no INTEGER,\
        title TEXT,\
        album_name TEXT,\
        artist_name TEXT,\
        release_year INT,\
        duration FLOAT,\
        cover_art TEXT,\
        track_location TEXT\
        );";
    // Save statement variable to save prepared stmt output
    sqlite3_stmt *stmt;
    // Prepare make table query
    exit = sqlite3_prepare_v2(db, sql_tasks.c_str(), -1, &stmt, nullptr);
    // Check for errors on preparing query
    if (exit != SQLITE_OK) {
        cout << "Error preparing tasks table query:\t" << sqlite3_errmsg(db) << endl;
        return;
    }

    // Reset error catching variable
    exit = 0;
    // Execute make table query
    exit = sqlite3_step(stmt);
    // Release resources from statement
    sqlite3_finalize(stmt);

    if (exit != SQLITE_DONE) {
        cout << "Error creating table:\t" << sqlite3_errmsg(db) << endl;
        return;
    }

    // Make playlists table
    exit = 0;
        // Save query to variable
        string sql_playlist = "CREATE TABLE IF NOT EXISTS playlists(\
            track_id INT,\
            name TEXT,\
            duration FLOAT,\
            track_count INT,\
            PRIMARY KEY (track_id, name)\
            FOREIGN KEY (track_id) REFERENCES tracks(track_id)\
        );";
        // Prepare make table query
        exit = sqlite3_prepare_v2(db, sql_playlist.c_str(), -1, &stmt, nullptr);
        // Check for errors on preparing query
        if (exit != SQLITE_OK) {
            cout << "Error preparing playlists table query:\t" << sqlite3_errmsg(db) << endl;
            return;
        }

        // Reset error catching variable
        exit = 0;
        // Execute make table query
        exit = sqlite3_step(stmt);
        // Release resources from statement
        sqlite3_finalize(stmt);

        if (exit != SQLITE_DONE) {
            cout << "Error creating playlists table:\t" << sqlite3_errmsg(db) << endl;
            return;
        }

    exit = 0;
    // Save query to variable
    string sql_tags = "CREATE TABLE IF NOT EXISTS tags(\
        track_id INTEGER,\
        tag TEXT,\
        PRIMARY KEY(track_id, tag)\
        FOREIGN KEY (track_id) REFERENCES tasks(task_id));";
    // Prepare make table query
    exit = sqlite3_prepare_v2(db, sql_tags.c_str(), -1, &stmt, nullptr);
    // Check for errors on preparing query
    if (exit != SQLITE_OK) {
        cout << "Error preparing tags table query:\t" << sqlite3_errmsg(db) << endl;
        return;
    }

    // Reset error catching variable
    exit = 0;
    // Execute make table query
    exit = sqlite3_step(stmt);
    // Release resources from statement
    sqlite3_finalize(stmt);

    if (exit != SQLITE_DONE) {
        cout << "Error creating tags table:\t" << sqlite3_errmsg(db) << endl;
        return;
    }
}

void Database::scanMusicFolder(string folder=""){
    // Scans music folder and prints details
    // If no folder is passed read default music directory
    if (folder == "")
    {
        folder = conf.getMusicLocation();
    }

    // Loop over all items in the directory
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::filesystem::path outfilename = entry.path();
        

        // Check if an entry is a directory or not
        if (std::filesystem::is_directory(outfilename))
        {
            // Recurse on directories
            Database::scanMusicFolder(outfilename);
        }
        else
        {
            // Print elements of a path
            std::cout << outfilename << std::endl;
            // Define a reference to the file
            TagLib::FileRef f(outfilename.string().c_str());
            
            // Check tags on the music file
            if (!f.isNull() && f.file())
            {
                // Assign a tag pointer to the current file.
                TagLib::Tag *tag = f.file()->tag();
                if (tag != nullptr)
                {
                    if (tag->isEmpty())
                        cout << "tag is empty" << endl;
                    else
                    {
                        cout << "tag is not empty" << endl;
                        cout << "Track:\t" << tag->title().toCString() << endl;
                        cout << "Album name:\t" << tag->album().toCString() << endl;
                        cout << "Artist:\t" << tag->artist().toCString() << endl;
                    }
                }
                
                // cout << "Track:\t" << tag->title().toCString() << endl;
            }
            else
            {
                cout << "File:\t" << outfilename <<"\t either f.isnull() or f.file() returns NULL" << endl;
            }
        }

    }
}


void Database::addTrack(string path)
{
    // Reads file at path and tries to add it into the database
    // Must pull all values from tag data and exit if the file
    // is not a music file.
    
    // Define parameters.
    int track_no;
    string title;
    string genre;
    string album_name;
    string artist_name;
    int release_year;
    // float duration;
    // string cover_art;

    // Define a reference to the file
    TagLib::FileRef f(path.c_str());
    
    // Check tags on the music file
    if (!f.isNull() && f.file())
    {
        // Assign a tag pointer to the current file.
        TagLib::Tag *tag = f.file()->tag();
        if (tag != nullptr)
        {
            if (tag->isEmpty()) {
                // TODO: Verify that this branch only happens for non-music files.
                // I think this branch may mean it IS a music file, but has no metadata.
                // This is not a music file. Exit function.
                title = path.substr(path.find_last_of('/') + 1);
                artist_name = "";
                album_name = "";
                genre = "";
                release_year = 0;
                track_no = 0;

                return;
            }
            else
            {
                // Pull parameters from tag
                title = tag->title().toCString();
                artist_name = tag->artist().toCString();
                album_name = tag->album().toCString();
                genre = tag->genre().toCString();
                release_year = tag->year();
                track_no = tag->track();
            }
        }
        
        // cout << "Track:\t" << tag->title().toCString() << endl;
    
        // Temp removed query code
                // track_no, \
                // duration, \
        // Write a string for the sqlite3 query
        string query = "INSERT INTO tracks "\
            "(\
                title, \
                album_name, \
                artist_name, \
                release_year, \
                cover_art, \
                track_location\
                ) \
            VALUES ( " + title + ", " + album_name + ", " + artist_name +\
                ", " + to_string(release_year) + ", " + "NULL, " + path + ");";
        
        // Initialize stmt variable and prepare query
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        // Save variable to check for errors
        int err = 0;

        // Execute statement and save output to err
        err = sqlite3_step(stmt);
        
        // Release resources from statement
        sqlite3_finalize(stmt);

        if (err != SQLITE_DONE) {
            cout << "Error adding task:\t" << path << "\n" << sqlite3_errmsg(db) << endl;
            return;
        }
    }
    else
    {
        cout << "File:\t" << path <<"\t either f.isnull() or f.file() returns NULL" << endl;
    }
};