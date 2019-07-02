#ifndef BMGBUILDER_H
#define BMGBUILDER_H

#include <vector>
#include <utility>

#include "Gene.h"
#include "Scenario.h"
#include "DiGraph.h"
#include "Matrix.h"

class BMGBuilder {
public:
  BMGBuilder(Scenario* ptrS,
             std::size_t outgroupLimit,
             bool restrictY, double epsilon=0.5,
             bool weightedMode=false)
   : m_ptrS(ptrS)
   , m_outgroupLimit(outgroupLimit)
   , m_restrictY(restrictY)
   , m_epsilon(epsilon)
   , m_weightedMode(weightedMode) { };

  void buildBMG();
  void printBMG();

private:
  Scenario* m_ptrS;
  std::size_t m_outgroupLimit;
  bool m_restrictY;
  float m_epsilon;
  bool m_weightedMode;

  Matrix<int> m_bmCandidates;
  DiGraph<Gene*> m_bmg;

  void buildCandidateMatrix();
  std::vector<Gene*> chooseOutgroups(const std::vector<Gene*>& outgroupCandidates);
  std::vector<Gene*> findBestMatches(const Gene* x,
                                     const std::vector<Gene*>& genesY,
                                     const std::vector<Gene*>& outgroupsZ);
  std::size_t supportedQuartetMajority(const Gene* x,
                                       const Gene* y1,
                                       const Gene* y2,
                                       const Gene* z);
  std::pair<std::size_t,double> supportedQuartetWeighted(const Gene* x,
                                                         const Gene* y1,
                                                         const Gene* y2,
                                                         const Gene* z);
};

#endif /* BMGBUILDER_H */
