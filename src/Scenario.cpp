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
