#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DMAX 19683

/********************************************************************************************
 *       Multiplication of Big Integers Using Gradeschool Multiplication Algorithm          *
 *                                    Created by QandAI                                     *
 *******************************************************************************************/

int MAX = 20;

int D;
int minCap, maxCap, repg;
string F;
int A[DMAX] = {0,}, B[DMAX] = {0,};
int a[DMAX] = {0,}, b[DMAX] = {0,}, c[DMAX*2]= {0,};
int NO = 0;

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

void multNormal(int *a, int *b, int tLen, int *c) {
    for(int i=0;i<tLen*2;i++) c[i]=0;
    for(int j=0;j<tLen;j++) {
        for(int i=0;i<tLen;i++) {
            c[j+i] += a[i] * b[j];
        }
    }
    return;
}

void display(int *a, int *b, int *c) {
    int aLen = MAX, bLen = MAX, cLen = MAX*2;
    while(a[aLen-1]==0) if(a[aLen-1]==0) aLen--;
    while(b[bLen-1]==0) if(b[bLen-1]==0) bLen --;
    while(c[cLen-1]==0) if(c[cLen-1]==0) cLen --;

    for(int i=aLen-1;i>=0;i--) {
        cout << a[i];
    } cout << endl;
    for(int i=bLen-1;i>=0;i--) {
        cout << b[i];
    } cout << endl;
    for(int i=cLen-1;i>=0;i--) {
        cout << c[i];
    } cout << endl;
    cout << endl;
    return;
}

void carryup(int *a, int tLen) {
    int cr = 0;
    for(int i=0;i<tLen;i++) {
        a[i] += cr;
        if(a[i] < 0) {
            cr = -(-(a[i] + 1) / 10 + 1);
        } else {
            cr = a[i] / 10;
        }
        a[i] -= cr * 10;
    }
    if(cr) cout << "ERROR!" << endl;
    return;
}

void op() {
    for(int j=0;j<D;j++) {
        A[j] = distr(gen); a[j] = A[j];
        B[j] = distr(gen); b[j] = B[j];
    }
    multNormal(a, b, MAX, c);
    carryup(c, MAX * 2);
    return;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    distr.param(uniform_int_distribution<>::param_type(1,9));
    cin >> minCap >> maxCap >> repg;
    F = to_string(minCap) + "_" + to_string(maxCap) + "_" + to_string(repg) + "toom3.csv";
    ofstream file(F, ios::app);
    if(file.tellp()==0) file << "n,time\n";
    for(int i=minCap;i<=maxCap;i++) {
        D = i; MAX = i;
        for(int j=0;j<repg;j++) {
            auto start = high_resolution_clock::now();
            op();
            auto end = high_resolution_clock::now();
            chrono::duration<double, std::micro> duration = end-start;
	        file << D << "," << duration.count()/1000.0 << '\n';
	        cout << D << " " << duration.count()/1000.0 << endl;
	    }
    }
    return 0;
}




