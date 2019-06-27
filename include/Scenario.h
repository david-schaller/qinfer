#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <unordered_map>
#include <string>

#include "Gene.h"

class Scenario {
public:
  void addGenes(std::vector<Gene>& g);
  void parseSpeciesGenes(const char* filepath);
  inline std::string getGeneSpecies(int i) const { return m_genes[i].getSpecies(); };

private:
  std::vector<Gene> m_genes;
  std::unordered_map<std::string, Gene*> m_geneAssignments;
  std::unordered_map<std::string, std::vector<Gene*>> m_speciesGenes;

  void rebuildMap();
  void parseSpeciesGenesLine(std::string line);
};

#endif /* SCENARIO_H */
