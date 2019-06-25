#ifndef QINFER_H
#define QINFER_H

#include <vector>
#include <string>

std::vector<std::vector<double>>
parseDistanceMatrix(char* filepath);

std::vector<double>
parseDistanceMatrixRow(std::size_t totalLines, std::string row);

#endif /* QINFER_H */
