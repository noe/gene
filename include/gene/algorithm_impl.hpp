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
         CombinationStrategy<Genotype>& combinationStrategy,
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
Population<Phenotype, Genotype> GeneticAlgorithm<Phenotype,Genotype>::iterate(Population<Phenotype, Genotype>&& population)
{
  typedef typename FitnessFunction<Phenotype, Genotype>::Fitness FitnessValue;
  typedef typename MatingStrategy<Phenotype, Genotype>::Mating Matingalue;
  typedef typename Matingalue::value_type MatingEntry;

  FitnessValue fitness = fitnessFunction_.calculate(population);
  Matingalue mating = std::move(matingStrategy_.mating(population));

  Population<Phenotype, Genotype> offspring;

  for(MatingEntry& entry : mating)
  {
    const Genotype& g1 = std::get<0>(entry)->second;
    const Genotype& g2 = std::get<1>(entry)->second;
    std::size_t numOffspring = std::get<2>(entry);

    for (std::size_t k = 0; k < numOffspring; ++k)
    {
      Genotype g {std::move(combinationStrategy_.combine(g1, g2))};
      Phenotype i {std::move(codec_.decode(g))};
      Individual<Phenotype, Genotype> child = make_pair(std::move(i), std::move(g));
      offspring.push_back(std::move(child));
    }
  }

  Population<Phenotype, Genotype> newPopulation =
     survivalPolicy_.selectSurvivors(std::move(population), std::move(offspring));

  return std::move(newPopulation);
}

}
