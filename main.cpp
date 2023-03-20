#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>


//A salt is used to prepend to a password this allows for better security
//For a salt to work properly it has to be different each time the program is used by someone
//It also has to be random and long this will get prepended to the hashed password


std::string generateSalt()
{
    std::srand(time(0)); //seeding random generator for c++ random numbers
    const int salt_size = 10; //How long we want the salt
    //A longer salt should be used so that is is not easy to create a solution to hashes
    char result[salt_size]; //Array to hold each char
    std::string salt;
    for (int i = 0; i < salt_size; i++)
    {
        int random_letter_val = std::rand()%25; //Random number for each letter of the alphabet
        char random_letter = static_cast<char> (random_letter_val + 'a'); // generating random char for the salt
        salt = salt + random_letter; //adds random char to the salt
    }
    return salt;
}

unsigned long hashPassword(std::string password, std::string salt)
{
    std::hash <std::string> hash; //Initialises Hash
    std::string complete_password = password + salt; //append salt to given password
    unsigned long hashed_password = hash(complete_password); //has complete password
    return hashed_password;
}

bool passwordCheck(std::string username, std::string given_password)
{
    std::ifstream users_read("users.csv");
    std::string name = "";

    std::string data;

    while (getline(users_read, data))
    {
        int end_of_name = data.find(",");
        if(data.substr(0, end_of_name) == username)
        {
            data = data.substr(end_of_name + 1, data.size()-end_of_name);
            int end_of_hashed_password = data.find(",");
            long saved_password = std::atol(data.substr(0, end_of_hashed_password).c_str());
            data = data.substr(end_of_hashed_password + 1, data.size()-end_of_hashed_password+1);
            int end_of_salt = data.find(",");
            std::string salt = data.substr(0, end_of_salt);
            if(hashPassword(given_password, salt) == saved_password) //If hashing given password with same salt makes saved password then it is correct password
            {
                return true;
            }
            return false;
        }
    }

    
}

void createNewAccount(std::string username, std::string password, int access)
{
    //This past will create a new account
    //It should ask the user for a password and username
    //A user access level should be assigned
    //1 = user 2= hospital staff
    
    std::string salt = generateSalt();
    unsigned long hashed_password = hashPassword(password, salt);
    
    std::ifstream users_read("users.csv"); //Opens the file in read mode
    std::string file_data = "";
    
    //Read current file first

    std::vector<std::string> all_users; //Makes a vector for all of the users data to be stored in
    
    //This will store the text from the file.
    std::string data = "";

    //This while loop will output each of the lines in the text file.
    while (getline(users_read, data))
    {
        all_users.push_back(data + "\n"); //Adds individuals data into the vecotr
    }

    users_read.close(); //Close the file

    std::string current_user_data = username + "," + std::to_string(hashed_password) + "," + salt + "," + std::to_string(access) + "\n"; // Creates the data in the form of csv for the next person

    all_users.push_back(current_user_data); //Adds to vector
    
    std::ofstream users("users.csv"); // The file which contains account details
    
    for(int i = 0; i < all_users.size(); i++)
    {
        users << all_users[i]; //Writing to CSV file
    }

    users.close();
}

bool checkForUsername(std::string username)
{
    std::ifstream users_read("users.csv");
    std::string name = "";

    std::string data;

    while (getline(users_read, data))
    {
        int end_of_name = data.find(",");
        if(data.substr(0, end_of_name) == username)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> accountCreation()
{
    std::vector<std::string> users_data;
    std::string given_username;

    while(true){
        std::cout << "What is the username for the account: ";
        std::cin >> given_username;

        if (!given_username.find(','))
        {
            std::cout << "Sorry, username cannot contain the character ','";
        }
        else
        {
            if(!checkForUsername(given_username))
            {
                break;
            }
            else
            {
                std::cout << "Sorry that username already exists, please choose another one";
            }
        }

        
    }

    std::cout << "What is the password for the account: ";
    std::string given_password;
    std::cin >> given_password;

    users_data.push_back(given_username);
    users_data.push_back(given_password);

    return users_data;
}

std::string getUserRank(std::string username)
{
    std::ifstream users_read("users.csv");
    std::string name = "";

    std::string data;

    while (getline(users_read, data))
    {
        int end_of_name = data.find(",");
        if(data.substr(0, end_of_name) == username)
        {
            return data.substr(data.size()-1, 1);
        }
    }
}

bool login()
{
    std::cout << "Enter your username: ";
    std::string username;
    std::cin >> username;

    std::cout << "Enter your password: ";
    std::string password;
    std::cin >> password;

    if(checkForUsername(username))
    {
        if(!passwordCheck(username, password))
        {
            std::cout << "Sorry that password was incorrect";
            return false;
        }else
        {
            std::string rank = getUserRank(username);
            return true;
        }
    }
    else
    {
        std::cout << "Sorry that username does not exist";
        return false;
    }

}

void startMenu()
{
    std::cout << "___Welcome to NHS Console___" << "\n\n";
    std::cout << "What would you like to do" << "\n";
    while(true)
    {
        std::cout << "1. Login" << "\n";
        std::cout << "2. Register for an account" << "\n";
        std::cout << "3. Exit" << "\n";
        int option;
        std::cin >> option;
        std::vector<std::string> users_data;
        switch(option)
        {
            case 1:
                login();
                break;
            case 2:
                users_data = accountCreation();
                createNewAccount(users_data[0], users_data[1], 1);
                break;
            case 3:
                exit(0); // Ends the program
                break;
            default:
                std::cout << "\n";
        }
    }

}

int main()
{
    startMenu();
    
    return 0;
}