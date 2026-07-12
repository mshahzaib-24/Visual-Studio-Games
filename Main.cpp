#include<iostream>
#include<cstdlib>
using namespace std;

class Item {
	string name;
	double price;
	bool status;
public:
	Item(string name = "", double price = 0.0) :name(name), price(price), status(false) {
	}
	Item& operator=(const Item& obj) {
		name = obj.name;
		price = obj.price;
		status = obj.status;
		return *this;
	}
	string getName() {
		return name;
	}
	void setPrice(double price) {
		this->price = price;
	}
	double getPrice() {
		return price;
	}
	void setStatus(bool status) {
		this->status = status;
	}
	bool getStatus() {
		return status;
	}
	void display() const {
		cout << name << ": $" << price << endl;
	}
};
class Barista {
	string name;
	double earnRate;
	double salary;
public:
	Barista(string name = "") :name(name) {
		earnRate = 5;
		salary = 50;
	}
	Barista& operator=(const Barista& obj) {
		name = obj.name;
		earnRate = obj.earnRate;
		salary = obj.salary;
		return *this;
	}
	string getName() {
		return name;
	}
	double getEarnRate() {
		return earnRate;
	}
	double getSalary() {
		return salary;
	}
	void setName(string name) {
		this->name = name;
	}
	void display() const {
		cout << "===============\n" << name << "\nEarnings/day: $" << earnRate << "\nSalary/month: $" << salary << "\n===============" << endl;
	}
};

void customer(Item[], Item[], int&, const int, double&);
void barista(Item[], Item[], const int, int&, double&, int&);
void manager(Barista[], int&, double&, double&, double&, double&);
void hireBarista(Barista[], int&, double&, double&, double&);
void fireBarista(Barista[], int&, double&, double&, double&);

int main() {
	double bank = 1;
	double earnings = 0;
	double loss = 0;
	double profit = 0;
	const int size = 10;
	const int capacity = 10;
	const int maxInventory = 100;
	int empCount = 0;
	int day = 1;
	int warningCount = 0;
	int shiftCount = 0;
	int invCount = 0;
	Item Menu[size] = {
		Item("Espresso",1),
		Item("Latte",1.5),
		Item("Cappuccino",3),
		Item("Flat White",4.5),
		Item("Macchiato",6),
		Item("Mocha",9),
		Item("Americano",12),
		Item("Hot Chocolate",20),
		Item("Ristretto",25),
		Item("Cortado",30)};
	Item Inventory[maxInventory]{};
	Barista Employees[capacity]{};

	cout << "WELCOME TO CAFE UMAI!\nA legendary coffee shop experience awaits." << endl;
	cout << "===========================\n|| IMPORTANT INFORMATION ||\n==========================="<<endl;
	cout << "This is a simple buying\nand selling game. Using the\nCUSTOMER mode, you fill up\nyour inventory. The items\nin your inventory can be\nsold for 50% more than you\nbought them at in the BARISTA\nmode. Finally, the MANAGER\nmode is used to passively\nearn income by hiring and\nfiring baristas.\n===========================" << endl;
	while (true) {
		if (day % 30 == 0) {
			profit = earnings - loss;
		}
		else {
			profit = earnings;
		}
		cout << "====================\n|| Day: " << day;
		cout << "\n|| Bank: $" << bank;
		cout << "\n|| Profit: $" << profit;
		if (warningCount != 3)
			cout << "\n|| Barista Shifts: " << shiftCount << "\n====================" << endl;
		else
			cout << "===============" << endl;
		int mode;
		cout << "===========\n|| MODES ||\n===========\n(0) Exit\n(1) Customer\n(2) Barista\n(3) Manager\n(4) Check Inventory\n(5) Advance Day\nChoose a mode: ";
		cin >> mode;
		switch (mode) {
		case 0:
			cout << "Exiting..." << endl;
			return 1;
		case 1:
			customer(Menu, Inventory, invCount, size, bank);
			break;
		case 2:
			if (invCount == 0) {
				cout << "====================\nYour inventory is empty!" << endl;
				continue;
			}
			if (warningCount == 3) {
				cout << "====================\nYou have been fired! You can no longer access this mode." << endl;
				continue;
			}
			if (shiftCount == 3) {
				cout << "====================\nYou cannot take another shift for today." << endl;
				continue;
			}
			else {
				shiftCount++;
				barista(Menu, Inventory, size, invCount, bank, warningCount);
			}
			break;
		case 3:
			manager(Employees, empCount, bank, earnings, loss, profit);
			break;
		case 4:
			if (invCount == 0) {
				cout << "====================\nYour inventory is empty! There is nothing to display." << endl;
			}
			cout << "===============\n|| INVENTORY ||\n===============" << endl;
			for (int i = 0; i < invCount; i++) {
				Inventory[i].display();
			}
			break;
		case 5:
			day++;
			bank += profit;
			shiftCount = 0;
			break;
		default:
			cout << "Invalid input, please try again." << endl;
			continue;
		}
	}
	return 0;
}

