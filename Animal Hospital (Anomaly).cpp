#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

class Patient {
	int id;
	bool anomaly;
	int roomNum;
	bool treated;
	int CCTVPhoto;
public:
	Patient():id(0),roomNum(0),treated(false) {
		CCTVPhoto = rand() % 3;
		anomaly = rand() % 2;
	}
	void setRoomNum(int roomNum) {
		this->roomNum = roomNum;
	}
	void setId(int id) {
		this->id = id;
	}
	void setTreated(bool treated) {
		this->treated = treated;
	}
	void setAnomaly(bool anomaly) {
		this->anomaly = anomaly;
	}int getCCTVPhoto() {
		return CCTVPhoto;
	}
	int getRoomNum() {
		return roomNum;
	}
	bool getAnomaly() {
		return anomaly;
	}
};

void startGame();
void reception(Patient[], const int, int&);
void medicalWing(Patient[], const int);

int main() {
	srand(time(0));
	int MMChoice; //Main Menu Choice
	cout << "Welcome to Animal Hospital (Anomaly)!" << endl;
	while (true) {
		cout << "(1) Help\n(2) Start Game\n(3) Exit Game\nEnter: ";
		cin >> MMChoice;
		switch (MMChoice) {
		case 1:
			cout << "\nYou will meet patients in the check-in area.\nBy checking the CCTV footage and photo of the patient, you can figure out if they are an anomaly or not.\nOnce you check in a patient, they will tell which room to treat them in.\nA device will display their illnesses to you and you must select the correct treatment or risk killing the patient.\nIf an anomaly is treated he will attack you and reduce your sanity by 20 points.\nThere are no consequences for killing an anomaly.\nKilling a patient, however, will reduce your total income for that shift.\nDo your best to treat as many patients as possible correctly and prevent anomalies.\nThe game ends if you kill three patients or your sanity reduces to zero.\n-----Illness -> Treatment List-----\nBleeding -> Bandage\nRash -> Ointment\nBruises -> Medical Kit\nFever -> Thermometer\nStomach-ache -> Herbs\nHead-ache -> Medicine\nDehydration -> IV Drip\nDry eyes -> Eye Drops\nLow Sugar -> Maple Syrup\nFlu -> Cough Syrup" << endl;
			cout << endl;
			break;
		case 2:
			startGame();
			cout << endl;
			break;
		case 3:
			cout << "Exiting..." << endl;
			return 1;
			break;
		default:
			cout << "There's no option for that!" << endl;
			cout << endl;
			continue;
		}
	}
	return 0;
}

void startGame() {
	int shift = 1;

	const int patientSize = 5;
	Patient patients[patientSize] = {};
	int patientCount = 0;

	int treatedPatientCount = 0;
	int deadPatientCount = 0;

	while (true) {
		int HMChoice; //Hospital Menu choice
		//Next shift check
		if (treatedPatientCount == patientSize) {
			shift++;
			patientCount = 0;
		}

		//Hospital Menu
		cout << "\n-----SHIFT " << shift << "-----" << endl;
		cout << "-----HOSPITAL-----\n(1) Reception\n(2) Medical Wing\n(3) Return to Main Menu\nEnter: ";
		
		cin >> HMChoice;
		switch (HMChoice) {
		case 1:
			reception(patients, patientSize, patientCount);
			break;
		case 2:
			medicalWing(patients, patientSize);
			break;
		case 3:
			cout << "Returning to Main Menu..." << endl;
			return;
			break;
		default:
			//incorrect input check
			cout << "Pressing that won't lead anywhere!" << endl;
			cout << endl;
			continue;
		}
	}
}

