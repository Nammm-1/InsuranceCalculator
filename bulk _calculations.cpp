// for bulk calulations

#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

const double INSURANCE_RATE = 0.05;

void welcomeMessage() {
    cout << "=========================================" << endl;
    cout << "  Welcome to the Insurance Calculator!  " << endl;
    cout << "=========================================" << endl << endl;
}

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
        case 2:
            third_party_rate = 392.0;
            break;
        case 3:
            third_party_rate = 539.0;
            break;
    }
    return third_party_rate;
}

double selectCurrency(string &currency_symbol) {
    int choice;
    double exchangeRate = 1.0;

    cout << "Available Currencies:\n1. GHC\n2. USD\n3. EUR\n";
    cout << "Enter your choice (1-3): ";

    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid input! Please enter a valid option (1-3): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    switch (choice) {
        case 1:
            currency_symbol = "GHC";
            exchangeRate = 1.0;
            break;
        case 2:
            currency_symbol = "USD";
            cout << "Enter exchange rate: ";
            while (!(cin >> exchangeRate) || exchangeRate <= 0) {
                cout << "Invalid exchange rate! Please enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 3:
            currency_symbol = "EUR";
            cout << "Enter exchange rate: ";
            while (!(cin >> exchangeRate) || exchangeRate <= 0) {
                cout << "Invalid exchange rate! Please enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
    }
    return exchangeRate;
}

double getMultiplier() {
    int days_insured;
    cout << "Enter the number of days you want to insure the vehicle: ";
    while (!(cin >> days_insured) || days_insured <= 0 || days_insured > 365) {
        cout << "Invalid input! Please enter a number between 1 and 365: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return static_cast<double>(days_insured) / 365.0;
}

double getPropertyDamageCoverage(double exchangeRate) {
    cout << "Enter your preferred property damage coverage amount (minimum 6000 GHC): ";
    double coverage_amount;

    while (!(cin >> coverage_amount) || coverage_amount < 6000) {
        cout << "Invalid input! Please enter an amount greater than or equal to 6000: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    double additional_coverage = (coverage_amount - 6000) * 0.02 / exchangeRate;
    return additional_coverage;
}

double calculateInsurancePremium(double exchangeRate, double third_party_rate, string currency_symbol) {
    double value_of_vehicle;
    cout << "Enter the Value of the vehicle: ";
    while (!(cin >> value_of_vehicle) || value_of_vehicle <= 0) {
        cout << "Invalid vehicle value! Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    double property_coverage = getPropertyDamageCoverage(exchangeRate);
    double multiplier = getMultiplier();
    double third_party_coverage = (third_party_rate / exchangeRate) * multiplier;
    double premium = INSURANCE_RATE * value_of_vehicle * multiplier;
    double total_cost = third_party_coverage + premium + property_coverage;

    cout << fixed << setprecision(2);
    cout << "\n=========================================" << endl;
    cout << " Third-Party Rate: " << third_party_coverage * exchangeRate << " " << currency_symbol << endl;
    cout << " Insurance Premium: " << premium * exchangeRate << " " << currency_symbol << endl;
    cout << " Total Cost: " << total_cost * exchangeRate << " " << currency_symbol << endl;
    cout << "=========================================" << endl;
    return total_cost * exchangeRate;
}

void bulkCalculation(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    double grand_total = 0.0;
    string currency_symbol;
    double exchangeRate = 1.0;

    // Print table header
    cout << "| No. | Vehicle Type      | Value      | Days Insured | Premium      | Currency |" << endl;
    cout << "---------------------------------------------------------------" << endl;

    int index = 1;

    // Read each line from the CSV file
    while (getline(file, line)) {
        stringstream ss(line);
        double third_party_rate = 0.0;
        double vehicle_value;
        int days_insured;
        string vehicle_type;

        // Parse CSV line
        getline(ss, vehicle_type, ',');
        ss >> vehicle_value;
        ss.ignore();
        ss >> days_insured;

        // Determine third-party rate based on vehicle type
        if (vehicle_type == "Private Individual" || vehicle_type == "Private Corporate") {
            third_party_rate = 392.0;
        } else if (vehicle_type == "Taxi") {
            third_party_rate = 539.0;
        } else {
            cout << "Invalid vehicle type: " << vehicle_type << endl;
            continue;
        }

        // Calculate multiplier based on days insured
        double multiplier = static_cast<double>(days_insured) / 365.0;
        double premium = INSURANCE_RATE * vehicle_value * multiplier;
        double third_party_coverage = (third_party_rate / exchangeRate) * multiplier;
        double total_cost = third_party_coverage + premium;

        grand_total += total_cost;

        // Print the row data
        cout << "| " << setw(3) << index 
             << " | " << setw(17) << vehicle_type 
             << " | " << setw(10) << vehicle_value 
             << " | " << setw(12) << days_insured 
             << " | " << setw(10) << total_cost * exchangeRate 
             << " | " << currency_symbol << " |" << endl;

        index++;
    }

    cout << "---------------------------------------------------------------" << endl;
    cout << "\nGRAND TOTAL: " << grand_total * exchangeRate << " " << currency_symbol << endl;

    file.close();
}

int main() {
    welcomeMessage();
    double grand_total = 0;
    string currency_symbol;
    char choice;

    do {
        cout << "Do you want to calculate insurance for a single vehicle (s) or in bulk from a CSV file (b)? (s/b): ";
        cin >> choice;
        while (choice != 's' && choice != 'b') {
            cout << "Invalid input! Enter 's' for single or 'b' for bulk: ";
            cin >> choice;
        }

        if (choice == 's') {
            double third_party_rate = selectMotorVehicleType();
            double exchangeRate = selectCurrency(currency_symbol);
            double total_premium = calculateInsurancePremium(exchangeRate, third_party_rate, currency_symbol);
            grand_total += total_premium;
        } else {
            string filename;
            cout << "Enter the CSV file name (with extension): ";
            cin >> filename;
            double exchangeRate = selectCurrency(currency_symbol);
            bulkCalculation(filename);
        }

        cout << "\nDo you want to calculate another premium? (y/n): ";
        cin >> choice;
        while (choice != 'y' && choice != 'n') {
            cout << "Invalid input! Enter 'y' for yes or 'n' for no: ";
            cin >> choice;
        }
    } while (choice == 'y');

    cout << "\n=========================================" << endl;
    cout << " GRAND TOTAL: " << grand_total << " " << currency_symbol << endl;
    cout << "=========================================" << endl;
    return 0;
}