#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <deque>
#include <unordered_map>
#include <string>

#include "Gene.h"
#include "Matrix.h"
#include "Tree.h"

class Scenario {
public:
  void addGenes(std::deque<Gene>& g);

  void parseDistanceMatrix(const char* filepath);
  void parseSpeciesGenes(const char* filepath);
  void parseSTreeSubtrees(const char* filepath, bool subtreeFiles);

  inline const std::string& getGeneSpecies(int i) const {
    return m_genes[i].getSpecies();
  };
  inline std::deque<Gene>& getGenes() { return m_genes; };
  inline Gene* getGenePtr(std::size_t idx) { return &(m_genes[idx]); }
  inline const std::vector<Gene*>& getSpeciesGenes(std::string species) const {
    return m_speciesGenes.at(species);
  };
  inline const double& getDistance(std::size_t row, std::size_t column) const {
    return m_distanceMatrix.at(row, column);
  };
  inline Tree& getSTree() { return m_STree; };

  const std::vector<Gene*>& getOutgroups(Gene* genePtr) const;
  const std::vector<std::string>& getSpeciesSubtree(std::size_t subtreeIdx) const;

private:
  std::deque<Gene> m_genes;
  Matrix<double> m_distanceMatrix;
  std::unordered_map<std::string, Gene*> m_geneAssignments;
  std::unordered_map<std::string, std::vector<Gene*>> m_speciesGenes;
  Tree m_STree;

  // list of lists of species identifiers belonging to the same m_STreeSubtree
  // of the species tree root and corresponding outgroup genes
  std::vector<std::vector<std::string>> m_STreeSubtrees;
  std::vector<std::vector<Gene*>> m_outgroups;

  void rebuildMap();
  void parseDistanceMatrixRow(std::string row, std::size_t rowIdx);
  void parseSpeciesGenesLine(std::string line);
  void parseNewickAndCheck(std::string line);
  void parseSTreeSubtreeLine(std::string line);
  void checkSpeciesAvailability();
  void buildOutgroupInfo();
};

#endif /* SCENARIO_H */
