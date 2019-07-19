#ifndef OUTGROUPCHOICE_H
#define OUTGROUPCHOICE_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Gene.h"
#include "Scenario.h"
#include "Matrix.h"
#include "Tree.h"
#include "Quartets.h"

class OutgroupChoice {
public:
  OutgroupChoice(Scenario* ptrS, Quartets* ptrQ,
                 std::size_t outgroupLimit,
                 bool weightedMode = false,
                 double incongruentThreshold = 0.2)
    : m_ptrS(ptrS)
    , m_ptrQ(ptrQ)
    , m_outgroupLimit(outgroupLimit)
    , m_weightedMode(weightedMode)
    , m_incongruentThreshold(incongruentThreshold) { };

  void initialize();
  std::vector<Gene*> getClosest(Gene* x, std::vector<Gene*>& genesY);

private:
  Scenario* m_ptrS;
  Quartets* m_ptrQ;
  std::size_t m_outgroupLimit;
  bool m_weightedMode;
  double m_incongruentThreshold;

  Matrix<std::size_t> m_I;
  Matrix<std::size_t> m_lcaS;

  std::unordered_map<std::string, std::size_t> m_speciesLeafIdx;
  std::vector<std::vector<std::shared_ptr<TreeNode>>> m_subtreeSpecies;
  std::vector<std::vector<Gene*>> m_subtreeGenes;
  std::unordered_map<int, std::unordered_set<std::string>> m_lcaOutgroups;

  void quicksortRow(std::size_t row, int l, int r);
  void buildIMatrix();
  void computeLcaS();
  void computeOutgroups();
};


#endif /* OUTGROUPCHOICE_H */