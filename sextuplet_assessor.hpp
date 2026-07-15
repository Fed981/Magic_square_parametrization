#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct res {
 long long r;
 long long u;
 long long v;
};

res gcd(res x, res y) {
 //cout << "entered u = " << x.u << endl;
 const long long hi = x.r;
 const long long lo = y.r;
 if (lo == 0) {
  return x;
 }
 const long long q = hi / lo;
 res next;
 next.r = hi - q*lo;
 next.u = x.u - q*y.u;
 next.v = x.v - q*y.v;
 return gcd(y, next);
}

res mygcd(const long long a, const long long b) {
 const long long pa = abs(a);
 const long long pb = abs(b);
 const long long hi = pa>pb?pa:pb;
 const long long lo = pa<pb?pa:pb;
 res x, y;
 x.r = hi;
 x.u = 1;
 x.v = 0;
 y.r = lo;
 y.u = 0;
 y.v = 1;
 res resout = gcd(x, y);
 cout << resout.u << " * " << hi << " + " << resout.v << " * " << lo << " = " << resout.r << endl;
 return resout;
}

long long maxcoeff(const long long a, const long long b) {
 res z = mygcd(a, b);
 const long long pu = abs(z.u);
 const long long pv = abs(z.v);
 const long long w = pu>pv?pu:pv;
 return w;
}

long long assessvectorinner(vector <long long> s) {
 if (s.size() != 4) {
  return 0;
 }
 vector <long long> d;
 for (long i = 0; i < 3; i++) {
  for (long j = i+1; j < 4; j++) {
   d.push_back(maxcoeff(s[i], s[j]));
  }
 }
 long long m = 0;
 for (long k = 0; k < d.size(); k++) {
  if (d[k] > m) {
   m = d[k];
  }
 }
 //cout << s[0] << ", " << s[1] << ", " << s[2];
 //cout << ", " << s[3] << ": " << m << endl;
	return m;
}

long long assessvector(vector <long long> t) {
 if (t.size() != 6) {
  return 0;
 }
 long long mm = 0;
 for (long i = 0; i < 2; i++) {
  for (long j = i+1; j < 3; j++) {
   vector <long long> s;
   s.push_back(t[2*i]);
   s.push_back(t[2*i+1]);
   s.push_back(t[2*j]);
   s.push_back(t[2*j+1]);
   const long long m = assessvectorinner(s);
   if (m > mm) {
	   mm = m;
   }
  }
 }
 return mm;
}

long long assess_sextuplet(const long long n1, const long long n2, const long long n3, const long long n4, const long long n5, const long long n6) {
 vector <long long> t;
 t.push_back(n1);
 t.push_back(n2);
 t.push_back(n3);
 t.push_back(n4);
 t.push_back(n5);
 t.push_back(n6);
 return assessvector(t);
}

