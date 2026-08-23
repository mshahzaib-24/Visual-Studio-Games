#include<windows.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define ID_MANAGE_CARS 1
#define ID_MANAGE_EMPLOYEES 2
#define ID_SAVE 3
#define ID_LOAD 4
#define ID_EXIT 5
#define ID_VIEW_GARAGE 6
#define ID_VIEW_DEALERSHIP 7
#define ID_HIRING 8
#define ID_FIRING 9
#define ID_BACK 10
#define ID_BACK_CARS_MENU 11
#define ID_BACK_EMPLOYEE_MENU 12
#define ID_STATS 13

#define ID_SALARY 98
#define ID_BANK 99
#define ID_CAR_START 100

class Car {
	string name;
	int price;
	int earnings;
	int salary;
	bool owned;
	bool assigned;
public:
	Car(string name = "", int price = 0) {
		this->name = name;
		this->price = price;
		owned = false;
		assigned = false;
		salary = 0;
		earnings = (int)(price * 0.015);
	}
	string getName() { return name; }
	int getPrice() { return price; }
	int getEarnings() { return earnings; }
	int getSalary() { return salary; }
	bool getOwned() { return owned; }
	bool getAssigned() { return assigned; }
	void setOwned(bool owned) { this->owned = owned; }
	void setAssigned(bool assigned) { this->assigned = assigned; }
	void setSalary(int salary) { this->salary = salary; }
};

int employeeCount = 0;
int ownedCars = 0;
int grossIncome = 0;
int bank = 2000;
int totalSalaries = 0;
const int maxCars = 27;
Car cars[maxCars] = {
	Car("Suzuki Mehran", 1800),
	Car("Suzuki Alto", 2500),
	Car("Suzuki Cultus", 3400),
	Car("Changan Alsvin", 4500),
	Car("Suzuki Swift", 5900),
	Car("Suzuki WagonR", 7600),
	Car("Toyota Yaris", 9600),
	Car("Honda City", 12000),
	Car("Toyota Corolla", 15000),
	Car("Honda Civic", 18500),
	Car("Hyundai Elantra", 22500),
	Car("Hyundai Sonata", 27000),
	Car("Honda BR-V", 32000),
	Car("Haval H6", 38000),
	Car("Hyundai Tucson", 45000),
	Car("Kia Sportage", 53000),
	Car("Hyundai Santa Fe", 62000),
	Car("Kia Sorento", 72000),
	Car("Audi A6", 83000),
	Car("Mercedes E-Class", 95000),
	Car("Mercedes C63 AMG", 108000),
	Car("Toyota Fortuner", 123000),
	Car("Toyota Prado", 140000),
	Car("Toyota Land Cruiser", 160000),
	Car("Range Rover", 185000),
	Car("BMW iX", 215000),
	Car("Lamborghini Huracan", 300000)
};

enum class Scene {
	MAIN_MENU,
	EMPLOYEES,
	HIRING,
	FIRING,
	CARS,
	GARAGE,
	DEALERSHIP,
	STATISTICS
};

Scene currentScene = Scene::MAIN_MENU;
HWND hMainWindow;

HBRUSH currentBgBrush = NULL;
COLORREF currentBgColor = RGB(240, 240, 240);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowMainMenu(HWND hwnd);
void ShowEmployeeMenu(HWND hwnd);
void ShowCarMenu(HWND hwnd);
void ShowGarage(HWND hwnd);
void ShowDealership(HWND hwnd);
void ShowHiringAndFiring(HWND hwnd);
void ShowStats(HWND hwnd);
void loadGame(HWND hwnd);
void saveGame(HWND hwnd);
void ClearWindow(HWND hwnd);
void ListCars(HWND hwnd, bool ownedOnly, bool markAssigned = false);
void HandleDealershipClick(HWND hwnd, int carIndex);
void HandleGarageClick(HWND hwnd, int carIndex);
void HandleHiringClick(HWND hwnd, int carIndex);
void HandleFiringClick(HWND hwnd, int carIndex);
void SetSceneBackground(HWND hwnd, COLORREF color);
int GetEmployeeHireCost(int currentEmployees);

