#ifndef BMGBUILDER_H
#define BMGBUILDER_H

#include <vector>
#include <utility>

#include "Gene.h"
#include "Scenario.h"
#include "DistanceMatrix.h"
#include "BMCandidates.h"
#include "DiGraph.t"

class BMGBuilder {
public:
  BMGBuilder(Scenario* ptrS, DistanceMatrix* ptrDm, BMCandidates* ptrBmc,
             std::size_t outgroupLimit, bool restrictY,
             bool weightedMode=false)
    : m_ptrS(ptrS)
    , m_ptrDm(ptrDm)
    , m_ptrBmc(ptrBmc)
    , m_outgroupLimit(outgroupLimit)
    , m_restrictY(restrictY)
    , m_weightedMode(weightedMode) { };

  void buildBMG();
  void printBMG();

private:
  Scenario* m_ptrS;
  DistanceMatrix* m_ptrDm;
  BMCandidates* m_ptrBmc;
  std::size_t m_outgroupLimit;
  bool m_restrictY;
  bool m_weightedMode;

  DiGraph<Gene*> m_bmg;

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
