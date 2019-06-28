#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <vector>
#include <unordered_map>

template<typename T>
class DiGraph {
public:
  void addNode(T* v);
  void addEdge(T* u, T* v);

  inline const std::vector<T*>& getNeighbors(T* u) const {
    return &m_adjList[u];
  };

private:
  std::unordered_map<T*, std::vector<T*>> m_adjList;
};

#endif /* DIGRAPH_H */
