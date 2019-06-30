#ifndef BMGBUILDER_H
#define BMGBUILDER_H

#include "Gene.h"
#include "Scenario.h"
#include "DistanceMatrix.h"
#include "BMCandidates.h"
#include "DiGraph.t"

class BMGBuilder {
public:
  BMGBuilder(Scenario* ptrS, DistanceMatrix* ptrDm, BMCandidates* ptrBmc)
    : m_ptrS(ptrS)
    , m_ptrDm(ptrDm)
    , m_ptrBmc(ptrBmc) { };

private:
  Scenario* m_ptrS;
  DistanceMatrix* m_ptrDm;
  BMCandidates* m_ptrBmc;

  DiGraph<Gene*> m_bmg;
};

#endif /* BMGBUILDER_H */
