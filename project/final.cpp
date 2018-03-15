#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cctype>

using namespace std;

typedef pair<unsigned int, string> entry;
typedef list<entry> bucket;
typedef vector<bucket> database;
typedef struct wordsseq{
	string assemble;
	int ifprep;
} wordsseq;

database prepdb;

string norprep[20] = {"of", "to", "in", "for", "with", "on", "at", "by", "from", "up", "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};

int prepexist;
vector<string> edit1;
vector<string> edit2;
vector<wordsseq> seq;
vector<entry> output;

void insertprepdb(const unsigned int& key, const entry& t){
	int x = key % 217645229;
	prepdb[x].push_back(t);
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

void builddb1(){
	ifstream fin1("/tmp2/dsa2016_project/2gm.small.txt");
	string tmp;
	while(getline(fin1, tmp)){
		entry t;
		int idx;
		int len = tmp.length();
		for(idx = 0; !isdigit(tmp[idx]); idx++);  //find where the first number
		t.second.assign(tmp, 0, idx-1);  //assign the sentences
		t.first = (tmp[idx] - '0');   //assign the freq
		for(int i = idx+1; i < len; i++){
			t.first *= 10;
			t.first += (tmp[i] - '0');
		}
		unsigned int key = hashcode(t.second);  //assign the key;
		insertprepdb(key, t);
	}
	fin1.close();
	return;
}

void builddb2(){
	ifstream fin2("/tmp2/dsa2016_project/3gm.small.txt");
	string tmp;
	while(getline(fin2, tmp)){
		entry t;
		int idx;
		int len = tmp.length();
		for(idx = 0; !isdigit(tmp[idx]); idx++);  //find where the first number
		t.second.assign(tmp, 0, idx-1);  //assign the sentences
		t.first = (tmp[idx] - '0');   //assign the freq
		for(int i = idx+1; i < len; i++){
			t.first *= 10;
			t.first += (tmp[i] - '0');
		}
		unsigned int key = hashcode(t.second);  //assign the key;
		insertprepdb(key, t);
	}
	fin2.close();
	return;
}

void builddb3(){
	ifstream fin3("/tmp2/dsa2016_project/4gm.small.txt");
	string tmp;
	while(getline(fin3, tmp)){
		entry t;
		int idx;
		int len = tmp.length();
		for(idx = 0; !isdigit(tmp[idx]); idx++);  //find where the first number
		t.second.assign(tmp, 0, idx-1);  //assign the sentences
		t.first = (tmp[idx] - '0');   //assign the freq
		for(int i = idx+1; i < len; i++){
			t.first *= 10;
			t.first += (tmp[i] - '0');
		}
		unsigned int key = hashcode(t.second);  //assign the key;
		insertprepdb(key, t);
	}
	fin3.close();
	return;
}

void builddb4(){
	ifstream fin4("/tmp2/dsa2016_project/5gm.small.txt");
	string tmp;
	while(getline(fin4, tmp)){
		entry t;
		int idx;
		int len = tmp.length();
		for(idx = 0; !isdigit(tmp[idx]); idx++);  //find where the first number
		t.second.assign(tmp, 0, idx-1);  //assign the sentences
		t.first = (tmp[idx] - '0');   //assign the freq
		for(int i = idx+1; i < len; i++){
			t.first *= 10;
			t.first += (tmp[i] - '0');
		}
		unsigned int key = hashcode(t.second);  //assign the key;
		insertprepdb(key, t);
	}
	fin4.close();
	return;
}

int isprepandhmwds(const string& tmp){
	int len = tmp.length();
	int prefix = 0, suffix;
	string tmp2;
	int successive = -1;  //the successive condition of the first word is what I haven't known, if successive == 1, then it represent that the last word is prep; however, if successive == 0, then it represent that the last word is nonprep. 
	int hmwds = 0;
	for(int i = 0; i <= len; i++){
		int prepflag = 0;   //if it's nonprep originally
		wordsseq wdsq;
		if(tmp[i] == ' ' || tmp[i] == '\0'){
			hmwds++;
			suffix = i;
			tmp2.assign(tmp, prefix, suffix-prefix);
			for(int j = 0; j < 20; j++){
				if(tmp2.compare(norprep[j]) == 0){    //whether it's the normal prep
					prepexist = 1;
					prepflag = 1;
					break;
				}
			}
			
			if(prepflag == 1){ //"prepflag == 1" represents prep
				if(successive == 0 || successive == -1){   //last word is not prep, and the first word is included here.
					wdsq.ifprep = 1;
					wdsq.assemble = tmp2;
					seq.push_back(wdsq);
				}
				else{  //last word is prep
					int s = seq.size();	
					seq[s-1].assemble.append(" " + tmp2);
				}
				successive = prepflag;
			}
			else{    //"prepflag == 0" represents nonprep; it's nonsuccessive, too.
				if(successive == 1 || successive == -1){
					wdsq.ifprep = 0;
					wdsq.assemble = tmp2;
					seq.push_back(wdsq);
				}
				else{
					int s = seq.size();
					seq[s-1].assemble.append(" " + tmp2);
				}
				successive = prepflag;
			}
			prefix = suffix + 1;
		}
	}
	return hmwds;
}

void isinprepdbandbuildoutput(const unsigned int& key, const string& tmp){
	int idx = key % 217645229;
	bucket::iterator iter;
	for(iter = prepdb[idx].begin(); iter != prepdb[idx].end(); iter++){
		if(tmp.compare(iter->second) == 0){  // if in the google web 1T
			output.push_back(*iter);
		}
	}
	return;
}

bool compare(entry i, entry j){
	if(i.first > j.first){
		return true;
	}
	else if(i.first == j.first){
		if(i.second.compare(j.second) < 0){
			return true;
		}
		else if(i.second.compare(j.second) == 0){
			return false;
		}
		else if(i.second.compare(j.second) > 0){
			return false;
		}
	}
	else if(i.first < j.first){
		return false;
	}
}

bool entryequal(entry i, entry j){
	return ((i.first == j.first)&&(i.second == j.second));
}

void result(const string& tmp){
	cout << "query: " << tmp << endl;
	if(output.size() >= 10){
		cout << "output: " << "10" << endl;
	}
	else{
		cout << "output: " << output.size() << endl;
	}
	vector<entry>::iterator iter;
	int cnt = 0;
	for(iter = output.begin(); iter != output.end(); iter++){
		if(cnt >= 10){
			break;
		}
		else{
			cout << iter->second << "\t" << iter->first << endl;
			cnt++;
		}
	}
	return;
}



void query(){
	string tmp;
	while(getline(cin, tmp)){
		prepexist = 0;
		int hmwds = isprepandhmwds(tmp);
		if(prepexist){
			vector<wordsseq>::iterator iter;
			vector<string> maksent;
			vector<string> scratch;
			for(iter = seq.begin(); iter != seq.end(); iter++){
				vector<string> candidate;
				if(iter->ifprep == 1){  //the query include the prep
					//insertion
					int len = iter->assemble.length();
					for(int i = 0; i <= len; i++){
						if(i == 0){
							for(int j = 0; j < 20; j++){
								string tmp2 = norprep[j] + " " + iter->assemble;
								candidate.push_back(tmp2);
							}
						}
						
						if(iter->assemble[i] == ' ' || iter->assemble[i] == '\0'){
							for(int j = 0; j < 20; j++){
								string tmp2 = iter->assemble;
								tmp2.insert(i, " "+norprep[j]);
								candidate.push_back(tmp2);
							}
						}
					}
					//deletion
					int label = 0;
					for(int i = 0; i <= len; i++){
						if(iter->assemble[i] == ' ' || iter->assemble[i] == '\0'){
							string tmp2 = iter->assemble;
							if(label == 0){
								tmp2.erase(label, i+1);
							}
							else{
								tmp2.erase(label, i-label);
							}
							candidate.push_back(tmp2);
							label = i;
						}
					}
					//subtitution
					int label2 = 0;
					for(int i = 0; i <= len; i++){
						if(iter->assemble[i] == ' ' || iter->assemble[i] == '\0'){
							if(label2 == 0){
								for(int j = 0; j < 20; j++){
									string tmp2 = iter->assemble;
									tmp2.replace(label2, i, norprep[j]);
									candidate.push_back(tmp2);
								}
							}
							else{
								for(int j = 0; j < 20; j++){
									string tmp2 = iter->assemble;
									tmp2.replace(label2, i-label2, norprep[j]);
									candidate.push_back(tmp2);
								}
							}
							label2 = i + 1;
						}
					}
				}
				else{
					candidate.push_back(iter->assemble);
				}
				//begin to combine the sentence by the extended-prep according to the cardesian product
				if(maksent.size()){
					vector<string>::iterator iter1;
					vector<string>::iterator iter2;
					for(iter1 = maksent.begin(); iter1 != maksent.end(); iter1++){
						for(iter2 = candidate.begin(); iter2 != candidate.end(); iter2++){
							if((iter1->length() > 0)&&(iter2->length() > 0)){
								string tmp2 = *iter1 + " " + *iter2;
								scratch.push_back(tmp2);
							}
							else if((iter1->length() > 0) && (iter2->length() == 0)){
								string tmp2 = *iter1;
								scratch.push_back(tmp2);
							}
							else if((iter1->length() == 0) && (iter2->length() > 0)){
								string tmp2 = *iter2;
								scratch.push_back(tmp2);
							}
						}
					}
					
					maksent.clear();
					
					vector<string>::iterator iter3;
					for(iter3 = scratch.begin(); iter3 != scratch.end(); iter3++){
						maksent.push_back(*iter3);
					}
					
					scratch.clear();
				}
				else{  //initial the structure = initial a sentence
					vector<string>::iterator iter;
					for(iter = candidate.begin(); iter != candidate.end(); iter++){
						maksent.push_back(*iter);
					}
				}
			}
			//making sentences is over
			int cnt = 0;
			int len = tmp.length();
			for(int i = 0; i <= len; i++){
				if(tmp[i] == ' ' || tmp[i] == '\0'){
					cnt++;
				}
			}
			if((cnt >= 2) && (cnt <= 5)){
				unsigned int key = hashcode(tmp);
				isinprepdbandbuildoutput(key, tmp);
			}
			vector<string>::iterator iter2;
			for(iter2 = maksent.begin(); iter2 != maksent.end(); iter2++){
				int cnt2 = 0;
				int len2 = iter2->length();
				for(int i = 0; i <= len2; i++){
					if((*iter2)[i] == ' ' || (*iter2)[i] == '\0'){
						cnt2++;
					}
				}
				if((cnt2 >= 2) && (cnt2 <= 5)){
					unsigned int key2 = hashcode(*iter2);
					isinprepdbandbuildoutput(key2, *iter2);
				}
			}
			sort(output.begin(), output.end(), compare);
			vector<entry>::iterator it;
			it = unique(output.begin(), output.end(), entryequal);
			output.erase(it, output.end());
			result(tmp);
			output.clear();
			seq.clear();
		}
		else{
			if(hmwds > 5){  //howmanywords > 5
				result(tmp);
			}
			else if(hmwds == 5){   //howmanywords == 5
				unsigned int key = hashcode(tmp);
				isinprepdbandbuildoutput(key, tmp);
				result(tmp);
				output.clear();
			}
			else{     //howmanywords < 5
				int len = tmp.length();
				for(int i = 0; i <= len; i++){
					if(i == 0){
						for(int j = 0; j < 20; j++){
							string tmp2 = norprep[j] + " " + tmp;
							edit1.push_back(tmp2);
						}
					}
					
					if(tmp[i] == ' ' || tmp[i] == '\0'){
						for(int j = 0; j < 20; j++){
							string tmp2 = tmp;
							tmp2.insert(i, " " + norprep[j]);
							edit1.push_back(tmp2);
						}
					}
				}
				
				if(hmwds == 4){    //howmanywords == 4
					unsigned int key = hashcode(tmp);
					isinprepdbandbuildoutput(key, tmp);
					vector<string>::iterator iter;
					for(iter = edit1.begin(); iter != edit1.end(); iter++){
						unsigned int key = hashcode(*iter);
						isinprepdbandbuildoutput(key, *iter);
					}
					sort(output.begin(), output.end(), compare);
					vector<entry>::iterator it;
					it = unique(output.begin(), output.end(), entryequal);
					output.erase(it, output.end());
					result(tmp);
					edit1.clear();
					output.clear();
				}	
				else{        //howmanywords < 4
					vector<string>::iterator iter;
					for(iter = edit1.begin(); iter != edit1.end(); iter++){
						int len2 = (*iter).length();
						for(int j = 0; j <= len2; j++){
							if(j == 0){
								for(int k = 0; k < 20; k++){
									string tmp3 = norprep[k] + " " + (*iter);
									edit2.push_back(tmp3);
								}
							}
							
							if((*iter)[j] == ' ' || (*iter)[j] == '\0'){
								for(int k = 0; k < 20; k++){
									string tmp3 = (*iter);
									tmp3.insert(j, " " + norprep[k]);
									edit2.push_back(tmp3);
								}
							}
						}
					}
					unsigned int key = hashcode(tmp);
					isinprepdbandbuildoutput(key, tmp);
					vector<string>::iterator iter2;
					for(iter2 = edit1.begin(); iter2 != edit1.end(); iter2++){
						unsigned int key2 = hashcode(*iter2);
						isinprepdbandbuildoutput(key2, *iter2);
					}	
					vector<string>::iterator iter3;
					for(iter3 = edit2.begin(); iter3 != edit2.end(); iter3++){
						unsigned int key3 = hashcode(*iter3);
						isinprepdbandbuildoutput(key3, *iter3);
					}
					sort(output.begin(), output.end(), compare);
					vector<entry>::iterator it;
					it = unique(output.begin(), output.end(), entryequal);
					output.erase(it, output.end());
					result(tmp);
					edit1.clear();
					edit2.clear();
					output.clear();
					
				}				
			}
			seq.clear();
		}
	}
	
	return;
}	
		
int main(int argc, char *argv[]){
	prepdb.resize(217645229);
	builddb1();
	builddb2();
	builddb3();
	builddb4();
	query();
	return 0;
}

