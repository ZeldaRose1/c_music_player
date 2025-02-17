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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Custom library imports
#include "../config/Config.h"
#include "../database/Database.h"
#include "../view/View.h"

using namespace std;

Track::Track(
    int t_id,
    int t_no,
    string title,
    string album_name,
    string artist_name,
    int r_year,
    float dur,
    string c_art,
    string t_loc,
    int tmp_index
) {
    // Constructor for the Track that autofills the attributes
    this->track_id = t_id;
    this->track_no = t_no;
    this->title = title;
    // this->genre = ;
    this->album_name = album_name;
    this->artist_name = artist_name;
    this->release_year = r_year;
    this->duration = dur;
    this->track_location = t_loc;
    this->cover_art = c_art;
    this->tmp_id = tmp_index;
};

// Track copy constructor
Track::Track(const Track &T){
    track_id = T.track_id;
    track_no = T.track_no;
    title = T.title;
    genre = T.genre;
    album_name = T.album_name;
    artist_name = T.artist_name;
    release_year = T.release_year;
    duration = T.duration;
    track_location = T.track_location;
    cover_art = T.cover_art;
    tmp_id = T.tmp_id;
};


// Define Track Getters
int Track::GetTrackId(){return track_id;};
int Track::GetTrackNo(){return track_no;};
string Track::GetTitle(){return title;};
// string Track::GetGenre(){return genre;};
string Track::GetAlbumName(){return album_name;};
string Track::GetArtistName(){return artist_name;};
int Track::GetReleaseYear(){return release_year;};
float Track::GetDuration(){return duration;};
string Track::GetTrackLocation(){return track_location;};
string Track::GetCoverArt(){return cover_art;};
int Track::GetTmpId(){return tmp_id;};

// Define Track setters
void Track::SetTrackId(int i) {track_id = i;};
void Track::SetTrackNo(int i) {track_no = i;};
void Track::SetTitle(string s) {title = s;};
void Track::GetGenre(string s) {genre = s;};
void Track::SetAlbumName(string s) {album_name = s;};
void Track::SetArtistName(string s) {artist_name = s;};
void Track::SetReleaseYear(int i) {release_year = i;};
void Track::SetDuration(float f) {duration = f;};
void Track::SetTrackLocation(string s) {track_location = s;};
void Track::SetCoverArt(string s) {cover_art = s;};
void Track::SetTmpId(int i) {tmp_id = i;};


// void Database::subscribe(View v) {view_list.push_back(v);}; // Add view to the list of views

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
    // cout << "database location:\t" << conf.getDatabaseLocation() << endl;

    if (exit){
        cerr << "Error opening sqlite database:\n" << sqlite3_errmsg(db) << endl;
    }

    // Create tables if they do not exist
    createTables();
};


// void Database::unsubscribe(View v)
// {
//     for (int i = 0; i < view_list.size(); i++)
//     {
//         // Check for equality in view vector
//         if (view_list[i] == v)
//         {
//             // Pull all views one space earlier to overwrite space i
//             for (int j = i + 1; j < view_list.size(); j++)
//             {
//                 view_list[j - 1] = view_list[j];
//             }
            
//             // Delete the last element that is now duplicated
//             view_list.pop_back();
            
