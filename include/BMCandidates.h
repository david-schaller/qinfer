#ifndef BMCANDIDATES_H
#define BMCANDIDATES_H

#include <vector>

#include "Scenario.h"
#include "DistanceMatrix.h"

class BMCandidates {
public:
  BMCandidates(Scenario* ptrS): m_ptrS(ptrS) { };

  void buildCandidateMatrix(const DistanceMatrix& dm, float epsilon);

  int& operator()(std::size_t row, std::size_t col);
  // for const best match distance matrices
  const int& operator()(std::size_t row, std::size_t col) const;
private:
  Scenario* m_ptrS;
  std::size_t m_dim;
  std::vector<std::vector<int>> m_candidateMatrix;
};

#endif /* BMCANDIDATES_H */
