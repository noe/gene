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
         MutationRate<Phenotype, Genotype>& mutationRate,
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
GeneticAlgorithm<Phenotype,Genotype>::iterate(Population<Phenotype, Genotype>&& p)
{
  Population<Phenotype, Genotype>&& population {std::move(p)};

  std::mt19937 generator {std::random_device{}()};

  // Mutate individuals
  auto mutationRate = mutationRate_.mutationProbability(population);
  for (auto& i : population)
  {
    std::bernoulli_distribution mutation(mutationRate[&i]);
    if (mutation(generator))
    {
      i = mutationStrategy_.mutate(i, codec_);
    }
  }

  // Calculate fitness of the whole population
  Fitness<Phenotype, Genotype> fitness = fitnessFunction_.calculate(population);

  // Determine the mating among individuals of the population
  auto mating = move(matingStrategy_.mating(population, fitness));

  // Combine each of the pairs specified in the calculated mating
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

  // select survivors
  return move(survivalPolicy_.selectSurvivors(move(population),
                                              move(offspring),
                                              fitness));
}

}
