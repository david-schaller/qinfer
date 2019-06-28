#include <fstream>
#include <string>

#include <iostream>

#include "DistanceMatrix.h"

void
DistanceMatrix::parseFromFile(const char* filepath)
{
  std::ifstream filestream(std::string(filepath), std::ios::binary);

  if(!filestream) {
    throw std::runtime_error("Failed to open file " + std::string(filepath));
  }

  auto v = std::vector<std::vector<double>>();
  auto g = std::deque<Gene>();
  std::size_t lineCounter = 0;
  std::string line;
  while(std::getline(filestream, line)) {
    // We are going to read the first line which contains the dimension
    if(lineCounter == 0) {
      m_dim = std::stoi(line);
    }
    else {
      parseRow(line, g);
    }

    ++lineCounter;
  }

  m_ptrS->addGenes(g);
}

void
DistanceMatrix::parseRow(std::string row, std::deque<Gene>& g)
{
  auto v = std::vector<double>(m_dim, 0.0);

  const std::string delimiter = "\t";

  std::size_t idx = 0;
  size_t pos = 0;
  std::string token;
  while ((pos = row.find(delimiter)) != std::string::npos) {
    token = row.substr(0, pos);
    row.erase(0, pos + delimiter.length());

    // Skip the element row which contains the identifier
    if(idx == 0) {
      g.push_back(Gene(token, g.size()));
    } else {
      v[idx - 1] = std::stod(token);
    }

    ++idx;
  }
  // parse the last element
  v[idx - 1] = std::stod(row);

  m_distanceMatrix.push_back(v);
}

double&
DistanceMatrix::operator()(std::size_t row, std::size_t col)
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_distanceMatrix[row][col];
}

const double&
DistanceMatrix::operator()(std::size_t row, std::size_t col) const
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_distanceMatrix[row][col];
}
