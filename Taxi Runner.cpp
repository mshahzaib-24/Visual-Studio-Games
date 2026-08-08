#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

class Car {
	string name;
	double price;
	double earnings;
	bool owned;
	bool assigned;
public:
	Car(string name = "", double price = 0) {
		this->name = name;
		this->price = price;
		owned = false;
		assigned = false;
		earnings = price * 0.10;
	}
	string getName() {
		return name;
	}
	double getPrice() {
		return price;
	}
	double getEarnings() {
		return earnings;
	}
	bool getOwned() {
		return owned;
	}
	bool getAssigned() {
		return assigned;
	}
	void setOwned(bool owned) {
		this->owned = owned;
	}
	void setAssigned(bool assigned) {
		this->assigned = assigned;
	}
	void display() const {
		cout << name << " | $" << price << " | ($" << earnings << "/second)\n";
	}
};

//global variables
double bank = 1000.0;
double total = 0.0;
const double employeeSalary = 50.0;
int employeeCount = 0;
const int maxCars = 5;
Car cars[maxCars]{ 
Car("Toyota AE86", 800.0),
Car("Suzuki Mehran", 2000.0),
Car("Suzuki Alto", 4000.0),
Car("Suzuki Wagon R", 7500.0),
Car("Honda BRV", 14000.0)};

void renderMenu();
void handleMenuInput(char);
void manageEmployees();
void hireEmployee();
void fireEmployee();
void assignEmployee();
void buyCar();
void displayStats();

void displayCars() {
	cout << "ID & NAME | PRICE | EARNINGS PER SECOND\n";
	for (int i = 0; i < maxCars; i++) {
		cout << "[" << i << "] ";
		cars[i].display();
	}
}
void displayOwnedCars() {
	cout << "ID & NAME | PRICE | EARNINGS PER SECOND\n";
	for (int i = 0; i < maxCars; i++) {
		if (cars[i].getOwned() == true) {
			cout << "[" << i << "] ";
			cars[i].display();
		}
	}
}

int main() {
	ULONGLONG lastTick = GetTickCount64();
	const ULONGLONG interval_ms = 1000;

	cout << "Welcome to Taxi Runner!\n";
	renderMenu();
	while (true) {
		ULONGLONG now = GetTickCount64();
		if (now - lastTick >= interval_ms) {
			lastTick = now;
			bank += total;
		}
		if (_kbhit()) {
			char key = _getch();
			if (key == '4') {
				cout << "\n\nExiting...\n";
				return 1;
			}
			handleMenuInput(key);
			renderMenu();
		}
	}
	return 0;
}

void renderMenu() {
	cout << "\n--- Taxi Company Menu ---\n";
	cout << "[1] Manage Employees\n";
	cout << "[2] Buy Car\n";
	cout << "[3] View Statistics\n";
	cout << "[4] Exit\n";
	cout << "Enter: ";
}
void handleMenuInput(char key) {
	switch (key) {
	case '1': manageEmployees(); break;
	case '2': buyCar(); break;
	case '3': displayStats(); break;
	default: cout << "\nPressing that won't get you anywhere!\n"; break;
	}
}
void manageEmployees() {
	cout << "\n\n--- Employees Menu ---\n";
	cout << "[1] Hire Employee\n";
	cout << "[2] Fire Employee\n";
	cout << "[3] Assign Employee\n";
	cout << "[4] Return to Main Menu\n";
	cout << "Enter: ";
	char key;
	cin >> key;
	switch (key) {
	case '1': hireEmployee(); break;
	case '2': fireEmployee(); break;
	case '3': assignEmployee(); break;
	case '4': cout << "\nReturning to main menu...\n"; return;
	default: cout << "\nPressing this won't lead to anywhere either!\n";
	}
}
void hireEmployee() {
	cout << "\nAn employee costs $" << employeeSalary << " per second." << endl;
	cout << "Do you want to continue with this purchase? (y/n): ";
	char confirm;
	cin >> confirm;
	if ((confirm == 'y' || confirm == 'Y') && (bank == 0.0 && total <= 0.0)) {
		cout << "\nYou cannot buy an employee yet.\n";
	} else if (confirm == 'y' || confirm == 'Y') {
		cout << "\nEmployee hired!\n";
		total -= employeeSalary;
		employeeCount++;
	}
	else {
		cout << "\nPurchase cancelled.\n";
	}
}
void fireEmployee() {
	cout << "Are you sure you want to fire an employee? (y/n): ";
	char confirm;
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y') {
		total += employeeSalary;
		employeeCount--;
		cout << "\nEmployee fired!\n";
	}
	else {
		cout << "\nProcess cancelled.\n";
	}
}
void assignEmployee() {
	cout << "\n---- Garage ----\n";
	displayOwnedCars();
	cout << "Enter: ";
	int choice;
	cin >> choice;
	if (employeeCount>=1 && cars[choice].getOwned() == true && cars[choice].getAssigned() == false) {
		cars[choice].setAssigned(true);
		total += cars[choice].getEarnings();
		employeeCount--;
		cout << "\nEmployee assigned!\n";
	}
	else if (employeeCount == 0 && cars[choice].getOwned() == true) {
		cout << "\nYou do not have any employees!\n";
	}
	else if (employeeCount>=1 && cars[choice].getAssigned() == true) {
		cout << "\nYou cannot assign another driver!\n";
	}
	else if (cars[choice].getOwned()==false) {
		cout << "\nYou do not own this car!\n";
	}
	else {
		cout << "\nUnhandled exception.\n";
	}
}
void buyCar() {
	cout << "\n\n---- Dealership ----\n";
	displayCars();
	cout << "[" << maxCars << "] Exit\n";
	cout << "Enter: ";
	int choice;
	cin >> choice;
	if (choice == maxCars) {
		cout << "\nReturning...\n";
		return;
	}
	if (cars[choice].getOwned() == true) {
		cout << "\nYou already own this vehicle!" << endl;
	}
	else if (cars[choice].getPrice() >= bank) {
		cout << "\nYou cannot buy this car!" << endl;
	}
	else {
		bank -= cars[choice].getPrice();
		cars[choice].setOwned(true);
		cout << "\nPurchase successful!\n";
	}
}
void displayStats() {
	cout << "\n\n---- Status ----\n";
	cout << "Bank: $" << bank << endl;
	cout << "Total Earnings: $" << total << endl;
}
