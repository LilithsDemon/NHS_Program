#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include "accounts.h"
#include "files.h"


//A salt is used to prepend to a password this allows for better security
//For a salt to work properly it has to be different each time the program is used by someone
//It also has to be random and long this will get prepended to the hashed password

void clearTerm()
{
    // This clears out the terminal so that the terminal looks better
    std::cout << "\033[2J\033[1;1H";
}

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
    std::ifstream users_read("users.csv"); //Reads file for the user
    std::string name = "";

    std::string data;

    while (getline(users_read, data))
    {
        int end_of_name = data.find(",");
        if(data.substr(0, end_of_name) == username) //Finds this users data
        {
            data = data.substr(end_of_name + 1, data.size()-end_of_name); 
            int end_of_hashed_password = data.find(","); 
            long saved_password = std::atol(data.substr(0, end_of_hashed_password).c_str());
            data = data.substr(end_of_hashed_password + 1, data.size()-end_of_hashed_password+1);
            int end_of_salt = data.find(",");
            //Sub strings are used to be able to break down the csv file for the information we need
            std::string salt = data.substr(0, end_of_salt);
            if(hashPassword(given_password, salt) == saved_password) //If hashing given password with same salt makes saved password then it is correct password
            {
                users_read.close();
                return true;
            }
        }
    }
    users_read.close();
    return false;
}

void createNewAccount(std::string username, std::string password, std::string age, int access)
{
    //This past will create a new account
    //It should ask the user for a password and username
    //A user access level should be assigned
    //1 = user 2 = doctor/nurse 3 = headdoctor 4 = pharmacist
    //Verified file finds if a user has done the form yet
    
    std::string salt = generateSalt();
    unsigned long hashed_password = hashPassword(password, salt);

    std::string current_user_data = username + "," + std::to_string(hashed_password) + "," + salt + "," + age + "," + std::to_string(access); // Creates the data in the form of csv for the next person
    std::string current_verification_data = username + "," + "false";

    writeToCSV("users.csv", current_user_data);
    writeToCSV("verify.csv", current_verification_data);
    // Writes to files what we need
}

