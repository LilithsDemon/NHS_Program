#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <fstream>


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

bool passwordCheck(std::string given_password, unsigned long saved_password, std::string salt)
{
    if(hashPassword(given_password, salt) == saved_password) //If hashing given password with same salt makes saved password then it is correct password
    {
        return true;
    }
    return false;
}

void createNewAccount(std::string username, std::string password, int access)
{
    //This past will create a new account
    //It should ask the user for a password and username
    //A user access level should be assigned
    //1 = user 2= hospital staff
    
    std::string salt = generateSalt();
    unsigned long hashed_password = hashPassword(password, salt);
    
    std::ifstream users_read("users.csv");
    std::string file_data = "";
    
    //Read current file first

    //Closes our text file.
    textFile.close();
    
    std::ofstream users("users.csv"); // The file which contains account details
    
    users << username << "," << hashed_password << "," << salt << "," << access << "\n"; //Writing to CSV file
    
    users.close();
}

void accountCreation()
{
    std::cout << "What is your username: ";
    std::string username;
    std::cin >> username;
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
        switch(option)
        {
            case 1:
                createNewAccount("Bob", "password", 1);
                break;
            case 2:
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