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

class OutgroupChoice {
public:
  OutgroupChoice(Scenario* ptrS, bool weightedMode = false)
    : m_ptrS(ptrS)
    , m_weightedMode(weightedMode) { };

private:
  Scenario* m_ptrS;
  bool m_weightedMode;
  Matrix<std::size_t> m_I;
  Matrix<std::size_t> m_lcaS;
  std::vector<std::vector<std::shared_ptr<TreeNode>>> m_subtreeSpecies;
  std::vector<std::vector<Gene*>> m_subtreeGenes;
  std::unordered_map<int, std::unordered_set<std::string>> m_lcaOutgroups;

  void quicksortRow(std::size_t row, std::size_t l, std::size_t r);
  void buildIMatrix();
  void computeLcaS();
  void computeOutgroups();
  std::size_t supportedQuartetMajority(const Gene* x,
                                       const Gene* y1,
                                       const Gene* y2,
                                       const Gene* z);
  std::pair<std::size_t,double> supportedQuartetWeighted(const Gene* x,
                                                         const Gene* y1,
                                                         const Gene* y2,
                                                         const Gene* z);
};


#endif /* OUTGROUPCHOICE_H */
