#include <iostream>
#include <filesystem>
#include <string>

#include "Scenario.h"
#include "BMGBuilder.h"

namespace fs = std::filesystem;

auto
main(int argc, char* argv[]) -> int
{
  // We need three input files (+ program name as first arg)
  if (argc < 4) {
    std::cerr << "At least 3 input files required. "
              << argc - 1
              << " given. Abort!"
              << std::endl;
    return -1;
  }

  // Check if files exist
  const std::size_t fileCount = 3;
  auto fileError = false;
  for(std::size_t i = 1; i < fileCount + 1; ++i) {
    const fs::path filepath(argv[i]);

    if(!fs::exists(filepath) || !fs::is_regular_file(filepath)) {
      std::cerr << "File "
                << filepath
                << " does not exist or is no file!"
                << std::endl;
      fileError = true;
    }
  }

  if(fileError) return -2;

  // parse optional arguments
  bool restrictY = false;
  double epsilon = 0.5;
  bool weightedMode = false;
  if(argc > 4){
    int pos = 4;
    while (pos < argc) {
      if(std::basic_string(argv[pos]) == "-w"){
        weightedMode = true;
        ++pos;
      } else if (std::basic_string(argv[pos]) == "-e"){
        if(pos+1 < argc){
          restrictY = true;
          epsilon = std::stod(std::basic_string(argv[pos+1]));
          pos += 2;
        } else {
          std::cerr << "Missing epsilon value for '-e' option!"
                    << std::endl;
          return -3;
        }
      }
    }
  }

  auto s = Scenario();
  s.parseDistanceMatrix(argv[1]);
  s.parseSpeciesGenes(argv[2]);
  s.parseSTreeSubtrees(argv[3]);

  std::size_t outgroupLimit = 10;
  auto bmgB = BMGBuilder(&s, outgroupLimit, restrictY,
                         epsilon=epsilon,
                         weightedMode=weightedMode);
  bmgB.buildBMG();
  bmgB.printBMG();

  return 0;
}
