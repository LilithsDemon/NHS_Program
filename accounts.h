#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <files.h>

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

        bool changePrescription(std::string given_username, int condition, int prescription)
        {
            std::vector<std::vector<std::string>> all_prescriptions = extractCSV("prescriptions.csv");
            std::vector<std::string> current_patient;
            bool found = false;

            for(int i = 0; i < all_prescriptions.size(); i++)
            {
                if(given_username == all_prescriptions[i][0])
                {
                    current_patient = all_prescriptions[i];
                    found = true;
                    break;
                }
            }

            if (found == true)
            {
                current_patient[condition] = prescription;
                return true;
            }
            else
            {
                return false;
            }
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

        bool changePrescription(std::string given_username, int condition, int prescription)
        {

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