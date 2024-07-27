#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
using namespace std::chrono;

int maxCap;
vector<ull> D;

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

void op(ull loc, int dig) {
    if(D.size()<=loc)
        D.push_back(dig);
    else
        D[loc] += dig;
    //cleaning
    ull tmp = 0;
    for(ull i=0;i<D.size();i++) {
        D[i] += tmp;
        tmp = D[i]/10;
        D[i] %= 10;
    }
    while(tmp) {
        D.push_back(tmp%10);
        tmp /= 10;
    }
    return;
}

void multiply(string A, string B) {
    ull idx = 0, ans;
    int loc;
    const int sz = A.size();
    auto start = high_resolution_clock::now();
    for(int i=sz-1;i>=0;i--) {
        ull tidx = 0;
        for(int j=sz-1;j>=0;j--) {
            ans = (B[j]-'0')*(A[i]-'0');
            loc = idx+tidx;
            op(loc, ans);
            tidx ++;
        }
        idx ++;
    }
    auto end = high_resolution_clock::now();
    auto totalDuration = duration_cast<microseconds>(end-start);
    cout << totalDuration.count() << "\n";
    return;
}

string make(ull sz) {
    string tp = "";
    for(int i=0;i<sz;i++) {
        tp += distr(gen)+'0';
    }
    return tp;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cin >> maxCap;
    distr.param(uniform_int_distribution<>::param_type(1,9));
    string x = make(maxCap), y=make(maxCap);
    cout << x << ' ' << y << "\n";
    multiply(x,y);
    for(int i=D.size()-1;i>=0;i--) 
        cout << D[i];
    return 0;
}
