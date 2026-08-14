#include<iostream>
#include<cstdlib>
using namespace std;

void shuffle();
int random(int, int);
void swap(int, int);

void left(int, int[]);
void output(int, int[]);
int arraySum(int, int[]);

void getCard(int&, int[]);
void results(int&, int&);
void round(int&, int&);

int const dMaxSize = 11;
int dSize = dMaxSize;
int deck[dMaxSize] = { 1,2,3,4,5,6,7,8,9,10,11 };
int p1Deck[dMaxSize];
int p2Deck[dMaxSize];

int points1 = 0;
int points2 = 0;

int main() {

	int c1 = 0;
	int c2 = 0;

	cout << "Welcome to Simple 21!\n" << endl;
	cout << "Choose a Player 1 and Player 2 between yourselves.\n" << endl;
	shuffle();
	//output(dSize, deck);

	cout << "Player 1: " << endl;
	getCard(c1, p1Deck);
	output(c1, p1Deck);
	cout << "Player 2: " << endl;
	getCard(c2, p2Deck);
	output(c2, p2Deck);
	cout << endl;

	while (true) {
		bool ans;
		round(c1, c2);
		cout << "Do you want to quit? 1 for yes 0 for no" << endl;
		cin >> ans;
		if (ans == true) {
			break;
		}
		else {
			continue;
		}
	}
	//output(dSize, deck);

	return 0;
}

void round(int& c1, int& c2) {
	bool choice1 = true;
	bool choice2 = true;

	//output(dSize, deck);
	cout << "Player 1: \nStay(0) or Hit(1)?" << endl;
	cin >> choice1;
	if (choice1) {
		getCard(c1, p1Deck);
		cout << "Player 1 Cards: " << endl;
		output(c1, p1Deck);
	}
	else {
		cout << "Player 1 Cards: " << endl;
		output(c1, p1Deck);
	}
	cout << endl;

	cout << "Player 2: \nStay(0) or Hit(1)?" << endl;
	cin >> choice2;
	if (choice2) {
		getCard(c2, p2Deck);
		cout << "Player 2 Cards: " << endl;
		output(c2, p2Deck);
	}
	else {
		cout << "Player 2 Cards: " << endl;
		output(c2, p2Deck);
	}
	cout << endl;

	if (choice1 == false && choice2 == false) {
		results(c1, c2);
		return;
	}
}

void results(int& c1, int& c2) {
	int sum1, sum2;

	sum1 = arraySum(c1, p1Deck);
	sum2 = arraySum(c2, p2Deck);

	if (sum1 == sum2) {
		cout << "It's a draw!" << endl;
		return;
	}

	if (((sum1 < 21 && sum2 < 21) || (sum1>21 && sum2>21)) && ((21 - sum1) < (21 - sum2))) {
		points1++;
		cout << "Player 1 wins!" << endl;
	}
	if (((sum1 < 21 && sum2 < 21) || (sum1 > 21 && sum2 > 21)) && ((21 - sum1) > (21 - sum2))) {
		points2++;
		cout << "Player 2 wins!" << endl;
	}

	if (sum1 == 21 && sum2 != 21) {
		points1++;
		cout << "Player 1 wins!" << endl;
	}
	if (sum2 == 21 && sum1 != 21) {
		points2++;
		cout << "Player 2 wins!" << endl;
	}
}

int arraySum(int size, int arr[]) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	return sum;
}

void getCard(int &count, int arr[]) {
	arr[count] = deck[0];
	left(0, deck);
	count++;
}

void output(int size, int arr[]) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

void left(int start, int arr[]) {
	for (int i = start; i < dSize; i++) {
		arr[i] = arr[i + 1];
	}
	dSize--;
}

int random(int high, int low) {
	int x = rand();
	int y = x % (high - low + 1) + low;

	return y;
}

void swap(int index1, int index2) {
	int temp = deck[index1];
	deck[index1] = deck[index2];
	deck[index2] = temp;
}

void shuffle() {
	for (int i = 0; i < dSize; i++) {
		int x = random(dSize - 1, i);
		swap(i, x);
	}
}
