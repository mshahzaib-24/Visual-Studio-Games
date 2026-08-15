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

#define ID_BANK 99
#define ID_CAR_START 100

class Car {
	string name;
	int price;
	int earnings;
	bool owned;
	bool assigned;
public:
	Car(string name = "", int price = 0) {
		this->name = name;
		this->price = price;
		owned = false;
		assigned = false;
		earnings = (int)(price * 0.02);
	}
	string getName() { return name; }
	int getPrice() { return price; }
	int getEarnings() { return earnings; }
	bool getOwned() { return owned; }
	bool getAssigned() { return assigned; }
	void setOwned(bool owned) { this->owned = owned; }
	void setAssigned(bool assigned) { this->assigned = assigned; }
};

int employeeCount = 0;
int ownedCars = 0;
int income = 0;
int bank = 2000;
const int salary = 30;
const int maxCars = 27;
Car cars[maxCars] = {
	Car("Suzuki Mehran",1800),
	Car("Suzuki Alto", 3160),
	Car("Suzuki WagonR", 3850),
	Car("Suzuki Cultus", 4340),
	Car("Toyota Yaris", 4900),
	Car("Suzuki Swift", 5000),
	Car("Changan Alsvin", 5300),
	Car("Honda City", 6000),
	Car("Toyota Corolla", 6500),
	Car("Honda BR-V", 6800),
	Car("Honda Civic", 7200),
	Car("Hyundai Elantra", 7500),
	Car("Haval H6", 9500),
	Car("Hyundai Tucson", 10000),
	Car("Kia Sportage", 10100),
	Car("Toyota Fortuner", 12500),
	Car("Hyundai Santa Fe", 13000),
	Car("Hyundai Sonata", 13400),
	Car("Kia Sorento", 15150),
	Car("Toyota Prado", 20000),
	Car("Mercedes C63 AMG",24000),
	Car("Mercedes E-Class", 28000),
	Car("Audi A6", 30000),
	Car("Toyota Land Cruiser", 32000),
	Car("Range Rover", 36500),
	Car("BMW iX",55000),
	Car("Lamborghini Huracan", 150000)
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
void ListCars(HWND hwnd, bool ownedOnly);
void HandleDealershipClick(HWND hwnd, int carIndex);
void HandleGarageClick(HWND hwnd, int carIndex);
void HandleHiringClick(HWND hwnd, int carIndex);
void HandleFiringClick(HWND hwnd, int carIndex);

wstring ToWide(const string& s) {
	return wstring(s.begin(), s.end());
}
HWND MakeButton(HWND parent, const string& text, int x, int y, int w, int h, int id) {
	return CreateWindow(L"BUTTON", ToWide(text).c_str(), WS_VISIBLE | WS_CHILD, x, y, w, h, parent, (HMENU)(INT_PTR)id, NULL, NULL);
}
HWND MakeLabel(HWND parent, const string& text, int x, int y, int w, int h, int id = 0) {
	return CreateWindow(L"STATIC", ToWide(text).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, x, y, w, h, parent, (HMENU)(INT_PTR)id, NULL, NULL);
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
			bank += income;
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
	case WM_DESTROY: {
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ClearWindow(HWND hwnd) {
	HWND child;
	while ((child = GetWindow(hwnd, GW_CHILD)) != NULL) {
		DestroyWindow(child);
	}
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void ListCars(HWND hwnd, bool ownedOnly) {
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
		string carText = cars[i].getName() + " | $" + to_string(price) + " | $" + to_string(cars[i].getEarnings()) + "/second";
		if (ownedOnly == false && cars[i].getOwned() == true) {
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
	else {
		bank -= salary;
		income -= salary;
		income += cars[carIndex].getEarnings();
		cars[carIndex].setAssigned(true);
		employeeCount++;
		MessageBox(hwnd, L"Employee hired & assigned!", L"Hiring", MB_OK);
		ClearWindow(hwnd);
		ShowHiringAndFiring(hwnd);
	}
}
void HandleFiringClick(HWND hwnd, int carIndex) {
	if (cars[carIndex].getAssigned() == false) {
		MessageBox(hwnd, L"There is no employee assigned.", L"Firing", MB_OK);
	}
	else {
		bank += salary;
		income += salary;
		income -= cars[carIndex].getEarnings();
		cars[carIndex].setAssigned(false);
		employeeCount--;
		MessageBox(hwnd, L"Employee de-assigned & fired!", L"Firing", MB_OK);
		ClearWindow(hwnd);
		ShowHiringAndFiring(hwnd);
	}
}

void ShowMainMenu(HWND hwnd) {
	MakeLabel(hwnd, "Bank: $" + to_string(bank), 300, 20, 200, 20, ID_BANK);
	MakeButton(hwnd, "Manage Cars", 300, 140, 200, 50, ID_MANAGE_CARS);
	MakeButton(hwnd, "Manage Employees", 300, 200, 200, 50, ID_MANAGE_EMPLOYEES);
	MakeButton(hwnd, "View Statistics", 300, 260, 200, 50, ID_STATS);
	MakeButton(hwnd, "Save", 300, 320, 200, 50, ID_SAVE);
	MakeButton(hwnd, "Load", 300, 380, 200, 50, ID_LOAD);
	MakeButton(hwnd, "Exit", 300, 440, 200, 50, ID_EXIT);
}
void ShowCarMenu(HWND hwnd) {
	MakeButton(hwnd, "View Garage", 300, 140, 200, 50, ID_VIEW_GARAGE);
	MakeButton(hwnd, "View Dealership", 300, 200, 200, 50, ID_VIEW_DEALERSHIP);
	MakeButton(hwnd, "Back", 300, 260, 200, 50, ID_BACK);
}
void ShowEmployeeMenu(HWND hwnd) {
	MakeButton(hwnd, "Hire Employee", 300, 140, 200, 50, ID_HIRING);
	MakeButton(hwnd, "Fire Employee", 300, 200, 200, 50, ID_FIRING);
	MakeButton(hwnd, "Back", 300, 260, 200, 50, ID_BACK);
}
void ShowGarage(HWND hwnd) {
	ListCars(hwnd, true);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_CARS_MENU);
}
void ShowDealership(HWND hwnd) {
	ListCars(hwnd, false);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_CARS_MENU);
}
void ShowHiringAndFiring(HWND hwnd) {
	ListCars(hwnd, true);
	MakeButton(hwnd, "Back", 420, 520, 360, 30, ID_BACK_EMPLOYEE_MENU);
}
void ShowStats(HWND hwnd) {
	string text = "Income/5s = $" + to_string(income) + "\n" + "Cars Owned = " + to_string(ownedCars) + "\n" + "Employees = " + to_string(employeeCount);
	MakeLabel(hwnd, text, 300, 200, 200, 50, ID_STATS);
	MakeButton(hwnd, "Back", 300, 260, 200, 50, ID_BACK);
}

void saveGame(HWND hwnd) {
	ofstream write("savefile.txt");
	if (write.is_open() == true) {
		write << bank << endl;
		write << income << endl;
		write << employeeCount << endl;
		write << ownedCars << endl;
		for (int i = 0; i < maxCars; i++) {
			if (cars[i].getAssigned() == true) {
				write << i << endl;
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
		}
	}
	ifstream read("savefile.txt");
	if (read.is_open() == true) {
		read >> bank;
		read >> income;
		read >> employeeCount;
		read >> ownedCars;
		int index;
		for (int i = 0; i < ownedCars; i++) {
			read >> index;
			cars[index].setAssigned(true);
			cars[index].setOwned(true);
		}
		read.close();
		MessageBox(hwnd, L"File loading!", L"Loading", MB_OK);
	}
	else {
		MessageBox(hwnd, L"File not opening.", L"Loading", MB_OK);
	}
}