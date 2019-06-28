#include <fstream>
#include <iostream>

#include "Scenario.h"

void
Scenario::addGenes(std::deque<Gene>& g)
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
  auto g = std::deque<Gene>();
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
  std::size_t pos = 0;
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
        m_geneAssignments.at(token)->setSpecies(species);
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
    m_geneAssignments.at(line)->setSpecies(species);
  } catch(const std::out_of_range&) {
    std::cout << "WARNING: Species-to-genes-file contains additional entries: "
              << line
              << std::endl;
  }

  m_speciesGenes.insert(std::pair<std::string, std::vector<Gene*>>(species, g));
}

void
Scenario::parseSTreeSubtrees(const char* filepath){
  std::ifstream filestream(std::string(filepath), std::ios::binary);

  if(!filestream) {
    throw std::runtime_error("Failed to open file " + std::string(filepath));
  }

  std::string line;
  while(std::getline(filestream, line)) {
    parseSTreeSubtreeLine(line);
  }

  // for(auto it = m_STreeSubtrees.begin(); it != m_STreeSubtrees.end(); ++it){
  //   for(auto it2 = (*it).begin(); it2 != (*it).end(); ++it2){
  //     std::cout << *it2 << "  ";
  //   }
  //   std::cout << std::endl;
  // }

  // build the corresponding lists of outgroup genes
  buildOutgroupLists();
}

void
Scenario::parseSTreeSubtreeLine(std::string line){
  m_STreeSubtrees.push_back(std::vector<std::string>());

  const std::string delimiter = "\t";
  std::size_t pos = 0;
  std::string token;

  while ((pos = line.find(delimiter)) != std::string::npos) {
    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());

    if(m_speciesGenes.find(token) == m_speciesGenes.end()){
      std::cout << "WARNING: Species-subtrees-file contains species without genes: "
                << token
                << " (omitted)"
                << std::endl;
    } else {
      m_STreeSubtrees.back().push_back(token);
    }
  }
  // push back last line element
  if(m_speciesGenes.find(line) == m_speciesGenes.end()){
    std::cout << "WARNING: Species-subtrees-file contains species without genes: "
              << line
              << " (omitted)"
              << std::endl;
  } else {
    m_STreeSubtrees.back().push_back(line);
  }
}

void
Scenario::buildOutgroupLists(){
  for(std::size_t i = 0; i < m_STreeSubtrees.size(); ++i){
    m_outgroups.push_back(std::vector<Gene*>());
    for(std::size_t j = 0; j < m_STreeSubtrees.size(); ++j){
      if(i != j){
        for(auto species : m_STreeSubtrees[j]){
          m_outgroups.back().insert(m_outgroups.back().end(),
                                    m_speciesGenes[species].begin(),
                                    m_speciesGenes[species].end());
        }
      }
    }
  }

  for(auto it = m_outgroups.begin(); it != m_outgroups.end(); ++it){
    for(auto it2 = (*it).begin(); it2 != (*it).end(); ++it2){
      std::cout << (*it2)->getIdentifier() << "  ";
    }
    std::cout << std::endl;
  }
}
