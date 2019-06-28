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

  void parseSTreeSubtrees(const char* filepath);

private:
  std::deque<Gene> m_genes;
  std::unordered_map<std::string, Gene*> m_geneAssignments;
  std::unordered_map<std::string, std::vector<Gene*>> m_speciesGenes;

  // list of lists of species identifiers belonging to the same m_STreeSubtree
  // of the species tree root and corresponding outgroup genes
  std::vector<std::vector<std::string>> m_STreeSubtrees;
  std::vector<std::vector<Gene*>> m_outgroups;

  void rebuildMap();
  void parseSpeciesGenesLine(std::string line);
  void parseSTreeSubtreeLine(std::string line);
  void buildOutgroupLists();
};

#endif /* SCENARIO_H */
