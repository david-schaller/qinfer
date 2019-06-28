#ifndef GENE_H
#define GENE_H

#include <string>

class Gene {
public:
  Gene(std::string identifier, std::size_t idx)
    : m_identifier(identifier)
    , m_idx (idx) { };

  inline std::string getIdentifier() const { return m_identifier; };
  inline std::string getSpecies() const { return m_species; };
  inline std::size_t getIndex() const { return m_idx; };

  inline void setSpecies(std::string species) { m_species = species; }

  inline bool operator==(const Gene &otherGene) const
    {
        return (m_identifier == otherGene.m_identifier);
    }
private:
  std::string m_identifier;
  std::string m_species;
  std::size_t m_idx;
};

#endif /* GENE_H */
