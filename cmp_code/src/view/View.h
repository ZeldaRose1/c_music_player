#pragma once

#include <string>
#include "../database/Database.h"

class Database;

class View{
    private:
        static int view_count;
        Database *d;
    public:
        // virtual void update();
        void setDatabase(Database&);
        // TODO: fix the equality check once the views are distinguished
        bool operator==(const View& rhs) {
            return true;
        }
};

class MainView : public View{
    // This view displays the tracks and allows the user to 
    // control which track to play.
    private:
    public:
        MainView(Database); // Constructor that will set the DB pointer
        // void update();
};

class EditorView : public View{
    public:
        EditorView(Database, int); // Constructor
        // ~EditorView(); // Destructor
        // void update();
};

// class PlaylistView : public View{
//     public:
//         void update();
// };