void customer(Item Menu[], Item Inventory[], int& count, const int size, double& bank) {
	cout << "====================\n|| Bank: $" << bank << "\n====================" << endl;
	cout << "==========\n|| MENU ||\n==========" << endl;
	for (int i = 0; i < size; i++) {
		cout << "(" << i << ") ";
		Menu[i].display();
	}
	int item;
	cout << "Select your order (end order using '-1'): ";
	while (true) {
		//detect zero money
		if (bank == 0) {
			cout << "===========================\nYou've run out of money!\n===========================" << endl;
			break;
		}
		cin >> item;
		//detect the end of order
		if (item == -1)
			break;

		//check for an incorrect item input
		if (item >= size || item < -1) {
			cout << "Incorrect! Select a valid item from the menu: ";
			continue;
		}
		//check item price > bank
		if (bank < Menu[item].getPrice()) {
			cout << "You cannot buy this! Select another item: ";
			continue;
		}
		else {
			Inventory[count] = Menu[item];
			bank -= Inventory[count].getPrice();
			Inventory[count].setStatus(true);
			count++;
		}
	}
}
void barista(Item Menu[], Item Inventory[], const int size, int& invCount, double& bank, int& warningCount) {
	cout << "====================\n";
	double earnings = 0;
	int randNum;
	int itemNum;
	int count = 0;
	int wrong = 0;
	while (count < 5 && count <= invCount) {
		randNum = rand() % invCount;
		cout << "Customer " << count + 1 << ":\"" << Inventory[randNum].getName() << " please.\"" << endl;
		cout << "Enter the correct item number: ";
		cin >> itemNum;
		//
		while (itemNum < 0 || itemNum >= size) {
			cout << "Invalid item, please try again: ";
			cin >> itemNum;
		}
		//
		if (Menu[itemNum].getName() == Inventory[randNum].getName()) {
			cout << "Customer " << count + 1 << ":\"Thank you!\" :)" << endl;
			earnings += Inventory[randNum].getPrice()*1.5;
		}
		else {
			cout << "Customer " << count + 1 << ":\"That's a star off for poor service!\" >:(" << endl;
			wrong++;
		}
		//shrink inventory
		for (int i = randNum; i < invCount - 1; i++) {
			Inventory[i] = Inventory[i + 1];
		}
		invCount--;
		Inventory[invCount] = Item();
		//
		count++;
	}
	if (wrong == count) {
		warningCount++;
		cout << "====================\nYou have " << 3 - warningCount << "/3 warnings left before you are fired." << endl;
	}
	cout << "====================\n|| Earnings: $" << earnings << "\n====================" << endl;
	bank += earnings;
}

void manager(Barista Employees[], int& count, double& bank, double& earnings, double& loss, double& profit)
{
	int option;
	while (true) {
		cout << "====================\n(0) Return to Main Menu\n(1) Hire Barista (-$50)\n(2) Fire Barista\n(3) Check Stats\nEnter an option: ";
		cin >> option;
		switch (option) {
		case 0:
			cout << "====================\nReturning back to main menu..." << endl;
			return;
		case 1:
			hireBarista(Employees, count, bank, earnings, loss);
			break;
		case 2:
			fireBarista(Employees, count, bank, earnings, loss);
			break;
		case 3:
			cout << "====================\n|| Earnings: $" << earnings;
			cout << "\n|| Loss/month: $" << loss;
			cout << "\n|| Profit: $" << profit <<"\n===================="<<endl;
			break;
		default:
			cout << "====================\nInvalid option!\n===========================" << endl;
			continue;
		}
	}
}
void hireBarista(Barista Employees[], int& count, double& bank, double& earnings, double& loss) {
	//
	Barista newHire;
	if (bank < newHire.getSalary()) {
		cout << "====================\nYou do not have enough money!\n===========================" << endl;
		return;
	}
	else {
		bank -= newHire.getSalary();
	}
	//
	string name;
	cout << "====================\nChoose a name for you barista: ";
	cin >> name;
	newHire.setName(name);
	//
	Employees[count] = newHire;
	//
	earnings += Employees[count].getEarnRate();
	loss += (Employees[count].getSalary());
	//
	Employees[count].display();
	count++;
}
void fireBarista(Barista Employees[], int& count, double& bank, double& earnings, double& loss) {
	if (count == 0) {
		cout << "===========================\nThere are no employees!\n===========================" << endl;
		return;
	}
	//
	for (int i = 0; i < count; i++) {
		Employees[i].display();
	}
	//
	string name;
	cout << "Enter the name of the barista you wish to fire: ";
	cin >> name;
	//
	for (int i = 0; i < count; i++) {
		if (name == Employees[i].getName()) {
			earnings -= Employees[i].getEarnRate();
			loss -= Employees[i].getSalary();
			///shift left employees
			for (int j = i; j < count - 1; j++) {
				Employees[j] = Employees[j + 1];
			}
			count--;
			//blank slot
			Employees[count] = Barista();
			break;
		}
	}
}