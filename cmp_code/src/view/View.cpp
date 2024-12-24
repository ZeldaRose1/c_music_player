// Standard library imports
#include <string>

// Third party imports
#include <ncurses.h>
#include <panel.h>

// Custom imports
#include "../view/View.h"
#include "../database/Database.h"


void View::setDatabase(Database &d){
    this->d = &d;
}

MainView::MainView(Database d){
    // Function for the main view of music player

    // Initialize database
    setDatabase(d);
    sqlite3 *db = d.getDatabase();
    // cout << sqlite3_errmsg(db) << endl;
    int row = 0;

    // Start ncurses window
    initscr();

    // TODO: refactor the sql pull into it's own function
    // query database for all tracks
    // Write query
    string query = "SELECT title, artist_name, album_name FROM tracks;";
    // Initialize statement object to store results
    sqlite3_stmt* stmt;
    // Prepare query
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    
    // Define variables
    vector<string> title;
    vector<string> artist;
    vector<string> album;
    bool done = false;
    while (!done)
    {
        switch(sqlite3_step(stmt))
        {
            case SQLITE_ROW:
                // Process row into vectors
                title.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
                artist.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                album.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
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
    sqlite3_finalize(stmt);


    // Request input
    row++;
    for (int i = 0; i < title.size(); i++) {
        mvprintw(row, 0, "%s\t%s\t%s\n", title[i].c_str(), artist[i].c_str(), album[i].c_str());
        row++;
    }
    
    refresh();
    int ch = getch();

    // End ncurses window and return resources
    endwin();
}