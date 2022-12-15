#include <bits/stdc++.h>
using namespace std;

struct dinics {
	struct edge { int from, to, cap; edge *reverse; };
		
	vector<vector<edge*>> adj;
	vector<int> level, ptr;
	int n;

	dinics(int n) : n(n) {
		adj.resize(n);
	}
		
	void add_edge(int u, int v, int w) {
		edge *forwards = new edge{u, v, w};
		edge *backwards = new edge{v, u, 0};
		forwards->reverse = backwards;
		backwards->reverse = forwards;
		adj[u].push_back(forwards);
		adj[v].push_back(backwards);
	}

		
	bool bfs() {
		level.assign(n, -1);
		
		queue<int> node_q;
		node_q.push(n - 2);
		level[n - 2] = 0;
		
		while (!node_q.empty()) {
			int u = node_q.front(); node_q.pop();
			for (auto e : adj[u]) {
				if (e->cap && level[e->to] == -1) {
					level[e->to] = level[u] + 1;
					node_q.push(e->to);
		
					if (e->to == n - 1) return true;
				}
			}
		}
		
		return false;
	}
		
	int dfs(int u, int flow) {
		if (u == n - 1) return flow;
		
		int sent = 0;
		while (flow && ptr[u] < (int) adj[u].size()) {
			auto e = adj[u][ptr[u]++];
			if (e->cap && level[e->to] == level[u] + 1) {
				int pushed = dfs(e->to, min(flow, e->cap));
				sent += pushed, flow -= pushed;
				e->cap -= pushed, e->reverse->cap += pushed;
			}
		}
		
		return sent;
	}
		
	int max_flow() {
		int answer = 0;
		while (bfs()) {
			ptr.assign(n, 0);
			answer += dfs(n - 2, INT_MAX);
		}
		return answer;
	}

	vector<bool> min_cut() {
		vector<bool> in_s(n);
		for (int i = 0; i < n; ++i) {
			in_s[i] = level[i] >= 0;
		}
		return in_s;
	}
};

int main() {
	int n, m; cin >> n >> m;

	dinics flow_solver(n);
	for (int i = 0; i < m; ++i) {
		int u, v, w; cin >> u >> v >> w;
		flow_solver.add_edge(u, v, w);
	}

	auto start_time = clock();

	cout << flow_solver.max_flow() << "\n";

	vector<bool> cut = flow_solver.min_cut();
	for (int i = 0; i < n; ++i) {
		cout << cut[i];
	}
	cout << "\n";

	cout << "Time elapsed: " << 1.0 * (clock() - start_time) / CLOCKS_PER_SEC << "\n";
}