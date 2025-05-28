#include "graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <stack>
#include <map>


DisjointSet::DisjointSet(int numVertices) {
    subsets.resize(numVertices);
    for (int i = 0; i < numVertices; i++) subsets[i].parent = i;}

Vertex DisjointSet::findSet(Vertex x) {
    if (subsets[x].parent != x) {
        subsets[x].parent = findSet(subsets[x].parent);}
    return subsets[x].parent;}

void DisjointSet::makeSet(Vertex x) {
    subsets[x].parent = x;
}

void DisjointSet::unionSets(Vertex x, Vertex y) {
    Vertex forXs = findSet(x);
    Vertex forYs = findSet(y);
    if (forXs != forYs) { subsets[forYs].parent = forXs; }
}

void error(string msg) {
    cout << "error: unable to open file " << msg << endl;
    return;
    }

bool compareEdges(const Edge &a, const Edge &b){
    return a.weight < b.weight;
}

Graph Graph::sort_edges() const {
    Graph organize = *this;
    sort(organize.begin(), organize.end(), compareEdges);
    return organize;
}

VertexList Graph::edges_from(Vertex vertex) const {
    VertexList edgVlist;
    for (const Edge &eg : *this) {
        if (eg.u == vertex)
            edgVlist.push_back(eg.v);
    }
    return edgVlist;
}

EdgeList Kruskals(const Graph& G) {
    EdgeList edgElist; Graph organize = G.sort_edges(); DisjointSet djs(G.numVertices);
    for (Vertex i = 0; i < G.numVertices; i++) {djs.makeSet(i);}
    for (const Edge &eg : organize) {
        if (djs.findSet(eg.u) != djs.findSet(eg.v)) {edgElist.push_back(eg);djs.unionSets(eg.u, eg.v);}}
    return edgElist;}

int sum_weights(EdgeList const& L) {
    int total = 0;
    for (const Edge &eg : L) {total += eg.weight;}
    return total;}

void file_to_graph(string filename, Graph & G) {
    ifstream inFile(filename);
    if (!inFile) {error(filename);}
    inFile >> G;}

string get_arg(int argc, char *argv[], string def) {
    if (argc > 1) return argv[1];
    return def;}

VertexList dfs(const Graph& graph, Vertex startVertex) {
    vector<bool> order(graph.numVertices, false); order[startVertex] = true; VertexList result; stack<Vertex> st; st.push(startVertex);
    while (!st.empty()) {
        Vertex current = st.top(); st.pop(); result.push_back(current);
        for (auto temp : graph.edges_from(current)) {
            if (!order[temp]) { order[temp] = true; st.push(temp);}}} return result;}

VertexList bfs(const Graph& graph, Vertex startVertex) {
    VertexList bfsTraversal; vector<bool> discovered(graph.numVertices, false); queue<Vertex> que;
    discovered[startVertex] = true; que.push(startVertex); 
    while (!que.empty()) bfsCheck(graph, que, discovered, bfsTraversal);
    return bfsTraversal;
}

static void bfsCheck(const Graph& graph, queue<Vertex>& que, vector<bool>& discovered, VertexList& bfsTraversal) {
    Vertex fsv = que.front(); que.pop(); bfsTraversal.push_back(fsv);
    VertexList neighborList = graph.edges_from(fsv); 
    for (Vertex tmp : neighborList) {
    if (!discovered[tmp]) { discovered[tmp] = true; que.push(tmp); }}
}