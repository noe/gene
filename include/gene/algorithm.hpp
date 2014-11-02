// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENETIC_ALGORITHM_HEADER_SEEN___
#define GENETIC_ALGORITHM_HEADER_SEEN___

#include <random>
#include "gene/policies.hpp"

namespace gene {

/******************************************************************************
 *
 *****************************************************************************/
template<typename Phenotype, typename Genotype>
struct GeneticAlgorithm
{
  GeneticAlgorithm (Codec<Phenotype, Genotype>& codec,
                    FitnessFunction<Phenotype, Genotype>& fitnessFunction,
                    MutationStrategy<Phenotype, Genotype>& mutationStrategy,
                    MutationRate<Phenotype, Genotype>& mutationRate,
                    MatingStrategy<Phenotype, Genotype>& matingStrategy,
                    CombinationStrategy<Phenotype, Genotype>& combinationStrategy,
                    SurvivalPolicy<Phenotype, Genotype>& survivalPolicy);

  Population<Phenotype, Genotype> iterate(Population<Phenotype, Genotype>&& population);

  private:

    Codec<Phenotype, Genotype>& codec_;
    FitnessFunction<Phenotype, Genotype>& fitnessFunction_;
    MutationStrategy<Phenotype, Genotype>& mutationStrategy_;
    MutationRate<Phenotype, Genotype>& mutationRate_;
    MatingStrategy<Phenotype, Genotype>& matingStrategy_;
    CombinationStrategy<Phenotype, Genotype>& combinationStrategy_;
    SurvivalPolicy<Phenotype, Genotype>& survivalPolicy_;
};

}

#include "gene/algorithm_impl.hpp"
#endif
