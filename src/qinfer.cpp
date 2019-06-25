#include <fstream>
#include <string>

#include <iostream>

#include "qinfer.h"

std::vector<std::vector<double>>
parseDistanceMatrix(char* filepath)
{
  std::ifstream filestream(std::string(filepath), std::ios::binary);

  if(!filestream) {
    std::cerr << "Failed to open " << filepath << std::endl;

    throw std::runtime_error("Failed to open file " + std::string(filepath));
  }

  auto v = std::vector<std::vector<double>>();
  std::size_t lineCounter = 0;
  std::size_t totalLines = 0;
  std::string line;
  while(std::getline(filestream, line)) {
    // We are going to read the first line which contains the dimension
    if(lineCounter == 0) {
      totalLines = std::stoi(line);
    }
    else {
      v.push_back(parseDistanceMatrixRow(totalLines, line));
    }

    ++lineCounter;
  }

  return v;
}

std::vector<double>
parseDistanceMatrixRow(std::size_t totalLines, std::string row)
{
  auto v = std::vector<double>(totalLines, 0.0);

  const std::string delimiter = "\t";

  std::size_t idx = 0;
  size_t pos = 0;
  std::string token;
  while ((pos = row.find(delimiter)) != std::string::npos) {
    token = row.substr(0, pos);
    row.erase(0, pos + delimiter.length());

    // Skip the first row which contains the identifier
    if(idx > 0) {
      v[idx - 1] = std::stod(token);
    }

    ++idx;
  }

  return v;
}
