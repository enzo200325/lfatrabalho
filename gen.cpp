#include <bits/stdc++.h>
using namespace std; 
#define endl '\n'

//debug
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif
#ifndef ONLINE_JUDGE
#define debarr(arr, n) cerr << "arr: ["; forn(i, n) cerr << arr[i] << (i == n-1 ? "" : ", "); cerr << "]" << endl;
#else
#define debarr(x...)
#endif

// -------------------- = -----------------------

typedef long long ll; 
template<typename T> using pqmin = priority_queue<T, vector<T>, greater<T>>;
template<typename T> using pqmax = priority_queue<T>; 
typedef pair<int, int> pii; 
using pll = pair<ll, ll>;
using ld = long double;
template <typename T> using vc = vector<T>;
template <typename T> using vvc = vector<vc<T>>;
template <typename T> using vvvc = vector<vvc<T>>;
using vi = vc<int>;
using vl = vc<ll>;
using vpi = vc<pii>;

#define pb push_back
#define eb emplace_back
#define mp make_pair
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define rall(x) x.rbegin(), x.rend()
#define sor(x) sort(all(x))
#define rev(x) reverse(all(x))

#define forn(i, n) for (int i = 0; i < n; i++)
#define fora(i, a, b) for (int i = a; i <= b; i++) 
#define rofn(i, n) for (int i = n-1; i >= 0; i--) 
#define rofa(i, a, b) for (int i = a; i >= b; i--)
#define rep(n) forn(__rep_i_problem_dont_use_this_varible__, n)

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

int main() {
    ios_base::sync_with_stdio(0); 
    cin.tie(0); 

    int S = 0; 
    int F = 10; 

    vector<string> b = {"Chassi", "Motor", "Cambio", "Direcao", "Suspensao", "Freios", "Carroceria", "Pintura", "Sistema Eletrico", "Interior"}; 
    vector<int> a = {8, 4, 2, 2, 1, 2, 4, 5, 1, 3}; 
    vector<tuple<int, int, char, char, char, char>> T; 
    int N = a.size() + 2; 

    for (int j = 0; j < a[0]; j++) {
        T.emplace_back(0, 1, (char)('0'+j), '$', (char)('0'+j), '$'); 
    } 
    for (int i = 1; i <= a.size(); i++) {
        for (int j = 0; j < a[i]; j++) {
            T.emplace_back(i, i+1, (char)('0'+j), '$', '$', '$'); 
        } 
    } 
    for (int i = 0; i < a.size(); i++) {
        T.emplace_back(i, i, '^', '$', (char)('A'+i), '$'); 
    } 
    for (int i = 0; i < 8; i++) {
        T.emplace_back(F, F, '$', (char)('0'+i), '$', (char)('0'+i)); 
    } 
    for (int i = 0; i < a.size(); i++) {
        T.emplace_back(F, F, '$', (char)('A'+i), '$',  (char)('A'+i)); 
    } 

    T.emplace_back(F, S, '$', '?', '?', '$'); 
    T.emplace_back(S, F+1, '$', '?', '?', '&'); 

    int M = T.size(); 
    cout << N << " " << M << " " << 0 << endl; 
    for (auto [b, c, d, e, f, g] : T) {
        cout << b << " " << c << " " << d << " " << e << " " << f << " " << g << endl;
    } 


    assert(a.size() == b.size()); 

    for (int i = 0; i < N-1; i++) cout << 0 << " "; 
    cout << 1 << endl;

    cout << endl;
}






