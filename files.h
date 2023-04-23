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
    std::ofstream file_write(file_name);
        
    if(type == 1){
        std::vector<std::string> all_data;
        std::string data;

        //First read all file
        std::ifstream file_read(file_name);

        while(getline(file_read, data))
        {
            all_data.push_back(data + "\n");
        }

        file_read.close();

        all_data.push_back(to_write);

        for(int i = 0; i < all_data.size(); i++)
        {
            file_write << all_data[i];
        }
    }
    else{
        file_write << to_write;
    }

    file_write.close();
}

std::vector<std::vector<std::string>> extractCSV(std::string file_name)
{
    std::ifstream file(file_name);
    std::string data;
    std::string delim = ",";

    std::vector<std::vector<std::string>> all_data;
    std::vector<std::string> current_data;

    while(getline(file, data))
    {
        current_data.clear();
        while(true)
        {
            if(strstr(data.c_str(), delim.c_str()))
            {
                int pos = data.find(',');
                current_data.push_back(data.substr(0, pos));
                data = data.substr(pos+1, data.size());
            }
            else
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