bool checkForUsername(std::string username)
{
    //This will go through the csv file and check for the name if it is there
    std::vector<std::vector<std::string>> users = extractCSV("users.csv");
    for(int i = 0; i < users.size(); i++)
    {
        if(username == users[i][0])
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
        clearTerm();
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

    std::cout << "And finally what is your age: ";
    std::string age;
    std::cin >> age;

    users_data.push_back(given_username);
    users_data.push_back(given_password);
    users_data.push_back(age);

    return users_data;
}

std::string getUserRank(std::string username)
{
    //Returns the rank of 1-4 dependant of their role in the company
    std::vector<std::vector<std::string>> users = extractCSV("users.csv");
    for(int i = 0; i < users.size(); i++)
    {
        if(username == users[i][0])
        {
            return users[i][4];
        }
    }

    return "";
}

void verifyUser(std::string username)
{
    //This verifys the user as having done the form
    std::vector<std::vector<std::string>> verifications = extractCSV("verify.csv");
    std::string string_to_write = "";
    for(int i = 0; i < verifications.size(); i++)
    {
        if(verifications[i][0] == username){
            verifications[i][1] = "true";
        }
        string_to_write += verifications[i][0] + "," + verifications[i][1] + "\n";
    }
    std::cout << string_to_write;
    writeToCSV("verify.csv", string_to_write);
}

void patientForm(std::string username)
{
    //This is a form the patient needs to do to be able to get access to the entire program.
    std::string cancer;
    int cancer_type = 0;
    std::string diabetes;
    int diabetes_type = 0;
    std::string smoking;
    int smoking_type = 0;
    clearTerm();
    std::cout << "\nWelcome to the NHS program, this is a questionaire so that you can get your medical data\n";
    std::string string_to_write = username;
    while(true){
        std::cout << "First do you have cancer: (y/n): ";
        std::cin >> cancer;
        if(cancer == "y" || cancer == "Y")
        {
            while(true){
                std::cout << "What stage cancer are you in: 1-4: ";
                std::cin >> cancer_type;
                if(cancer_type > 0 && cancer_type < 5)
                {
                    break;
                }
                else
                {
                    std::cout << "Please select an option between 1 and 4";
                }
            }
            string_to_write += "," + std::to_string(cancer_type);
            break;
        }
        else if(cancer == "n" || cancer == "N")
        {
            string_to_write += ",0";
            break;
        }
        else
        {
            std::cout << "Sorry please select yes (y) or no (n)";
        }
    }
    while(true)
    {
        clearTerm();
        std::cout << "Do you have diabetes: (y/n): ";
        std::cin >> diabetes;
        if(diabetes == "y" || diabetes == "Y")
        {
            while(true)
            {
                std::cout << "Do you have type 1 or type 2 diabetes (1/2): ";
                std::cin >> diabetes_type;
                if(diabetes_type > 0 && diabetes_type < 3)
                {
                    string_to_write += "," + std::to_string(diabetes_type);
                    break;
                }
                else
                {
                    std::cout << "Sorry please enter a value between 1 and 2";
                }
            }
            break;
        }
        else if(diabetes == "n" || diabetes == "N")
        {
            string_to_write += ",0";
            break;
        }
        else
        {
            std::cout << "Please enter y or n for yes or no";
        }
    }
    while(true)
    {
        clearTerm();
        std::cout << "Do you smoke (y/n): ";
        std::cin >> smoking;
        if(smoking == "y" || smoking == "Y")
        {
            while(true)
            {
                std::cout << "How long does it take you to smoke 1 pack: 1: A month, 2: A week, 3: A Day: ";
                std::cin >> smoking_type;
                if(smoking_type > 0 && smoking_type < 4)
                {
                    string_to_write += "," + std::to_string(smoking_type);
                    break;
                }
                else
                {
                    std::cout << "Please enter a value between 1 and 3";
                }
            }
            break;
        }
        else if(smoking == "n" || smoking == "N")
        {
            string_to_write += ",0";
            break;
        }
        else
        {
            std::cout << "Please enter y for yes or n for no: ";
        }
    }
    std::cout << "Thank you for completing this form";
    writeToCSV("conditions.csv", string_to_write, 0);
    verifyUser(username); //writes to file to ensure that everyone can see
}

void paitentMenu(std::string username)
{
    std::vector<std::vector<std::string>> verification = extractCSV("verify.csv");
    bool verify = false;
    for(int i = 0; i< verification.size(); i++)
    {
        if(verification[i][0] == username)
        {
            if(verification[i][1] == "true")
            {
                verify = true;
                break;
            }
        }
    }

    if(verify == false)
    {
        patientForm(username);
    }

    Patient user(username); //Uses a class for the patient from accounts.h

    int choice = 0;
    bool loggedIn = true;

    clearTerm();

    while (loggedIn == true){
        std::cout << "Welcome patient: What would you like to do:\n";
        std::cout << "1. Redo form\n";
        std::cout << "2. Get cost for treatments\n";
        std::cout << "3. Log out\n";
        std::cout << ":: ";
        std::cin >> choice;
        switch(choice)
        {
            case 1:
                patientForm(username);
                break;
            case 2:
                std::cout << "$" << user.userCost() << " per day\n";
                std::cout << "$" << user.userCost()*7 << " per week\n";
                std::cout << "$" << user.userCost()*28 << " per month (4 week period)\n";
                std::cout << "$" << user.userCost()*7*52 << " per year (52 week period)\n";
                break;
            case 3:
                loggedIn = false;
                break;
            default:
                std::cout << "Sorry please enter a correct value\n";
                break;
        }
    }
}

std::vector<std::string> change_prescription()
{
    std::string patient_username;
    std::vector<std::vector<std::string>> prescriptions;
    std::vector<std::vector<std::string>> all_prescriptions = extractCSV("treatments.csv");
    std::string prescription_data = "";
    std::string prescription_choice = "";
    int condition = 0;
    int base_value = 0;
    std::vector<std::string> patients;
    
    std::cout << "Please enter patient's username: ";
    std::cin >> patient_username;
    clearTerm();
    bool found = false;
    for(int i = 0; i < patients.size(); i++)
    {
        if(patients[i] == patient_username)
        {
            found = true;
        }
    }
    std::cout << (found) ? "\n" : "User not found.\n";
    if(found == false)
    {
        return {" "};
    }
    bool correct_choice = false;
    while(correct_choice = false)
    {
        std::cout << "\nWhat would you like to change: \n";
        int option = 0;
        std::cout << "1. Prescription for diabetes:\n";
        std::cout << "2. Prescription for smoking:\n";
        std::cout << "3. Prescription for cancer:\n";
        std::cout << ":: ";
        std::cin >> option;
        switch (option)
        {
        case 1:
            prescriptions = extractCSV("diabetes.csv");
            base_value = 0; //base value is for reading from treatments, gives information for everything
            condition = 1;
            correct_choice = true;
            break;
        case 2:
            prescriptions = extractCSV("smoking.csv");
            base_value = 2;
            condition = 2; //Condition value
            correct_choice = true;
            break;
        case 3:
            prescriptions = extractCSV("cancer.csv");
            base_value = 6;
            condition = 3;
            correct_choice = true;
            break;
        default:
            std::cout << "Sorry please pick between 1 and 3 for the correct option\n"; 
            break;
        }
    }
    correct_choice = false;
    while(correct_choice == false)
    {
        std::cout << "What would you like to change the prescription to be\n";
        std::cout << "0. No prescription\n";
        for(int i = 0; i < prescriptions.size(); i++)
        {
            prescription_data = std::to_string(i+1) + ". " + all_prescriptions[i + base_value][2] + ", " + all_prescriptions[i + base_value][3] + ", " + all_prescriptions[i + base_value][4] + ", " + all_prescriptions[i + base_value][5] + "\n";
            std::cout << prescription_data + ":: "; // Data to be written to the display
        }
        std::cin >> prescription_choice;
        try{  //used to stop some possible errors that might occur
            if(std::stoi(prescription_choice) >= 0 && std::stoi(prescription_choice) <= prescriptions.size())
            {
                correct_choice = true;
            }
            else
            {
                std::cout << "Sorry that number is not a correct value\n";
            }
        }
        catch(std::string prescription_choice)
        {
            std::cout << "Please enter only a number value\n";
        }
    }
    return {patient_username, std::to_string(condition), prescription_choice};
}

void doctorMenu(std::string username)
{
    Doctor user(username); // Doctors class from accounts.h
    int choice = 0;
    bool loggedIn = true;

    std::vector<std::string> patients = user.getPatients();

    clearTerm();

    bool found = false;

    std::vector<std::string> change_prescription_data;

    while (loggedIn == true){
        std::cout << "Welcome Doctor: What would you like to do:\n";
        std::cout << "1. Get a list of your assigned patients\n";
        std::cout << "2. Change a patients prescriptions\n";
        std::cout << "3. Get a patients conditions and prescriptions\n";
        std::cout << "4. Log out\n";
        std::cout << ":: ";
        std::cin >> choice;
        std::string patient_username;
        switch(choice)
        {
            case 1:
                for(int i = 0; i < patients.size(); i++)
                {
                    std::cout << patients[i] << "\n";
                }
                break;
            case 2:
                change_prescription_data = change_prescription();
                if(change_prescription_data.size() == 1)
                {
                    std::cout << "Sorry that did not work\n";
                }
                else
                {
                    if(user.changePrescription(change_prescription_data[0], std::stoi(change_prescription_data[1]), std::stoi(change_prescription_data[2])))
                    {
                        std::cout << "The change has been made \n";
                    }
                    else
                    {
                        std::cout << "Sorry that user is not assigned to you \n";
                    }
                }
                break;
            case 3:
                std::cout << "Please enter patient's username: ";
                std::cin >> patient_username;
                clearTerm();
                found = false;
                for(int i = 0; i < patients.size(); i++)
                {
                    if(patients[i] == patient_username)
                    {
                        found = true;
                    }
                }
                std::cout << (found) ? user.getPatientInfo(patient_username) : "\nUser not found\n"; //Tenary (1 line if statement)
                break;
            case 4:
                loggedIn = false;
                break;
            default:
                std::cout << "Sorry please enter a correct value\n";
                break; 
        }
    }
}

void headDoctorMenu(std::string username)
{
    HeadDoctor user(username);
    int choice = 0;
    bool loggedIn = true;

    std::vector<std::string> patients = user.getPatients();
    std::vector<std::string> doctors = user.getAllDoctors();

    clearTerm();

    while (loggedIn == true){
        std::cout << "Welcome Head Doctor: What would you like to do:\n";
        std::cout << "1. Get a list of your assigned patients\n";
        std::cout << "2. Get a patients conditions and prescriptions\n";
        std::cout << "3. Change a patients prescriptions\n";
        std::cout << "4. Get a list of the unassigned patients\n";
        std::cout << "5. Assign a patient to a doctor\n";
        std::cout << "6. Create a new doctor\n";
        std::cout << "7. Upgrade a Doctor to head doctor\n";
        std::cout << "8. Log out\n";
        std::cout << ":: ";
        std::cin >> choice;
        std::string patient_username;
        std::string doctor_username;
        std::string new_doctor;
        std::string password;
        std::string doctor;
        int age;
        bool found = false;
        std::vector<std::string> change_prescription_data;
        switch(choice)
        {
            case 1:
                for(int i = 0; i < patients.size(); i++)
                {
                    std::cout << patients[i] << "\n";
                }
                break;
            case 2:
                std::cout << "Please enter patient's username: ";
                std::cin >> patient_username;
                clearTerm();
                found = false;
                patients = user.getPatients();
                for(int i = 0; i < patients.size(); i++)
                {
                    if(patients[i] == patient_username)
                    {
                        found = true;
                    }
                }
                std::cout << (found) ? user.getPatientInfo(patient_username) : "User not found";
                break;
            case 3:
                change_prescription_data = change_prescription();
                if(change_prescription_data.size() == 1)
                {
                    std::cout << "Sorry that did not work\n";
                }
                else
                {
                    if(user.changePrescription(change_prescription_data[0], std::stoi(change_prescription_data[1]), std::stoi(change_prescription_data[2])))
                    {
                        std::cout << "The change has been made \n";
                    }
                    else
                    {
                        std::cout << "SOrry that user is not assigned to you \n";
                    }
                }
                break;
            case 4:
                patients = user.getUnassignedPatients();
                for(int i = 0; i < patients.size(); i++)
                {
                    std::cout << patients[i];
                }
            case 5:
                patients = user.getPatients();
                std::cout << "What is the name of the patient you want to assign: ";
                std::cin >> patient_username;
                for(int i = 0; i < patients.size(); i++)
                {
                    if(patients[i] == patient_username)
                    {
                        found = true;
                    }
                }
                if(found == false)
                {
                    std::cout << "Sorry they were not found";
                    break;
                }
                std::cout << "What is the name of the doctor you want to assign them to: ";
                std::cin >> doctor_username;
                for(int i = 0; i < doctors.size(); i++)
                {
                    if(doctors[i] == doctor_username)
                    {
                        found = true;
                    }
                }
                if(found == false)
                {
                    std::cout << "Sorry they were not found";
                    break;
                }
                user.assignPatient(patient_username, doctor_username);
                break; 
            case 6:
                std::cout << "What is the username of the doctor: ";
                std::cin >> new_doctor;
                std::cout << "What is their password: ";
                std::cin >> password;
                std::cout << "What is their age: ";
                std::cin >> age;
                createNewAccount(new_doctor, password, std::to_string(age), 2);
                break;
            case 7:
                std::cout << "What is the username of the doctor";
                std::cin >> new_doctor; 
                found = user.upgradeDoctor(new_doctor);
                std::cout << (found) ? "Doctor has been updated to a head doctor\n" : "Sorry that did not work, either doctor does not exist or is already a head doctor";
                break;
            case 8:
                loggedIn = false;
                break;
            default:
                std::cout << "Sorry please enter a correct value\n";
                break; 
        }
    }
}

void pharmacistMenu(std::string username)
{
    Pharmacist user(username);
    int choice = 0;
    bool loggedIn = true;

    clearTerm();

    while (loggedIn == true){
        std::cout << "Welcome Doctor: What would you like to do:\n";
        std::cout << "1. Get a patients prescriptions\n";
        std::cout << "2. Change a patients prescriptions\n";
        std::cout << "3. Create a new pharmacist\n";
        std::cout << "4. Log out\n";
        std::cout << ":: ";
        std::cin >> choice;
        std::string patient_username;
        std::string new_pharmacist;
        std::string password;
        int age;
        bool found = false;
        std::vector<std::vector<std::string>> all_user_data = extractCSV("users.csv");
        std::vector<std::string> patients;
        std::vector<std::string> change_prescription_data;
        for(int i = 0; i < all_user_data.size(); i++)
        {
            if(all_user_data[i][4] == "1")
            {
                patients.push_back(all_user_data[i][0]);
            }
        }
        switch(choice)
        {
            case 1:
                std::cout << "Please enter patient's username: ";
                std::cin >> patient_username;
                clearTerm();
                found = false;
                for(int i = 0; i < patients.size(); i++)
                {
                    if(patients[i] == patient_username)
                    {
                        found = true;
                    }
                }
                std::cout << (found) ? user.getPatientInfo(patient_username) : "User not found";
                break;
            case 2:
                change_prescription_data = change_prescription();
                if(change_prescription_data.size() == 1)
                {
                    std::cout << "Sorry that did not work\n";
                }
                else
                {
                    user.changePrescription(change_prescription_data[0], std::stoi(change_prescription_data[1]), std::stoi(change_prescription_data[2]));
                }
                break;
            case 3:
                std::cout << "What is the username of the pharmacist: ";
                std::cin >> new_pharmacist;
                std::cout << "What is their password: ";
                std::cin >> password;
                std::cout << "What is their age: ";
                std::cin >> age;
                createNewAccount(new_pharmacist, password, std::to_string(age), 4);
                break;
        }
    }
}

bool login()
{
    clearTerm();
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
            int int_rank = std::stoi(rank);
            switch (int_rank)
            {
            case 1:
                paitentMenu(username);
                break;
            case 2:
                doctorMenu(username);
                break;
            case 3:
                headDoctorMenu(username);
                break;
            case 4:
                pharmacistMenu(username);
                break;
            }
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
    clearTerm();
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
                clearTerm();
                break;
            case 2:
                users_data = accountCreation();
                createNewAccount(users_data[0], users_data[1], users_data[2], 1);
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