#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

class Entry {
	int id;
	string date; //format yyyy-mm-dd
	string description;
public:
	Entry() {}
	Entry(int id, string date, string description){
		this->id = id;
		this->date = date;
		this->description = description;
	}
	int getId() {
		return id;
	}
	string getDate() {
		return date;
	}
	string getDescription() {
		return description;
	}
	void setDate(string& newDate) {
		date = newDate;
	}
	void setDescription(string& newDescription) {
		description = newDescription;
	}
	string serialise() const {
		return "ENTRY|" + to_string(id) + "|" + date + "|" + description;
	}
	static Entry deserialise(const string& line) {
		//parsing
		size_t pos1 = line.find('|');
		size_t pos2 = line.find('|', pos1 + 1);
		size_t pos3 = line.find('|', pos2 + 1);
		//saving
		string date = line.substr(pos2+1,pos3-pos2-1);
		string description = line.substr(pos3+1);
		int id = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
		//creating
		Entry newEntry(id, date, description);
		return newEntry;
	}
};

int main() {
	return 0;
}
