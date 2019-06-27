#include <fstream>
#include <iostream>

#include "Scenario.h"

void
Scenario::addGenes(std::vector<Gene>& g)
{
  m_genes.clear();

  m_genes.insert(m_genes.begin(), std::make_move_iterator(g.begin()),
                 std::make_move_iterator(g.end()));
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
      try {
        g.push_back(m_geneAssignments.at(token));
      } catch(const std::out_of_range&) {
        std::cout << "WARNING: Species-to-genes-file contains additional entries: "
                  << token
                  << std::endl;
      }
    }
  }
  // push back last line element
  try {
    g.push_back(m_geneAssignments.at(line));
  } catch(const std::out_of_range&) {
    std::cout << "WARNING: Species-to-genes-file contains additional entries: "
              << line
              << std::endl;
  }

  m_speciesGenes.insert(std::pair<std::string, std::vector<Gene*>>(species, g));
}
