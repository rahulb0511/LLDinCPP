#include <bits/stdc++.h>
#include <iostream>
using namespace std;

//Disjoint set:-  a data structure used to maintain all the set which are non overlapping
//MST:- to convert the graph into a graph where we should have n nodes and n-1 edges and all nodes are connected to each 
    // other and sum of their edge weight should be minimum


//TC = O(4 alpha) = approx O(1)
class DisjointSet {
public:
    vector<int> size;
    vector<int> par;

    DisjointSet(int n) {
        size.resize(n+1, 1);
        par.resize(n+1);
        for(int i=0; i<=n; i++) {
            par[i] = i;
        }
    }

    void unionBySize(int u, int v) {
        int up1 = findUpar(u);
        int up2 = findUpar(v);

        if(up1==up2) return;
        if(size[up1] > size[up2]) {
            size[up1] += size[up2];
            par[up2] = up1;
        } else {
            size[up2] += size[up1];
            par[up1] = up2;
        }
    }

    int findUpar(int u) {
        if(par[u]==u) return u;
        return par[u] = findUpar(par[u]);
    }

};

int main() {    
    DisjointSet dj(6);
    dj.unionBySize(1, 3);
    dj.unionBySize(3, 6);
    cout<< " " << dj.findUpar(6);
    return 0;           
}