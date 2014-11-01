// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#include "gene/algorithm.hpp"

namespace gene {

///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
GeneticAlgorithm<Phenotype,Genotype>::GeneticAlgorithm (
         Codec<Phenotype, Genotype>& codec,
         FitnessFunction<Phenotype, Genotype>& fitnessFunction,
         MutationStrategy<Phenotype, Genotype>& mutationStrategy,
         MutationRate<Phenotype>& mutationRate,
         MatingStrategy<Phenotype, Genotype>& matingStrategy,
         CombinationStrategy<Phenotype, Genotype>& combinationStrategy,
         SurvivalPolicy<Phenotype, Genotype>& survivalPolicy)
  : codec_(codec),
    fitnessFunction_(fitnessFunction),
    mutationStrategy_(mutationStrategy),
    mutationRate_(mutationRate),
    matingStrategy_(matingStrategy),
    combinationStrategy_(combinationStrategy),
    survivalPolicy_(survivalPolicy)
{
  // do nothing
}

///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
Population<Phenotype, Genotype>
GeneticAlgorithm<Phenotype,Genotype>::iterate(Population<Phenotype, Genotype>&& population)
{
  Fitness<Phenotype> fitness = fitnessFunction_.calculate(population);
  auto mating = move(matingStrategy_.mating(population, fitness));

  Population<Phenotype, Genotype> offspring;

  for (auto& entry : mating)
  {
    const Individual<Phenotype, Genotype>& i1 = *std::get<0>(entry);
    const Individual<Phenotype, Genotype>& i2 = *std::get<1>(entry);
    std::size_t numOffspring = std::get<2>(entry);

    for (std::size_t k = 0; k < numOffspring; ++k)
    {
      offspring.push_back(move(combinationStrategy_.combine(i1, i2, codec_)));
    }
  }

  return move(survivalPolicy_.selectSurvivors(move(population),
                                              move(offspring),
                                              fitness));
}

}
