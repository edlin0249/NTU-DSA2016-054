#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;
#define swap(x, y) {double t; t = x; x = y; y = t;}
#define absv(x) ((x)>0? (x) : (-x))
double mat[300][300] = {};
double orimat[300][300] = {};
double inv[300][300] = {};
double err[300][300] = {};
int main(void)
{
	int n;
	cin >> n;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cin >> mat[i][j];
			orimat[i][j] = mat[i][j];
		}
	}
	//the right-half side of the argumented matrix is a identity matrix
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i == j){    
				inv[i][j] = 1;
			}
			else{
				inv[i][j] = 0;
			}
		}
	}
	
	//searchinverse by Gaussian-jordan elimiation : straightly perform row-operation and 
	for(int i = 0; i < n; i++){  //i-th col  
		
		for(int j = i+1; j < n; j++){
			if(mat[i][i] != 0){
				break;
			}
			else{
				if(mat[j][i] == 0){
					continue;
				}
				else{
					for(int k = 0; k < n; k++){
						swap(mat[i][k], mat[j][k]);
						swap(inv[i][k], inv[j][k]);
					}
					break;
				}
			}
		}

		for(int j = 0; j < n; j++){  //run "row" from 0 to n-1  
			for(int k = n-1; k >= 0; k--){  //run "col" from the tail to the head
				if(i == j){
					inv[j][k] /= mat[i][i];		
				}
				else{
					inv[j][k] -= mat[j][i]/mat[i][i] * inv[i][k];
				}
			}
			for(int k = n-1; k >= i; k--){
				if(i == j){
					mat[j][k] /= mat[i][i];
				}
				else{
					mat[j][k] -= mat[j][i]/mat[i][i] * mat[i][k];
				}
			}
		}
	}
	//error:matrix multi => matrix substraction => choose max element
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			for(int k = 0; k < n; k++){
				err[i][j] += orimat[i][k]*inv[k][j];
			}
			if(i == j){
				err[i][j] -= 1;
			}
		}
	}
	double maxerr = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(absv(err[i][j]) > absv(maxerr)){
				maxerr = absv(err[i][j]);
			}
		}
	}
	//print the inverse
	cout << fixed << setprecision(12) << maxerr << endl;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout << fixed << setprecision(12) << inv[i][j];
			if(j < n-1){
				cout << ' ';
			}
		}
		cout << endl;
	}
	return 0;
}