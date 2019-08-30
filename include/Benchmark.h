#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iosfwd>

class Benchmark {
public:
  Benchmark(bool disableQuartet, bool restrictY, bool weightedMode, bool subtreeFiles, bool relativeOutgroups,
            double epsilon, double incongruentThreshold)
   : m_disableQuartet(disableQuartet)
   , m_restrictY(restrictY)
   , m_weightedMode(weightedMode)
   , m_subtreeFiles(subtreeFiles)
   , m_relativeOutgroups(relativeOutgroups)
   , m_epsilon(epsilon)
   , m_incongruentThreshold(incongruentThreshold)
   , m_initTime(std::chrono::high_resolution_clock::now())
   , m_readFilesStart()
   , m_sortDistancesStart()
   , m_epsilonStart()
   , m_outgroupInitStart()
   , m_buildBMGStart()
   , m_chooseOutrgroupsStart()
   , m_readFilesDuration()
   , m_sortDistancesDuration()
   , m_epsilonDuration()
   , m_outgroupInitDuration()
   , m_buildBMGDuration()
   , m_chooseOutrgroupsDuration() {
     // initialize start times to now, duration to 0 (this is a bit dirty)
     m_readFilesStart = std::chrono::high_resolution_clock::now();
     m_sortDistancesStart = std::chrono::high_resolution_clock::now();
     m_epsilonStart = std::chrono::high_resolution_clock::now();
     m_outgroupInitStart = std::chrono::high_resolution_clock::now();
     m_buildBMGStart = std::chrono::high_resolution_clock::now();
     m_chooseOutrgroupsStart = std::chrono::high_resolution_clock::now();
     m_readFilesDuration = m_readFilesStart - m_readFilesStart;
     m_sortDistancesDuration = m_readFilesStart - m_readFilesStart;
     m_epsilonDuration = m_readFilesStart - m_readFilesStart;
     m_outgroupInitDuration = m_readFilesStart - m_readFilesStart;
     m_buildBMGDuration = m_readFilesStart - m_readFilesStart;
     m_chooseOutrgroupsDuration = m_readFilesStart - m_readFilesStart;
     m_totalDuration = m_readFilesStart - m_readFilesStart;
    };

  void startReadFiles() { m_readFilesStart = std::chrono::high_resolution_clock::now(); };
  void endReadFiles() { m_readFilesDuration = std::chrono::high_resolution_clock::now() - m_readFilesStart; };

  void startSortDistances() { m_sortDistancesStart = std::chrono::high_resolution_clock::now(); };
  void endSortDistances() { m_sortDistancesDuration = std::chrono::high_resolution_clock::now() - m_sortDistancesStart; };

  void startEpsilon() { m_epsilonStart = std::chrono::high_resolution_clock::now(); };
  void endEpsilon() { m_epsilonDuration = std::chrono::high_resolution_clock::now() - m_epsilonStart; };

  void startOutgroupInit() { m_outgroupInitStart = std::chrono::high_resolution_clock::now(); };
  void endOutgroupInit() { m_outgroupInitDuration = std::chrono::high_resolution_clock::now() - m_outgroupInitStart; };

  void startBuildBMG() { m_buildBMGStart = std::chrono::high_resolution_clock::now(); };
  void endBuildBMG() { m_buildBMGDuration = std::chrono::high_resolution_clock::now() - m_buildBMGStart; };

  void startChooseOutgroups() { m_chooseOutrgroupsStart = std::chrono::high_resolution_clock::now(); };
  void endChooseOutgroups() { m_chooseOutrgroupsDuration += std::chrono::high_resolution_clock::now() - m_chooseOutrgroupsStart; };

  void endTotal() { m_totalDuration += std::chrono::high_resolution_clock::now() - m_initTime; };

  void flush(std::basic_ofstream<char>& stream);

private:
  bool m_disableQuartet, m_restrictY, m_weightedMode, m_subtreeFiles, m_relativeOutgroups;
  double m_epsilon, m_incongruentThreshold;

  std::chrono::time_point<std::chrono::high_resolution_clock> m_initTime;

  std::chrono::time_point<std::chrono::high_resolution_clock> m_readFilesStart,
                                                              m_sortDistancesStart,
                                                              m_epsilonStart,
                                                              m_outgroupInitStart,
                                                              m_buildBMGStart,
                                                              m_chooseOutrgroupsStart;

  std::chrono::duration<double> m_readFilesDuration,
                                m_sortDistancesDuration,
                                m_epsilonDuration,
                                m_outgroupInitDuration,
                                m_buildBMGDuration,
                                m_chooseOutrgroupsDuration,
                                m_totalDuration;
};

#endif /* BENCHMARK_H */
