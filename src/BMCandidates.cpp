#include <unordered_map>

#include "BMCandidates.h"

void
BMCandidates::buildCandidateMatrix(const DistanceMatrix& dm, float epsilon){
  m_dim = dm.getDim();
  m_candidateMatrix = std::vector<std::vector<int>>(
                        m_dim, std::vector<int>(m_dim, 0)
                      );

  for(std::size_t i = 0; i < m_dim; ++i){
    auto speciesMap = std::unordered_map<std::string, double>();
    auto species_i = m_ptrS->getGeneSpecies(i);
    std::size_t j;

    for(j = 0; j < m_dim; ++j){
      auto species_j = m_ptrS->getGeneSpecies(j);
      if(species_i != species_j){
        if(speciesMap.find(species_j) == speciesMap.end()){
          // initialize minimum if key did not exist so far
          speciesMap[species_j] = dm.at(i, j);
        } else {
          // update the minimum
          if(dm.at(i, j) < speciesMap[species_j]){
            speciesMap[species_j] = dm.at(i, j);
          }
        }
      }
    }

    for(j = 0; j < m_dim; ++j){
      auto species_j = m_ptrS->getGeneSpecies(j);
      if( (species_i != species_j) &&
          (dm.at(i, j) <= (1 + epsilon) * speciesMap[species_j])){
        m_candidateMatrix[i][j] = 1;
      }
    }
  }

  // // test output of the matrix
  // for(std::size_t i = 0; i < m_dim; ++i){
  //   for(std::size_t j = 0; j < m_dim; ++j){
  //       std::cout << m_candidateMatrix[i][j] << "  ";
  //   }
  //   std::cout << std::endl;
  // }
}

int&
BMCandidates::at(std::size_t row, std::size_t col)
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_candidateMatrix[row][col];
}

const int&
BMCandidates::at(std::size_t row, std::size_t col) const
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_candidateMatrix[row][col];
}
