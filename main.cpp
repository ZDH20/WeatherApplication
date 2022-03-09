/*
Author: Zachary Haskins
Date: 3/9/2022
Purpose:
    Basically a macro to take data from a file and
    open it in a web browser for displaying weather.
    Parses file and reports error statuses.
    Will create a new file if one is not present.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "List.h"

#define STATIONS_FILE_NAME "stations.txt"
#define STATIONS_FILE_CONTENTS "@ Under here, put a station name, and the next line have the website you want to open with it.\n@ It must be:\n@   station\n@   weather website address\n@   station\n@   weather website address\n@   etc.\n@\n@ Websites must begin with http.\n@ Having empty lines in between stations and addresses are fine.\n@ Put '@' signs at the beginning of a line to comment it out.\n\n@Example:\n\nWXYX\nhttps://forecast.weather.gov/MapClick.php?lat=35.965152200000034&lon=-86.88696169999997\n\nAnother Station\nhttps://www.weather.gov/\n"

/* Prototypes and Call Order. */
int         init_program(std::ifstream&);
int         read_file(std::ifstream &, std::string, int&);
List<char>  read_addr(std::string);
void        check_err(int, int&, bool);

int main(void){

    // Vars. 
    std::ifstream file(STATIONS_FILE_NAME);
    std::string line;
    int line_num = 0, err_status;

    // Run program. Check for errors.
    check_err(init_program(file), line_num, false);
    check_err(read_file(file, line, line_num), line_num, true);

    // End.
    file.close();
    return 0;
}

int init_program(std::ifstream &file){

    std::cout << "---Initializing program---\n" << std::endl;
    std::cout << "Opening console..." << std::endl;

    if(AllocConsole()){
        if(system("mode con")){
            // make an error box if the console can't be opened
            MessageBox(NULL, "Fatal Error!\nFailed both attempts to open the console.", "Error", MB_OK);
            exit(1);
        }
        std::cout << "AllocConsole() failed... Using system(mode con) instead." << std::endl;
    }
    std::cout << "Done." << std::endl;

    std::cout << "Opening file..." << std::endl;
    if(!file.is_open()){
        std::ofstream new_file(STATIONS_FILE_NAME);
        new_file << STATIONS_FILE_CONTENTS;
        return 1;
    }
    std::cout << "Done.\n" << std::endl;
    return 0;
}

int read_file(std::ifstream &file, std::string line, int &line_num){
    std::cout << "Reading file...\n" << std::endl;

    bool station = false, address = false;
    std::string station_name = "";
    
    while(std::getline(file, line)){

        line_num++;

        // Nothing to do if line is empty.
        if(line.length() == 0 || line[0] == '@')
            continue;

        // We know that this will be a station name. 
        if(line.substr(0, 4) != "http"){
            // If we have two stations, we need to exit with error code 1
            if(station)
                return 2;
            station_name = line;
            station = true;
            address = false;
            continue;
        }

        // If we have two addresses, we need to exit with error code 2
        if(address)
            return 3;
        address = true;
        station = false;

        // Run launch the browser with the url.
        std::cout << "Station: "<< station_name <<'\n';
        system(("start " + read_addr(line).to_string()).c_str());

        std::cout << "Enter [ENTER] to continue. . . ";
        std::cin.get();
        std::cout<<std::endl;
    }
    return 0;
}

List<char> read_addr(std::string in){

    List<char> address_list;

    for(int i = 0; i < in.length(); i++){
        // '&' needs to be read as a part of the URL, not as a console command
        if(in[i] == '&'){
            address_list.push_back('\"');
            address_list.push_back('&');
            address_list.push_back('\"');
            continue;
        }
        address_list.push_back(in[i]);
    }
    return address_list;
}

void check_err(int err_code, int &line_num, bool end){
    switch(err_code){
        case 0:
            std::cout << "---Done---\n" << std::endl;
            if(end){
                std::cout << "Press [ENTER] to exit." << std::endl;
                std::cin.get();
            }
            break; // No error.
        case 1:
            std::cout << "\nError: Cannot find file, 'stations.txt'.\nCreating file..." << std::endl;
            std::cout << "\nPress [ENTER] to exit." << std::endl;
            std::cin.get();
            exit(1);
        case 2:
            std::cout << "\nError: Found a station following after a station on line:\n\t" << line_num << std::endl;
            std::cout << "\nPlease check the file and make sure every station is directly followed by an address." << std::endl;
            std::cout << "\nPress [ENTER] to exit." << std::endl;
            std::cin.get();
            exit(1);
        case 3:
            std::cout << "\nError: Found address following after an address on line:\n\t" << line_num << std::endl;
            std::cout << "\nPlease check the file and make sure every station is directly followed by an address." << std::endl;
            std::cout << "\nPress [ENTER] to exit." << std::endl;
            std::cin.get();
            exit(1);
        default:
            std::cout << "\nError: Unknown error code " << err_code << std::endl;
            exit(1);
    }
}