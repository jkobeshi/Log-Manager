// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include "logman.h"

void game::read(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << "\n";
		exit(1);
	}
	string ts, cat, msg;
	uint32_t i = 0; uint32_t j; uint32_t k;
	while (getline(file, ts, '|')) {
		getline(file, cat, '|');
		getline(file, msg);
		masterList.emplace_back(logData(i, ts, cat, msg));
		++i;
	}
	cout << masterList.size() << " entries read\n";
	sort(masterList.begin(), masterList.end(), CompareLog);
	entryOrd.resize(masterList.size()); 
	for (i = 0; i < uint32_t(masterList.size()); ++i) { 
		entryOrd[masterList[i].entryID] = i;
		hashmap[masterList[i].cmpCat].push_back(i); j = 0;
		for (k = 0; k <= uint32_t(masterList[i].cmpCat.size()); ++k) {
			if (!isalnum(masterList[i].cmpCat[k])) {
				msg = masterList[i].cmpCat.substr(j, k - j);
				if (!msg.empty()) {
					auto iter = keysMap.find(msg);
					if ((iter == keysMap.end() || iter->second.back() != i)) {
						keysMap[msg].push_back(i);
					}
				}
				j = k + 1;
			}
		} j = 0;
		cat = masterList[i].msg;
		transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
		for (k = 0; k <= uint32_t(cat.size()); ++k) {
			if (!isalnum(cat[k])) {
				msg = cat.substr(j, k - j);
				if (!msg.empty()) {
					auto iter = keysMap.find(msg);
					if (iter == keysMap.end() || iter->second.back() != i) {
						keysMap[msg].push_back(i);
					}
				}
				j = k + 1;
			}
		}
	}
}

void game::run() {  
	char cmd; string disp;
	do { 
		cout << "% "; 
		cin >> cmd; 
		switch (cmd) {
			case 't':
				searchTsBound();
				break;  
			case 'm':
				searchTs();
				break;
			case 'c': 
				searchCat();
				break; 
			case 'k': 
				searchKeyW();
				break; 
			case 'a':
				appendExrp();
				break;
			case 'r':
				appendSearched();
				break;
			case 'd':
				delEtryExrp();
				break;
			case 'b':
				moveExrpBeg();
				break;  
			case 'e':
				moveExrpEnd(); 
				break;
			case 's':
				sortExrp();
				break;
			case 'l':
				clearExcerpt();  
				break;
			case 'g':
				printSearched();
				break;
			case 'p':
				printExcerpt();
				break;
			case '#':
				getline(cin, disp);
				break;
			default:
				if (cmd != 'q') {
					cerr << "Invalid Command\n";
				} 
				break;
		} 
	} while (cmd != 'q');
}

int main(int argc, char* argv[]) {
	if (argc > 2 || argc < 2) {
		cerr << "Error, too many or too few inputs!\n";
		exit(1);
	}
	else if (string(argv[1]) == "-h" || string(argv[1]) == "--help") {
		cout << "This is a Log managing program.\nEnter a log file to begin analyzing logs.\n";
		exit(0);
	}
	else {
		game Game; 
		Game.read(argv[1]);
		Game.run(); 
	}
	return 0;
}