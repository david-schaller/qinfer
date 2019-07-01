#ifndef BMGBUILDER_H
#define BMGBUILDER_H

#include <vector>

#include "Gene.h"
#include "Scenario.h"
#include "DistanceMatrix.h"
#include "BMCandidates.h"
#include "DiGraph.t"

class BMGBuilder {
public:
  BMGBuilder(Scenario* ptrS, DistanceMatrix* ptrDm, BMCandidates* ptrBmc,
             std::size_t outgroupLimit, bool restrictY)
    : m_ptrS(ptrS)
    , m_ptrDm(ptrDm)
    , m_ptrBmc(ptrBmc)
    , m_outgroupLimit(outgroupLimit)
    , m_restrictY(restrictY) { };

  std::vector<Gene*> chooseOutgroups(const std::vector<Gene*>& outgroupCandidates);
  void buildBMG();

private:
  Scenario* m_ptrS;
  DistanceMatrix* m_ptrDm;
  BMCandidates* m_ptrBmc;
  std::size_t m_outgroupLimit;
  bool m_restrictY;

  DiGraph<Gene*> m_bmg;
};

#endif /* BMGBUILDER_H */
