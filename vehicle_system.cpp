#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
using namespace std;

// Vehicle Structure
struct Vehicle {
    string regNo;
    string ownerName;
    string type;
    string company;
    int modelYear;
};

// Global STL Containers
vector<Vehicle> vehicles;                // maintains order 
map<string, Vehicle> vehicleMap;         // key = regNo,vehicle
set<string> vehicleTypes;                // sorted unique types
unordered_map<string, int> companyCount; // count by company

// 1. Register Vehicle
void registerVehicle() {
    Vehicle v;

    cout << "Enter Registration Number: ";
    cin >> v.regNo;

    if (vehicleMap.find(v.regNo) != vehicleMap.end()) {
        cout << "Error: Registration number already exists!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Owner Name: ";
    getline(cin, v.ownerName);

    cout << "Enter Vehicle Type (Car/Bike/Truck): ";
    cin >> v.type;

    cout << "Enter Company: ";
    cin >> v.company;

    cout << "Enter Model Year: ";
    cin >> v.modelYear;

    // Insert into all containers
    vehicles.push_back(v);
    vehicleMap[v.regNo] = v;
    vehicleTypes.insert(v.type);
    companyCount[v.company]++;

    cout << "Vehicle registered successfully!\n";
}
// 2. Search by Registration Number
void searchByRegNo() {
    string reg;
    cout << "Enter Registration Number to search: ";
    cin >> reg;

    auto it = vehicleMap.find(reg);
    if (it == vehicleMap.end()) {
        cout << "Vehicle not found.\n";
        return;
    }

    Vehicle v = it->second;
    cout << "Vehicle Found:\n";
    cout << "RegNo       : " << v.regNo << endl;
    cout << "Owner Name  : " << v.ownerName << endl;
    cout << "Type        : " << v.type << endl;
    cout << "Company     : " << v.company << endl;
    cout << "Model Year  : " << v.modelYear << endl;
}

// 3. Search by Owner Name
void searchByOwner() {
    cin.ignore();
    string name;
    cout << "Enter Owner Name to search: ";
    getline(cin, name);

    vector<Vehicle> results;

    // partial match using lambda
    for (auto &v : vehicles) {
        if (v.ownerName.find(name) != string::npos) {
            results.push_back(v);
        }
    }

    if (results.empty()) {
        cout << "No vehicles found for this owner.\n";
        return;
    }

    cout << "Vehicles registered under \"" << name << "\":\n";
    cout << "-----------------------------------------\n";
    for (auto &v : results) {
        cout << "RegNo       : " << v.regNo << endl;
        cout << "Type        : " << v.type << endl;
        cout << "Company     : " << v.company << endl;
        cout << "Model Year  : " << v.modelYear << endl;
        cout << "-----------------------------------------\n";
    }
}
// 4. Update Owner Name
void updateOwner() {
    string reg;
    cout << "Enter Registration Number to update: ";
    cin >> reg;

    auto it = vehicleMap.find(reg);
    if (it == vehicleMap.end()) {
        cout << "Registration number not found.\n";
        return;
    }

    cin.ignore();
    string newOwner;
    cout << "Enter new Owner Name: ";
    getline(cin, newOwner);

    // Update map
    it->second.ownerName = newOwner;

    // Update vector
    for (auto &v : vehicles) {
        if (v.regNo == reg) {
            v.ownerName = newOwner;
        }
    }

    cout << "Owner updated successfully!\n";
}
// 5. Delete Vehicle
void deleteVehicle() {
    string reg;
    cout << "Enter Registration Number to delete: ";
    cin >> reg;

    auto it = vehicleMap.find(reg);
    if (it == vehicleMap.end()) {
        cout << "Registration number not found. Cannot delete.\n";
        return;
    }

    string comp = it->second.company;
    string type = it->second.type;

    // Remove from map
    vehicleMap.erase(reg);

    // Remove from vector
    vehicles.erase(
        remove_if(vehicles.begin(), vehicles.end(),
                  [&](const Vehicle &v) { return v.regNo == reg; }),
        vehicles.end()
    );

    // Decrease company count
    companyCount[comp]--;
    if (companyCount[comp] == 0) companyCount.erase(comp);

    // Check if type still exists
    bool typeExists = false;
    for (auto &v : vehicles) {
        if (v.type == type) {
            typeExists = true;
            break;
        }
    }
    if (!typeExists) vehicleTypes.erase(type);

    cout << "Vehicle deleted successfully!\n";
}

//--------------------------------------
// 6. List All Vehicles (sorted by model year)
//--------------------------------------
void listAllVehicles() {
    if (vehicles.empty()) {
        cout << "No vehicles to list.\n";
        return;
    }

    vector<Vehicle> sortedVehicles = vehicles;

    sort(sortedVehicles.begin(), sortedVehicles.end(),
         [](const Vehicle &a, const Vehicle &b) {
             return a.modelYear < b.modelYear;
         });

    cout << "All Registered Vehicles (Sorted by Model Year)\n";
    cout << "---------------------------------------------------------\n";
    cout << left << setw(12) << "RegNo" << setw(15) << "Owner"
         << setw(10) << "Type" << setw(12) << "Company"
         << setw(6) << "Year" << endl;
    cout << "---------------------------------------------------------\n";

    for (auto &v : sortedVehicles) {
        cout << left << setw(12) << v.regNo
             << setw(15) << v.ownerName
             << setw(10) << v.type
             << setw(12) << v.company
             << setw(6) << v.modelYear << endl;
    }
    cout << "---------------------------------------------------------\n";
}

//--------------------------------------
// 7. Statistics
//--------------------------------------
void showStatistics() {
    cout << "---------------- Vehicle Statistics ----------------\n";
    cout << "Total Vehicles: " << vehicles.size() << endl;

    // company count
    cout << "Count by Company:\n";
    for (auto &p : companyCount) {
        cout << p.first << " -> " << p.second << endl;
    }

    // vehicle types
    cout << "Vehicle Types Available: ";
    for (auto &t : vehicleTypes) cout << t << " ";
    cout << endl;

    if (!vehicles.empty()) {
        auto oldest = min_element(vehicles.begin(), vehicles.end(),
                                  [](auto &a, auto &b) { return a.modelYear < b.modelYear; });

        auto newest = max_element(vehicles.begin(), vehicles.end(),
                                  [](auto &a, auto &b) { return a.modelYear < b.modelYear; });

        cout << "Oldest Model Year : " << oldest->modelYear << endl;
        cout << "Newest Model Year : " << newest->modelYear << endl;
    }

    cout << "Enter a year to check count after that year: ";
    int yr;
    cin >> yr;

    int count = count_if(vehicles.begin(), vehicles.end(),
                         [&](const Vehicle &v) { return v.modelYear > yr; });

    cout << "Vehicles after " << yr << ": " << count << endl;
}

// MAIN MENU
int main() {
    int choice;

    while (true) {
        cout << "\n-----------------------------------------\n";
        cout << "     VEHICLE REGISTRATION SYSTEM\n";
        cout << "-----------------------------------------\n";
        cout << "1. Register Vehicle\n";
        cout << "2. Search by Registration Number\n";
        cout << "3. Search by Owner Name\n";
        cout << "4. Update Owner\n";
        cout << "5. Delete Vehicle\n";
        cout << "6. List All Vehicles\n";
        cout << "7. Statistics\n";
        cout << "8. Exit\n";
        cout << "-----------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerVehicle(); break;
            case 2: searchByRegNo(); break;
            case 3: searchByOwner(); break;
            case 4: updateOwner(); break;
            case 5: deleteVehicle(); break;
            case 6: listAllVehicles(); break;
            case 7: showStatistics(); break;
            case 8: cout << "Exiting the system... Thank you!\n"; return 0;
            default: cout << "Invalid choice! Try again.\n"; break;
        }
    }
}
