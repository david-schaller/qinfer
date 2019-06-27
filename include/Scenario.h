#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <map>
#include <string>

#include "Gene.h"

class Scenario {
public:
  void addGenes(std::vector<Gene>& g);
  void parseSpeciesGenes(const char* filepath);

private:
  std::vector<Gene> m_genes;
  std::map<std::string, Gene*> m_geneAssignments;
  std::map<std::string, std::vector<Gene*>> m_speciesGenes;

  void rebuildMap();
  void parseSpeciesGenesLine(std::string line);
};

#endif /* SCENARIO_H */