wstring ToWide(const string& s) {
	return wstring(s.begin(), s.end());
}
HWND MakeButton(HWND parent, const string& text, int x, int y, int w, int h, int id) {
	return CreateWindow(L"BUTTON", ToWide(text).c_str(), WS_VISIBLE | WS_CHILD, x, y, w, h, parent, (HMENU)(INT_PTR)id, NULL, NULL);
}
HWND MakeLabel(HWND parent, const string& text, int x, int y, int w, int h, int id = 0) {
	return CreateWindow(L"STATIC", ToWide(text).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, x, y, w, h, parent, (HMENU)(INT_PTR)id, NULL, NULL);
}

int GetEmployeeHireCost(int currentEmployees) {
	int cost = 50;
	for (int i = 0; i < currentEmployees; i++) {
		cost = (int)(cost * 1.15);
	}
	return cost;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"TaxiRunnerWindow";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(0, className, L"Taxi Runner", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return 0;
	}
	hMainWindow = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	SetTimer(hwnd, 1, 5000, NULL);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE: {
		ShowMainMenu(hwnd);
		return 0;
	}
	case WM_TIMER: {
		if (wParam == 1) {
			int netIncome = grossIncome - totalSalaries;
			bank += netIncome;
			SetWindowText(GetDlgItem(hwnd, ID_BANK), ToWide("Bank: $" + to_string(bank)).c_str());
		}
		return 0;
	}
	case WM_COMMAND: {
		int buttonID = LOWORD(wParam);
		if (buttonID >= ID_CAR_START && buttonID < ID_CAR_START + maxCars) {
			int carIndex = buttonID - ID_CAR_START;
			if (currentScene == Scene::DEALERSHIP) {
				HandleDealershipClick(hwnd, carIndex);
			}
			else if (currentScene == Scene::GARAGE) {
				HandleGarageClick(hwnd, carIndex);
			}
			else if (currentScene == Scene::HIRING) {
				HandleHiringClick(hwnd, carIndex);
			}
			else if (currentScene == Scene::FIRING) {
				HandleFiringClick(hwnd, carIndex);
			}
		}
		if (buttonID == ID_MANAGE_CARS) {
			currentScene = Scene::CARS;
			ClearWindow(hwnd);
			ShowCarMenu(hwnd);
		}
		else if (buttonID == ID_VIEW_GARAGE) {
			currentScene = Scene::GARAGE;
			ClearWindow(hwnd);
			ShowGarage(hwnd);
		}
		else if (buttonID == ID_VIEW_DEALERSHIP) {
			currentScene = Scene::DEALERSHIP;
			ClearWindow(hwnd);
			ShowDealership(hwnd);
		}
		else if (buttonID == ID_MANAGE_EMPLOYEES) {
			currentScene = Scene::EMPLOYEES;
			ClearWindow(hwnd);
			ShowEmployeeMenu(hwnd);
		}
		else if (buttonID == ID_HIRING) {
			currentScene = Scene::HIRING;
			ClearWindow(hwnd);
			ShowHiringAndFiring(hwnd);
		}
		else if (buttonID == ID_FIRING) {
			currentScene = Scene::FIRING;
			ClearWindow(hwnd);
			ShowHiringAndFiring(hwnd);
		}
		else if (buttonID == ID_STATS) {
			currentScene = Scene::STATISTICS;
			ClearWindow(hwnd);
			ShowStats(hwnd);
		}
		else if (buttonID == ID_SAVE) {
			saveGame(hwnd);
		}
		else if (buttonID == ID_LOAD) {
			loadGame(hwnd);
		}
		else if (buttonID == ID_EXIT) {
			DestroyWindow(hwnd);
		}
		else if (buttonID == ID_BACK) {
			currentScene = Scene::MAIN_MENU;
			ClearWindow(hwnd);
			ShowMainMenu(hwnd);
		}
		else if (buttonID == ID_BACK_CARS_MENU) {
			currentScene = Scene::CARS;
			ClearWindow(hwnd);
			ShowCarMenu(hwnd);
		}
		else if (buttonID == ID_BACK_EMPLOYEE_MENU) {
			currentScene = Scene::EMPLOYEES;
			ClearWindow(hwnd);
			ShowEmployeeMenu(hwnd);
		}
		return 0;
	}
	case WM_CTLCOLORSTATIC: {
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		if (currentBgBrush == NULL) {
			currentBgBrush = CreateSolidBrush(currentBgColor);
		}
		return (LRESULT)currentBgBrush;
	}
	case WM_DESTROY: {
		KillTimer(hwnd, 1);
		if (currentBgBrush != NULL) {
			DeleteObject(currentBgBrush);
			currentBgBrush = NULL;
		}
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		if (currentBgBrush == NULL) {
			currentBgBrush = CreateSolidBrush(currentBgColor);
		}
		FillRect(hdc, &ps.rcPaint, currentBgBrush);
		EndPaint(hwnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void SetSceneBackground(HWND hwnd, COLORREF color) {
	if (currentBgBrush != NULL) {
		DeleteObject(currentBgBrush);
		currentBgBrush = NULL;
	}
	currentBgColor = color;
	currentBgBrush = CreateSolidBrush(color);
	InvalidateRect(hwnd, NULL, TRUE);
}

void ClearWindow(HWND hwnd) {
	HWND child;
	while ((child = GetWindow(hwnd, GW_CHILD)) != NULL) {
		DestroyWindow(child);
	}
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void ListCars(HWND hwnd, bool ownedOnly, bool markAssigned) {
	int left = 0;
	int right = 0;
	bool any = false;
	for (int i = 0; i < maxCars; i++) {
		if (ownedOnly == true) {
			if (cars[i].getOwned() == false) {
				continue;
			}
		}
		any = true;
		int price;
		if (ownedOnly == true) {
			price = (int)(cars[i].getPrice() * 0.8);
		}
		else {
			price = cars[i].getPrice();
		}
		string carText = cars[i].getName() + " | $" + to_string(price) + " | $" + to_string(cars[i].getEarnings()) + "/tick";
		if (ownedOnly == false && cars[i].getOwned() == true) {
			carText += " [X]";
		}
		if (markAssigned == true && cars[i].getAssigned() == true) {
			carText += " [X]";
		}
		if (i < 14) {
			MakeButton(hwnd, carText, 20, left, 360, 30, ID_CAR_START + i);
			left += 40;
		}
		else {
			MakeButton(hwnd, carText, 420, right, 360, 30, ID_CAR_START + i);
			right += 40;
		}
	}
	if (ownedOnly == true) {
		if (any == false) {
			MakeLabel(hwnd, "No cars in garage.", 300, 20, 200, 20);
		}
	}
}

void HandleDealershipClick(HWND hwnd, int carIndex) {
	if (cars[carIndex].getOwned() == true) {
		MessageBox(hwnd, L"You already own this car.", L"Dealership", MB_OK);
	}
	else if (bank >= cars[carIndex].getPrice()) {
		bank -= cars[carIndex].getPrice();
		cars[carIndex].setOwned(true);
		ownedCars++;
		MessageBox(hwnd, L"Car purchased!", L"Dealership", MB_OK);
		ClearWindow(hwnd);
		ShowDealership(hwnd);
	}
	else {
		MessageBox(hwnd, L"You don't have enough money.", L"Dealership", MB_OK);
	}
}
void HandleGarageClick(HWND hwnd, int carIndex) {
	if (cars[carIndex].getAssigned() == true) {
		MessageBox(hwnd, L"You must fire the working employee before selling.", L"Garage", MB_OK);
	}
	else {
		bank += (int)(cars[carIndex].getPrice() * 0.8);
		cars[carIndex].setOwned(false);
		ownedCars--;
		MessageBox(hwnd, L"Car sold!", L"Garage", MB_OK);
		ClearWindow(hwnd);
		ShowGarage(hwnd);
	}
}
void HandleHiringClick(HWND hwnd, int carIndex) {
	if (cars[carIndex].getAssigned() == true) {
		MessageBox(hwnd, L"An employee is already assigned.", L"Hiring", MB_OK);
	}
	else if (ownedCars == 0) {
		MessageBox(hwnd, L"You need to own a car before hiring an employee.", L"Hiring", MB_OK);
	}
	else if (employeeCount >= ownedCars) {
		MessageBox(hwnd, L"You do not need another employee.", L"Hiring", MB_OK);
	}
	else {
		int hireCost = GetEmployeeHireCost(employeeCount);
		if (bank < hireCost) {
			MessageBox(hwnd, L"You don't have enough money.", L"Hiring", MB_OK);
		}
		else {
			int newSalary = (int)(hireCost * 0.1);
			bank -= hireCost;
			grossIncome += cars[carIndex].getEarnings();
			totalSalaries += newSalary;
			cars[carIndex].setSalary(newSalary);
			cars[carIndex].setAssigned(true);
			employeeCount++;
			MessageBox(hwnd, L"Employee hired & assigned!", L"Hiring", MB_OK);
			ClearWindow(hwnd);
			ShowHiringAndFiring(hwnd);
		}
	}
}
void HandleFiringClick(HWND hwnd, int carIndex) {
	if (cars[carIndex].getAssigned() == false) {
		MessageBox(hwnd, L"There is no employee assigned.", L"Firing", MB_OK);
	}
	else {
		grossIncome -= cars[carIndex].getEarnings();
		totalSalaries -= cars[carIndex].getSalary();
		cars[carIndex].setSalary(0);
		cars[carIndex].setAssigned(false);
		employeeCount--;
		MessageBox(hwnd, L"Employee de-assigned & fired!", L"Firing", MB_OK);
		ClearWindow(hwnd);
		ShowHiringAndFiring(hwnd);
	}
}

void ShowMainMenu(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(235, 235, 235));
	MakeLabel(hwnd, "Bank: $" + to_string(bank), 300, 20, 200, 20, ID_BANK);
	MakeButton(hwnd, "Manage Cars", 300, 80, 200, 50, ID_MANAGE_CARS);
	MakeButton(hwnd, "Manage Employees", 300, 140, 200, 50, ID_MANAGE_EMPLOYEES);
	MakeButton(hwnd, "View Statistics", 300, 200, 200, 50, ID_STATS);
	MakeButton(hwnd, "Save", 300, 260, 200, 50, ID_SAVE);
	MakeButton(hwnd, "Load", 300, 320, 200, 50, ID_LOAD);
	MakeButton(hwnd, "Exit", 300, 380, 200, 50, ID_EXIT);
}
void ShowCarMenu(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(230, 240, 255));
	MakeButton(hwnd, "View Garage", 300, 140, 200, 50, ID_VIEW_GARAGE);
	MakeButton(hwnd, "View Dealership", 300, 200, 200, 50, ID_VIEW_DEALERSHIP);
	MakeButton(hwnd, "Back", 300, 260, 200, 50, ID_BACK);
}
void ShowEmployeeMenu(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(230, 255, 235));
	string label = "Employees: " + to_string(employeeCount) + " / " + to_string(ownedCars) + " owned cars";
	if (employeeCount < ownedCars) {
		label += "\nNext hire: $" + to_string(GetEmployeeHireCost(employeeCount));
	}
	MakeLabel(hwnd, label, 300, 20, 230, 50, ID_SALARY);
	MakeButton(hwnd, "Hire Employee", 300, 140, 200, 50, ID_HIRING);
	MakeButton(hwnd, "Fire Employee", 300, 200, 200, 50, ID_FIRING);
	MakeButton(hwnd, "Back", 300, 260, 200, 50, ID_BACK);
}
void ShowGarage(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(230, 240, 255));
	ListCars(hwnd, true);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_CARS_MENU);
}
void ShowDealership(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(230, 240, 255));
	ListCars(hwnd, false);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_CARS_MENU);
}
void ShowHiringAndFiring(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(235, 245, 255));
	ListCars(hwnd, true, true);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_EMPLOYEE_MENU);
}
void ShowStats(HWND hwnd) {
	SetSceneBackground(hwnd, RGB(245, 235, 255));
	int netIncome = grossIncome - totalSalaries;
	int inactiveCars = ownedCars - employeeCount;
	int garageValue = 0;
	for (int i = 0; i < maxCars; i++) {
		if (cars[i].getOwned() == true) {
			garageValue += (int)(cars[i].getPrice() * 0.8);
		}
	}
	string text = "Bank: $" + to_string(bank) + "\n";
	text += "Garage Value: $" + to_string(garageValue) + "\n";
	text += "------------------------------\n";
	text += "Cars Owned: " + to_string(ownedCars) + " / " + to_string(maxCars) + "\n";
	text += "Employees: " + to_string(employeeCount) + "\n";
	text += "Assigned Cars: " + to_string(employeeCount) + "\n";
	text += "Inactive Cars: " + to_string(inactiveCars) + "\n";
	text += "------------------------------\n";
	text += "Gross Income: $" + to_string(grossIncome) + "/tick\n";
	text += "Employee Salaries: $" + to_string(totalSalaries) + "/tick\n";
	text += "Net Income: $" + to_string(netIncome) + "/tick\n";
	MakeLabel(hwnd, text, 250, 140, 300, 260, ID_STATS);
	MakeButton(hwnd, "Back", 300, 380, 200, 50, ID_BACK);
}

