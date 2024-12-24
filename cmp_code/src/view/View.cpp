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
    int choice = 0; // keyboard input
    int highlight = 1; // controls which row is selected
    bool active = true;

    // Start ncurses window
    initscr();

    // TODO: refactor the sql pull into it's own function
    // query database for all tracks
    // Write query
    string query = "\
    SELECT\
        SUM(1) OVER (ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW),\
        track_id,\
        title,\
        artist_name,\
        album_name\
    FROM\
        tracks;";
    // Initialize statement object to store results
    sqlite3_stmt* stmt;
    // Prepare query
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    
    // Define variables
    vector<int> temp_ind;
    vector<int> track_no;
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
                temp_ind.push_back(sqlite3_column_int(stmt, 0));
                track_no.push_back(sqlite3_column_int(stmt, 1));
                title.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                artist.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                album.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
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

    // Initialize and pull window parameters
    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    // Begin menu
    WINDOW *main_w = newwin(yMax, xMax, 0, 0);
    refresh();
    wrefresh(main_w);
    keypad(main_w, true);
    while (active) // Start infinite loop for menu
    {
        // Print output and make menu
        for (int i = 0; i < title.size(); i++)
        {
            // Process highlighted row printing
            // TODO: change the print statements to have more visual structure
            if (i + 1 == highlight)
            {
                wattron(main_w, A_REVERSE);
                mvwprintw(main_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
            }
            // Process non-highlighted row printing
            else
            {
                wattroff(main_w, A_REVERSE);
                mvwprintw(main_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
            }
        }

        // Pull keyboard input
        choice = wgetch(main_w);

        // Commands based on input
        switch(choice)
        {
            case KEY_UP:
                if (highlight > 1)
                    highlight--;
                break;
            case KEY_DOWN:
                if (highlight < temp_ind.size() + 1)
                    highlight++;
                break;
            case 'q': // q
                active = false;
                break;
            case 'Q': // Q
                active = false;
                break;
            default:
                break;
        }
        if (choice == 10) // 10 is enter key
            mvwprintw(main_w, yMax - 2, 0, "Your choice was: %s", title[highlight - 1].c_str());
    }
    
    refresh();

    // End ncurses window and return resources
    endwin();
}