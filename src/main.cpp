#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>

#include "Scenario.h"
#include "BMGBuilder.h"

namespace fs = std::filesystem;

auto
main(int argc, char* argv[]) -> int
{
  // We need three input files (+ program name as first arg)
  if (argc < 4) {
    std::cerr << "At least 3 arguments required. "
              << argc - 1
              << " given. Abort!"
              << std::endl;
    return -1;
  }

  // parse optional arguments
  bool disableQuartet = false;
  bool restrictY = false;
  double epsilon = 0.5;
  bool weightedMode = false;
  bool subtreeFiles = false;
  bool relativeOutgroups = false;
  double incongruentThreshold = 0.2;

  int pos = 3;
  while (pos < argc) {

    if((std::string(argv[pos]) == "-d") ||
       (std::string(argv[pos]) == "--disable-quartet")){
      disableQuartet = true;
      ++pos;
    } else if((std::string(argv[pos]) == "-w") ||
       (std::string(argv[pos]) == "--weighted")){
      weightedMode = true;
      ++pos;

    // use subtree files instead of newick species tree
    } else if((std::string(argv[pos]) == "-s") ||
              (std::string(argv[pos]) == "--subtree-files")){
      subtreeFiles = true;
      ++pos;

    // use all (also relative) outgroups in the species tree
    } else if((std::string(argv[pos]) == "-a") ||
              (std::string(argv[pos]) == "--all-outgroups")){
      relativeOutgroups = true;
      ++pos;
      if(pos < argc){
        try {
          incongruentThreshold = std::stod(std::string(argv[pos]));
          ++pos;
        } catch (const std::invalid_argument&) {
          // restore default value
          incongruentThreshold = 0.2;
        }
      }

    } else if((std::string(argv[pos]) == "-e") ||
              (std::string(argv[pos]) == "--epsilon")){
      if(pos+1 < argc){
        restrictY = true;
        epsilon = std::stod(std::string(argv[pos+1]));
        pos += 2;
      } else {
        std::cerr << "Missing epsilon value for '-e' option!"
                  << std::endl;
        return -3;
      }

    } else {
      ++pos;
    }
  }

  // some logic checks
  if(subtreeFiles && relativeOutgroups){
    std::cerr << "All outgroups option is not available in combination "
              << "with subtree files! Use a Newick-tree file instead."
              << std::endl;
    return -4;
  }
  if(incongruentThreshold < 0.0){ incongruentThreshold = 0.0; }
  else if(incongruentThreshold > 1.0){ incongruentThreshold = 1.0; }

  // Check if files exist
  const std::size_t fileCount = (disableQuartet) ? 2 : 3;
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

  auto s = Scenario();
  s.parseDistanceMatrix(argv[1]);
  s.parseSpeciesGenes(argv[2]);
  if(!disableQuartet){
    s.parseSTreeSubtrees(argv[3], subtreeFiles);
  }

  std::size_t outgroupLimit = 10;
  auto bmgBuilder = BMGBuilder(&s, outgroupLimit, restrictY, epsilon,
                               weightedMode, disableQuartet, relativeOutgroups,
                               incongruentThreshold);

  bmgBuilder.buildBMG();
  bmgBuilder.printBMG();

  return 0;
}
