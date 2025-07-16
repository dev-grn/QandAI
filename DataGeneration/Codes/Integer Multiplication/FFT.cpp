#include <bits/stdc++.h>
#include <gmpxx.h>
#include <gmp.h>
using namespace std;
using namespace std::chrono;

/********************************************************************************************
 * Multiplication of Big Integers Using the GNU Multiple Precision Arithmetic Library (GMP) *
 *                                    Created by QandAI                                     *
 *******************************************************************************************/

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

int minCap, maxCap, rep;

string make(int sz) {
    string tp = "";
    for(int i=0; i<sz; i++) tp += distr(gen)+'0';
    return tp;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    distr.param(uniform_int_distribution<>::param_type(1,9));

    cin >> minCap >> maxCap >> rep;

    for(int i = minCap; i<= maxCap; i++) {
        for(int j=0; j<rep; j++) {
            mpz_t A, B, C;
            string x = make(i), y = make(i);
            const char *a = x.c_str(), *b = y.c_str();
	    mpz_init(A); mpz_init(B); mpz_init(C);
            mpz_set_str(A, a, 10);
            mpz_set_str(B, b, 10);
            auto start = high_resolution_clock::now();
            mpz_mul(C, A, B);
            auto end = high_resolution_clock::now();
	    string minstr = to_string(minCap),maxstr=to_string(maxCap), repstr = to_string(rep);
	    string f = minstr + "_" + maxstr + "_" + repstr + "gmp.csv";
	    ofstream file(f, ios::app);
	    if(file.tellp()==0) file << "n,time\n";
            chrono::duration<double, std::micro> duration = end - start;
            file << i << "," << duration.count() / 1000.0 << "\n";
	    cout << i << " " << duration.count()/1000.0 << endl;
        }
    }
    return 0;
}

