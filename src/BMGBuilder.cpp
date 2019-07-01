#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "BMGBuilder.h"

std::vector<Gene*>
BMGBuilder::chooseOutgroups(const std::vector<Gene*>& outgroupCandidates){
  auto outgroups = std::vector<Gene*>();

  size_t limit = std::min(m_outgroupLimit, outgroupCandidates.size());
  std::sample(outgroupCandidates.begin(), outgroupCandidates.end(),
              std::back_inserter(outgroups),
              limit,
              std::mt19937{std::random_device{}()});

  // for (auto genePtr : outgroups)
  //     std::cout << "outgroup" << genePtr->getIdentifier() << std::endl;

  return outgroups;
}

void
BMGBuilder::buildBMG(){

  // add all gene pointers to the graph
  for(Gene& gene : m_ptrS->getGenes()){
    m_bmg.addNode(&gene);
  }

  // main algorithm
  for(Gene& x : m_ptrS->getGenes()){
    const std::vector<Gene*>& outgroupCandidates = m_ptrS->getOutgroups(&x);
    std::vector<Gene*> outgroupsZ = chooseOutgroups(outgroupCandidates);

    for(const std::string& speciesY : m_ptrS->getSpeciesSubtree(x.getSubtree())){

      // skip the own species
      if(x.getSpecies() == speciesY){
        continue;
      }

      const std::vector<Gene*>& allGenesY = m_ptrS->getSpeciesGenes(speciesY);
      if(allGenesY.size() == 0){
        std::cerr << "Warning: Species without genes: "
                  << speciesY
                  << std::endl;

      } else if (allGenesY.size() == 1){
        // trivial case: only one gene in the species of Y
        m_bmg.addEdge(&x, allGenesY[0]);

      } else {

        // build the gene set Y
        std::vector<Gene*> genesY;
        if(!m_restrictY){
          genesY = std::vector<Gene*>(allGenesY);
        } else {
          genesY = std::vector<Gene*>();
          for(Gene* genePtr : allGenesY){
            if(m_ptrBmc->operator()(x.getIndex(), genePtr->getIndex())){
              genesY.push_back(genePtr);
            }
          }
        }

        // find best matches in gene set Y

      }
    }
  }
}
