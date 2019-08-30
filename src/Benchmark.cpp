#include <fstream>

#include <Benchmark.h>

void
Benchmark::flush(std::basic_ofstream<char>& stream) {
  if(stream.is_open()) {
    stream << std::chrono::duration_cast<std::chrono::nanoseconds>(m_initTime.time_since_epoch()).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_readFilesDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_sortDistancesDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_epsilonDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_outgroupInitDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_buildBMGDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_chooseOutrgroupsDuration).count() << ","
           << std::chrono::duration_cast<std::chrono::nanoseconds>(m_totalDuration).count() << ","
           << (m_disableQuartet ? 1 : 0) << ","
           << (m_restrictY ? 1 : 0) << ","
           << (m_weightedMode ? 1 : 0) << ","
           << (m_subtreeFiles ? 1 : 0) << ","
           << (m_relativeOutgroups ? 1 : 0) << ","
           << m_epsilon << ","
           << m_incongruentThreshold
           << "\n";
  }
}
