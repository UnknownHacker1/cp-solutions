/*
ID: BidoTeima
LANG: C++11
TASK:
*/
#include <bits/stdc++.h>
#include "dreaming.h"
using namespace std;
using ll = long long; 

const int N = 1e5;
vector<pair<int,int>> adj[N];  
int cur_farthest_node, cur_farthest_node_dist;
bool vis[N];
vector<int>cc;
void set_farthest_node(int node, int prev, int cur_dist){
    vis[node] = 1;
    cc.push_back(node);
    if(adj[node].empty() || (adj[node].size() == 1 && prev != -1)){
        if(cur_dist > cur_farthest_node_dist){
            cur_farthest_node_dist = cur_dist;
            cur_farthest_node = node;
        }
        return;
    }
    for(auto& edge : adj[node]){
        if(edge.first != prev){
            set_farthest_node(edge.first, node, cur_dist + edge.second);
        }
    }
}
int dist[N][2];
void precompute_dist_dfs(int node, int prev, int cur_dist, bool b){
    dist[node][b] = cur_dist;
    for(auto& edge : adj[node]){
        if(edge.first != prev){
            precompute_dist_dfs(edge.first, node, cur_dist + edge.second, b);
        }
    }
}
void precompute_dist(int node, bool b){
    for(int i : cc)
        dist[i][b] = 1e9;
    precompute_dist_dfs(node, -1, 0, b);
}
int travelTime(int n, int m, int l, int a[], int b[], int t[])
{      
    for(int i = 0; i < m; i++){ 
        adj[a[i]].push_back({b[i], t[i]});
        adj[b[i]].push_back({a[i], t[i]});
    }
    multiset<pair<int, int>>ms;
    for(int i = 0; i < n; i++){
        if(vis[i])
            continue;
        cc.clear();
        cur_farthest_node_dist = -1;
        set_farthest_node(i, -1, 0);
        int e1 = cur_farthest_node;
        cur_farthest_node_dist = -1;
        set_farthest_node(e1, -1, 0);
        int e2 = cur_farthest_node;
        precompute_dist(e1, 0);
        precompute_dist(e2, 1);
        int root = e1, min_max_dist = dist[e2][0];
        assert(dist[e2][0] == dist[e1][1]); 
        for(int node : cc){
            if(max(dist[node][0], dist[node][1]) < min_max_dist){
                min_max_dist = max(dist[node][0], dist[node][1]);
                root = node;
            }
        }
        ms.insert({min_max_dist, dist[e2][0]});
    }
    while(ms.size() != 1){
        auto it1 = ms.begin(), it2 = prev(ms.end());
        int a = it1->first, b = it2->first, d1 = it1->second, d2 = it2->second;
        ms.erase(ms.begin());
        ms.erase(prev(ms.end()));
        ms.insert({max(b, a + l), max({d1, d2, a + b + l})});
    }
    return ms.begin()->second;
}
