// MPM max flow algorithm
// https://www.cs.cornell.edu/courses/cs4820/2013sp/Handouts/DinicMPM.pdf
#include <bits/stdc++.h>
#define ll long long
using namespace std;
ll MAX = 1e9+10;
struct Edge { 
    int u, v; 
    ll cap; 
    Edge *rev; 
    Edge(int a, int b, ll c){
        u = a; v = b, cap = c;
    }
    
};

struct MPM {
    vector<vector<Edge*>> adj;
    vector<int> levels;
    vector<list<Edge*>> in, out;
    vector<ll> pin, pout; 
    vector<bool> alive;
    vector<ll> dem;  
    int s, t; 
    MPM(int n, int m){
        s = 1;
        t = n;
        levels.resize(n+1);
        adj.resize(n+1);
        in.resize(n+1);
        out.resize(n+1);
        pin.resize(n+1);
        pout.resize(n+1);
        alive.resize(n+1);
        dem.resize(n+1);
    }
    void add_edge(int a, int b, ll c){
        Edge* e = new Edge(a, b, c);
        Edge* f = new Edge(b, a, 0);
        e->rev = f; f->rev = e;
        adj[a].push_back(e);
        adj[b].push_back(f);
    }

    bool BFS(int start=1){
        // create level graph 
        assert(start==s);
        fill(levels.begin(), levels.end(), -1); 
        levels[start] = 0;
        queue<int> q;
        q.push(start);
        while (!q.empty()){
            int cur = q.front();
            q.pop();
            for (Edge* e : adj[cur]){
                if (e->cap > 0 && levels[e->v] == -1){
                    levels[e->v] = levels[cur] + 1;
                    q.push(e->v);
                }
            }
        }
        return levels[t]!=-1;
    }

    void add_admissible_edge(Edge* e){
        out[e->u].push_back(e);
        in[e->v].push_back(e);
        pout[e->u]+=e->cap;
        pin[e->v]+=e->cap;
    }

    void level_graph(){
        // constructs the level graph with in and out 
        fill(pout.begin(), pout.end(), 0);
        fill(pin.begin(), pin.end(), 0);
        pin[s] = pout[t] = MAX;
        for (int i = 1; i <= t; i++){
            out[i].clear(); in[i].clear();
        }
        for (int i = 1; i <= t; i++){
            for (Edge* e : adj[i]){
                assert(e->u == i);
                if (levels[e->u] + 1 == levels[e->v] && e->cap > 0) add_admissible_edge(e);
            }
        }
    }

    void remove_node(int i){
        // removes all edges corresponding to node from admissible graph 
        alive[i] = false; 
        for (Edge* e : in[i]){
            out[e->u].remove(e);
            pout[e->u]-=e->cap;
        }
        for (Edge* e : out[i]){
            in[e->v].remove(e);
            pin[e->v]-=e->cap;
        }
    }

    ll pot(int i){ //potential 
        return min(pin[i], pout[i]);
    }

    int min_pot(){
        int n = 0;
        for (int i = 1; i <= t; i++){
            if (!alive[i]) continue;
            if (n == 0 || pot(i) < pot(n)){
                n = i;
            }
        }
        return n;
    }

    void push_flow(Edge* e, ll val){
        e->cap-=val;
        e->rev->cap+=val;
        pout[e->u]-=val;
        pin[e->v]-=val;
        if (e->cap == 0){
            out[e->u].remove(e);
            out[e->v].remove(e);
        }
    }

    void push(int v, ll f, bool dir){
        // push f units of flow from v 
        // to t if dir is true, to s if dir is false 
        int end = dir ? t : s;
        fill(dem.begin(), dem.end(), 0);
        queue<int> q;
        int cur = v;
        dem[cur] = f;
        while(cur!=end){
            for (Edge* e : (dir ? out[cur] : in[cur])){
                ll val = min(e->cap, dem[cur]);
                push_flow(e, val);
                int x = (dir ? e->v : e->u);
                if (dem[x] == 0) q.push(x);
                dem[x]+=val;
                dem[cur]-=val; 
                if (dem[cur] == 0) break;
            }
            cur = q.front(); q.pop();
        }
    }

    ll solve(){
        // returns max flow value 
        ll f = 0;
        while(BFS()){
            level_graph();
            fill(alive.begin(), alive.end(), true); // all nodes are live 
            while(int node=min_pot()){
                ll p = pot(node);
                if (p == 0) remove_node(node);
                else {
                    push(node, p, false);
                    push(node, p, true);
                }   
                f += p;
            }
        }
        return f;
    }

    void print_edge(Edge* e){
        cout << e->u << ' ' << e->v << ' ' << e->cap << ' ' << e->rev->cap << endl;
    }
    void print_level(){
        cout << "-------printing--------" << endl;
        for (int i = 1; i <= t; i++){
            cout << "node: " << i << " potential: " << pot(i) << endl;
            cout << "edges: " << endl;
            for (Edge* e : out[i]){
                print_edge(e);
            }
        }
        cout << "----------------------" << endl;
    }
    void print_all(){
        cout << "-------printing--------" << endl;
        for (int i = 1; i <= t; i++){
            for (Edge* e : adj[i]){
                print_edge(e);
            }
        }
        cout << "----------------------" << endl;
    }
};


int main (void) {
    // uncomment to read input from file 
    // ifstream cin("graph.txt"); ofstream cout("out.txt");
    int n, m;
    cin >> n >> m;
    MPM d(n, m);
    for (int i = 0; i < m; i++){
        int a, b; ll c;
        cin >> a >> b >> c;
        d.add_edge(a, b, c);
    }
    cout << d.solve() << endl;
    // cin.close();
    // cout.close();


}