// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#ifndef LOGMAN_H
#define LOGMAN_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

enum class Status : uint8_t {
	Empty,
	Key,
	Categ,
	Time
};

struct logData {
	uint32_t entryID; string ts, cat, msg, cmpCat;
	logData(const uint32_t &ID, const string &a, const string &b, const string &c) : entryID{ ID }, ts{ a },
		cat{ b }, msg{ c }, cmpCat{ b } {
		transform(cmpCat.begin(), cmpCat.end(), cmpCat.begin(), ::tolower);
	}
	void printLog() {
		cout << entryID << "|" << ts << "|" << cat << "|" << msg << "\n";
	}
};

class TimeComp {
public:
	bool operator()(const logData &a, const logData &b) const {
		return a.ts.compare(b.ts) < 0;
	}
}CompareTime;

class LogComp {
public:
	bool operator()(const logData &a, const logData &b) const {
		if (a.ts.compare(b.ts)) {
			return CompareTime.operator()(a, b);
		}
		else if (a.cmpCat.compare(b.cmpCat) != 0) {
			return a.cmpCat.compare(b.cmpCat) < 0;
		}
		else {
			return a.entryID < b.entryID;
		}
	}

}CompareLog;

class game {
public:
	//reading and running cmds and files
	void read(string filename);
	void run();

private:
	//cmds
	//printing
	void printExcerpt();
	void printSearched();

	//appending
	void appendExrp();
	void appendSearched();

	//deleting
	void clearExcerpt();
	void delEtryExrp();

	//moving
	void moveExrpBeg();
	void moveExrpEnd();
	void sortExrp();

	//searching
	void searchTs();
	void searchTsBound();
	void searchCat();
	void searchKeyW();

	Status status = Status::Empty;
	vector<logData> masterList;
	vector<uint32_t> entryOrd;
	deque<uint32_t> excerptList;
	vector<uint32_t> searched;
	unordered_map<string, vector<uint32_t>> hashmap;
	unordered_map<string, vector<uint32_t>> keysMap;
};

void game::printExcerpt() {
	for (uint32_t i = 0; i < uint32_t(excerptList.size()); ++i) {
		cout << i << "|";
		masterList[excerptList[i]].printLog();
	}
}

void game::printSearched() {
	switch (status) {
		case Status::Empty:
			cerr << "Search before you print\n";
			return;
		case Status::Key:
			for (uint32_t i = 0; i < uint32_t(searched.size()); ++i) {
				masterList[searched[i]].printLog();
			}
			break;
		case Status::Time:
			if (!searched.empty()) {
				for (uint32_t i = searched[0]; i <= searched[1]; ++i) {
					masterList[i].printLog();
				}
			}
			break;
		case Status::Categ:
			if (!searched.empty()) {
				auto iter = hashmap.find(masterList[searched.back()].cmpCat);
				for (uint32_t i = 0; i < uint32_t(iter->second.size()); ++i) {
					masterList[iter->second[i]].printLog();
				}
			}
			break;
	}
}

void game::clearExcerpt() {
	cout << "excerpt list cleared\n";
	if (excerptList.empty()) {
		cout << "(previously empty)\n";
	}
	else {
		cout << "previous contents:\n0|";
		masterList[excerptList.front()].printLog();
		cout << "...\n" << excerptList.size() - 1 << "|";
		masterList[excerptList.back()].printLog();
		excerptList.clear();
	}
}

void game::appendExrp() {
	uint32_t apos;
	cin >> apos;
	if (apos >= uint32_t(masterList.size())) {
		cerr << "Append: Invalid Position\n";
		return;
	}
	excerptList.push_back(entryOrd[apos]);
	cout << "log entry " << apos << " appended\n";
}

void game::appendSearched() {
	size_t siz = excerptList.size();
	switch (status) {
		case Status::Empty:
			cerr << "Search before you append\n";
			return;
		case Status::Key:
			for (uint32_t i = 0; i < uint32_t(searched.size()); ++i) {
				excerptList.push_back(searched[i]);
			}
			break;
		case Status::Time:
			if (!searched.empty()) {
				for (uint32_t i = searched[0]; i <= searched[1]; ++i) {
					excerptList.push_back(i);
				}
			}
			break;
		case Status::Categ:
			if (!searched.empty()) {
				auto iter = hashmap.find(masterList[searched[0]].cmpCat);
				for (uint32_t i = 0; i < uint32_t(iter->second.size()); ++i) {
					excerptList.push_back(iter->second[i]);
				}
			}
			break;
	}
	cout << excerptList.size() - siz << " log entries appended\n";
}

void game::delEtryExrp() {
	uint32_t dpos;
	cin >> dpos;
	if (dpos >= uint32_t(excerptList.size())) {
		cerr << "Delete: Invalid Position\n";
		return;
	}
	excerptList.erase(excerptList.begin() + dpos);
	cout << "Deleted excerpt list entry " << dpos << "\n";
}

