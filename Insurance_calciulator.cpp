#include <iostream>
#include <limits> // For input validation
using namespace std;

const double INSURANCE_RATE = 0.05; // 5% insurance premium rate

void welcomeMessage() {
    cout << "=========================================\n";
    cout << "  Welcome to the Insurance Calculator!  \n";
    cout << "=========================================\n\n";
}

// Function for selecting motor vehicle type
double selectMotorVehicleType() {
    int choice;
    double third_party_rate = 0.0;
    
    cout << "Available Motor Vehicle Types:\n";
    cout << "1. Private Individual\n2. Private Corporate\n3. Taxi\n";
    cout << "Enter your choice (1-3): ";
    
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid input! Please enter a valid option (1-3): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    switch (choice) {
        case 1:
            cout << "Selected Motor Vehicle Type: Private Individual\n\n";
            third_party_rate = 392.0;
            break;
        case 2:
            cout << "Selected Motor Vehicle Type: Private Corporate\n\n";
            third_party_rate = 392.0;
            break;
        case 3:
            cout << "Selected Motor Vehicle Type: Taxi\n\n";
            third_party_rate = 539.0;
            break;
    }
    cout<<endl;
    return third_party_rate;
    
}

// Function to select currency and get exchange rate
double selectCurrency() {
    int choice;
    double exchangeRate = 1.0; // Default for GHC
    
    cout << "Available Currencies:\n";
    cout << "1. GHC\n2. USD\n3. EUR\n";
    cout << "Enter your choice (1-3): ";
    
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid input! Please enter a valid option (1-3): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    switch (choice) {
        case 1:
            cout << "Selected Currency: GHC\n\n";
            exchangeRate = 1.0;
            break;
        case 2:
            cout << "Selected Currency: USD\nEnter exchange rate: ";
            while (!(cin >> exchangeRate) || exchangeRate <= 0) {
                cout << "Invalid exchange rate! Please enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 3:
            cout << "Selected Currency: EUR\nEnter exchange rate : ";
            while (!(cin >> exchangeRate) || exchangeRate <= 0) {
                cout << "Invalid exchange rate! Please enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
    }
    cout << endl;
    return exchangeRate;
    
}

// Function to get the multiplier based on the number of insured days
double getMultiplier() {
    int days_insured;
    
    cout << "Enter the number of Days you want to insure the vehicle: ";
    while (!(cin >> days_insured) || days_insured <= 0 || days_insured > 365) {
        cout << "Invalid input! Please enter a number between 1 and 365: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return static_cast<double>(days_insured) / 365.0;
}

// Function for property damage coverage
double getPropertyDamageCoverage(double exchangeRate) {
    cout << "Enter the property damage coverage percentage (greater than 6000): ";
    double coverage_percentage;
    
    // Input validation to ensure the value is greater than 6000
    while (!(cin >> coverage_percentage) || coverage_percentage < 6000) {
        cout << "Invalid coverage percentage! Please enter a number greater than 6000: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Calculate the difference above 6000
    double difference = coverage_percentage - 6000;
    
    // Apply the 2% rule if the coverage is above 6000
    double coverage = (difference / exchangeRate) * 0.02;
    
    cout << "Total Property Damage Coverage: " << coverage << endl;
    
    return coverage;
}

// Function to calculate insurance premium
void calculateInsurancePremium(double exchangeRate, double third_party_rate) {
    double value_of_vehicle;
    
    cout << "Enter the Value of the vehicle: ";
    while (!(cin >> value_of_vehicle) || value_of_vehicle <= 0) {
        cout << "Invalid vehicle value! Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    double property_coverage = getPropertyDamageCoverage(exchangeRate);
    
    // Get the multiplier for the insurance duration
    double multiplier = getMultiplier();
    
    // Convert third-party rate to selected currency (without applying multiplier here)
    double third_party_coverage = (third_party_rate / exchangeRate) * multiplier;
    
    // Calculate the insurance premium using the multiplier
    double premium = INSURANCE_RATE * value_of_vehicle * multiplier;
    
    // Calculate total cost
    double total_cost = third_party_coverage + premium + property_coverage;
    
    cout << "\n=========================================\n";
    cout << " Third-Party Rate: " << third_party_coverage << " GHC\n";
    cout << " Insurance Premium: " << premium << " GHC\n";
    cout << " Total Cost: " << total_cost << " GHC\n";
    cout << "=========================================\n";
}

int main() {
    welcomeMessage();
    double third_party_rate = selectMotorVehicleType();
    double exchangeRate = selectCurrency();
    calculateInsurancePremium(exchangeRate, third_party_rate);
    
    return 0;
}