void saveGame(HWND hwnd) {
	ofstream write("savefile.txt");
	if (write.is_open() == true) {
		write << bank << endl;
		write << grossIncome << endl;
		write << employeeCount << endl;
		write << ownedCars << endl;
		for (int i = 0; i < maxCars; i++) {
			if (cars[i].getOwned() == true) {
				int assignedFlag = 0;
				if (cars[i].getAssigned() == true) {
					assignedFlag = 1;
				}
				write << i << " " << assignedFlag << " " << cars[i].getSalary() << endl;
			}
		}
		write.close();
		MessageBox(hwnd, L"File saved!", L"Saving", MB_OK);
	}
	else {
		MessageBox(hwnd, L"File not opening.", L"Saving", MB_OK);
	}
}
void loadGame(HWND hwnd) {
	for (int i = 0; i < maxCars; i++) {
		if (cars[i].getOwned() == true) {
			cars[i].setOwned(false);
			cars[i].setAssigned(false);
			cars[i].setSalary(0);
		}
	}
	totalSalaries = 0;
	ifstream read("savefile.txt");
	if (read.is_open() == true) {
		read >> bank;
		read >> grossIncome;
		read >> employeeCount;
		read >> ownedCars;
		int index;
		int assignedFlag;
		int loadedSalary;
		for (int i = 0; i < ownedCars; i++) {
			read >> index;
			read >> assignedFlag;
			read >> loadedSalary;
			cars[index].setOwned(true);
			if (assignedFlag == 1) {
				cars[index].setAssigned(true);
				cars[index].setSalary(loadedSalary);
				totalSalaries += loadedSalary;
			}
		}
		read.close();
		MessageBox(hwnd, L"File loading!", L"Loading", MB_OK);
	}
	else {
		MessageBox(hwnd, L"File not opening.", L"Loading", MB_OK);
	}
}