void game::moveExrpBeg() {
	uint32_t dpos;
	cin >> dpos;
	if (dpos >= uint32_t(excerptList.size())) {
		cerr << "Move Begin: Invalid Position \n";
		return;
	}
	excerptList.push_front(excerptList[dpos]);
	excerptList.erase(excerptList.begin() + dpos + 1);
	cout << "Moved excerpt list entry " << dpos << "\n";
}

void game::moveExrpEnd() {
	uint32_t dpos;
	cin >> dpos;
	if (dpos >= uint32_t(excerptList.size())) {
		cerr << "Move End: Invalid Position\n";
		return;
	}
	excerptList.push_back(excerptList[dpos]);
	excerptList.erase(excerptList.begin() + dpos);
	cout << "Moved excerpt list entry " << dpos << "\n";
}

void game::sortExrp() {
	cout << "excerpt list sorted\n";
	if (excerptList.empty()) {
		cout << "(previously empty)\n";
	}
	else {
		cout << "previous ordering:\n0|";
		masterList[excerptList.front()].printLog();
		cout << "...\n" << excerptList.size() - 1 << "|";
		masterList[excerptList.back()].printLog();
		cout << "new ordering:\n0|";
		sort(excerptList.begin(), excerptList.end());
		masterList[excerptList.front()].printLog();
		cout << "...\n" << excerptList.size() - 1 << "|";
		masterList[excerptList.back()].printLog();
	}
}

void game::searchTs() {
	status = Status::Time;
	searched.clear();
	string ts;
	cin >> ts;
	if (ts.size() != 14) {
		cerr << "Invalid timestamp\n";
		return;
	}
	logData sTs(uint32_t(masterList.size()), ts, "", "");
	auto iter = lower_bound(masterList.begin(), masterList.end(), sTs, CompareTime);
	auto endIter = upper_bound(iter, masterList.end(), sTs, CompareTime);
	if (iter != endIter) {
		--endIter;
		searched.push_back(entryOrd[iter->entryID]);
		searched.push_back(entryOrd[endIter->entryID]);
		cout << "Timestamp search: " << searched[1] - searched[0] + 1 << " entries found\n";
	}
	else { cout << "Timestamp search: 0 entries found\n"; }
}

void game::searchTsBound() {
	status = Status::Time;
	searched.clear();
	string tss;
	cin >> tss;
	if (tss.size() != 29) {
		cerr << "Invalid timestamp bounds\n";
		return;
	}
	logData ts1(uint32_t(masterList.size()), tss.substr(0, 14), "", "");
	logData ts2(uint32_t(masterList.size()), tss.substr(15, 14), "", "");
	auto iter = lower_bound(masterList.begin(), masterList.end(), ts1, CompareTime);
	auto endIter = upper_bound(iter, masterList.end(), ts2, CompareTime);
	if (iter != endIter) {
		--endIter;
		searched.push_back(entryOrd[iter->entryID]);
		searched.push_back(entryOrd[endIter->entryID]);
		cout << "Timestamps search: " << searched[1] - searched[0] + 1 << " entries found\n";
	}
	else { cout << "Timestamps search: 0 entries found\n"; }
}

void game::searchCat() {
	status = Status::Categ;
	searched.clear();
	string category;
	getline(cin, category);
	category.erase(0, 1);
	transform(category.begin(), category.end(), category.begin(), ::tolower);
	auto iter = hashmap.find(category);
	if (iter != hashmap.end()) {
		searched.push_back(iter->second.front());
		cout << "Category search: " << iter->second.size() << " entries found\n";
	}
	else {
		cout << "Category search: 0 entries found\n";
	}
}

void game::searchKeyW() {
	status = Status::Key; string KeyWords;
	getline(cin, KeyWords);
	KeyWords.erase(0, 1);
	transform(KeyWords.begin(), KeyWords.end(), KeyWords.begin(), ::tolower);
	uint32_t j = 0; string word; vector<uint32_t> temp; char c = 'f';
	for (uint32_t i = 0; i <= uint32_t(KeyWords.size()); ++i) {
		if (!isalnum(KeyWords[i])) {
			word = KeyWords.substr(j, i - j);
			if (!word.empty()) {
				auto iter = keysMap.find(word);
				if (iter == keysMap.end()) {
					searched.clear();
					break;
				}
				if (c == 'f') {
					searched = iter->second; c = 'r';
				}
				else {
					temp.clear();
					set_intersection(searched.begin(), searched.end(),
						iter->second.begin(), iter->second.end(), back_inserter(temp));
					swap(searched, temp);
				}
				if (searched.empty()) {
					break;
				}
			}
			j = i + 1;
		}
	}
	cout << "Keyword search: " << searched.size() << " entries found\n";
}

#endif