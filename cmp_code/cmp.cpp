#include "Config.h"
#include "Database.h"
#include "taglib/tag_c.h"

int main() {
    // Config c;
    // c.readFile("config.txt");

    // cout << "database location\t" << c.getDatabaseLocation() << endl;
    // cout << "music location\t" << c.getMusicLocation() << endl;

    Database d;
    d.scanMusicFolder();

};