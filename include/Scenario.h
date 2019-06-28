#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <deque>
#include <unordered_map>
#include <string>

#include "Gene.h"

class Scenario {
public:
  void addGenes(std::deque<Gene>& g);
  void parseSpeciesGenes(const char* filepath);
  inline std::string getGeneSpecies(int i) const { return m_genes[i].getSpecies(); };

private:
  std::deque<Gene> m_genes;
  std::unordered_map<std::string, Gene*> m_geneAssignments;
  std::unordered_map<std::string, std::vector<Gene*>> m_speciesGenes;

  void rebuildMap();
  void parseSpeciesGenesLine(std::string line);
};

#endif /* SCENARIO_H */
