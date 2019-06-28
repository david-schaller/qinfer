#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>

template<typename T>
class DiGraph {
public:
  void addNode(T v);
  void addEdge(T u, T v);

  std::vector<std::set<T>> stronglyConnectedComponent();

  inline const std::vector<T>& getNeighbors(T u) {
    return m_adjList[u];
  };

private:
  std::unordered_map<T, std::vector<T>> m_adjList;
};

template<typename T>
void
DiGraph<T>::addNode(T v){
  if(m_adjList.find(v) == m_adjList.end()){
    m_adjList.insert(std::pair<T, std::vector<T>>(v, std::vector<T>()));
  }
}

template<typename T>
void
DiGraph<T>::addEdge(T u, T v){

  // check if u is in the graph and if not insert it first
  if(m_adjList.find(u) == m_adjList.end()){
    m_adjList.insert(std::pair<T, std::vector<T>>(u, std::vector<T>()));
  }

  // check if v is in the graph and if not insert it first
  if(m_adjList.find(v) == m_adjList.end()){
    m_adjList.insert(std::pair<T, std::vector<T>>(v, std::vector<T>()));
  }
  //
  // insert the directed edge (check for existence not implemented!)
  m_adjList[u].push_back(v);
}

template<typename T>
std::vector<std::set<T>>
DiGraph<T>::stronglyConnectedComponent(){

  auto sccs = std::vector<std::set<T>>();

  auto nbrs = std::map<T, std::vector<T>*>();
  auto preorder = std::map<T, int>();
  auto lowLink = std::map<T, int>();
  auto sccFound = std::set<T>();
  auto scc = std::stack<T>();

  for(auto source = m_adjList.begin(); source != m_adjList.end(); ++source){
    if(sccFound.find(source->first) != sccFound.end()){

    }
  }

  return sccs;
}

#endif /* DIGRAPH_H */
