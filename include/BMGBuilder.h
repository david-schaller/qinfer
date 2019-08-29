#ifndef BMGBUILDER_H
#define BMGBUILDER_H

#include <vector>
#include <utility>

#include "Gene.h"
#include "Scenario.h"
#include "DiGraph.h"
#include "Matrix.h"
#include "Quartets.h"
#include "OutgroupChoice.h"
#include "Benchmark.h"

class BMGBuilder {
public:
  BMGBuilder(Scenario* ptrS,
             size_t outgroupLimit,
             bool restrictY, double epsilon = 0.5,
             bool weightedMode = false,
             bool disableQuartet = false,
             bool relativeOutgroups = false,
             double incongruentThreshold = 0.2,
             Benchmark* bm = nullptr)
   : m_ptrS(ptrS)
   , m_outgroupLimit(outgroupLimit)
   , m_restrictY(restrictY)
   , m_epsilon(epsilon)
   , m_disableQuartet(disableQuartet)
   , m_relativeOutgroups(relativeOutgroups)
   , m_quartets(Quartets(ptrS, weightedMode))
   , m_outgroupChoice (OutgroupChoice(ptrS, &m_quartets,
                                      outgroupLimit,
                                      weightedMode,
                                      incongruentThreshold,
                                      bm))
   , m_benchmark(bm) { };

  void buildBMG();
  void printBMG();

private:
  Scenario* m_ptrS;
  size_t m_outgroupLimit;
  bool m_restrictY;
  float m_epsilon;
  bool m_disableQuartet;
  bool m_relativeOutgroups;
  Quartets m_quartets;
  OutgroupChoice m_outgroupChoice;
  Benchmark* m_benchmark;

  Matrix<int> m_bmCandidates;
  DiGraph<Gene*> m_bmg;

  // COMMON FUNCTIONS
  void
  epsilonMethod();

  void
  buildCandidateMatrix();

  // OUTGROUP METHOD I
  std::vector<Gene*>
  chooseOutgroups(const std::vector<Gene*>& outgroupCandidates);

  void
  buildRootOutgroups();

  // OUTGROUP METHOD II
  void
  buildRelativeOutgroups();
};

#endif /* BMGBUILDER_H */
