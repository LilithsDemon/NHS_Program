#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>


//A salt is used to prepend to a password this allows for better security
//For a salt to work properly it has to be different each time the program is used by someone
//It also has to be random and long this will get prepended to the hashed password

void clearTerm()
{
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
    //1 = user 2= hospital staff
    
    std::string salt = generateSalt();
    unsigned long hashed_password = hashPassword(password, salt);

    std::string current_user_data = username + "," + std::to_string(hashed_password) + "," + salt + "," + age + "," + std::to_string(access); // Creates the data in the form of csv for the next person
    std::string current_verification_data = username + "," + "false";

    writeToCSV("users.csv", current_user_data);
    writeToCSV("verify.csv", current_verification_data);

}

bool checkForUsername(std::string username)
{
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
    std::string cancer;
    int cancer_type = 0;
    std::string diabetes;
    int diabetes_type = 0;
    std::string smoking;
    int smoking_type = 0;
    clearTerm();
    std::cout << "\nWelcome to the NHS program, this is a qestionaire so that you can get your medical data\n";
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
    verifyUser(username);
}

class User
{
    protected:
        std::string username;
};


class Pharmacist : public User
{
    public:
        Pharmacist(std::string given_username)
        {
            username = given_username;
        }
        Pharmacist()
        {

        }

        void changePrescription(std::string given_username, int condition, int prescription)
        {
            std::vector<std::vector<std::string>> all_prescriptions = extractCSV("prescriptions.csv");
            std::vector<std::string> current_patient;

            for(int i = 0; i < all_prescriptions.size(); i++)
            {
                if(given_username == all_prescriptions[i][0])
                {
                    current_patient = all_prescriptions[i];
                    break;
                }
            }

            current_patient[condition] = prescription;
        }
};

class Doctor : public Pharmacist
{
    protected:
        std::vector<std::string> assigned_patients;

        std::vector<std::string> getAssignedPatients()
        {
            std::vector<std::vector<std::string>> all_patients = extractCSV("assigned.csv");
            std::vector<std::string> patients;
            for(int i = 0; i < all_patients.size(); i++)
            {
                if(all_patients[i][0] == username)
                {
                    patients.push_back(all_patients[i][1]);
                }
            }
            
            return patients;
        }

    public:
        Doctor(std::string given_username)
        {
            username = given_username;
            assigned_patients = getAssignedPatients();
        }

        Doctor()
        {
            
        };

        std::vector<std::string> getPatients()
        {
            return assigned_patients;
        }

        std::string getPatientInfo(std::string given_username)
        {
            std::string data = given_username + ": Medical Data: ____\n__________________________________________\n";
            data += "Conditions: ____\n";
            
            Patient patient(given_username);
            std::vector<std::string> conditions = patient.userConditions();
            std::vector<std::string> prescriptions = patient.userPrescriptions();
            std::vector<std::vector<std::string>> prescription_data;
            int val;
            for(int i = 1; i < conditions.size(); i++)
            {
                val = std::stoi(conditions[i]);
                if(val == 0)
                {
                    data += "    Patient does not ";
                    switch(i)
                    {
                        case 1:
                            data += "have diabetes\n";
                            break;
                        case 2:
                            data += "smoke\n";
                            break;
                        case 3:
                            data += "have cancer\n";
                            break;
                    }
                }
                else
                {
                    val -= 1;
                    switch(i)
                    {
                        case 1:
                            prescription_data = extractCSV("diabetes.csv");
                            data += "    Patient has: ";
                            break;
                        case 2:
                            prescription_data = extractCSV("smoking.csv");
                            data += "    Patient is: ";
                            break;
                        case 3:
                            prescription_data = extractCSV("cancer.csv");
                            data += "    Patient has: ";
                            break;
                    }
                    
                    data += prescription_data[val][1] + "\n";
                }
            }
            data += "\nPrescriptions____\n";
            int prescription_base_val;
            for(int i = 1; i < prescriptions.size(); i++)
            {
                val = std::stoi(prescriptions[i]);
                if(val > 0)
                {
                    val -= 1;
                    prescription_data = extractCSV("treatments.csv");
                    switch(i)
                    {
                        case 1:
                            prescription_base_val = 1;
                            break;
                        case 2:
                            prescription_base_val = 3;
                            break;
                        case 3:
                            prescription_base_val = 7;
                            break;
                    }
                    data += "    Patient takes " + prescription_data[prescription_base_val + val][2] + prescription_data[prescription_base_val + val][3] + prescription_data[prescription_base_val + val][5] + "\n";
                }
            }
            return data;
        }
};

