#include <iostream>
#include <filesystem>

#include <vector>

#include "qinfer.h"

namespace fs = std::filesystem;

auto
main(int argc, char* argv[]) -> int
{
  // We need three input files (+ program name as first arg)
  if (argc != 4) {
    std::cerr << "3 input files required. "
              << argc - 1
              << " given. Abort!"
              << std::endl;
    return -1;
  }

  // Check if files exist
  std::size_t fileCount = 3;
  bool fileError = false;
  for(std::size_t i = 1; i < fileCount + 1; ++i) {
    fs::path filepath(argv[i]);

    if(!fs::exists(filepath) || !fs::is_regular_file(filepath)) {
      std::cerr << "File "
                << filepath
                << " does not exist or is no file!"
                << std::endl;
      fileError = true;
    }
  }

  if(fileError) return -2;

  std::vector<std::vector<double>> distanceMatrix = parseDistanceMatrix(argv[1]);

  return 0;
}
