// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#include <map>
#include <utility>
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
  std::mt19937 generator {std::random_device{}()};

  // Calculate fitness of the whole population
  PopulationFitness fitness = fitnessFunction_.calculate(p);

  // Apply selection policy
  Survivors survivors { survivalPolicy_.selectSurvivors(p, fitness) };

  // filter fitness for dropped individuals
  Population<Phenotype, Genotype> population = survivalPolicy_.select(move(p), survivors);
  fitness = survivalPolicy_.select(move(fitness), survivors);
  size_t populationSize = population.size();

  // Select elite for later
  std::multimap<FitnessType, PopulationIndex> fitnessMap;
  for (PopulationIndex k = 0; k < populationSize; ++k) fitnessMap.insert(std::make_pair(fitness[k], k));
  float elitePercentage_ = 0.50;
  std::size_t eliteCount = populationSize * elitePercentage_;
  auto it = fitnessMap.rbegin();
  auto end = fitnessMap.rend();
  Population<Phenotype, Genotype> elite;
  for (size_t k = 0; it != end && k < eliteCount; ++it, ++k)
  {
    PopulationIndex index = it->second;
    const Individual<Phenotype, Genotype>& i = population[index];
    elite.push_back(i);
  }

  // Determine the mating among individuals of the population
  auto mating = matingStrategy_.mating(population, fitness);

  // Combine each of the pairs specified in the calculated mating
  Population<Phenotype, Genotype> offspring;
  for (const auto& entry : mating)
  {
    PopulationIndex index1 = std::get<0>(entry);
    PopulationIndex index2 = std::get<1>(entry);
    NumberOfChildren numOffspring = std::get<2>(entry);
  
    const Individual<Phenotype, Genotype>& i1 = population[index1];
    const Individual<Phenotype, Genotype>& i2 = population[index2];

    for (std::size_t k = 0; k < numOffspring; ++k)
    {
      offspring.push_back(combinationStrategy_.combine(i1, i2, codec_));
    }
  }

  // Mutate offspring
  PopulationMutationRates rates = mutationRate_.mutationProbability(offspring);
  std::size_t offspringSize = offspring.size();
  for (std::size_t k = 0; k < offspringSize; ++k)
  {
    std::bernoulli_distribution mutation(rates[k]);
    if (mutation(generator))
    {
      offspring[k] = mutationStrategy_.mutate(offspring[k], codec_);
    }
  }

  // Use offspring as base for the new population...
  Population<Phenotype, Genotype> newPopulation (std::move(offspring));

  // ...but keep the best from the previous generation (i.e. elitism)
  newPopulation.insert(newPopulation.end(),
                       std::make_move_iterator(elite.begin()),
                       std::make_move_iterator(elite.end()));


  return newPopulation;
}

}
