// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENETIC_ALGORITHM_HEADER_SEEN___
#define GENETIC_ALGORITHM_HEADER_SEEN___

#include "gene/mutation.hpp"
#include "gene/combination.hpp"
#include "gene/individual.hpp"

namespace gene {

template<typename Individual, typename Genotype>
struct GeneticAlgorithm
{
  typename std::vector<std::unique_ptr<Individual>> Population;

  GeneticAlgorithm (IndividualFactory<Individual, Genotype>& factory,
                    MutationStrategy<Genotype>& mutationStrategy,
                    MutationRate<Individual>& mutationRate,
                    AttractionMeter<Individual>& attractionMeter,
                    CombinationStrategy<Genotype>& combinationStrategy);

  Population iterate(const Population& population);

  private:

    IndividualFactory<Individual, Genotype> factory_;
    MutationStrategy<Genotype>& mutationStrategy_;
    MutationRate<Individual>& mutationRate_;
    AttractionMeter<Individual>& attractionMeter_;
    CombinationStrategy<Genotype> combinationStrategy_;
};

}
#endif
