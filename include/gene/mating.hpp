// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_MATING_HEADER_SEEN_
#define GENE_MATING_HEADER_SEEN_

#include "gene/policies.hpp"
#include "gene/fitness.hpp"

namespace gene
{

template<typename Phenotype, typename Genotype>
struct FitnessProportionateMating : public MatingStrategy<Phenotype, Genotype>
{
  using typename MatingStrategy<Phenotype,Genotype>::Mating; 

  Mating mating(const Population<Phenotype, Genotype>& population,
                const PopulationFitness& fitness) override
  {
    Wheel wheel = computeWheel(fitness);
    std::mt19937 generator {std::random_device{}()};
    std::uniform_real_distribution<> distribution (0.0f, 1.0f);
    std::size_t offspringCount = population.size() / 2;

    Mating result;

    for (std::size_t k = 0 ; k < offspringCount; ++k)
    {
      PopulationIndex i1 = wheel.lower_bound(distribution(generator))->second;
      PopulationIndex i2 = wheel.lower_bound(distribution(generator))->second;
      result.emplace_back(std::make_tuple(i1, i2, 1));
    }
    return result; 
  }
};


}
#endif
