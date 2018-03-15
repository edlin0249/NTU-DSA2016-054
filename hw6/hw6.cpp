#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define abs(x) ((x)>0?(x):-(x))

#define min(x, y) ((x)<(y)?(x):(y))

double pv[1005];
double nv[105];
double routmap[1005][105];
int ans[105];
int l;

int compr(const void *a, const void *b){
	if(routmap[l][*(int*)a] < routmap[l][*(int*)b]){ 
		return -1; 
	}
	else if(routmap[l][*(int*)a] == routmap[l][*(int*)b]){
		return 0;
	}
	else if(routmap[l][*(int*)a] > routmap[l][*(int*)b]){
		return 1;
	}
}

double dp(int m, int n){

	if(m == 0 && n == 0){      //stopping condition
		return routmap[m][n] = abs(pv[m] - nv[n]); 
	}
	
	if(routmap[m][n] > 0){    //memorizing by two-array to search can speed up
		return routmap[m][n];
	}

	if(m == n){
		return (routmap[m][n] = (abs(pv[m]-nv[n]) + dp(m-1, n-1)));
	}
	
	if(m > 0 && n == 0){          //dp keep
		return (routmap[m][n] = (abs(pv[m]-nv[n]) + dp(m-1, n)));
	}
	
	return (routmap[m][n] = (abs(pv[m]-nv[n]) + min(dp(m-1, n), dp(m-1, n-1))));

}

int main(void){
	int m;
	scanf("%d", &m);
	for(int i = 0; i < m; i++){
		scanf("%lf", &pv[i]);
	}

	int n;
	scanf("%d", &n);
	for(int i = 0; i < n; i++){
		scanf("%lf", &nv[i]);
	}

	memset(routmap, 0, sizeof(routmap));

	for(int i = 0; i < n; i++){  //dp along the rightest col
		ans[i] = i;
		dp(m-1, i);
	}

	l = m-1;
	qsort(ans, n, sizeof(int), compr);
	
	int x = m-1, y = ans[0];
	int recpath[105];
	int size = 0;
	while(x != 0){
		if(y != 0){
			if(x == y){
				recpath[size] = x;
				size++;
				x--;
				y--;
			}
			else{ 
				if(min(routmap[x-1][y], routmap[x-1][y-1]) == routmap[x-1][y-1]){
					recpath[size] = x;
					size++;
					x--;
					y--;
				}
				else{
					x--;
				}
			}	
		}
		else{
			x--;
		}
	}
	recpath[size] = 0;
	size++;

	printf("%f\n", routmap[m-1][ans[0]]);
	if(size > 1){
		printf("%d", recpath[size-1]);
		for(int i = size-2; i >= 0; i--){
			printf(" %d", recpath[i]);
		}
	}
	else{
		printf("%d", recpath[0]);
	}
	printf("\n");	
	return 0;
}
