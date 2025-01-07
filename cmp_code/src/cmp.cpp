// #include "Config.h"
#include "database/Database.h"
// #include "control/Control.h"
#include "view/View.h"
#include "taglib/tag_c.h"
#include <ncurses.h>

using namespace std;

int main(int argc, char ** argv) {

    Database d;
    d.scanMusicFolder("");
    // Control c(d);
    // Control::instance = &c;
    // MainView v(d, *Control::instance);
    
    Control *c = new Control(d);
    // Control::instance = c;
    MainView v(d, *c);
    // d.updateReleaseYear(1, 2017);

    // TODO: Rework Views so they are handling Track objects directly
    // database should handle the querying and there should be a simple
    // refresh() command to update the information for a given view

    // TODO: Find a way to get the music to play. OpenAL already imported
    // to View.cpp with the cmake file.

    return 0;
};