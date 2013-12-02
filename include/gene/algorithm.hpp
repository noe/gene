// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENETIC_ALGORITHM_HEADER_SEEN___
#define GENETIC_ALGORITHM_HEADER_SEEN___

#include "gene/policies.hpp"

namespace gene {

/******************************************************************************
 *
 *****************************************************************************/
template<typename Individual, typename Genotype>
struct GeneticAlgorithm
{
  GeneticAlgorithm (IndividualFactory<Individual, Genotype>& factory,
                    FitnessFunction<Individual>& fitnessFunction,
                    MutationStrategy<Genotype>& mutationStrategy,
                    MutationRate<Individual>& mutationRate,
                    AttractionMeter<Individual>& attractionMeter,
                    CombinationStrategy<Genotype>& combinationStrategy,
                    SurvivalPolicy<Individual>& survivalPolicy);

  Population<Individual> iterate(const Population<Individual>& population);

  private:

    IndividualFactory<Individual, Genotype>& factory_;
    FitnessFunction<Individual>& fitnessFunction_;
    MutationStrategy<Genotype>& mutationStrategy_;
    MutationRate<Individual>& mutationRate_;
    AttractionMeter<Individual>& attractionMeter_;
    CombinationStrategy<Genotype>& combinationStrategy_;
    SurvivalPolicy<Individual>& survivalPolicy_;
};

}

#include "gene/algorithm_impl.hpp"
#endif
