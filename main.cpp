#include <bits/stdc++.h>
using namespace std;

struct Vec {
    long long x, y;
};

long long norm2(const Vec &v) {
    return v.x * v.x + v.y * v.y;
}

int n;
vector<int> child[10005];
vector<Vec> dp[10005];

void dfs(int u) {
    if (child[u].empty()) return; // leaf

    for (int v : child[u])
        dfs(v);

    vector<Vec> result;

    int k = child[u].size();
    vector<int> idx(k, 0);

    // Enumerate all combinations: pick one vector from each child
    function<void()> gen = [&]() {
        vector<Vec> cur(k);
        for (int i = 0; i < k; i++)
            cur[i] = dp[child[u][i]][idx[i]];

        // Try each winner
        for (int win = 0; win < k; win++) {
            long long sx = 0, sy = 0;
            for (int i = 0; i < k; i++) {
                if (i == win) {
                    sx += cur[i].x;
                    sy += cur[i].y;
                } else {
                    sx -= cur[i].x;
                    sy -= cur[i].y;
                }
            }
            result.push_back({sx, sy});
        }
    };

    while (true) {
        gen();
        int i = 0;
        while (i < k) {
            idx[i]++;
            if (idx[i] < (int)dp[child[u][i]].size())
                break;
            idx[i] = 0;
            i++;
        }
        if (i == k) break;
    }

    dp[u] = result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        int k;
        cin >> k;
        if (k == 0) {
            long long x, y;
            cin >> x >> y;
            dp[i].push_back({x, y});
        } else {
            child[i].resize(k);
            for (int j = 0; j < k; j++)
                cin >> child[i][j];
        }
    }

    dfs(1);

    long long ans = 0;
    for (auto &v : dp[1])
        ans = max(ans, norm2(v));

    cout << ans << "\n";
    return 0;
}
