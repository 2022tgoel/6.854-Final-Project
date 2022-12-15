#include <bits/stdc++.h>
using namespace std;

#define SOURCE n - 2
#define SINK n - 1

struct edge { int from, to, cap; edge *reverse; };

struct link_cut_tree {
 
	link_cut_tree *par, *left, *right;
	edge *up, *path;
	int path_value;

	bool flip;
	int tag;
	
	link_cut_tree(edge *e) {
		par = left = right = nullptr;
		path = up = e;
		path_value = e->cap;
		flip = false;
	}
 
};
 
edge * path(link_cut_tree *u) {
  	return u ? u->path : new edge{-1, -1, INT_MAX};
}

void update(link_cut_tree *u, int x) {
	if (u) {
		u->up->cap += x;
		if (u->up->reverse) u->up->reverse->cap -= x;
		u->path_value += x;
		u->tag += x;
	}
}

void pull(link_cut_tree *u) {
	if (u) {
		u->path = u->up;
		u->path_value = u->up->cap;
		for (auto v : {u->left, u->right}) {
			if (v && v->path_value < u->path_value) {
				u->path = v->path;
				u->path_value = v->path_value;
			}
		}
	}
}
 
void push(link_cut_tree *u) {
	if (u) {
		if (u->flip) {
			swap(u->left, u->right);
			if (u->left) u->left->flip ^= true;
			if (u->right) u->right->flip ^= true;
			u->flip = false;
		}

		if (u->tag) {
			update(u->left, u->tag);
			update(u->right, u->tag);
			u->tag = 0;
		}
  }
}
 
bool root(link_cut_tree *u) {
  	return !u->par || (u->par->left != u && u->par->right != u);
}
 
bool left(link_cut_tree *u) {
  	return u->par->left == u;
}
 
void assign(link_cut_tree *u, link_cut_tree *v, bool d) {
	(d ? u->left : u->right) = v;
	if (v) v->par = u;
}
 
void rotate(link_cut_tree *u) {
	link_cut_tree *p = u->par, *g = p->par;
	bool d = left(u);
	assign(p, d ? u->right : u->left, d);
	if (root(p)) {
		u->par = g;
	} else {
		assign(g, u, left(p));
	}
	assign(u, p, !d);
}
 
void splay(link_cut_tree *u) {
	while (!root(u)) {
		link_cut_tree *p = u->par, *g = p->par;
		push(g), push(p), push(u);
		if (!root(p)) {
			if (left(u) == left(p)) {
				rotate(p);
			} else {
				rotate(u);
			}
		}
		rotate(u);
		pull(g), pull(p), pull(u);
	}
	push(u), pull(u);
}
 
void access(link_cut_tree *u) {
	link_cut_tree *c = nullptr;
	for (link_cut_tree *v = u; v; v = v->par) {
		splay(v);
		v->right = c;
		pull(v);
		c = v;
	}
	splay(u);
}
 
void reroot(link_cut_tree *u) {
	access(u);
	u->flip ^= true;
}

link_cut_tree * find_root(link_cut_tree *u) {
	access(u);
	while (u->left) u = u->left;
	access(u);
	return u;
}
 
void link(link_cut_tree *u, link_cut_tree *v) {
	link_cut_tree *w = find_root(v);

	reroot(v), access(u);
	v->par = u;
	pull(u);

	reroot(w);
}
 
void cut(link_cut_tree *u, link_cut_tree *v) {
	link_cut_tree *w = find_root(v);

	reroot(v), access(u);
	u->left = nullptr;
	v->par = nullptr;
	pull(u);

	reroot(w);
}

void assign(link_cut_tree *u, edge *e) {
	access(u);
	u->up = e;
	pull(u);
}

struct dinics_with_lct {

	vector<link_cut_tree*> lct;
	vector<vector<int>> children;
		
	vector<vector<edge*>> adj;
	vector<int> level, ptr;
	int n;

	dinics_with_lct(int n) : n(n) {
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
		node_q.push(SOURCE);
		level[SOURCE] = 0;
		
		while (!node_q.empty()) {
			int u = node_q.front(); node_q.pop();
			for (auto e : adj[u]) {
				if (e->cap && level[e->to] == -1) {
					level[e->to] = level[u] + 1;
					node_q.push(e->to);
		
					if (e->to == SINK) return true;
				}
			}
		}
		
		return false;
	}

	// 	currently at node u = root(source)
	// 		if we're at sink
	// 			let [f, v] = (
	//					min capacity along source to sink path, 
	//					node that had this min capacity
	//			)
	//			subtract f from source to sink path
	//			cut (v, parent(v))
	//		otherwise
	//			let v = next outwards edge from u in level graph
	//			if v does not exist
	//				cut u from all neighbors
	//			otherwise
	//				link (u, v)

	int dfs() {
		int pushed = 0;
		lct.resize(n);

		children.assign(n, vector<int>());

		ptr.assign(n, 0);

		for (int i = 0; i < n; ++i) {
			lct[i] = new link_cut_tree(new edge{i, -1, INT_MAX});
		}

		while (true) {
			int u = find_root(lct[SOURCE])->up->from;
			if (u == SINK) {
				access(lct[SOURCE]);

				edge cut_edge = *path(lct[SOURCE]);
				pushed += lct[SOURCE]->path_value;
				update(lct[SOURCE], -lct[SOURCE]->path_value);

				cut(lct[cut_edge.to], lct[cut_edge.from]);
			} else {
				edge *e = nullptr;
				while (!e && ptr[u] < (int) adj[u].size()) {
					e = adj[u][ptr[u]++];
					if (e->cap == 0 || level[e->to] != level[u] + 1)
						e = nullptr;
				}

				if (!e) {
					if (u == SOURCE) break;
					for (auto c : children[u]) {
						if (find_root(lct[c]) == lct[u]) {
							cut(lct[u], lct[c]);
						}
					}
					children[u].clear();
				} else {
					assign(lct[u], e);
					link(lct[u], lct[e->to]);
					children[e->to].push_back(u);
				}
			}
		}

		for (int i = 0; i < n; ++i) {
			splay(lct[i]);
		}

		return pushed;
	}
		
	int max_flow() {
		int answer = 0;
		while (bfs()) {
			answer += dfs();
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

	dinics_with_lct flow_solver(n);
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