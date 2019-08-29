#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <deque>
#include <unordered_map>
#include <string>

#include "Gene.h"
#include "Matrix.h"
#include "Tree.h"
#include "Benchmark.h"

class Scenario {
public:
  Scenario(Benchmark* bm = nullptr)
  : m_genes()
  , m_distanceMatrix()
  , m_geneAssignments()
  , m_speciesGenes()
  , m_STree()
  , m_benchmark(bm) { };

  void addGenes(std::deque<Gene>& g);

  void parseDistanceMatrix(const char* filepath);
  void parseSpeciesGenes(const char* filepath);
  void parseSTreeSubtrees(const char* filepath, bool subtreeFiles);

  const std::string&
  getGeneSpecies(int i) const { return m_genes[i].getSpecies();};

  std::deque<Gene>&
  getGenes() { return m_genes; };

  Gene*
  getGenePtr(size_t idx) { return &(m_genes[idx]); }

  const std::vector<Gene*>&
  getSpeciesGenes(std::string species) const { return m_speciesGenes.at(species); };

  const double&
  getDistance(size_t row, size_t column) const { return m_distanceMatrix.at(row, column); };

  Tree&
  getSTree() { return m_STree; };

  const std::vector<Gene*>&
  getOutgroups(Gene* genePtr) const;

  const std::vector<std::string>&
  getSpeciesSubtree(size_t subtreeIdx) const;

private:
  std::deque<Gene> m_genes;
  Matrix<double> m_distanceMatrix;
  std::unordered_map<std::string, Gene*> m_geneAssignments;
  std::unordered_map<std::string, std::vector<Gene*>> m_speciesGenes;
  Tree m_STree;
  Benchmark* m_benchmark;

  // list of lists of species identifiers belonging to the same m_STreeSubtree
  // of the species tree root and corresponding outgroup genes
  std::vector<std::vector<std::string>> m_STreeSubtrees;
  std::vector<std::vector<Gene*>> m_outgroups;

  void rebuildMap();
  void parseDistanceMatrixRow(std::string row, size_t rowIdx);
  void parseSpeciesGenesLine(std::string line);
  void parseNewickAndCheck(std::string line);
  void parseSTreeSubtreeLine(std::string line);
  void checkSpeciesAvailability();
  void buildOutgroupInfo();
};

#endif /* SCENARIO_H */
