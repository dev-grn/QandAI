#include <bits/stdc++.h>
#define DMAX 32768
#define ull unsigned long long
using namespace std;
using namespace std::chrono;

/********************************************************************************************
 *   Multiplication of Big Integers Using the Karatsuba (TOOM2) Multiplication Algorithm    *
 *                                    Created by QandAI                                     *
 *******************************************************************************************/

int maxCap, minCap, repg;
int A[DMAX] = {0,}, B[DMAX] = {0,};
int a[DMAX] = {0,}, b[DMAX] = {0,}, c[DMAX*2] = {0,};
string F;
int MAX = 16;
int D;

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

void carryup(int *a, int tLen) {
	int cr = 0;
	for(int i=0; i<tLen; i++) {
		a[i] += cr;
		if(a[i] < 0) {
			cr = -(-(a[i] + 1) / 10 + 1);
		} else {
			cr = a[i] / 10;
		}
		a[i] -= cr * 10;
	}
	if(cr) cout << "NO" << endl;
	return;
}

void add(int *a, int *b, int tLen, int *c) {
	for(int j=0; j<tLen; j++) {
		c[j] = a[j] + b[j];
	}
	return;
}

void multNormal(int *a, int *b, int tLen, int *c) {
	for(int i = 0; i < tLen * 2; i++) c[i] = 0;
	for (int j = 0; j < tLen; j++) {
		for (int i = 0; i < tLen; i++) {
			c[j + i] += a[i] * b[j];
		}
	}
	return;
}

void subtractFrom(int *t, int *a, int tLen) {
	for(int i=0; i<tLen; i++) {
		t[i] -= a[i];
	}
	return;
}

void multKara(int *a, int *b, int tLen, int *c) {
	if(tLen<=9) {
		multNormal(a,b,tLen,c);
		return;
	}

	int *a0 = &a[0], *a1 = &a[tLen/2];
	int *b0 = &b[0], *b1 = &b[tLen/2];
	int *c0 = &c[0], *c1 = &c[tLen/2], *c2 = &c[tLen];
	int z0[tLen]= {0,}, z2[tLen]= {0,}, z1[tLen]= {0,};

	multKara(a0,b0,tLen/2,z0);
	multKara(a1,b1,tLen/2,z2);

	int asum[tLen/2] = {0,}, bsum[tLen/2] = {0,};
	for(int i=0;i<tLen/2;i++) {
	    asum[i] = a1[i] + a0[i];
	    bsum[i] = b1[i] + b0[i];
	}

	multKara(asum,bsum,tLen/2,z1);

	subtractFrom(z1,z0,tLen);
	subtractFrom(z1,z2,tLen);

	for(int i=0; i<tLen*2; i++) c[i] = 0;

	for(int i=0; i<tLen; i++) c0[i] += z0[i];
	for(int i=0; i<tLen; i++) c1[i] += z1[i];
	for(int i=0; i<tLen; i++) c2[i] += z2[i];
	return;
}

void display(int *a, int *b, int *c) {
    //Use this function if desiring to check answer (displays numbers A,B and their product C)
	int aLen = MAX, bLen = MAX, cLen = MAX*2;
	while(c[cLen-1]==0) if(c[cLen-1]==0) cLen --;
	for(int i=cLen-1; i>=0; i--) {
		cout << c[i];
	}
	cout << endl;
	return;
}

void op() {
	int i=D%2;
	if(i) {
		A[0] = 0;
		B[0] = 0;
		a[0] = A[0];
		b[i] = B[0];
	}
	for(int j=0; j<D; j++) {
		int x = i+j;
		A[x] = distr(gen);
		a[x] = A[x];
		B[x] = distr(gen);
		b[x] = B[x];
	}
	for(int j=D+i; j<MAX+i; j++) {
		a[j] = 0;
		b[j] = 0;
	}
	return;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	distr.param(uniform_int_distribution<>::param_type(1, 9));
	cin >> minCap >> maxCap >> repg;
	F = to_string(minCap) + "_" + to_string(maxCap) + "_" + to_string(repg) + "kararecur.csv";
	ofstream file(F,ios::app);
	if(file.tellp()==0) file << "n,time\n";
    while(MAX<minCap) {
        MAX = MAX << 1;
    }
	for(int i=minCap;i<=maxCap;i++) {
	    D = i;
	    for(int j=0;j<repg;j++) {
	        op();
	        auto start = high_resolution_clock::now();
	        multKara(a,b,MAX,c);
	        carryup(c,MAX*2);
	        auto end = high_resolution_clock::now();
	        chrono::duration<double,std::micro> duration = end-start;
	        file << i << "," << duration.count()/1000.0 << '\n';
	        cout << i << " " << duration.count()/1000.0 << endl;
	    }
	    if(i==MAX) MAX = MAX << 1;
	}
	return 0;
}