void reception(Patient patients[], const int patientSize, int& patientCount) {
	int RMChoice; //Reception Menu choice
	int roomNum;
	int anomaly;

	while (true) {
		roomNum = rand() % 5 + 1;

		patients[patientCount].setId(patientCount);
		patients[patientCount].setRoomNum(roomNum);
		
		if (patientCount == patientSize)
			break;

		cout << "\nPatients Left to Check-In: " << patientSize-patientCount << endl;
		cout << "Patient " << patientCount + 1 << ": I would like to check-in as a patient." << endl;
		cout << "\n-----Reception Menu-----\n(1) Accept Patient\n(2) Decline Patient\n(3) Check CCTV\n(4) Check Photo\n(5) Return to Hospital\nEnter: ";
		cin >> RMChoice;

		switch (RMChoice) {
		case 1:
			cout << "\nPatient " << patientCount + 1 << ": I will be in room " << roomNum << "." << endl;
			patientCount++;
			break;
		case 2:
			cout << "\nPatient " << patientCount + 1 << ": How dare you kick me out!" << endl;
			patientCount++;
			break;
		case 3:
			//anomaly & CCTVPhoto check
			if (patients[patientCount].getAnomaly()==true && (patients[patientCount].getCCTVPhoto() == 2 || patients[patientCount].getCCTVPhoto() == 1)) {
				cout << "\nSomething is staring straight at you." << endl;
			}
			break;
		case 4:
			//anomaly & CCTVPhoto check
			if (patients[patientCount].getAnomaly() == true && (patients[patientCount].getCCTVPhoto() == 2 || patients[patientCount].getCCTVPhoto() == 0)) {
				cout << "\nTheir smile keeps growing." << endl;
			}
			break;
		case 5:
			cout << "\nReturning to Hospital..." << endl;
			return;
		default:
			//incorrect input check
			cout << "\nStop trying to travel to non-existent dimensions." << endl;
			cout << endl;
			continue;
		}
	}
}
void medicalWing(Patient patients[], const int patientSize) {
	int MWChoice;//Medical Wing choice

	const int illTreatSize = 10;//Maximum size of illnesses and treatments
	string illness[illTreatSize] = { "Bleeding","Rash","Bruises","Fever","Stomach-ache","Head-ache","Dehydration","Dried Eyes","Low Sugar","Flu" };
	string treatment[illTreatSize] = { "Bandage","Ointment","Medical Kit","Thermometer","Herbs","Medicine","IV Drip","Eye Drops","Maple Syrup","Cough Syrup" };

	int illnessIndex[illTreatSize];

	int randIllnessCount;
	int randIllness;
	int index;
	bool treated = false;

	while (true) {
		cout << "\n-----Medical Wing Menu-----\nRoom (1)\nRoom (2)\nRoom (3)\nRoom (4)\nRoom (5)\nEnter: ";
		cin >> MWChoice;
		//object intialisation handling
		if (MWChoice == 0) {
			cout << "\nThere's no secret room hiding in this wing." << endl;
			continue;
		}
		for (int i = 0; i < patientSize; i++) {
			if (patients[i].getRoomNum() == MWChoice) {
				//patient illness display
				randIllnessCount = rand() % 3;
				cout << "\n-----Illnesses-----" << endl;
				for (int j = 0; j < randIllnessCount; j++) {
					randIllness = rand() % illTreatSize;
					illnessIndex[j] = randIllness;
					cout << "(" << j+1 << ") " << illness[randIllness] << endl;
				}
				//treatment menu
				cout << "\n-----Treatments-----" << endl;
				for (int j = 0; j < illTreatSize; j++) {
					cout << "(" << j << ") " << treatment[j] << endl;
				}
				//correct and incorrect treatment handling
				for (int j = 0; j < randIllnessCount; j++) {
					cout << "Enter the correct treatments: ";
					cin >> index;
					int k;
					for (k = 0; k < randIllnessCount; k++) {
						if (index == illnessIndex[k]) {
							treated = true;
							break;
						}
					}
				}
				//killed patient and anomaly messages
				if (treated == false && patients[i].getAnomaly()==false) {
					cout << "\nYou killed the patient!" << endl;
				}
				else if (treated == false && patients[i].getAnomaly() == true) {
					cout << "\nYou killed the patient, but it was an anomaly." << endl;
				}
				else {
					cout << "\nThe patient recovered :)" << endl;
				}
				patients[i].setTreated(treated);
			} 
			else if (MWChoice <= patientSize && MWChoice > 0) {
				cout << "\nThis room is empty." << endl;
				break;
			}
			else {
				cout << "\nThere's no secret room hiding in this wing." << endl;
				break;
			}
		}
	}
}
