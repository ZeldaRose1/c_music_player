#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class Config{
    private:
        string databaseLocation;
        string musicLocation;
    public:
        Config(); // Constructor
        Config(string); // Constructor
        string getDatabaseLocation();
        string getMusicLocation();
        void setDatabaseLocation(string s);
        void setMusicLocation(string s);

        void readFile(string location);
        void parseLines();
};

