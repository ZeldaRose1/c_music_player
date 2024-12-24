// #include "Config.h"
#include "database/Database.h"
#include "view/View.h"
#include "taglib/tag_c.h"
#include <ncurses.h>

using namespace std;

int main(int argc, char ** argv) {

    Database d;
    // MainView v(d);
    d.updateReleaseYear(1, 2017);

    return 0;
};