//             // Exist loop to ensure only first view match is deleted
//             break;
//         }
//     }
// };


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
    string c_query;
    int row_count;

    // Check if table exists
    c_query = "SELECT COUNT(*) FROM tracks;";
    sqlite3_stmt *stmt;
    bool track_table = false;
    sqlite3_prepare_v2(db, c_query.c_str(), -1, &stmt, nullptr);
    while(sqlite3_step(stmt) == SQLITE_OK)
    {
        row_count = sqlite3_column_int(stmt, 0);
        if (row_count >= 0)
        {
            cout << "Tracks has " << row_count << " rows" << endl;
            track_table = true;
        }
    }
    sqlite3_finalize(stmt);

    if (!track_table)
    {
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
            track_location TEXT UNIQUE\
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
    }

    // Check if playlists table exists
    c_query = "SELECT COUNT(*) FROM playlists;";
    bool playlists_table = false;
    sqlite3_prepare_v2(db, c_query.c_str(), -1, &stmt, nullptr);
    while(sqlite3_step(stmt) == SQLITE_OK)
    {
        row_count = sqlite3_column_int(stmt, 0);
        if (row_count >= 0)
        {
            cout << "Playlists has " << row_count << " rows" << endl;
            playlists_table = true;
        }
    }
    sqlite3_finalize(stmt);
    
    if (!playlists_table)
    {
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
    }


    // Check if tags table exists
    c_query = "SELECT COUNT(*) FROM tags;";
    bool tags_table = false;
    sqlite3_prepare_v2(db, c_query.c_str(), -1, &stmt, nullptr);
    while(sqlite3_step(stmt) == SQLITE_OK)
    {
        row_count = sqlite3_column_int(stmt, 0);
        if (row_count >= 0)
        {
            cout << "Tags has " << row_count << " rows" << endl;
            tags_table = true;
        }
    }
    sqlite3_finalize(stmt);
    
    if (!tags_table)
    {
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
        std::filesystem::path outfilename = filesystem::canonical(entry.path());
        

        // Check if an entry is a directory or not
        if (std::filesystem::is_directory(outfilename))
        {
            // Recurse on directories
            Database::scanMusicFolder(outfilename);
        }
        else
        {
            // Print elements of a path

            // Define a reference to the file
            TagLib::FileRef f(outfilename.string().c_str());
            
            // Check tags on the music file
            if (!f.isNull() && f.file())
            {
                // Assign a tag pointer to the current file.
                addTrack(outfilename);
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
                title = path.substr(path.find_last_of('/') + 1, path.substr(path.find_last_of('/')).find_last_of('.') - 1);
                artist_name = "";
                album_name = "";
                genre = "";
                release_year = 0;
                track_no = 0;
                // cout << "Path:\t" << path << "\nis empty, but title reads as:\t" << title << endl;
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
                track_location \
                ) \
            VALUES ( '" + title + "', '" + album_name + "', '" + artist_name +\
                "', " + to_string(release_year) + ", " + "'', '" + path + "');";
        
        // Save variable to check for errors
        int err = 0;
        // cout << "Query to insert track:\n" << query << endl;

        // Initialize stmt variable and prepare query
        sqlite3_stmt *stmt;
        err = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        // Check for errors in preparing SQL statement
        if (err != SQLITE_OK)
        {
            // cout << "Error parsing track insert query\n" << sqlite3_errmsg(db) << endl;
        }

        // Execute statement and save output to err
        err = sqlite3_step(stmt);
        
        // Release resources from statement
        sqlite3_finalize(stmt);

        if (err != SQLITE_DONE) {
            // cout << "Error adding task:\t" << path << "\n" << sqlite3_errmsg(db) << endl;
            return;
        }
    }
    else
    {
        cout << "File:\t" << path <<"\t either f.isnull() or f.file() returns NULL" << endl;
    }
};

sqlite3 * Database::getDatabase(){
    return this->db;
};

void Database::updateTrackNo(int id, int t_no){
    // Update track_no of track with id of id to t_no
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET track_no = ";
    q.append(to_string(t_no));
    q.append(" WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");
    // cout << q << endl;

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateTrackNo prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Step statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateTrackNo step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};

void Database::updateTitle(int id, string t){
    // Update title of track with id of id to t
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET title = '";
    q.append(t);
    q.append("' WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");
    // cout << q << endl;

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateTitle prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Step statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateTitle step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};

