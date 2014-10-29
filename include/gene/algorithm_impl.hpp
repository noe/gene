// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#include "gene/algorithm.hpp"

namespace gene {

///////////////////////////////////////////////////////////////////////////////
template<typename Individual, typename Genotype>
GeneticAlgorithm<Individual,Genotype>::GeneticAlgorithm (
         IndividualCodec<Individual, Genotype>& codec,
         FitnessFunction<Individual, Genotype>& fitnessFunction,
         MutationStrategy<Individual, Genotype>& mutationStrategy,
         MutationRate<Individual>& mutationRate,
         MatingStrategy<Individual, Genotype>& matingStrategy,
         CombinationStrategy<Genotype>& combinationStrategy,
         SurvivalPolicy<Individual, Genotype>& survivalPolicy)
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
template<typename Individual, typename Genotype>
Population<Individual, Genotype> GeneticAlgorithm<Individual,Genotype>::iterate(                                                                 Population<Individual, Genotype>&& population)
{
  typedef typename FitnessFunction<Individual, Genotype>::Fitness FitnessValue;
  FitnessValue fitness = fitnessFunction_.calculate(population);
  std::size_t populationSize = population.size();
  std::vector<std::tuple<Individual*, Individual*, std::size_t>> mating =
      matingStrategy_.mating(population);

  for(std::tuple<Individual*,Individual*, std::size_t> couple: mating)
  {
    Individual i1 = std::get<0>(couple);
    Individual i2 = std::get<1>(couple);
    std::size_t numOffspring = std::get<2>(couple);

    for (std::size_t k = 0; k < numOffspring; ++k)
    {
      std::unique_ptr<Genotype> offspring = combinationStrategy_.combine();
    }
  }
}

}
