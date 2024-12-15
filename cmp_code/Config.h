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
        string getDatabaseLocation(){return this->databaseLocation;};
        string getMusicLocation(){return this->musicLocation;};
        void setDatabaseLocation(string s) {databaseLocation = s;};
        void setMusicLocation(string s) {musicLocation = s;};

        void readFile(string location);
        void parseLines();
};

void Config::readFile(string location){
    // Load configuration details from file

    // Convert filename to full path
    string full_path = filesystem::current_path();
    full_path = full_path.append("/").append(location);

    // Open file
    fstream file(full_path.c_str());
    // Check if file exists
    if (!file.is_open()) {
        cout << "file does not exist\n" << endl;
        
        // Initialize default parameters
        this->databaseLocation = "working/database/cmp.db";
        
        // Default to home directory for music folder
        string home = getenv("HOME");
        this->musicLocation = home.append("/Music");
        return;
    }

    // Initialize storage variables
    string keyword = "";
    string variable = "";

    // Save variable to hold individual lines
    string line;
    // Loop while lines still exist in the file
    while (getline(file, line))
        {
            // Load individual line into its own istream
            fstream is_line(line);

            // Save position of '='
            int pos = line.find("=");
            
            // Separate line before and after equal sign
            keyword = line.substr(0, pos);
            variable = line.substr(pos + 1);

            // Assign variables based on text file
            if (keyword == "databaseLocation")
                this->databaseLocation = variable;
            if (keyword == "musicLocation"){
                string mloc = "";
                // Process shortcut for home directory
                cout << "Config.h: variable[0] is:\t" << variable[0] << endl;
                if (variable[0] == '~'){
                    mloc = getenv("HOME");
                    mloc = mloc.append(variable.substr(1));
                }
                else
                    mloc = variable;
                this->musicLocation = mloc;
            }
        }
};

Config::Config(){
    // Constructor for config class
    readFile("config.txt");
};

Config::Config(string path){
    // Constructor for config class
    readFile(path.c_str());
};