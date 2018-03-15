#include <stdio.h>
#include <vector>
#include <set>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef struct data{
	int bid;
	int cid;
	int p;
	int sc;
} data;

typedef struct transaction{
	int berid;
	int serid;
	int tp;
	int tsc;
}transaction;

class vectorcompletetree{
private:
	vector<data> vct;
public:
	typedef vector<data>::iterator position;
protected:
	position pos(int i){
		return (vct.begin() + i);		
	}
	
	int idx(const position& p) const{
		return (p - vct.begin());		
	}
	
public:
	vectorcompletetree():vct(1){}  //constructor
	int size() const {return (vct.size() - 1); }
	position left(const position& p)  {return pos(2*idx(p)); }
	position right(const position& p)  {return pos(2*idx(p)+1); }
	position parent(const position& p) {return pos(idx(p)/2); }
	bool hasleft(const position& p) const {return (2*idx(p) <= size() ); }
	bool hasright(const position& p) const {return ((2*idx(p)+1) <= size()); }
	bool isroot(const position& p) const {return (idx(p) == 1); }
	position root()  {return pos(1); }
	position last()  {return pos(size()); }
	void addlast(const data& e)   {vct.push_back(e); }
	void removelast()   {vct.pop_back(); }
	void swap(const position& p, const position& q)  {data e = *q; *q = *p; *p = e; } 
};		

class minheappriorityqueue{
public:
	int size() const;
	bool empty() const;
	void insert(const data& e);
	data& min();
	void removemin();
private:
	vectorcompletetree  minvct;
	bool isless(data& a, data& b);

	typedef vectorcompletetree::position position;
};

int minheappriorityqueue::size() const {return minvct.size(); }
bool minheappriorityqueue::empty() const {return (size() == 0); }
void minheappriorityqueue::insert(const data& e)  {
	minvct.addlast(e);   //add e to heap
	position v = minvct.last();  //e's position
	while(!minvct.isroot(v)) {   //up-heap bubbling
		position u = minvct.parent(v);
		if(!isless(*v, *u)){   //if v in order, we're done
			break;
		}
		minvct.swap(v, u);   // ...else swap with parent
		v = u;
	}
}
data& minheappriorityqueue::min() {return *(minvct.root()); }   //*(minvct .root()) = data
void minheappriorityqueue::removemin(){
	if(size() == 1){
		minvct.removelast();
	}
	else{
		position u = minvct.root();
		minvct.swap(u, minvct.last());
		minvct.removelast();
		while(minvct.hasleft(u)) {
			position v = minvct.left(u);
			if(minvct.hasright(u) && isless(*(minvct.right(u)), *v)){
				v = minvct.right(u);
			}
			if(isless(*v, *u)){
				minvct.swap(u, v);
				u = v;
			}
			else {
				break;
			}
		}
	}
}
bool minheappriorityqueue::isless(data& a, data& b){
	if(a.p < b.p){
		return true;
	}
	else if(a.p == b.p){
		if(a.bid < b.bid){
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

class maxheappriorityqueue{
public:
	int size() const;
	bool empty() const;
	void insert(const data& e);
	data& max();
	void removemax();
private:
	vectorcompletetree  maxvct;
	bool isgreater(data& a, data& b);

	typedef vectorcompletetree::position position;
};

int maxheappriorityqueue::size() const {return maxvct.size(); }
bool maxheappriorityqueue::empty() const {return (size() == 0); }
void maxheappriorityqueue::insert(const data& e)  {
	maxvct.addlast(e);   //add e to heap
	position v = maxvct.last();  //e's position
	while(!maxvct.isroot(v)) {   //up-heap bubbling
		position u = maxvct.parent(v);
		if(!isgreater(*v, *u)){   //if v in order, we're done
			break;
		}
		maxvct.swap(v, u);   // ...else swap with parent
		v = u;
	}
}
data& maxheappriorityqueue::max() {return *(maxvct.root()); }   //*(minvct .root()) = data
void maxheappriorityqueue::removemax(){
	if(size() == 1){
		maxvct.removelast();
	}
	else{
		position u = maxvct.root();
		maxvct.swap(u, maxvct.last());
		maxvct.removelast();
		while(maxvct.hasleft(u)) {
			position v = maxvct.left(u);
			if(maxvct.hasright(u) && isgreater(*(maxvct.right(u)), *v)){
				v = maxvct.right(u);
			}
			if(isgreater(*v, *u)){
				maxvct.swap(u, v);
				u = v;
			}
			else {
				break;
			}
		}
	}
}
bool maxheappriorityqueue::isgreater(data& a, data& b){
	if(a.p > b.p){
		return true;
	}
	else if(a.p == b.p){
		if(a.bid < b.bid){
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

set<int> cancel;
maxheappriorityqueue buyer;
minheappriorityqueue seller;
vector<transaction> result;

int iscancelled(int b, int s){
	int t = 0;
	if(cancel.find(b) != cancel.end()){  //buyer.bid is in the 'cancel' chart
		buyer.removemax();
		t = 1;	
	}

	if(cancel.find(s) != cancel.end()){  //seller.bid is in the 'cancel' chart
		seller.removemin();
		t = 1;
	}

	if(t == 1){  //iscancelled
		return 1;
	}
	else{
		return 0;  //not iscancelled
	}
}

int& choose(int& a, int& b){
	return (a <= b ? a : b);
}

int main(void)
{
	int a;
	data t;
	while(scanf("%d%d%d%d%d", &t.bid, &t.cid, &a, &t.p, &t.sc) == 5){
	
		if(a == 0){ //build buyer's max-heap
			buyer.insert(t);	
		}
		else if(a == 1){ //build seller's min heap
			seller.insert(t);
		}
		else if(a == 2){  //build cancaller's set chart
			cancel.insert(t.p);
			continue; //not sure to program
		}


		while(!buyer.empty() && !seller.empty() && buyer.max().p >= seller.min().p){  //if (!buyer.isempty || !seller.isempty || buyer.max >= seller.min), the transaction can be continued
			if(!iscancelled(buyer.max().bid, seller.min().bid)){ //but if either the buyer's or seller's bidid appeared in the 'cancel' chart before the transation, continue to the next
				//one transaction starts
				transaction deal;
				deal.berid = buyer.max().cid;
				deal.serid = seller.min().cid;
				deal.tp = seller.min().p;
				deal.tsc = choose(buyer.max().sc, seller.min().sc);
				result.push_back(deal);
				//one transaction is over
				//rearrange the maxheap of 'buyer' and the minheap of 'seller'
				buyer.max().sc -= deal.tsc;
				seller.min().sc -= deal.tsc;

				if(buyer.max().sc == 0){
					buyer.removemax();
				}
				
				if(seller.min().sc == 0){
					seller.removemin();
				}
				//next
			}
		}	
	} //total data is read over
	
	vector<transaction>::iterator iter;
	int i;
	for(iter = result.begin(), i = 0; iter != result.end(); iter++, i++){  //output the final result
		printf("%d\t%d\t%d\t%d\t%d\n", i, iter->berid, iter->serid, iter->tp, iter->tsc);
	}

	return 0;
}
