#pragma once

#include <string>
#include "../database/Database.h"
#include "../control/Control.h"

class Database;
class Track;
class Control;

class View{
    private:
        static int view_count;
    protected:
        Database *d;
        Control *c;
    public:
        // virtual void update();
        void setDatabase(Database&);
        void setControl(Control&);
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
        MainView(Database, Control); // Constructor that will set the DB pointer
        // void update();
};

class EditorView : public View{
    public:
        EditorView(Database, Track); // Constructor
        // ~EditorView(); // Destructor
        // void update();
};

// class PlaylistView : public View{
//     public:
//         void update();
// };
