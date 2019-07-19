#ifndef QUARTETS_H
#define QUARTETS_H

#include <vector>
#include <utility>

#include "Gene.h"
#include "Scenario.h"

class Quartets {
public:
  Quartets(Scenario* ptrS,
           bool weightedMode)
   : m_ptrS(ptrS)
   , m_weightedMode(weightedMode) { };

   std::vector<Gene*> findBestMatches(const Gene* x,
                                      const std::vector<Gene*>& genesY,
                                      const std::vector<Gene*>& outgroupsZ);
   std::size_t supportedQuartetMajority(const Gene* x,
                                        const Gene* y1,
                                        const Gene* y2,
                                        const Gene* z);
   std::pair<std::size_t,double> supportedQuartetWeighted(const Gene* x,
                                                          const Gene* y1,
                                                          const Gene* y2,
                                                          const Gene* z);
private:
  Scenario* m_ptrS;
  bool m_weightedMode;
};

#endif /* QUARTETS_H */
