#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

class Car {
	string name;
	double price;
	double earnings;
	bool owned;
public:
	Car(string name = "", double price = 0) {
		this->name = name;
		this->price = price;
		owned = false;
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
	void setStatus(bool owned) {
		this->owned = owned;
	}
	void display() const {
		cout << name << " | $" << price << " | ($" << earnings << "/second)\n";
	}
};

//global variables
bool running = true;
double bank = 0.0;
double total = 0.0;
const int maxCars = 5;
Car cars[maxCars]{ 
Car("Toyota AE86",1000.0),
Car("Suzuki Mehran", 1500.0),
Car("Suzuki Alto", 4000.0),
Car("Suzuki Wagon R", 7500.0),
Car("Honda BRV", 14000.0)};

void renderMenu();
void handleMenuInput(char);
void hireEmployee();
void buyCar();

void displayCars() {
	cout << "ID & NAME | PRICE | EARNINGS PER SECOND\n";
	for (int i = 0; i < maxCars; i++) {
		cout << "[" << i << "] ";
		cars[i].display();
	}
}

int main() {
	ULONGLONG startTick = GetTickCount64();
	ULONGLONG lastTick = startTick;
	const ULONGLONG interval_ms = 1000; //1 second
	//const ULONGLONG duration_ms = 30 * 1000;

	cout << "Welcome to Taxi Runner!\n";
	renderMenu();
	while (running) {
		ULONGLONG now = GetTickCount64();
		if (now - lastTick >= interval_ms) {
			lastTick = now;
			bank += total;
			//cout << "Money earned! Total: " << money << endl;
		}
		if (_kbhit()) {
			char key = _getch();
			handleMenuInput(key);
			renderMenu();
		}
	}
	return 0;
}

void renderMenu() {
	cout << "\n--- Taxi Company Menu ---\n";
	cout << "[1] Hire Employee\n";
	cout << "[2] Buy Car\n";
	cout << "[3] View Status\n";
	cout << "[4] Exit\n";
	cout << "Enter: ";
}
void handleMenuInput(char key) {
	switch (key) {
	case '1': hireEmployee(); break;
	case '2': buyCar(); break;
	case '3': /* show status */ break;
	case '4': running = false; break;
	}
}
void hireEmployee() {
	double salary = 50.0;

	cout << "Bank: " << bank << endl;
	cout << "An employee costs $" << salary << " per second." << endl;
	cout << "Do you want to continue with this purchase? (y/n): ";
	char confirm;
	cin >> confirm;
	if ((confirm == 'y' || confirm == 'Y') && (bank == 0.0 && total <= 0.0)) {
		cout << "You cannot buy an employee yet.\n";
	} else if (confirm == 'y' || confirm == 'Y') {
		cout << "Employee hired!\n";
		total -= salary;
	}
	else {
		cout << "Purchase cancelled.\n";
	}
}
void buyCar() {
	cout << "\n---- Dealership ----\n";
	displayCars();
	cout << "[" << maxCars << "] Exit\n";
	cout << "Enter: ";
	int choice;
	cin >> choice;
	total += cars[choice].getEarnings();
}
void displayStatus() {

}