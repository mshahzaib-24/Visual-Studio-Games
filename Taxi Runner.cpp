#include<iostream>
#include<fstream>
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
int unassignedEmployeeCount = 0;
int assignedEmployeeCount = 0;
int ownedCarsCount = 0;
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
void unassignEmployee();
void manageCars();
void viewGarage();
void buyCar();
void sellCar();
void displayStats();
void saveGame();
void loadGame();

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
			if (key == '6') {
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
	cout << "[2] Manage Cars\n";
	cout << "[3] View Statistics\n";
	cout << "[4] Save Game\n";
	cout << "[5] Load Game\n";
	cout << "[6] Exit\n";
	cout << "Enter: ";
}
void handleMenuInput(char key) {
	switch (key) {
	case '1': manageEmployees(); break;
	case '2': manageCars(); break;
	case '3': displayStats(); break;
	case '4': saveGame(); break;
	case '5': loadGame(); break;
	default: cout << "\n\nPressing that won't get you anywhere!\n"; break;
	}
}
void manageEmployees() {
	cout << "\n\n--- Employees Menu ---\n";
	cout << "[1] Hire Employee\n";
	cout << "[2] Fire Employee\n";
	cout << "[3] Assign Employee\n";
	cout << "[4] Un-assign Employee\n";
	cout << "[5] Return to Main Menu\n";
	cout << "Enter: ";
	char key;
	cin >> key;
	switch (key) {
	case '1': hireEmployee(); break;
	case '2': fireEmployee(); break;
	case '3': assignEmployee(); break;
	case '4': unassignEmployee(); break;
	case '5': cout << "\nReturning to main menu...\n"; return;
	default: cout << "\nPressing that won't lead to anywhere either!\n";
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
		unassignedEmployeeCount++;
	}
	else {
		cout << "\nPurchase cancelled.\n";
	}
}
void fireEmployee() {
	cout << "\nAre you sure you want to fire an employee? (y/n): ";
	char confirm;
	cin >> confirm;
	if ((confirm == 'y' || confirm == 'Y') && unassignedEmployeeCount > 0) {
		total += employeeSalary;
		unassignedEmployeeCount--;
		cout << "\nEmployee fired!\n";
	}
	else if ((confirm == 'y' || confirm == 'Y') && unassignedEmployeeCount == 0) {
		cout << "\nYou don't have any (unassigned) employees to fire!\n";
	}
	else {
		cout << "\nProcess cancelled.\n";
	}
}
void assignEmployee() {
	if (ownedCarsCount == 0) {
		cout << "\nYou don't own any cars!\n";
		return;
	}
	if (unassignedEmployeeCount == 0) {
		cout << "\nYou don't have any employees!\n";
		return;
	}
	cout << "\n---- Garage ----\n";
	displayOwnedCars();
	cout << "[" << ownedCarsCount << "] Return to main menu\n";
	cout << "Enter: ";
	int choice;
	cin >> choice;
	if (choice == ownedCarsCount) {
		cout << "\nReturning to main menu...\n";
		return;
	}
	if (cars[choice].getOwned() == true && cars[choice].getAssigned() == false) {
		cars[choice].setAssigned(true);
		total += cars[choice].getEarnings();
		unassignedEmployeeCount--;
		assignedEmployeeCount++;
		cout << "\nEmployee assigned!\n";
	}
	else if (cars[choice].getAssigned() == true) {
		cout << "\nYou cannot assign another driver!\n";
	}
	else if (cars[choice].getOwned()==false) {
		cout << "\nYou don't own this car!\n";
	}
	else {
		cout << "\nUnhandled exception.\n";
	}
}
void unassignEmployee() {
	if (ownedCarsCount == 0) {
		cout << "\nYou don't own any cars!\n";
		return;
	}
	else if (assignedEmployeeCount == 0) {
		cout << "\nThere's no assigned employees!\n";
		return;
	}
	cout << "\n---- Garage ----\n";
	displayOwnedCars();
	cout << "[" << ownedCarsCount << "] Return to main menu\n";
	cout << "Enter: ";
	int choice;
	cin >> choice;
	if (choice == ownedCarsCount) {
		cout << "\nReturning to main menu...\n";
		return;
	}
	if (cars[choice].getOwned() == true && cars[choice].getAssigned() == true && assignedEmployeeCount > 0) {
		cars[choice].setAssigned(false);
		total -= cars[choice].getEarnings();
		assignedEmployeeCount--;
		unassignedEmployeeCount++;
		cout << "\nEmployee un-assigned!\n";
	}
	else if (cars[choice].getOwned() == false) {
		cout << "\nYou don't own this car!\n";
	}
	else if (cars[choice].getOwned() == true && cars[choice].getAssigned() == false) {
		cout << "\nThis car doesn't have any assigned employees!\n";
	}
	else {
		cout << "\nUnhandled exception!\n";
	}
}
void manageCars() {
	cout << "\n\n--- Cars Menu ---\n";
	cout << "[1] View Garage\n";
	cout << "[2] Buy Car\n";
	cout << "[3] Sell Car\n";
	cout << "[4] Return to Main Menu\n";
	cout << "Enter: ";
	char key;
	cin >> key;
	switch (key) {
	case '1': viewGarage(); break;
	case '2': buyCar(); break;
	case '3': sellCar(); break;
	case '4': cout << "\nReturning to main menu...\n"; return;
	default: cout << "\nPressing that won't lead to anywhere either!\n";
	}
}
void viewGarage() {
	if (ownedCarsCount == 0) {
		cout << "\nYou don't own any cars!\n";
		return;
	}
	cout << "\n---- Garage ----\n";
	displayOwnedCars();
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
		ownedCarsCount++;
		cout << "\nPurchase successful!\n";
	}
}
void sellCar() {

}
void displayStats() {
	cout << "\n\n---- Status ----\n";
	cout << "Bank: $" << bank << endl;
	cout << "Total Earnings: $" << total << endl;
	cout << "Employees: " << assignedEmployeeCount+ unassignedEmployeeCount << endl;
	cout << "Cars: " << ownedCarsCount << endl;
}
void saveGame() {
	ofstream write("savefile.txt");
	if (write.is_open()) {
		write << bank << endl;
		write << total << endl;
		write << unassignedEmployeeCount << endl;
		write << assignedEmployeeCount << endl;
		write << ownedCarsCount << endl;
		for (int i = 0; i < maxCars; i++) {
			if (cars[i].getAssigned() == true) {
				write << i << endl;
			}
		}
		write.close();
		cout << "\n\nFile saved.\n";
	}
	else {
		cout << "Error: could not open file for writing!\n";
	}
}
void loadGame() {
	for (int i = 0; i < maxCars; i++) {
		if (cars[i].getOwned() == true) {
			cars[i].setOwned(false);
			cars[i].setAssigned(false);
		}
	}
	ifstream read("savefile.txt");
	if (read.is_open()) {
		read >> bank;
		read >> total;
		read >> unassignedEmployeeCount;
		read >> assignedEmployeeCount;
		read >> ownedCarsCount;
		int index;
		for (int i = 0; i < ownedCarsCount; i++) {
			read >> index;
			cars[index].setAssigned(true);
			cars[index].setOwned(true);
		}
		read.close();
		cout << "\n\nFile loaded.\n";
	}
	else {
		cout << "Error: could not open file for reading!\n";
	}
}
