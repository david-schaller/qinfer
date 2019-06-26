#ifndef QINFER_H
#define QINFER_H

#include <vector>
#include <string>

class DistanceMatrix {
public:
  void parseFromFile(const char* filepath);

  double& operator()(std::size_t row, std::size_t col);
  // for const distance matrices
  const double& operator()(std::size_t row, std::size_t col) const;
private:
  std::size_t m_dim;
  std::vector<std::vector<double>> m_distanceMatrix;
  std::vector<std::string> m_genes;

  void parseRow(std::string row);
};

#endif /* QINFER_H */
