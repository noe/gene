// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef BITSTRING_GENOTIPE_HEADER_SEEN___
#define BITSTRING_GENOTIPE_HEADER_SEEN___

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/noncopyable.hpp>
#include "gene/policies.hpp"

namespace gene { namespace bitstring {

/******************************************************************************
 * PoD representing a chromosome.
 * Immutable sequence of encoded genes.
 *****************************************************************************/
struct Chromosome : boost::noncopyable
{
  const std::vector<uint8_t> encodedGenes;
};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotype : boost::noncopyable
{
  const std::vector<std::unique_ptr<Chromosome>> chromosomes;
};

/****************************************************************************
 * Implementation of mutation with a fixed probability.
 ***************************************************************************/
struct BitFlipMutation : public gene::MutationStrategy<Genotype>,
                         boost::noncopyable
{
  BitFlipMutation (float bitMutationProbability);

  std::unique_ptr<Genotype> mutate(const Genotype&);

  private: const float percentageOfMutation_;
};  

/****************************************************************************
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct OnePointCrossover : public CombinationStrategy<Genotype>,
                           boost::noncopyable
{
  OnePointCrossover ();
  std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&);
  private: const std::size_t numberOfPoints_;
};

}}

#include "gene/bitstring_impl.hpp"

#endif
