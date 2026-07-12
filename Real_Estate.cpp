#include<iostream>
using namespace std;

class Building {
	string name;
	int price;
	int loan;
	int rent;
	bool rentStatus;
public:
	Building(string name = " ", int price = 0) : name(name), price(price) {
		loan = 0;
		rent = 0;
		rentStatus = false;
	}
	void setName(string name) {
		this->name = name;
	}
	string getName() {
		return name;
	}
	void setPrice(int price) {
		this->price = price;
	}
	int getPrice() {
		return price;
	}
	void setLoan(int loan) {
		this->loan = loan;
	}
	int getLoan() {
		return loan;
	}
	void setRent(int rent) {
		this->rent = rent;
	}
	int getRent() {
		return rent;
	}
	void setRentStatus(bool status) {
		rentStatus = status;
	}
	bool getRentStatus() {
		return rentStatus;
	}
};

void startMenu();
void marketplace();
void buildlist();
void buy();
void sell();
void rent();
void loan();
int totalRent();
int totalLoan();

int day = 0;
int bank = 5000;
int ownedBuildCount = 0;
const int maxbuild = 10;
Building buildings[maxbuild] = { Building("Tiny House",5000),Building("Small House",10000),Building("Medium House",15000),Building("Big House",20000),Building("Large House",30000), Building("Tiny Apartment",15000), Building("Small Apartment",30000), Building("Medium Apartment",45000), Building("Large Apartment",60000), Building("Penthouse",100000)};
Building ownedBuildings[maxbuild];

int main() {
	startMenu();
	return 0;
}

void startMenu() {
	int option;
	while (true) {
		if (bank < 0) {
			cout << "YOU HAVE GONE BANKRUPT!\nClosing game..." << endl;
			break;
		}
		cout << "-----DETAILS-----" << endl;
		cout << "DAY: " << day << endl;
		cout << "BANK: $" << bank << endl;
		cout << "TOTAL RENT: $" << totalRent() << endl;
		cout << "TOTAL LOAN: $" << totalLoan() << endl;
		cout << "-----START MENU-----" << endl;
		cout << "(1) Progress Day\n(2) Buy\n(3) Sell\n(4) Rent\n(5) Loan" << endl;
		cout << "Choose an option: ";
		cin >> option;
		switch (option) {
		case 1:
			bank += totalRent();
			bank -= totalLoan();
			day++;
			break;
		case 2:
			buy();
			break;
		case 3:
			sell();
			break;
		case 4:
			rent();
			break;
		case 5:
			loan();
			break;
		default:
			cout << "Invalid response entered..." << endl;
			break;

		}
	}
}
void marketplace() {
	cout << "\n-----MARKETPLACE-----" << endl;
	for (int i = 0; i < maxbuild; i++) {
		if (buildings[i].getName() == " ")
			break;
		cout << "-----(" << i << ")-----" << endl;
		cout << "Name: " << buildings[i].getName() << endl;
		cout <<	"Value: $" << buildings[i].getPrice() << endl;
		cout << "Deposit: $" << buildings[i].getPrice()*0.2 << endl;
	}
}
void buildlist() {
	cout << "\n-----HOUSE LIST-----" << endl;
	for (int i = 0; i < ownedBuildCount; i++) {
		if (ownedBuildings[i].getName() == " ")
			break;
		cout << "-----(" << i << ")-----" << endl;
		cout << "Name: " << ownedBuildings[i].getName() << endl;
		cout << "Value: $" << ownedBuildings[i].getPrice() << endl;
		cout << "Rent: $" << ownedBuildings[i].getRent() << endl;
	}
}
void buy() {
	int choice;
	marketplace();
	cout << "Choose a building: ";
	cin >> choice;
	bank -= buildings[choice].getPrice();
	ownedBuildings[ownedBuildCount].setName(buildings[choice].getName());
	ownedBuildings[ownedBuildCount].setPrice(buildings[choice].getPrice());
	ownedBuildings[ownedBuildCount].setRent(buildings[choice].getPrice()*0.04);
	ownedBuildCount++;
}
void sell() {
	int choice;
	buildlist();
	cout << "Choose a building: ";
	cin >> choice;
	bank += ownedBuildings[choice].getPrice();
	ownedBuildings[choice].setName(" ");
	ownedBuildings[choice].setPrice(0);
	startMenu();
}
void rent() {
	int choice;
	buildlist();
	cout << "Choose a building: ";
	cin >> choice;
	ownedBuildings[choice].setRentStatus(true);
}
void loan() {
	int choice, duration;
	double payable, loan, deposit;

	marketplace();
	cout << "Choose a building: ";
	cin >> choice;
	payable = buildings[choice].getPrice() * 0.8;

	cout << "-----DURATIONS-----" << endl;
	cout << "(24) days: $" << payable / 24 << "/day" << endl;
	cout << "(36) days: $" << payable / 36 << "/day" << endl;
	cout << "(48) days: $" << payable / 48 << "/day" << endl;
	cout << "Choose a duration: ";
	cin >> duration;

	loan = payable / duration;
	deposit = buildings[choice].getPrice() * 0.2;

	bank -= deposit;
	ownedBuildings[ownedBuildCount].setName(buildings[choice].getName());
	ownedBuildings[ownedBuildCount].setPrice(buildings[choice].getPrice());
	ownedBuildings[ownedBuildCount].setRent(buildings[choice].getPrice() * 0.04);
	ownedBuildings[ownedBuildCount].setLoan(loan);
	ownedBuildCount++;
}
int totalRent() {
	int total = 0;
	for (int i = 0; i < ownedBuildCount; i++) {
		if(ownedBuildings[i].getRentStatus())
			total += ownedBuildings[i].getRent();
	}
	return total;
}
int totalLoan() {
	int total = 0;
	for (int i = 0; i < ownedBuildCount; i++) {
		total += ownedBuildings[i].getLoan();
	}
	return total;
}