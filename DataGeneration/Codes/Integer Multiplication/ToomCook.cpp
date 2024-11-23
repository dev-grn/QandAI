#include <bits/stdc++.h>
#define ll long long int
using namespace std;
using namespace std::chrono;
#define DMAX 59049

/********************************************************************************************
 *   Multiplication of Big Integers Using the Toom Cook (TOOM3) Multiplication Algorithm    *
 *                                    Created by QandAI                                     *
 *******************************************************************************************/

int MAX = 27;

int D;
int minCap, maxCap, repg;
string F;
ll A[DMAX] = {0,}, B[DMAX] = {0,};
ll a[DMAX] = {0,}, b[DMAX] = {0,}, c[DMAX*2]= {0,};
int NO = 0;

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

void multNormal(ll *a, ll *b, int tLen, ll *c) {
    for(int i=0;i<tLen*2;i++) c[i]=0;
    for(int j=0;j<tLen;j++) {
        for(int i=0;i<tLen;i++) {
            c[j+i] += a[i] * b[j];
        }
    }
    return;
}

void multToom3(ll *a, ll *b, int tLen, ll *c) {
    ll *a0 = &a[0], *a1 = &a[tLen/3], *a2 = &a[tLen*2/3];
    ll *b0 = &b[0], *b1 = &b[tLen / 3], *b2 = &b[tLen * 2/ 3];
    ll *c0 = &c[(tLen / 3) *  0], *c2 = &c[(tLen / 3) *  2], *c4 = &c[(tLen / 3) *  4];
    ll c1[(tLen / 3) * 2], c3[(tLen / 3) * 2];
    ll a_m2[tLen / 3],a_m1[tLen / 3],a_0 [tLen / 3],a_1 [tLen / 3],a_inf[tLen / 3];
    ll b_m2[tLen / 3],b_m1[tLen / 3],b_0[tLen / 3],b_1[tLen / 3],b_inf[tLen / 3];
    ll c_m2[(tLen / 3) * 2],c_m1[(tLen / 3) * 2],c_0[(tLen / 3) * 2],c_1[(tLen / 3) * 2],c_inf[(tLen / 3) * 2];

    if(tLen<=9) {
        multNormal(a,b,tLen,c);
        return;
    }

    for(int i=0;i<tLen/3;i++) {
        a_m2[i] = (a2[i]<<2) - (a1[i]<<1) + a0[i];
        b_m2[i] = (b2[i]<<2) - (b1[i]<<1) + b0[i];
    }

    for(int i=0;i<tLen/3;i++) {
        a_m1[i] = a2[i] - a1[i] + a0[i];
        b_m1[i] = b2[i] - b1[i] + b0[i];
    }

    for(int i=0;i<tLen/3;i++) {
        a_0[i] = a0[i];
        b_0[i] = b0[i];
    }

    for(int i = 0; i < tLen / 3; i++) {
        a_1[i] = a2[i] + a1[i] + a0[i];
        b_1[i] = b2[i] + b1[i] + b0[i];
    }

    for(int i = 0; i < tLen / 3; i++) {
        a_inf[i] = a2[i];
        b_inf[i] = b2[i];
    }

    multToom3(a_m2,  b_m2,  tLen / 3, c_m2 );
    multToom3(a_m1,  b_m1,  tLen / 3, c_m1 );
    multToom3(a_0,   b_0,   tLen / 3, c_0  );
    multToom3(a_1,   b_1,   tLen / 3, c_1  );
    multToom3(a_inf, b_inf, tLen / 3, c_inf);

    for(int i = 0; i < (tLen / 3) * 2; i++)
        c4[i] = c_inf[i];

    for(int i = 0; i < (tLen / 3) * 2; i++) {
        c3[i]  = -c_m2[i];
        c3[i] += (c_m1[i] << 1) + c_m1[i];
        c3[i] -= (c_0[i] << 1) + c_0[i];
        c3[i] += c_1[i];
        c3[i] += (c_inf[i] << 3) + (c_inf[i] << 2);
        c3[i] /= 6;
    }

    for(int i = 0; i < (tLen / 3) * 2; i++) {
        c2[i]  = (c_m1[i] << 1) + c_m1[i];
        c2[i] -= (c_0[i] << 2) + (c_0[i] << 1);
        c2[i] += (c_1[i] << 1) + c_1[i];
        c2[i] -= (c_inf[i] << 2) + (c_inf[i] << 1);
        c2[i] /= 6;
    }

    for(int i = 0; i < (tLen / 3) * 2; i++) {
        c1[i]  = c_m2[i];
        c1[i] -= (c_m1[i] << 2) + (c_m1[i] << 1);
        c1[i] += (c_0[i] << 1) + c_0[i];
        c1[i] += (c_1[i] << 1);
        c1[i] -= (c_inf[i] << 3) + (c_inf[i] << 2);
        c1[i] /= 6;
    }

    for(int i = 0; i < (tLen / 3) * 2; i++)
        c0[i] = c_0[i];

    for(int i = 0; i < (tLen / 3) * 2; i++) c[i + tLen / 3] += c1[i];
    for(int i = 0; i < (tLen / 3) * 2; i++) c[i + (tLen / 3) * 3] += c3[i];
    return;
}

void carryup(ll *a, int tLen) {
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
    if(cr) cout << "NO" << endl;
    return;
}

void display(ll *a, ll *b, ll *c) {
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

void op() {
    int i=0;
    //A B 숫자를 array형식으로 구현
    while((D+i)%3) {
        A[i] = 0; B[i] = 0;
        a[i] = A[i]; b[i] = B[i];
        i ++;
    }
    for(int j=0;j<D;j++) {
        int x = i+j;
        A[x] = distr(gen); a[x] = A[x];
        B[x] = distr(gen); b[x] = B[x];
    }
    for(int j=D+i;j<MAX;j++) {
        a[j] = 0;
        b[j] = 0;
    }
    return;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    distr.param(uniform_int_distribution<>::param_type(1,9));
    cin >> minCap >> maxCap >> repg;
    F = to_string(minCap) + "_" + to_string(maxCap) + "_" + to_string(repg) + "toom3recursive.csv";
    ofstream file(F, ios::app);
    if(file.tellp()==0) file << "n,time\n";
    while(MAX<minCap) MAX *= 3;
    for(int i=minCap;i<=maxCap;i++) {
        D = i;
        for(int j=0;j<repg;j++) {
            op();
            auto start = high_resolution_clock::now();
            multToom3(a, b, MAX, c);
            carryup(c, MAX * 2);
            //display(a,b,c);
            auto end = high_resolution_clock::now();
            chrono::duration<double, std::micro> duration = end-start;
	        file << D << "," << duration.count()/1000.0 << '\n';
	        cout << D << " " << duration.count()/1000.0 << endl;
	}
	if(MAX<i+1) MAX *= 3;
    }
    return 0;
}
