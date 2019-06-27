#include <fstream>

#include <iostream>

#include "Scenario.h"

void
Scenario::addGenes(std::vector<Gene>& g)
{
  m_genes.clear();

  m_genes.insert(m_genes.end(), g.begin(), g.end());
  rebuildMap();
}

void
Scenario::rebuildMap()
{
  m_geneAssignments.clear();

  for(auto it = m_genes.begin(); it != m_genes.end(); ++it) {
    m_geneAssignments.insert(
      std::pair<std::string, Gene*>(it->getIdentifier(), &(*it))
    );
  }
}

void
Scenario::parseSpeciesGenes(const char* filepath){
  std::ifstream filestream(std::string(filepath), std::ios::binary);

  if(!filestream) {
    throw std::runtime_error("Failed to open file " + std::string(filepath));
  }

  auto v = std::vector<std::vector<double>>();
  auto g = std::vector<Gene>();
  std::string line;
  while(std::getline(filestream, line)) {
    // We are going to read the first line which contains the dimension
    parseSpeciesGenesLine(line);
  }
}

void
Scenario::parseSpeciesGenesLine(std::string line){
  auto g = std::vector<Gene*>();
  std::string species;

  const std::string delimiter = "\t";

  bool firstElement = true;
  size_t pos = 0;
  std::string token;

  while ((pos = line.find(delimiter)) != std::string::npos) {
    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());

    // the first element is the species identifier
    if(firstElement) {
      species = token;
      firstElement = false;
    } else {
      g.push_back(m_geneAssignments[token]);
    }
  }
  // push back last line element
  g.push_back(m_geneAssignments[line]);

  m_speciesGenes.insert(std::pair<std::string, std::vector<Gene*>>(species, g));
}
