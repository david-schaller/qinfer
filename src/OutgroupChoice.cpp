#include <algorithm>
#include <random>
#include <cmath>

#include "OutgroupChoice.h"

void
OutgroupChoice::quicksortRow(std::size_t row, std::size_t l, std::size_t r){
  if(r <= l){
    return;
  }
  std::size_t i = l;
  std::size_t j = r;
  int piv = m_ptrS->getDistance(row, m_I.at(row, (l+r)/2));

  while(i <= j){

    while(m_ptrS->getDistance(row, m_I.at(row, i)) < piv){
      ++i;
    }
    while(m_ptrS->getDistance(row, m_I.at(row, j)) > piv){
      --j;
    }

    if(i <= j){
      int temp = m_I.at(row, i);
      m_I.at(row, i) = m_I.at(row, j);
      m_I.at(row, j) = temp;
      ++i;
      --j;
    }
  }

  quicksortRow(row, l, j);
  quicksortRow(row, i, r);
}

void
OutgroupChoice::buildIMatrix(){
  std::size_t dim {m_ptrS->getGenes().size()};
  m_I.initMatrix(dim, 0);

  for(std::size_t j = 1; j < dim; ++j){
    for(std::size_t i = 0; i < dim; ++i){
      m_I.at(i,j) = j;
    }
  }

  for(std::size_t row = 0; row < dim; ++row){
    quicksortRow(row, 0, dim-1);
  }
}

void
OutgroupChoice::computeLcaS(){
  m_ptrS->getSTree().supplyLeaves();
  std::size_t nodeN = m_ptrS->getSTree().getNodeNumber();
  std::size_t leafN = m_ptrS->getSTree().getLeafNumber();

  m_lcaS.initMatrix(leafN, 0);
  m_subtreeSpecies = std::vector<std::vector<std::shared_ptr<TreeNode>>>(nodeN, std::vector<std::shared_ptr<TreeNode>>());
  m_subtreeGenes = std::vector<std::vector<Gene*>>(nodeN, std::vector<Gene*>());

  for(auto& v : m_ptrS->getSTree().getPreorder()){

    std::size_t vid = v->getNodeIdx();
    m_subtreeSpecies[vid].insert(m_subtreeSpecies[vid].end(),
                                 v->getLeaves().begin(),
                                 v->getLeaves().end());

    for(auto& leaf : m_subtreeSpecies[vid]){
      auto genes = m_ptrS->getSpeciesGenes(leaf->getValue());
      m_subtreeGenes[vid].insert(m_subtreeGenes[vid].end(),
                                 genes.begin(), genes.end());
    }

    auto& children = v->getChildren();
    if(!v->hasChildren()){
      m_lcaS.at(vid, vid) = vid;
    } else if (children.size() >= 2){
      for(std::size_t i = 0; i < children.size()-1; ++i){
        for(std::size_t j = i; j < children.size(); ++j){

          for(auto& l1 : children[i]->getLeaves()){
            for(auto& l2 : children[j]->getLeaves()){
              m_lcaS.at(l1->getLeafIdx(), l2->getLeafIdx()) = vid;
              m_lcaS.at(l2->getLeafIdx(), l1->getLeafIdx()) = vid;
            }
          }
        }
      }
    }
  }
}

void
OutgroupChoice::computeOutgroups(){

  for(std::size_t i = 0; i < m_subtreeSpecies.size(); ++i){
    m_lcaOutgroups[i] = std::unordered_set<std::string>();
  }

  // the outgroups on the root of S are fixed
  bool rootDone = false;

  for(auto& v : m_ptrS->getSTree().getPreorder()){
    std::size_t vid = v->getNodeIdx();
    if(v->hasParent()){
      auto parentsOutgroups = m_lcaOutgroups[v->getParent().lock()->getNodeIdx()];
      m_lcaOutgroups[vid].insert(parentsOutgroups.begin(),
                                 parentsOutgroups.end());
    }

    for(auto& c1 : v->getChildren()){
      std::size_t c1id = c1->getNodeIdx();
      for(auto& c2 : v->getChildren()){
        if(c1 == c2){
          continue;
        }
        std::size_t c2id = c2->getNodeIdx();

        if(rootDone){
          auto& genesC1 = m_subtreeGenes[c1id];
          auto& speciesC2 = m_subtreeSpecies[c2id];
          auto outgroupCandidates = std::unordered_set<std::shared_ptr<TreeNode>>(speciesC2.begin(),
                                                                                  speciesC2.end());
          if((genesC1.size() > 1) && (speciesC2.size() > 1)){
            for(std::size_t i = 0; i < speciesC2.size()-1; ++i){
              for(std::size_t j = i; j < speciesC2.size(); ++j){
                auto genesC2 = std::vector<Gene*>();
                genesC2.insert(genesC2.end(),
                      m_ptrS->getSpeciesGenes(speciesC2[i]->getValue()).begin(),
                      m_ptrS->getSpeciesGenes(speciesC2[i]->getValue()).end());
                genesC2.insert(genesC2.end(),
                      m_ptrS->getSpeciesGenes(speciesC2[j]->getValue()).begin(),
                      m_ptrS->getSpeciesGenes(speciesC2[j]->getValue()).end());

                std::vector<double> votes {0.0, 0.0};
                for(int k = 0; k < 20; ++k){
                  auto a_b = std::vector<Gene*>();
                  std::sample(genesC1.begin(), genesC1.end(),
                              std::back_inserter(a_b),
                              2, std::mt19937{std::random_device{}()});
                  auto c_d = std::vector<Gene*>();
                  std::sample(genesC2.begin(), genesC2.end(),
                              std::back_inserter(c_d),
                              2, std::mt19937{std::random_device{}()});
                  if(m_weightedMode){
                    auto voteAndWeight = supportedQuartetWeighted(a_b[0], a_b[1], c_d[0], c_d[1]);
                    if(voteAndWeight.first == 0){
                      votes[0] += voteAndWeight.second;
                    } else {
                      votes[1] += voteAndWeight.second;
                    }
                  } else {
                    if(supportedQuartetMajority(a_b[0], a_b[1], c_d[0], c_d[1]) == 0){
                      votes[0] += 1.0;
                    } else {
                      votes[1] += 1.0;
                    }
                  }
                }

                if(votes[1] / (votes[0]+votes[1]) >= 0.2){
                  outgroupCandidates.erase(speciesC2[i]);
                  outgroupCandidates.erase(speciesC2[j]);
                }
              }
            }
          }
          for(auto species : outgroupCandidates){
            m_lcaOutgroups[c1id].insert(species->getValue());
          }

        } else {
          for(auto species : m_subtreeSpecies[c2id]){
            m_lcaOutgroups[c1id].insert(species->getValue());
          }
        }
        rootDone = true;
      }
    }
  }
}

std::size_t
OutgroupChoice::supportedQuartetMajority(const Gene* x, const Gene* y1,
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
OutgroupChoice::supportedQuartetWeighted(const Gene* x, const Gene* y1,
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
