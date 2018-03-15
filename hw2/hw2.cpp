#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <stdio.h>
#include <set>
#include <algorithm>
#include <string.h>

using namespace std;
FILE *fin;
struct uit
{
	int itemsid;
	int timestamp;
	int result;
};

struct itu
{
	int usersid;
	int timestamp;
};

struct urst
{
	int usersid;
	int times;
};

class usnode{
public:
	int us;
	usnode* next;

	friend class uspeoplelinkedlist;
};

class uspeoplelinkedlist{
public:
	uspeoplelinkedlist();
	~uspeoplelinkedlist();
	bool empty() const ;
	const int& front() const ;
	void addfront(const int& e) ;
	void removefront();
	usnode* sendhead();
private:
	usnode* head;
};

uspeoplelinkedlist::uspeoplelinkedlist():head(NULL){}
uspeoplelinkedlist::~uspeoplelinkedlist(){while(!empty()) removefront();}
bool uspeoplelinkedlist::empty() const {return head == NULL;}
const int& uspeoplelinkedlist::front() const { return head->us; }
void uspeoplelinkedlist::addfront(const int& e){
	usnode *nnd = new usnode;
	nnd->us = e;
	nnd->next = head;
	head = nnd;
}
void uspeoplelinkedlist::removefront(){
	usnode *old = head;
	head = old->next;
	delete old;
}
usnode* uspeoplelinkedlist::sendhead(){ return head;}



bool uitacompare(uit lfs, uit rfs) {if (lfs.itemsid < rfs.itemsid) return true;
									else if(lfs.itemsid == rfs.itemsid) {
										if(lfs.timestamp < rfs.timestamp){
											return true;
										}
										else if(lfs.timestamp >= rfs.timestamp){
											return false;
										}
									}
									else if(lfs.itemsid > rfs.itemsid) return false;}

bool uitequal(uit lfs, uit rfs){ return ((lfs.itemsid == rfs.itemsid)&&(lfs.timestamp == rfs.timestamp));}

bool ituacompare(itu lfs, itu rfs) { if(lfs.usersid < rfs.usersid) return true;
									else if(lfs.usersid == rfs.usersid){
										if(lfs.timestamp < rfs.timestamp){
											return true;
										}
										else if(lfs.timestamp >= rfs.timestamp){
											return false;
										}
									} 
									else if(lfs.usersid > rfs.usersid) return false;}

bool ituequal(itu lfs, itu rfs) { return ((lfs.usersid == rfs.usersid)&&(lfs.timestamp == rfs.timestamp));}

bool urstcompare(urst lfs, urst rfs) {return (lfs.times < rfs.times);}

urst ut[1400000];
map<int, vector<uit> > items;
map<int, vector<itu> > users;

