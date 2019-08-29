#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <regex>
#include <map>

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

  std::map<std::string, std::pair<bool*, double*>> param = {

      { "e",                {&restrictY, &epsilon} },
      { "-epsilon",         {&restrictY, &epsilon} },

      { "d",                {&disableQuartet, nullptr} },
      { "-disable-quartet", {&disableQuartet, nullptr} },

      { "w",                {&weightedMode, nullptr} },
      { "-weighted",        {&weightedMode, nullptr} },

      { "s",                {&subtreeFiles, nullptr} },
      { "-subtree-files",   {&subtreeFiles, nullptr} },

      { "a",                {&relativeOutgroups, &incongruentThreshold} },
      { "-all-outgroups",   {&relativeOutgroups, &incongruentThreshold} }
  };

  for(size_t i = 3; i < (size_t)argc; ++i) {

    std::regex rgx1("-([A-Za-z0-9-]*)=([0-9]*\\.?[0-9]*)");
    std::smatch matches1;
    std::string arg1(argv[i]);
    std::regex rgx2("-([A-Za-z0-9-]*)");
    std::smatch matches2;
    std::string arg2(argv[i]);

    if(std::regex_search(arg1, matches1, rgx1)) {
      if(param.find(matches1[1].str()) != param.end()){
        *param[matches1[1].str()].first = true;
        if(param[matches1[1].str()].second != nullptr){
          *param[matches1[1].str()].second = std::stod(matches1[2].str());
        }
      } else if(i > 3) {
        throw std::runtime_error("Illegal parameter: " + matches1[1].str());
      }
    } else if(std::regex_search(arg2, matches2, rgx2))  {
      if(param.find(matches2[1].str()) != param.end()){
        *param[matches2[1].str()].first = true;
      } else if(i > 3) {
        throw std::runtime_error("Illegal parameter: " + matches2[1].str());
      }
    } else if(i > 3) {
      throw std::runtime_error("Illegal parameter format");
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
  const size_t fileCount = (disableQuartet) ? 2 : 3;
  auto fileError = false;
  for(size_t i = 1; i < fileCount + 1; ++i) {
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

  size_t outgroupLimit = 10;
  auto bmgBuilder = BMGBuilder(&s, outgroupLimit, restrictY, epsilon,
                               weightedMode, disableQuartet, relativeOutgroups,
                               incongruentThreshold);

  bmgBuilder.buildBMG();
  bmgBuilder.printBMG();

  return 0;
}
