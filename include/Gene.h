#ifndef GENE_H
#define GENE_H

#include <string>

class Gene {
public:
  Gene(std::string identifier): m_identifier(identifier) { };
  
  inline std::string getIdentifier() { return m_identifier; };
  inline std::string getSpecies() { return m_species; };
private:
  std::string m_identifier;
  std::string m_species;
};

#endif /* GENE_H */
