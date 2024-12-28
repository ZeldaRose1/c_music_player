// Standard library imports
#include <string>

// Third party imports
#include <ncurses.h>
#include <panel.h>

// Custom imports
#include "../view/View.h"
#include "../database/Database.h"

// TODO: Edit the main view display to show the track that's currently playing and the time


void View::setDatabase(Database &d){
    this->d = &d;
}


void View::setControl(Control &c){
    this->c = &c;
}


EditorView::EditorView(Database D, Track T){
    // Called from MainView; Allows editing of Track info
    // and inserts / updates / deletions from the tag table.
    // id is the TRACKS table track_id.

    // TODO: setup management of tags table
    
    // Set database for editor view
    setDatabase(D);
    sqlite3 *db = d->getDatabase();

    // Declare variables for use in the rest of the EditorView
    int error = 0;
    int choice = 0; // keyboard input
    int highlight = 0; // controls which row is selected
    bool active = true;

    char edit_input[80];
    string e_inp_str;

    vector<string> menu_txt = {
        "Track Number", "Title", "Album Name",
        "Artist Name", "Release Year", "Cover Path",
        "Edit Tags"
    };

    // Initialize and pull window parameters
    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    // Begin subWindow
    WINDOW *edit_w = newwin(yMax, xMax / 2, 0, xMax / 2);
    refresh();
    wrefresh(edit_w);
    keypad(edit_w, true);

    while(active)
    {
        // Value vector
        vector<string> value_vec = {
            to_string(T.GetTrackNo()), T.GetTitle(), T.GetAlbumName(),
            T.GetArtistName(), to_string(T.GetReleaseYear()), T.GetCoverArt(),
            ""
        };

        // Print menu
        for (int i = 0; i < 7; i++)
            {
               if (i == highlight)
                {
                    wattron(edit_w, A_REVERSE);
                    mvwprintw(edit_w, i, 0, "%s", menu_txt[i].c_str());
                    mvwprintw(edit_w, i, 20, "|");
                    mvwprintw(edit_w, i, 22, "%s", value_vec[i].c_str());
                }
                // Process non-highlighted row printing
                else
                {
                    wattroff(edit_w, A_REVERSE);
                    mvwprintw(edit_w, i, 0, "%s", menu_txt[i].c_str());
                    mvwprintw(edit_w, i, 20, "|");
                    mvwprintw(edit_w, i, 22, "%s", value_vec[i].c_str());
                    
                    // mvwprintw(edit_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
                }
            }

        // Handle input
        choice = wgetch(edit_w);

        // Process input
        switch(choice)
        {
            case KEY_UP:
                if (highlight > 0)
                    highlight--;
                break;
            case KEY_DOWN:
                if (highlight < 6)
                    highlight++;
                break;
            case 27: // Escape key
                active = false;
                break;
            
            case 10: // Enter key
                // Clear screen before accepting input
                wclear(edit_w);
                // Pull string as input
                wgetnstr(edit_w, edit_input, 79);
                // Convert back to string for data functions.
                e_inp_str.assign(edit_input);
                mvwprintw(edit_w, 20, 5, "edit_input\t%s", e_inp_str.c_str());
                
                // Control which update function to call
                switch(highlight)
                {
                    case 0: // track_no update
                        d->updateTrackNo(T.GetTrackId(), atoi(edit_input));
                        T.SetTrackNo(atoi(edit_input));
                        break;
                    case 1: // Title update
                        d->updateTitle(T.GetTrackId(), e_inp_str);
                        T.SetTitle(e_inp_str);
                        break;
                    case 2: // Album Name
                        d->updateAlbum(T.GetTrackId(), e_inp_str);
                        T.SetAlbumName(e_inp_str);
                        break;
                    case 3: // Artist Name
                        d->updateArtist(T.GetTrackId(), e_inp_str);
                        T.SetArtistName(e_inp_str);
                        break;
                    case 4: // Release Year update
                        d->updateReleaseYear(T.GetTrackId(), atoi(edit_input));
                        T.SetReleaseYear(atoi(edit_input));
                        break;
                    case 5: // Cover art path update
                        d->updateCoverArt(T.GetTrackId(), e_inp_str);
                        T.SetCoverArt(e_inp_str);
                        break;
                    default:
                        break;
                }
                // Break out of the enter key case
                break;
            
            default:
                mvwprintw(edit_w, 40, 5, "char:\t%d", choice);
                break;
        }

        // Edit this to play song instead of printing selection.
        // if (choice == 10) // 10 is enter key
        //     mvwprintw(edit_w, yMax - 2, 0, "Your choice was: %s", menu_txt[highlight].c_str());
    }

    // Terminate window
    wclear(edit_w);
    delwin(edit_w);
    // TODO: Find a way to clear the output from this window when closed.
};


