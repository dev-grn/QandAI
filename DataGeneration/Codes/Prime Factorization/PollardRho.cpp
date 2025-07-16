#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
using namespace std::chrono;

mt19937 gen(random_device{}());
uniform_int_distribution<> distr;

int mn, mx, rep;
string F;

__int128_t atoint128_t(std::string const & in) {
	__int128_t res = 0;
	size_t i = 0;
	bool sign = false;
	if (in[i] == '-') {
		i ++;
		sign = true;
	}
	if (in[i] == '+') i++;
	for (; i < in.size(); ++i) {
		const char c = in[i];
		res *= 10;
		res += c - '0';
	}
	if (sign)
		res *= -1;
	return res;
}

__int128 Power(__int128 x, __int128 y, __int128 mod) {
	x %= mod;
	__int128 ret = 1;
	while(y > 0) {
		if(y%2 == 1) ret = (ret*x)%mod;
		x = (x*x)%mod;
		y /= 2;
	}
	return (ull)ret;
}

bool checkPrime(ull n, ull a) {
	if(a%n == 0) return true;
	ull k = n-1;
	while(1) {
		ull temp = Power(a, k, n);
		if(temp == n-1) return true;
		if(k%2 == 1) return (temp == 1 || temp == n-1);
		k /= 2;
	}
}

bool isPrime(ull n) {
	if(n == 2 || n == 3) return true;
	if(n%2 == 0) return false;

	ull a[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	for(int i=0; i<12; i++)
		if(!checkPrime(n, a[i])) {
			return false;
			break;
		}
	return true;
}

__int128 GCD(__int128 a, __int128 b) {
	if(a < b) swap(a, b);
	while(b != 0) {
		ull r = a%b;
		a = b;
		b = r;
	}
	return a;
}

__int128 findDiv(__int128 n) {
	if(n%2 == 0) return 2;
	if(isPrime(n)) return n;

	__int128 x = rand()%(n-2) + 2, y = x, c = rand()%10 + 1, g = 1;
	while(g == 1) {
		x = (x*x%n + c)%n;
		y = (y*y%n + c)%n;
		y = (y*y%n + c)%n;
		g = GCD(abs(x-y), n);
		if(g == n) return findDiv(n);
	}
	if(isPrime(g)) return g;
	else return findDiv(g);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	distr.param(uniform_int_distribution<>::param_type(1,9));
	//length n of digit (maximum 39)
	cin >> mn >> mx >> rep;
	F = to_string(mn) + "_" + to_string(mx) + "_" + to_string(rep) + "pollardNorm.csv";
	ofstream file(F, ios::app);
	if(file.tellp()==0) file << "n,time\n";
	__int128 N, ln;
	vector<__int128> ans;
	string prod = "";
	for(int i=mn; i<=mx; i++) {
		for(int j=0; j<rep; j++) {
			prod = "";
			ans.clear();
			ln = 1;
			for(int k=0; k<i; k++)
				prod += distr(gen)+'0';
			N = atoint128_t(prod);
			auto start = high_resolution_clock::now();
			while(N > 1) {
				__int128 div = findDiv(N);
				ans.push_back(div);
				N /= div;
			}
			auto end = high_resolution_clock::now();
			auto dur = duration_cast<seconds>(end - start);
			chrono::duration<double,micro> duration = end-start;
			file << i << ","<< duration.count()/1000.0 << '\n';
			if(!(j%100)) cout << "HIT " << j/100 << endl;
		}
		cout << "OPERATION " << i << " COMPLETE" << endl;
	}
	return 0;
}
