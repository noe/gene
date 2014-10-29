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
  GeneticAlgorithm (IndividualCodec<Individual, Genotype>& codec,
                    FitnessFunction<Individual, Genotype>& fitnessFunction,
                    MutationStrategy<Individual, Genotype>& mutationStrategy,
                    MutationRate<Individual>& mutationRate,
                    MatingStrategy<Individual, Genotype>& matingStrategy,
                    CombinationStrategy<Genotype>& combinationStrategy,
                    SurvivalPolicy<Individual, Genotype>& survivalPolicy);

  Population<Individual, Genotype> iterate(Population<Individual, Genotype>&& population);

  private:

    IndividualCodec<Individual, Genotype>& codec_;
    FitnessFunction<Individual, Genotype>& fitnessFunction_;
    MutationStrategy<Individual, Genotype>& mutationStrategy_;
    MutationRate<Individual>& mutationRate_;
    MatingStrategy<Individual, Genotype>& matingStrategy_;
    CombinationStrategy<Genotype>& combinationStrategy_;
    SurvivalPolicy<Individual, Genotype>& survivalPolicy_;
};

}

#include "gene/algorithm_impl.hpp"
#endif