int hasEOF = 0;
int readchar() {
    static int N = 1<<20;
    static char buf[1<<20];
    static char *p = buf, *end = buf;
    if(p == end) {
        if((end = buf + fread(buf, 1, N, fin)) == buf) {
            hasEOF = 1;
            return EOF;   
        }
        p = buf;
    }
    return *p++;
}
int ReadInt(int *x) {
    char c, neg;
    while((c = readchar()) < '-')    {if(c == EOF) return 0;}
    neg = (c == '-') ? -1 : 1;
    *x = (neg == 1) ? c-'0' : 0;
    while((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c-'0';
    *x *= neg;
    return 1;
}

int acceptting(int u, int i, int t)
{

	vector<uit>::iterator iter;
	for(iter = items[u].begin(); iter != items[u].end(); iter++){
		if(i == iter->itemsid && t == iter->timestamp){
			return iter->result;
		}
	}
	return 0;
	
}

void itemming(int u1, int u2)   //  second 
{
	set<int> intersection;
	vector<uit>::iterator initer1 = items[u1].begin();
	vector<uit>::iterator initer2 = items[u2].begin();
	while((initer1 != items[u1].end()) && (initer2 != items[u2].end())){
		if(initer1->itemsid > initer2->itemsid){
			initer2++;
		}
		else if(initer1->itemsid < initer2->itemsid){
			initer1++;
		}
		else {
			intersection.insert(initer1->itemsid);
			initer1++;
			initer2++;
		}
	}

	if(intersection.size() > 0){
		for(set<int>::iterator p = intersection.begin(); p != intersection.end(); p++){
			cout << *p << endl;
		}
	}
	else{
		cout << "EMPTY" << endl;
	}
	return;
}

void usersing(int i1, int i2, int t1, int t2)
{
	set<int> intersection;
	vector<itu>::iterator iiniter1 = users[i1].begin();
	vector<itu>::iterator iiniter2 = users[i2].begin();

	while((iiniter1 != users[i1].end()) && (iiniter2 != users[i2].end())){
		if(iiniter1->usersid > iiniter2->usersid){
			iiniter2++;
		}
		else if(iiniter1->usersid < iiniter2->usersid){
			iiniter1++;
		}
		else{
			if(((t1 <= iiniter1->timestamp) && (iiniter1->timestamp <= t2)) && ((t1 <= iiniter2->timestamp) && (iiniter2->timestamp <= t2))){
				intersection.insert(iiniter1->usersid);
				iiniter1++;
				iiniter2++;
			}
			else if(((t1 <= iiniter1->timestamp) && (iiniter1->timestamp <= t2)) && ((t1 >= iiniter2->timestamp) || (iiniter2->timestamp >= t2))){
				iiniter2++;
			}
			else if(((t1 <= iiniter2->timestamp) && (iiniter2->timestamp <= t2)) && ((t1 >= iiniter1->timestamp) || (iiniter1->timestamp >= t2))){
				iiniter1++;
			}
			else if(((t1 >= iiniter1->timestamp) || (iiniter1->timestamp >= t2)) && ((t1 >= iiniter2->timestamp) || (iiniter2->timestamp >= t2))){
				iiniter1++;
				iiniter2++;
			}
		}
	}

	if(intersection.size() > 0){
		for(set<int>::iterator q = intersection.begin(); q != intersection.end(); q++){
			cout << *q << endl;
		}
	}
	else{
		cout << "EMPTY" << endl;
	}
	
	return;
}

void ratioing(int i, int threshold, int l)
{
	int idx;

	int j;
	for(j = 0; j < l; j++){
		if(ut[j].times > threshold){
			idx = j;
			break;
		}
	}

	int total;
	int accept = 0;
	if(j >= l){
		total = 0;
	}
	else{
		total = l - idx;
		for(int k = idx; k < l; k++){
			vector<uit>::iterator imiter;
			for(imiter = items[ut[k].usersid].begin(); imiter != items[ut[k].usersid].end(); imiter++){
				if(i == imiter->itemsid){
					if(acceptting(ut[k].usersid, i, imiter->timestamp) == 1){
						accept++;
						break;
					}
				}
			}
		}
	}
	
	cout << accept << "/" << total << endl;
	return;
}

void findtimeiteming(int i, usnode* u){    //advance the singly linked list i	
	usnode* ptr = u;
	int count3 = 0;
	set<int> sftis;
	while(ptr != NULL){
		vector<itu>::iterator inu;
		for(inu = users[i].begin(); inu != users[i].end(); inu++){
			if(ptr->us == inu->usersid){
				sftis.insert(inu->timestamp);
				count3++;
			}
		}
		ptr = ptr->next;
	}

	if(count3 == 0){
		cout << "EMPTY" << endl;
	}
	else{
		for(set<int>::iterator it = sftis.begin(); it != sftis.end(); it++){
			cout << *it << endl;
		}
	}

	return;
}

int main(void)
{
	fin = fopen("/tmp2/KDDCUP2012/track1/rec_log_train.txt", "rb");
	int u = 0, i = 0, r = 0, t = 0;  //u = userid, i = itemid, r = result, t = unix-timestamp
	int k = 0;

	while(ReadInt(&u) && ReadInt(&i) && ReadInt(&r) && ReadInt(&t)){
		uit temp1; //items' timestamp
		temp1.itemsid  = i;
		temp1.timestamp = t;
		temp1.result = r;
		items[u].push_back(temp1);
		itu temp2;
		temp2.timestamp = t;
		temp2.usersid = u;
		users[i].push_back(temp2);
		k++;
	}

	map<int, vector<uit> >::iterator outiter1;
	for(outiter1 = items.begin(); outiter1 != items.end(); outiter1++){
		vector<uit>::iterator initerstart1;
		vector<uit>::iterator initerend1;
		initerstart1 = outiter1->second.begin();
		initerend1 = outiter1->second.end();
		sort(initerstart1, initerend1, uitacompare);
		vector<uit>::iterator uitit;
  		uitit = unique(initerstart1, initerend1, uitequal);
  		outiter1->second.erase(uitit, initerend1);
	}
	
	map<int, vector<itu> >::iterator outiter2;
	for(outiter2 = users.begin(); outiter2 != users.end(); outiter2++){
		vector<itu>::iterator initerstart2;
		vector<itu>::iterator initerend2;
		initerstart2 = outiter2->second.begin();
		initerend2 = outiter2->second.end();
		sort(initerstart2, initerend2, ituacompare);
		vector<itu>::iterator ituit;
  		ituit = unique(initerstart2, initerend2, ituequal);
  		outiter2->second.erase(ituit, initerend2);
	}

	map<int, vector<uit> >::iterator outiter3;
	int len = 0;
	for(outiter3 = items.begin(); outiter3 != items.end(); outiter3++){
		ut[len].usersid = outiter3->first;
		ut[len].times = outiter3->second.size();
		len++;
	}

	sort(ut, ut+len, urstcompare);

	int n = 0;
	scanf("%d", &n);
	while(n--){
		char s[100];
		int u1 = 0, u2 = 0, i1 = 0, i2 = 0, t1 = 0, t2 = 0, threshold = 0; 
		cin >> s;
		if(!strcmp(s, "accept")){
			int gg;
			cin >> u1 >> i1 >> t1;
			gg = acceptting(u1, i1, t1);
			cout << gg << endl;
		}
		else if(!strcmp(s, "items")){
			cin >> u1 >> u2;
			itemming(u1, u2);
		}
		else if(!strcmp(s, "users")){
			cin >> i1 >> i2 >> t1 >> t2;
			usersing(i1, i2, t1, t2);
		}
		else if(!strcmp(s, "ratio")){
			cin >> i1 >> threshold;
			ratioing(i1, threshold, len);
		}
		else if(!strcmp(s, "findtime_item")){
			uspeoplelinkedlist userspeople;
			cin >> i1;
			int usid;
			while(scanf("%d", &usid) == 1){
				userspeople.addfront(usid);
			}
			findtimeiteming(i1, userspeople.sendhead());
		}
	}

	fclose(fin);

	return 0;
}