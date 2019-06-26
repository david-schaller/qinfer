#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <map>

#include "Gene.h"

class Scenario {
public:
  void addGenes(std::vector<Gene>& g);
private:
  std::vector<Gene> m_genes;
  std::map<std::string, Gene*> m_geneAssignments;

  void rebuildMap();
};

#endif /* SCENARIO_H */
