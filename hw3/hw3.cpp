#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <vector>

using namespace std; 

int row, col;
int importrow[30][30] = {}, importcol[30][30] = {};
int rowsize[30], colsize[30];
int currow[30] = {};
int freespace[30];
vector<int> rpst[30];
int see = 0;
int checkcurcolvl[30][30] = {}, checkcurcolnum[30][30] = {};
int remain[30] = {};
int cursumcol[30][30] = {};

void readimport();
void rowstats();
inline void rowperm(int rpos, int improwcur, int fsp, int perm, int shift);
inline int bits(int b);
inline void colstats();
inline void success();
inline int checkcol(int rpos);
inline void dfs(int rpos);

int main(void)
{
	readimport();   //ok
	rowstats();
	for(int i = 0; i < row; i++){    //every possible permutation of each row in bitwise representation
		rowperm(i, 0, freespace[i], 0, 0);
	}
	colstats();
	dfs(0);
	return 0;
}


void readimport()
{
  char h[5];
  scanf("%d%d", &row, &col);
  fgets(h, 5, stdin);
  for(int i = 0; i < row; i++){
    char s[35];
    char n[3] = "\0\0";
    int t = 0;
    fgets(s, 35, stdin);
    int j = 0;
    int l = 0;
    int k = 0;
    while(1){

      if(s[j] != ' ' && s[j] != '\n'){
        n[k] = s[j];
        k++;
      }
      else{
        t = 1;
      }
     
      if(t == 1){
        if(strlen(n)>0){
          importrow[i][l] = atoi(n);
          //printf("importrow[%d][%d] = %d \n", i, l, importrow[i][l]);
          l++;
          memset(n, 0, sizeof(n));
        }
        k = 0;
        t = 0;
      }

      if(s[j] == '\n'){
        rowsize[i] = l;
        break;
      }

      j++;
    }
  }

  for(int i = 0; i < col; i++){
    char s[35];
    char n[3] = "\0\0";
    int t = 0;
    fgets(s, 35, stdin);
    int j = 0;
    int l = 0;
    int k = 0;
    while(1){

      if(s[j] != ' ' && s[j] != '\n'){
        n[k] = s[j];
        k++;
      }
      else{
        t = 1;
      }

      if(t == 1){
        if(strlen(n)>0){
          importcol[i][l] = atoi(n);
          //printf("importcol[%d][%d] = %d \n", i, l, importcol[i][l]);
          l++;
          memset(n, 0, sizeof(n));
        }
        k = 0;
        t = 0;
      }

      if(s[j] == '\n'){
        colsize[i] = l;
        break;
      }
      j++;
    }
  }
  return;

}

void rowstats()
{
	int sum;
	for(int i = 0; i < row; i++){
		sum = 0;
		int l = rowsize[i];
		for(int j = 0; j < l; j++){
			sum += importrow[i][j];
		}
		freespace[i] = col - sum - (rowsize[i] - 1);
	}

	return;
}

inline void rowperm(int rpos, int improwcur, int fsp, int perm, int shift)
{
	if(improwcur == rowsize[rpos]){
		rpst[rpos].push_back(perm);
		return;
	}

	while(fsp >= 0){
		rowperm(rpos, improwcur+1, fsp, perm|(bits(importrow[rpos][improwcur]) << shift), shift+importrow[rpos][improwcur]+1);
		shift++;
		fsp--;
	}
	return;
}

inline int bits(int b)
{
	return ((1L<<b) - 1);
}

inline void colstats()
{
	int sum;
	for(int i = 0; i < col; i++){
		sum = 0;
		int l = colsize[i];
		for(int j = 0; j < l; j++){
			sum += importcol[i][j];
		}
		remain[i] = sum + (colsize[i] - 1);
	}
}

inline int checkcol(int rpos)
{
	int b = 1;
	if(rpos == 0){    //special case:'rpos == 0'
		for(int i = 0; i < col; i++){
			if((currow[0] >> i) & 1){
				checkcurcolnum[0][i] = 1;
				checkcurcolvl[0][i] = 1;
				cursumcol[0][i] = 1;
			}
			else{
				checkcurcolnum[0][i] = 0;
				checkcurcolvl[0][i] = 0;
				cursumcol[0][i] = 0;
			}
		}
	}
	else{
		for(int i = 0; i < col; i++){   //check by col from the rhs to the lhs right;
			if((currow[rpos] >> i) & 1){
				if(checkcurcolvl[rpos-1][i] == 0){
					checkcurcolnum[rpos][i] = checkcurcolnum[rpos-1][i] + 1;
					checkcurcolvl[rpos][i] = 1;
					if(checkcurcolnum[rpos][i] == 1){
						cursumcol[rpos][i] = cursumcol[rpos-1][i] + 1;   //if the first runs happen
					}
					else{
						cursumcol[rpos][i] = cursumcol[rpos-1][i] + 2;  
					}
				}
				else {
					checkcurcolnum[rpos][i] = checkcurcolnum[rpos-1][i];
					checkcurcolvl[rpos][i] = checkcurcolvl[rpos-1][i] + 1;
					cursumcol[rpos][i] = cursumcol[rpos-1][i] + 1;
				}
			}
			else{
				checkcurcolnum[rpos][i] = checkcurcolnum[rpos-1][i];
				checkcurcolvl[rpos][i] = 0;
				cursumcol[rpos][i] = cursumcol[rpos-1][i];
			}

			if((checkcurcolnum[rpos][i] > colsize[i]) || (checkcurcolvl[rpos][i] > importcol[i][checkcurcolnum[rpos][i]-1]) || (cursumcol[rpos][i] > remain[i])){   //special case:curcolnum > colsize or curcolvl > importcolvl => contradiction 
				b = 0;
				break;
			}
		}
	}
	if(b == 1){   //if "b == 1" is true, then it represents that the nono is not contradictory, however, it's "b == 0" when it's contradictory
		return 1;
	}
	else{
		return 0;
	}
}

inline void success()
{
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			if((currow[i] >> j) & 1){
				printf("#");
			}
			else{
				printf(".");
			}
		}
		printf("\n");
	}
	return;
}

inline void dfs(int rpos)
{
	if(see == 0){
		if(rpos == row){  //map is finished ,and then printed
			success();
			see = 1;
			return;
		}
		int s = rpst[rpos].size();
		for(int i = 0; i < s; i++){
			currow[rpos] = rpst[rpos][i];
			if(checkcol(rpos)){     //'checkcol' return '1' as it's ok
				dfs(rpos+1);
			}	
			currow[rpos] = 0;  //backtracking => restore the init
		}
		return;
	}
}