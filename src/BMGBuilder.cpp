#include <algorithm>
#include <iterator>
#include <iostream>
#include <random>
#include <cmath>

#include "BMGBuilder.h"
#include "DiGraph.h"

void
BMGBuilder::buildCandidateMatrix(){
  std::size_t dim {m_ptrS->getGenes().size()};
  m_bmCandidates.initMatrix(dim, 0);

  for(std::size_t i = 0; i < dim; ++i){
    auto speciesMap = std::unordered_map<std::string, double>();
    auto species_i = m_ptrS->getGeneSpecies(i);
    std::size_t j;

    for(j = 0; j < dim; ++j){
      auto species_j = m_ptrS->getGeneSpecies(j);
      if(species_i != species_j){
        if(speciesMap.find(species_j) == speciesMap.end()){
          // initialize minimum if key did not exist so far
          speciesMap[species_j] = m_ptrS->getDistance(i, j);
        } else {
          // update the minimum
          if(m_ptrS->getDistance(i, j) < speciesMap[species_j]){
            speciesMap[species_j] = m_ptrS->getDistance(i, j);
          }
        }
      }
    }

    for(j = 0; j < dim; ++j){
      auto species_j = m_ptrS->getGeneSpecies(j);
      if( (species_i != species_j) &&
          (m_ptrS->getDistance(i, j) <= (1 + m_epsilon) * speciesMap[species_j])){
        m_bmCandidates.at(i,j) = 1;
      }
    }
  }

  // // test output of the matrix
  // for(std::size_t i = 0; i < dim; ++i){
  //   for(std::size_t j = 0; j < dim; ++j){
  //       std::cout << m_bmCandidates.at(i,j) << "  ";
  //   }
  //   std::cout << std::endl;
  // }
}

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

std::vector<Gene*>
BMGBuilder::findBestMatches(const Gene* x,
                            const std::vector<Gene*>& genesY,
                            const std::vector<Gene*>& outgroupsZ){
  auto gamma = DiGraph<Gene*>();

  for(std::size_t i = 0; i < genesY.size(); ++i){
    Gene* y1 = genesY[i];
    for(std::size_t j = i+1; j < genesY.size(); ++j){
      Gene* y2 = genesY[j];

      std::vector<double> votes = {0.0, 0.0, 0.0, 0.0};

      for(const Gene* z : outgroupsZ){
        if(m_weightedMode){
          auto voteAndWeight = supportedQuartetWeighted(x, y1, y2, z);
          votes[voteAndWeight.first] += voteAndWeight.second;
        } else {
          votes[supportedQuartetMajority(x, y1, y2, z)] += 1.0;
        }
      }

      int quartet = std::distance(votes.begin(),
                                  std::max_element(votes.begin(), votes.end()));
      switch(quartet){
        case 0:
          // 0: xy1 | y2z
          gamma.addEdge(y2, y1);
          break;
        case 1:
          // 1: xy2 | y1z
          gamma.addEdge(y1, y2);
          break;
        default:
          // 2: xz | y1y2   or   3: star
          gamma.addEdge(y2, y1);
          gamma.addEdge(y1, y2);
      }
    }
  }

  return gamma.getSccWithoutOutedges();
}

std::size_t
BMGBuilder::supportedQuartetMajority(const Gene* x, const Gene* y1,
                                     const Gene* y2, const Gene* z){
  std::size_t xIdx = x->getIndex();
  std::size_t y1Idx = y1->getIndex();
  std::size_t y2Idx = y2->getIndex();
  std::size_t zIdx = z->getIndex();
  std::size_t quartet;

  double xy1_y2z = m_ptrS->getDistance(xIdx, y1Idx) + m_ptrS->getDistance(y2Idx, zIdx);
  double xy2_y1z = m_ptrS->getDistance(xIdx, y2Idx) + m_ptrS->getDistance(y1Idx, zIdx);
  double xz_y1y2 = m_ptrS->getDistance(xIdx, zIdx) + m_ptrS->getDistance(y1Idx, y2Idx);

  // 0: xy1 | y2z
  if(xy1_y2z < xy2_y1z && xy1_y2z < xz_y1y2){
    quartet = 0;
  // 1: xy2 | y1z
  } else if(xy2_y1z < xy1_y2z && xy2_y1z < xz_y1y2){
    quartet = 1;
  // 2: xz | y1y2
  } else if(xz_y1y2 < xy1_y2z && xz_y1y2 < xy2_y1z){
    quartet = 2;
  // 3: star topology
  } else {
    quartet = 3;
  }

  return quartet;
}

std::pair<std::size_t,double>
BMGBuilder::supportedQuartetWeighted(const Gene* x, const Gene* y1,
                                     const Gene* y2, const Gene* z){
  std::size_t xIdx = x->getIndex();
  std::size_t y1Idx = y1->getIndex();
  std::size_t y2Idx = y2->getIndex();
  std::size_t zIdx = z->getIndex();
  std::size_t quartet;
  double weight = 0.0;

  double xy1_y2z = m_ptrS->getDistance(xIdx, y1Idx) + m_ptrS->getDistance(y2Idx, zIdx);
  double xy2_y1z = m_ptrS->getDistance(xIdx, y2Idx) + m_ptrS->getDistance(y1Idx, zIdx);
  double xz_y1y2 = m_ptrS->getDistance(xIdx, zIdx) + m_ptrS->getDistance(y1Idx, y2Idx);

  // 0: xy1 | y2z
  if(xy1_y2z < xy2_y1z && xy1_y2z < xz_y1y2){
    quartet = 0;
  // 1: xy2 | y1z
  } else if(xy2_y1z < xy1_y2z && xy2_y1z < xz_y1y2){
    quartet = 1;
  // 2: xz | y1y2
  } else if(xz_y1y2 < xy1_y2z && xz_y1y2 < xy2_y1z){
    quartet = 2;
  // 3: star topology
  } else {
    quartet = 3;
  }

  std::vector<double> sums = {xy1_y2z, xy2_y1z, xz_y1y2};
  std::sort(sums.begin(), sums.end());

  if(sums[2] > 0){
    weight = (1 - sums[0]/sums[2]) * std::exp(sums[1]-sums[2]);
  }

  return std::make_pair(quartet, weight);
}

void
BMGBuilder::buildBMG(){
  // build a matrix to check whether y is a best match candidate for x
  if(m_restrictY){
    buildCandidateMatrix();
  }

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
            if(m_bmCandidates.at(x.getIndex(), genePtr->getIndex())){
              genesY.push_back(genePtr);
            }
          }
        }

        // find best matches in gene set Y
        for(Gene* bm : findBestMatches(&x, genesY, outgroupsZ)){
          m_bmg.addEdge(&x, bm);
        }
      }
    }
  }
}

void
BMGBuilder::printBMG(){
  for(const auto& vNbrsPair : m_bmg.getAdjacency()){
    if(vNbrsPair.second.size() > 0){
      for(const Gene* nbr : vNbrsPair.second){
        std::cout << vNbrsPair.first->getIdentifier()
                  << " "
                  << nbr->getIdentifier()
                  << std::endl;
      }
    }
  }
}
