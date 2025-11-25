
#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
#include<set>
#include<unordered_map>
using namespace std;

struct Vehicle
{
    string regNo;
    string owner;
    string vehicle_type;
    string vehicle_company;
    int year;
};

vector<Vehicle> vehicles; // for vehicles
map<string, Vehicle> mapVehicle;//for vehicles with regNo.
set<string> vehicleTypes;// for vehicle model
unordered_map<string,int> companyCount;//counting vehicle by company.

void vehicleRegistration()
{
    Vehicle v;
    cout<<"Enter Registration Number:\n";
    cin>>v.regNo;
    
    if(mapVehicle.find(v.regNo)!=mapVehicle.end())
    {
        cout << "Error: Registration number already exists!\n";
        return;
    }
    cout<<"Enter owner name\n";
    cin>>v.owner;

    cout<<"Enter Vehicle Type (Car/Bike/Truck):\n";
    cin>>v.vehicle_type;

    cout<<"Enter Company:\n";
    cin>>v.vehicle_company;

    cout<<"Enter Model Year:\n";
    cin>>v.year;

    vehicles.push_back(v);
    mapVehicle[v.regNo] = v;
    vehicleTypes.insert(v.vehicle_type);
    companyCount[v.vehicle_company]++;

    cout<<"Vehicle registered successfully!\n";


}
void searchByRegNo()
{
    string reg;
    cout<<"Enter Registration Number to search:\n";
    cin>>reg;

    auto it = mapVehicle.find(reg);
    //if not found
    if (it == mapVehicle.end()) {
        cout << "Vehicle not found.\n";
        return;
    }

    //if found
    Vehicle v = it->second;
    cout<<"Vehicle Found:\n"; 
    cout<<"RegNo       : "<<v.regNo<<"\n"; 
    cout<<" Owner Name  : "<<v.owner<<"\n";
    cout<<"Type        :"<<v.vehicle_type<<"\n";
    cout<<"Company     :"<<v.vehicle_company<<"\n";
    cout<<"Model Year  :"<<v.year<<"\n";
    

}
void searchByOwner()
{
    cin.ignore();
    string name;
    cout<<"Enter Owner Name to search:\n";
    getline(cin, name);
    
    vector<Vehicle> results;
    for (auto &v : vehicles) {
        if (v.owner.find(name) != string::npos) {
            results.push_back(v);
        }
    }

    //If Not found
    if (results.empty()) {
        cout << "No vehicles found for this owner.\n";
        return;
    }
    //If found
    cout<<"Vehicles registered under \""<<name<<"\"\n";
    for (auto &v : results) {
        cout<<"RegNo       :\n";
        cout<<"Type        :"<<v.vehicle_type<<"\n";
        cout<<"Company     :"<<v.vehicle_company<<"\n";
        cout<<"Model Year  :"<<v.year<<"\n";
    }

}
void updateOwner()
{
    string reg;
    cout<<"Enter Registration Number to update:\n";
    cin>>regNo;

    auto it = mapVehicle.find(reg);
    if(it==mapVehicle.end())
    {
        cout<<"Vehicle not found.\n";
        return;
    }
    string newOwner;
    cin.ignore();
    cout<<"Enter new Owner Name: ";
    getline(cin, newOwner);
    //update map
    it->second.owner = newOwner;

    //update vector
    for(auto &v : vehicles)
    {
        if(v.regNo==reg)
        {
            v.owner=newOwner;
            break;
        }
    }

    cout<<"Owner updated successfully!";

}
void deleteVehicle()
{
    string reg;
    cout<<"Enter Registration Number to delete:\n";
    cin>>regNo;

    auto it = mapVehicle.find(reg);
    //if not found.
    if (it == mapVehicle.end()) {
        cout << "Registration number not found. Cannot delete.\n";
        return;
    }
    //if found.

    string comp = it->second.vehicle_company;
    string type = it->second.vehicle_type;

    // Remove from map
    mapVehicle.erase(reg);

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
        if (v.vehicle_type == type) {
            typeExists = true;
            break;
        }
    }
    if (!typeExists) vehicleTypes.erase(type);
    cout<<"Vehicle deleted successfully!";


}
void showVehicles()
{
    if(vehicles.empty())
    {
        cout<<"No vehicle present to show.\n";
        return ;
    }

    vector<Vehicle> sortedVehicles = vehicles;
    sort(sortedVehicles.begin(), sortedVehicles.end(),
    [](const Vehicle &a, const Vehicle &b)
    {
        return a.year<b.year;
    });

    cout<<"------All Registered Vehicles (Sorted by Model Year)-------\n";
    cout<<"------RegNo"<<"        Owner"<<"        type"<<"        Company"<<"        Year\n";
    cout<<"------";
    for( auto &v: sortedVehicles)
    {
        cout<<v.regNo<<"        ";
        cout<<v.owner<<"        ";
        cout<<v.vehicle_type<<"        ";
        cout<<v.vehicle_company<<"        ";
        cout<<v.year<<"\n";

    }
    

}
void vehicleStatistics()
{
    
    cout<<"---------------- Vehicle Statistics ----------------\n";
    cout<<"Total Vehicles: "<< vehicles.size()<<"\n";

    cout<<"Count by Company: \n";
    for(auto &p : companyCount)
    {
        cout << p.first << " -> " << p.second << "\n";
    } 

    cout<<"Vehicle Types Available: "<<" "<<"\n";
    for(auto &t : vehicleTypes)
    {
        cout<<t<<" ";
    }
    cout<<"\n";

    if(!vehicles.empty())
    {
        auto oldestModelyr = min_element(vehicles.begin(),vehicles.end(),
        [](auto &a, auto &b){
        return a.year<b.year;
        });
        auto newestModelyr = max_element(vehicles.begin(),vehicles.end(),
        [](auto &a, auto &b){
        return a.year<b.year;
        });
        cout<<"Oldest Model Year : "<< oldestModelyr->year <<"\n";
        cout<<"Newest Model Year : "<< newestModelyr->year <<"\n";
    }

    int yr;
    cout<<"Enter a year to check count after that year:";
    cin>>yr;

    int count = count_if(vehicles.begin(),vehicles.end(),
    [&](const Vehicle &v ){
        return v.year>yr;
    });
    cout<<"Vehicles after "<<yr<<": "<<count<<"\n";

}

int main()
{
    int choice;
    cout<<"\n-----Vehicle Registration Sytem--------\n";
    cout<<"1.Enter Registration Number:\n";
    cout<<"2.Enter Registration Number to search:\n";
    cout<<"3.Enter Owner Name to search:\n";
    cout<<"4.Enter Registration Number to update:\n";
    cout<<"5.Enter Registration Number to delete:\n";
    cout<<"6.List all the Vehicles\n";
    cout<<"7.Statistics\n";
    cout<<"8.Exit\n";
    cout<<"Enter the choice:\n";

    cin>>choice;
    switch (choice)
    {
        case 1:vehicleRegistration();
        break;
        case 2:searchByRegNo();
        break;
        case 3:searchByOwner();
        break;
        case 4:updateOwner();
        break;
        case 5:deleteVehicle();
        break;
        case 6:showVehicles();
        break;
        case 7:vehicleStatistics();
        break;
        case 8:cout<<"Exiting the system... Thank you!";
        return 0;
    
    default:cout<<"Enter the valid number";
        break;
    }

    return 0;
}