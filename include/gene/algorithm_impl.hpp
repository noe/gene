// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#include "gene/algorithm.hpp"

namespace gene {

///////////////////////////////////////////////////////////////////////////////
template<typename Individual>
using Pairs = std::vector<std::pair<Individual*, Individual*>>;

///////////////////////////////////////////////////////////////////////////////
template<typename Individual, typename Genotype>
GeneticAlgorithm<Individual,Genotype>::GeneticAlgorithm (
         IndividualFactory<Individual, Genotype>& factory,
         FitnessFunction<Individual>& fitnessFunction,
         MutationStrategy<Genotype>& mutationStrategy,
         MutationRate<Individual>& mutationRate,
         MatingStrategy<Individual>& matingStrategy,
         CombinationStrategy<Genotype>& combinationStrategy,
         SurvivalPolicy<Individual>& survivalPolicy)
  : factory_(factory),
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
template<typename Individual>
void fillAttractionMatrix(float** attractionMatrix,
                          const Population<Individual>& population,
                          AttractionMeter<Individual>& attractionMeter)
{
  std::size_t populationSize = population.size();
  for (std::size_t i = 0; i < populationSize; ++i)
  {
    attractionMatrix[i][i] = 0;
    for (std::size_t j = 0; j < populationSize; ++j)
    {
      if (i == j) continue;
      Individual& individual1 = *population[i];
      Individual& individual2 = *population[j];

      attractionMatrix[i][j] = 
         attractionMeter.attractionBetween(individual1, individual2);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
template<typename Individual>
Pairs<Individual> pairing (float** attractionMatrix,
                           std::size_t populationSize)
{

}

///////////////////////////////////////////////////////////////////////////////
template<typename Individual, typename Genotype>
Population<Individual> GeneticAlgorithm<Individual,Genotype>::iterate(const Population<Individual>& population)
{
  typename FitnessFunction<Individual>::Fitness fitness = fitnessFunction_.calculate(population);
  std::size_t populationSize = population.size();
  //float attractionMatrix[populationSize][populationSize];
  //fillAttractionMatrix(attractionMatrix, population, attractionMeter_);
  //Pairs<Individual> pairs = pairing(attractionMatrix, populationSize);
  std::vector<std::tuple<Individual*, Individual*, std::size_t>> mating =
      matingStrategy_.mating(population);

  for(std::tuple<Individual*,Individual*, std::size_t> couple: mating)
  {
    // TODO    
  }
}

}
