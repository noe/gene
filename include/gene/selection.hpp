// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_SELECTION_HEADER_SEEN_
#define GENE_SELECTION_HEADER_SEEN_

#include "gene/fitness.hpp"
#include <random>

namespace gene
{

template<typename Phenotype, typename Genotype>
struct FitnessProportionateSelection : public SurvivalPolicy<Phenotype, Genotype>
{
  private: const std::size_t size_;

  public:
  
  FitnessProportionateSelection(std::size_t size) : size_(size) { }

  Survivors selectSurvivors (const Population<Phenotype, Genotype>& population,
                             const PopulationFitness& fitness) override
  {
    Wheel wheel = computeWheel(fitness);
    std::mt19937 generator {std::random_device{}()};
    std::uniform_real_distribution<> distribution (0.0f, 1.0f);

    Survivors result;

    for (std::size_t k = 0; k < size_; ++k)
    {
      float accumulatedFitness = distribution(generator);
      auto it = wheel.lower_bound(accumulatedFitness);
      result.insert(it->second);
    }
    return result;
  }
};

/*
template<typename Phenotype, typename Genotype>
struct StochasticUniversalSampling : public SurvivalPolicy<Phenotype, Genotype>
{
  {
    //TODO
  }
};

template<typename Phenotype, typename Genotype>
struct TournamentSelection : public SurvivalPolicy<Phenotype, Genotype>
{
  {
    //TODO
  }
};
*/
}
#endif
