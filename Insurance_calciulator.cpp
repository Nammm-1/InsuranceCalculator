#include <iostream>
#include <limits>
#include <iomanip>
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

int main() {
    welcomeMessage();
    double grand_total = 0;
    string currency_symbol;
    char choice;
    
    do {
        double third_party_rate = selectMotorVehicleType();
        double exchangeRate = selectCurrency(currency_symbol);
        double total_premium = calculateInsurancePremium(exchangeRate, third_party_rate, currency_symbol);
        grand_total += total_premium;
        
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