MainView::MainView(Database D, Control C){
    // Function for the main view of music player

    // Initialize database
    setDatabase(D);

    // Initialize Control
    setControl(C);
    // sqlite3 *db = d.getDatabase();
    // cout << sqlite3_errmsg(db) << endl;
    int choice = 0; // keyboard input
    int highlight = 1; // controls which row is selected
    bool active = true;
    vector<Track> tracks;

    // Start ncurses window
    initscr();

    // Clean tracks vector and repull
    d->pullTracks(tracks);

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
        for (int i = 0; i < tracks.size(); i++)
        {
            // Process highlighted row printing
            // TODO: change the print statements to have more visual structure
            if (i + 1 == highlight)
            {
                wattron(main_w, A_REVERSE);
                mvwprintw(main_w, i, 0, "%d", tracks[i].GetTrackId()); // Print track_id
                mvwprintw(main_w, i, 3, "|");
                mvwprintw(main_w, i, 5, "%s", tracks[i].GetTitle().c_str()); // Print title
                mvwprintw(main_w, i, 75, "|");
                mvwprintw(main_w, i, 77, "%s", tracks[i].GetArtistName().c_str()); // Print Artist
                mvwprintw(main_w, i, 97, "|");
                mvwprintw(main_w, i, 99, "%s", tracks[i].GetAlbumName().c_str()); // Print Album
                
                // mvwprintw(main_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
                // mvwprintw(main_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
            }
            // Process non-highlighted row printing
            else
            {
                wattroff(main_w, A_REVERSE);
                mvwprintw(main_w, i, 0, "%d", tracks[i].GetTrackId()); // Print track_id
                mvwprintw(main_w, i, 3, "|");
                mvwprintw(main_w, i, 5, "%s", tracks[i].GetTitle().c_str()); // Print title
                mvwprintw(main_w, i, 75, "|");
                mvwprintw(main_w, i, 77, "%s", tracks[i].GetArtistName().c_str()); // Print Artist
                mvwprintw(main_w, i, 97, "|");
                mvwprintw(main_w, i, 99, "%s", tracks[i].GetAlbumName().c_str()); // Print Album
                
                // mvwprintw(main_w, i, 0, "%d\t%s\t%s\t%s\n", track_no[i], title[i].c_str(), artist[i].c_str(), album[i].c_str());
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
                if (highlight < tracks.size())
                    highlight++;
                break;
            case 'q': // q
                active = false;
                break;
            case 'Q': // Q
                active = false;
                break;
            case 'e':
                EditorView(*d, tracks[highlight - 1]);
                wclear(main_w);
                d->pullTracks(tracks);
                break;
            case 10:
                // mvwprintw(main_w, yMax - 2, 0, "Your choice was: %s", tracks[highlight - 1].GetTitle().c_str());
                c->play(tracks[highlight - 1]);
                break;    
            default:
                break;
        }
    }
    
    refresh();

    // End ncurses window and return resources
    endwin();
    // sqlite3_finalize(stmt);
};




// EditorView::~EditorView(){};