class HeadDoctor : public Doctor
{
    private:
        std::vector<std::string> doctors;
    public:
        HeadDoctor(std::string given_username)
        {
            username = given_username;
            assigned_patients = getAssignedPatients();
            doctors = getAllDoctors();
        }

        void assignPatient(std::string username, std::string doctor)
        {
            std::vector<std::vector<std::string>> all_assigned = extractCSV("assigned.csv");
            bool found = false;
            for(int i = 0; i < all_assigned.size(); i++)
            {
                if(all_assigned[i][0] == username)
                {
                    found = true;
                    all_assigned[i][1] = doctor;
                }
            }
            if(found != true)
            {
                std::vector<std::string> to_add;
                to_add.push_back(username);
                to_add.push_back(doctor);
                all_assigned.push_back(to_add);
            }

            //Write to CSV
            std::string to_write;
            for(int i = 0; i < all_assigned.size(); i++)
            {
                to_write += all_assigned[i][0] + ", " + all_assigned[i][1] + "\n";
            }
            writeToCSV("assigned.csv", to_write);
        }

        std::vector<std::string> getAllDoctors()
        {
            std::vector<std::vector<std::string>> all_users = extractCSV("users.csv");
            std::vector<std::string> all_doctors;
            for(int i = 0; i < all_users.size(); i++)
            {
                if(all_users[i][4] == "2" || all_users[i][4] == "3")
                {
                    all_doctors.push_back(all_users[i][0]);
                }
            }
            return all_doctors;
        }

        std::vector<std::string> getUnassignedPatients()
        {
            std::vector<std::vector<std::string>> all_users = extractCSV("users.csv");
            std::vector<std::vector<std::string>> all_assigned = extractCSV("assigned.csv");
            std::vector<std::string> unassigned_patients;
            bool found = false;

            for(int i = 0; i < all_users.size(); i++)
            {
                if(all_users[i][4] == "1")
                {
                    found = false;
                    for(int x = 0; x < all_assigned.size(); x++)
                    {
                        if(all_users[i][4] == all_assigned[x][1])
                        {
                            found = true;
                            break;
                        }
                    }
                    if(found == false)
                    {
                        unassigned_patients.push_back(all_users[i][0]);
                    }
                }
            }

            return unassigned_patients;
        }

