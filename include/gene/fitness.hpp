// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)
#ifndef GENE_FITNESS_HEADER_SEEN_
#define GENE_FITNESS_HEADER_SEEN_

#include <limits>

namespace gene
{
  PopulationFitness normalize(const PopulationFitness& f)
  {
    float minFitness = std::numeric_limits<float>::min();
    float totalFitness = 0;
    std::size_t numEntries = 0;

    for (FitnessType value: f)
    {
      minFitness = std::min(value, minFitness);
      totalFitness += value; 
      ++numEntries;
    }

    float totalAfterShift = totalFitness - (minFitness * numEntries);

    std::size_t populationSize = f.size();
    PopulationFitness normalized; normalized.reserve(f.size());

    for (std::size_t k = 0; k < populationSize; ++k)
    {
      FitnessType oldValue = f[k];
      FitnessType newValue = (oldValue - minFitness) / totalAfterShift;
      normalized[k] = newValue;
    }

    return normalized;  
  }
  
  using Wheel = std::multimap<float, PopulationIndex>;

  Wheel computeWheel (const PopulationFitness& f)
  {
    PopulationFitness normalized = normalize(f);
    
    std::multimap<FitnessType, PopulationIndex> fitnessMap;
    for (PopulationIndex k = 0; k < f.size(); ++k) fitnessMap.insert(std::make_pair(normalized[k], k));

    float accumulated = 0;
    Wheel result;

    for (const auto& entry : fitnessMap)
    {
      accumulated += entry.first;
      result.insert(std::make_pair(accumulated, entry.second));
    }
    return result;
  } 
}
#endif
