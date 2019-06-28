#include "DiGraph.h"

template<typename T>
void
DiGraph<T>::addNode(T* v){
  if(m_adjList.find(v) == m_adjList.end()){
    m_adjList.insert(std::pair<T*, std::vector<T*>>(v, std::vector<T*>()));
  }
}

template<typename T>
void
DiGraph<T>::addEdge(T* u, T* v){

  // check if u is in the graph and if not insert it first
  if(m_adjList.find(u) == m_adjList.end()){
    m_adjList.insert(std::pair<T*, std::vector<T*>>(u, std::vector<T*>()));
  }

  // check if v is in the graph and if not insert it first
  if(m_adjList.find(v) == m_adjList.end()){
    m_adjList.insert(std::pair<T*, std::vector<T*>>(v, std::vector<T*>()));
  }

  // insert the directed edge (check for existence not implemented!)
  m_adjList[u].push_back(v);
}