void Database::updateArtist(int id, string a){
    // Update Artist of track with id of id to a
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET artist_name = '";
    q.append(a);
    q.append("' WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateArtist prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Execute statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateArtist step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};

void Database::updateAlbum(int id, string an){
    // Update album_name of track with id of id to an
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET album_name = '";
    q.append(an);
    q.append("' WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateAlbum prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Execute statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateAlbum step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};

void Database::updateReleaseYear(int id, int y){
    // Update release_year of track with id of id to y
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET release_year = ";
    q.append(to_string(y));
    q.append(" WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");
    // cout << q << endl;

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateReleaseYear prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Step statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateReleaseYear step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};

void Database::updateCoverArt(int id, string ca_path){
    // Update album_name of track with id of id to an
    
    // Variables
    int error = 0;
    sqlite3_stmt *stmt;

    // Prepare update query string.
    string q = "UPDATE tracks SET cover_art = '";
    q.append(ca_path);
    q.append("' WHERE track_id = ");
    q.append(to_string(id));
    q.append(";");

    // Prepare statement
    error = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
        cout << error << "\tupdateAlbum prepare error:\t" << sqlite3_errmsg(db) << endl;
    
    // Execute statement
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
        cout << error << "\tupdateAlbum step error:\t" << sqlite3_errmsg(db) << endl;
    
    sqlite3_finalize(stmt);
};


void Database::pullTracks(vector<Track> &tv) { // tv = track vector
    // Updates the vector passed in by reference to match DB
    
    // Prepare temporary variables to prepare Track object
    int tmp_index;
    int t_id;
    int t_no;
    string title;
    string album_name;
    string artist_name;
    int r_year;
    float dur;
    string c_art;
    string t_loc;

    // Clear the vector if it already contains data
    if (tv.size() > 0)
        tv.clear();
    
    // Write query
    string query = "\
    SELECT\
        SUM(1) OVER (ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW),\
        track_id,\
        track_no,\
        title,\
        album_name,\
        artist_name,\
        release_year,\
        duration,\
        cover_art,\
        track_location\
    FROM\
        tracks;";
    // Initialize statement object to store results
    sqlite3_stmt* stmt;
    // Prepare query
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    // Set control boolean
    bool done = false;
    while (!done)
    {
        switch(sqlite3_step(stmt))
        {
            case SQLITE_ROW:
                // Process row into vectors
                tmp_index = sqlite3_column_int(stmt, 0);
                t_id = sqlite3_column_int(stmt, 1);
                t_no = sqlite3_column_int(stmt, 2);
                title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                album_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                artist_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                r_year = sqlite3_column_int(stmt, 6);
                dur = sqlite3_column_double(stmt, 7);
                c_art = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
                t_loc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));

                tv.push_back(Track
                    (
                        t_id,
                        t_no,
                        title,
                        album_name,
                        artist_name,
                        r_year,
                        dur,
                        c_art,
                        t_loc,
                        tmp_index
                    )
                );
                break;
            
            case SQLITE_DONE:
                done = true;
                break;
            
            default:
                // TODO: implement error handling here
                fprintf(stderr, "Failed to grab tracks.\n");
                cout << (sqlite3_errmsg(db)) << endl;
                done = true;
                break;
        }
    }
    sqlite3_reset(stmt);
    cout << "pullTracks ran successfully" << endl;
}

// Playlist definitions
Playlist::Playlist(vector<Track> &tv){
    // Make a copy of the vector into playlist class
    copy(tv.begin(), tv.end(), back_inserter(track_list));
    cur_track = 0;
}


// Purge and refresh database
void Database::refreshDatabase()
{
    int error = 0;
    
    string query = "\
    DELETE\
    FROM\
        tracks;";
    // Initialize statement object to store results
    sqlite3_stmt* stmt;
    // Prepare query
    error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK)
    {
        cout << "Error in refreshDatabase(). Could not prepare statement." << endl;
        cout << error << endl;
        return;
    }
    
    // Execute query
    error = sqlite3_step(stmt);
    if (error != SQLITE_DONE)
    {
        cout << "Error in refreshDatabase(). Could not execute statement." << endl;
        cout << error << endl;
        return;
    }
    
    // Release resources from statement
    sqlite3_finalize(stmt);

    // Repopulate database
    scanMusicFolder("");
}