#include <vector>
#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <set>

using namespace std;

typedef pair<unsigned int , string> entry;
typedef list<entry> bucket;
typedef vector<bucket> hashdict ;

hashdict dict;

set<string> resemble;
set<string> resemble2;

void builddict(const unsigned int& i,const string& str){
	int x = i % 539897;
	bucket::iterator iter;
	for(iter = dict[x].begin(); iter != dict[x].end(); iter++){
		if((i == iter->first) && (str == iter->second)){
			return;
		}
	}
	dict[x].push_back(entry(i, str));
	return;
}


unsigned int hashcode(const string& str){
	unsigned int h = 0;
	int len = str.length();
	for(int i = 0; i < len; i++){
		h = (h << 5)|(h >> 27);
		h += (unsigned int) str[i];
	}
	return h;
}


int isindict(const unsigned int& key, const string& str){
	int idx = key % 539897;
	bucket::iterator iter;
	for(iter = dict[idx].begin(); iter != dict[idx].end(); iter++){
		if((iter->first == key) && (iter->second == str)){
			return 1;   //return '1' represents that the given string is in the dict 
		}
	}
	return 0;   //return '0' represents that the given string is not in the dict
}

void insertion(string& str){
	int len = str.length();
	for(int i = 0; i <= len; i++){
		for(int j = 97; j <= 122; j++){
			str.insert(i, 1, j);
			resemble.insert(str);
			str.erase(i, 1);
		}
	}
	return;
}

void deletion(string& str){
	int len = str.length();
	for(int i = 0; i < len; i++){
		char temp;
		temp = str[i];
		str.erase(i, 1);
		resemble.insert(str);
		str.insert(i, 1, temp);
	}
	return;
}

void subtitution(string& str){
	int len = str.length();
	for(int i = 0; i < len; i++){
		for(int j = 97; j <= 122; j++){
			if(str[i] != j){
				char temp;
				temp = str[i];
				str.erase(i, 1);
				str.insert(i, 1, j);
				resemble.insert(str);
				str.erase(i, 1);
				str.insert(i, 1, temp);
			}		
		}
	}
	return;		
}

void transposition(string& str){
	int len = str.length();
	for(int i = 0; i < (len-1); i++){
		char temp = str[i];
		str[i] = str[i+1];
		str[i+1] = temp;
		resemble.insert(str);
		char temp1 = str[i];
		str[i] = str[i+1];
		str[i+1] = temp1;
	}
	return;
}

void insertion2(string& str){
	int len = str.length();
	for(int i = 0; i <= len; i++){
		for(int j = 97; j <= 122; j++){
			str.insert(i, 1, j);
			unsigned int key = hashcode(str);
			if(isindict(key, str)){
				resemble2.insert(str);
			}
			str.erase(i, 1);
		}
	}
	return;
}

void deletion2(string& str){
	int len = str.length();
	for(int i = 0; i < len; i++){
		char temp;
		temp = str[i];
		str.erase(i, 1);
		unsigned int key = hashcode(str);
		if(isindict(key, str)){
			resemble2.insert(str);
		}
		str.insert(i, 1, temp);
	}
	return;
}

void subtitution2(string& str){
	int len = str.length();
	for(int i = 0; i < len; i++){
		for(int j = 97; j <= 122; j++){
			if(str[i] != j){
				char temp;
				temp = str[i];
				str.erase(i, 1);
				str.insert(i, 1, j);
				unsigned int key = hashcode(str);
				if(isindict(key, str)){
					resemble2.insert(str);
				}
				str.erase(i, 1);
				str.insert(i, 1, temp);	
			}		
		}
	}
	return;		
}

void transposition2(string& str){
	int len = str.length();
	for(int i = 0; i < (len-1); i++){
		char temp = str[i];
		str[i] = str[i+1];
		str[i+1] = temp;
		unsigned int key = hashcode(str);
		if(isindict(key, str)){
			resemble2.insert(str);
		}
		char temp1 = str[i];
		str[i] = str[i+1];
		str[i+1] = temp1;
	}
	return;
}

void getunion(){
	set<string>::iterator iter;
	for(iter = resemble2.begin(); iter != resemble2.end(); iter++){
		resemble.insert(*iter);
	}
	return;
}

int main(void){
	ifstream fin("/tmp2/dsa2016_hw5/cmudict-0.7b");
	int cnt = 1;
	string temp;
	dict.resize(539897);
	while(getline(fin, temp)){
		if(cnt >= 57){
			size_t found = temp.find(' ',0);
			string s = temp.assign(temp, 0, found);
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			unsigned int i = hashcode(s);
			builddict(i, s);
		}
		else{
			cnt++;
		}
	}

	//finish building the dictionary

	//start the input
	string temp2;
	while(getline(cin, temp2)){
		int founded;
		for(founded = 0; (temp2[founded] != '\0') && (temp2[founded] != ' ') && (temp2[founded] != '\t'); founded++);
		string ss = temp2.assign(temp2, 0, founded);
		unsigned int k = hashcode(ss);
		if(isindict(k, ss)){
			cout << ss << " ==> OK" << endl;
		}
		else{
			insertion(ss);
			deletion(ss);
			subtitution(ss);
			transposition(ss);

			set<string>::iterator iter;
			for(iter = resemble.begin(); iter != resemble.end(); iter++){
				string temp3 = *iter;
				insertion2(temp3);
				deletion2(temp3);
				subtitution2(temp3);
				transposition2(temp3);
			}

			for(iter = resemble.begin(); iter != resemble.end(); ){
				unsigned int key = hashcode(*iter);

				if(!isindict(key, *iter)){
					resemble.erase(iter++);
				}
				else{
					iter++;
				}
			}

			getunion();

			if(resemble.size()){
				cout << ss << " ==>";
				for(iter = resemble.begin(); iter != resemble.end(); iter++){
					cout << " " << *iter;
				}
				cout << endl;
			}
			else{
				cout << ss << " ==> NONE" << endl;
			}
		}
		resemble.clear();
		resemble2.clear();
	}
	return 0;
}
