#pragma once
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

void writeToCSV(std::string file_name, std::string to_write, bool type = 1)
{
    // type 1 is for appending to a file
    if(type == 1){
        std::ofstream outfile;

        outfile.open(file_name, std::ios_base::app); // append instead of overwrite
        outfile << to_write + "\n"; 
    }

    // the other type makes the file all the text you want it to be

    else{
        std::ofstream file_write(file_name);
        file_write << to_write;
        file_write.close();
    }

}

std::vector<std::vector<std::string>> extractCSV(std::string file_name)
{
    std::ifstream file(file_name);
    std::string data;
    std::string delim = ","; //deliminator for splitting a csv file

    std::vector<std::vector<std::string>> all_data;
    std::vector<std::string> current_data;

    while(getline(file, data))
    {
        current_data.clear();
        while(true)
        {
            //opens each line and splits at any ','
            if(strstr(data.c_str(), delim.c_str()))
            {
                int pos = data.find(',');
                current_data.push_back(data.substr(0, pos));
                data = data.substr(pos+1, data.size());
            }
            else //if there is no ',' then the rest must be the end of a ','
            {
                current_data.push_back(data);
                all_data.push_back(current_data);
                break;
            }
        }
    }

    file.close();

    return all_data;
}