        bool upgradeDoctor(std::string given_username)
        {
            std::vector<std::vector<std::string>> doctors = extractCSV("users.csv");
            bool found = false;
            for(int i = 0; i < doctors.size(); i++ )
            {
                if(doctors[i][0] == given_username)
                {
                    if(std::stoi(doctors[i][4]) == 2)
                    {
                        found = true;
                        doctors[i][4] = std::to_string(std::stoi(doctors[i][4]) + 1);
                        std::string to_write;
                        for(int x = 0; x < doctors.size(); x++)
                        {
                            to_write += doctors[x][0] + ", " + doctors[x][1] + ", " + doctors[x][2] + ", " + doctors[x][3] + ", " + doctors[x][4] + ", " + doctors[x][5] + "\n";
                        }
                        writeToCSV("users.csv", to_write);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            if(found == false)
            {
                return false;
            }
        }
};

class Patient : public User
{
    private:
        std::vector<std::string> conditions;
        std::vector<std::string> prescriptions;
        double cost; //per day

        std::vector<std::string> getConditions(std::string username, bool type = true)
        {
            //True = conditions, False = prescriptions
            std::vector<std::vector<std::string>> all_conditions = (type) ? extractCSV("conditions.csv") : extractCSV("prescriptions.csv");
            int pos = 0;
            for(int i = 0; i < all_conditions.size(); i++)
            {
                if(all_conditions[i][0] == username)
                {
                    pos = i;
                    break;
                }
            }

            std::vector<std::string> current_conditions;

            for(int i = 1; i < all_conditions[pos].size(); i++)
            {
                current_conditions.push_back(all_conditions[pos][i]);
            }
            return current_conditions;
        }

        double getCost (std::vector<std::string> conditions)
        {
            double cost = 0;
            std::map<int, std::string> condition_map{{0, "diabetes.csv"}, {1, "cancer.csv"}, {2, "smoking.csv"}};
            int val = 0;
            for(int i = 0; i < 3; i++)
            {
                val = std::stoi(conditions[i]);
                val -= 1;
                if(val != -1)
                {
                    cost = cost + (std::stod(extractCSV(condition_map[i])[val][3]) * std::stod(extractCSV(condition_map[i])[val][4]));
                }
            }
            return cost;
        }

    public:
        Patient(std::string given_username)
        {
            username = given_username;
            conditions = getConditions(username);
            prescriptions = getConditions(username, false);
            cost = getCost(conditions);
        }

        std::vector<std::string> userConditions() 
        {
            return conditions;
        }

        std::vector<std::string> userPrescriptions()
        {
            return prescriptions;
        }

        double userCost()
        {
            return cost;
        }

        void refresh()
        {
            conditions = getConditions(username);
            prescriptions = getConditions(username, false);
            cost = getCost(conditions);
        }
};

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

    Patient user(username);

    int choice = 0;
    bool loggedIn = true;

    clearTerm();

    while (loggedIn == true){
        std::cout << "Welcome patient: What would you like to do:\n";
        std::cout << "1. Redo form\n";
        std::cout << "2. Get daily cost for treatments\n";
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

void doctorMenu(std::string username)
{
    Doctor user(username);
    int choice = 0;
    bool loggedIn = true;

    std::vector<std::string> patients = user.getPatients();

    clearTerm();

    while (loggedIn == true){
        std::cout << "Welcome Doctor: What would you like to do:\n";
        std::cout << "1. Get a list of your assigned patients\n";
        std::cout << "2. Get a patients conditions and prescriptions\n";
        std::cout << "3. Log out\n";
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
                std::cout << (found) ? user.getPatientInfo(patient_username) : "User not found";
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
        std::cout << "3. Get a list of the unassigned patients\n";
        std::cout << "4. Assign a patient to a doctor\n";
        std::cout << "5. Create a new doctor\n";
        std::cout << "6. Upgrade a Doctor to head doctor\n";
        std::cout << "7. Log out\n";
        std::cout << ":: ";
        std::cin >> choice;
        std::string patient_username;
        std::string doctor_username;
        std::string new_doctor;
        std::string password;
        std::string doctor;
        int age;
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
                bool found = false;
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
                patients = user.getUnassignedPatients();
                for(int i = 0; i < patients.size(); i++)
                {
                    std::cout << patients[i];
                }
            case 4:
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
            case 5:
                std::cout << "What is the username of the doctor";
                std::cin >> new_doctor;
                std::cout << "What is their password";
                std::cin >> password;
                std::cout << "What is their age: ";
                std::cin >> age;
                createNewAccount(new_doctor, password, std::to_string(age), 2);
                break;
            case 6:
                std::cout << "What is the username of the doctor";
                std::cin >> new_doctor; 
                bool success = user.upgradeDoctor(new_doctor);
                std::cout << (success) ? "Doctor has been updated to a head doctor\n" : "Sorry that did not work, either doctor does not exist or is already a head doctor";
                break;
            case 7:
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
    clearTerm();
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
    //startMenu();

    HeadDoctor bob("Bob");

    return 0;
}