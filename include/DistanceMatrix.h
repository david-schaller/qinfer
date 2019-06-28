#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H

#include <vector>
#include <deque>
#include <string>
#include <map>

#include "Scenario.h"
#include "Gene.h"

class DistanceMatrix {
public:
  DistanceMatrix(Scenario* ptrS): m_ptrS(ptrS) { };
  void parseFromFile(const char* filepath);

  std::size_t getDim() const { return m_dim; };

  double& operator()(std::size_t row, std::size_t col);
  // for const distance matrices
  const double& operator()(std::size_t row, std::size_t col) const;
private:
  Scenario* m_ptrS;
  std::size_t m_dim;
  std::vector<std::vector<double>> m_distanceMatrix;

  void parseRow(std::string row, std::deque<Gene>& g);
};

#endif /* DISTANCEMATRIX_H */
