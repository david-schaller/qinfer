#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H

#include <vector>
#include <string>
#include <map>

#include "Gene.h"

class DistanceMatrix {
public:
  void parseFromFile(const char* filepath);

  double& operator()(std::size_t row, std::size_t col);
  // for const distance matrices
  const double& operator()(std::size_t row, std::size_t col) const;
private:
  std::size_t m_dim;
  std::vector<std::vector<double>> m_distanceMatrix;
  std::vector<Gene> m_genes;
  std::map<std::string, Gene*> m_geneAssignments;

  void parseRow(std::string row);
};

#endif /* DISTANCEMATRIX_H */
