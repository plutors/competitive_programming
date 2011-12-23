// Andrés Mejía
// Time limit. Shit!
using namespace std;
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <list>
#include <map>
#include <set>

////////////// Prewritten code follows. Look down for solution. ////////////////
#define foreach(x, v) for (typeof (v).begin() x=(v).begin(); x !=(v).end(); ++x)
#define For(i, a, b) for (int i=(a); i<(b); ++i)
#define D(x) cout << #x " is " << (x) << endl

const double EPS = 1e-9;
int cmp(double x, double y = 0, double tol = EPS) {
    return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
////////////////////////// Solution starts below. //////////////////////////////

const int MAXN = 2000005;
char buffer[2 * MAXN];
int border[2 * MAXN];
unsigned int hash[MAXN];

vector< string > words;

map< char, int > next[MAXN];
int score[MAXN], totalLength;

int repeats[MAXN];

void print(int cur) {
    for (char c = 'a'; c <= 'z'; ++c) {
        if (next[cur].count(c) == 0) continue;
        printf("next[%d][%c] => %d\n", cur, c, next[cur][c]);
        print(next[cur][c]);
    }
}

long long calculate(int W) {
    // Insert in trie
    for (int i = 0; i <= totalLength + 1; ++i) {
        next[i].clear();
        score[i] = 0;
    }
    //puts("Cleared trie");
    int nextNode = 1;
    
    for (int i = 0; i < W; ++i) {
        int current = 0;
        for (int j = words[i].size() - 1; j >= 0; --j) {
            char c = words[i][j];
            map< char, int >::iterator where = next[current].find(c);
            if (where != next[current].end()) {
                current = where->second;
            } else {
                next[current][c] = nextNode++;
                current = next[current][c];
            }
        }
        //printf("inserted %s reversed\n", words[i].c_str());
        score[current]++;
    }
    
    // print(0);
    // for (int i = 0; i <= totalLength + 1; ++i) {
    //     if (score[i] > 0) {
    //         printf("score[%d] = %d\n", i, score[i]);
    //     }
    // }

    long long ans = 0;

    for (int k = 0; k < W; ++k) {
        const string &s = words[k];
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            buffer[i] = s[n - 1 - i];
            buffer[n + 1 + i] = s[i];
        }
        buffer[n] = '$';
        buffer[2 * n + 1] = '\0';

        // Buffer == Reverse(s) + $ + s
        border[0] = 0;
        for (int i = 1; i < 2 * n + 1; ++i) {
            int size = border[i - 1];
            while (size > 0 and buffer[i] != buffer[size]) size = border[size - 1];
            if (buffer[i] == buffer[size]) size++;
            border[i] = size;
        }
        
        // Search for s[0..n-1] in the trie.
        int current = 0;
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            map<char, int>::iterator where = next[current].find(c);
            if (where != next[current].end()) {
                current = where->second;
                repeats[i] = score[current];
            } else { // No node, fuck!
                while (i < n) repeats[i++] = 0; // Walk to the end of the string, as it doesn't appear anymore.
            }
        }
      
        int len = border[2 * n];
        if (len > 0 and n - 1 - len == -1) { // The whole word is a palindrome
            len = border[len - 1];
        }
        while (len > 0) {
            assert(n - 1 - len >= 0);
            ans += repeats[n - 1 - len];
            len = border[len - 1];
        }
    }
    return ans;
}

int main(){
    int W;
    scanf("%d", &W);
    words.resize(W);
    totalLength = 0;
    for (int i = 0; i < W; ++i) {
        int len; scanf("%d %s ", &len, buffer);
        totalLength += len;
        words[i] = string(buffer);
    }
    
    long long ans = calculate(W); 
    for (int k = 0; k < W; ++k) { // Same length
        
        int current = 0;
        int n = words[k].size();
        for (int i = 0; i < n; ++i) {
            char c = words[k][i];
            map<char, int>::iterator where = next[current].find(c);
            if (where != next[current].end()) {
                current = where->second;
            } else { // No node, fuck!
                current = -1;
                break;
            }
        }
        
        if (current != -1) ans += score[current];
        
        reverse(words[k].begin(), words[k].end());
    }
    ans += calculate(W);
    
    printf("%lld\n", ans);

    return 0;
}