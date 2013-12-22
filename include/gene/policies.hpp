// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_POLICIES_HEADER_SEEN_
#define GENE_POLICIES_HEADER_SEEN_

#include <memory>
#include <vector>
#include <map>
#include <cassert>
#include <stdexcept>

namespace gene {

/****************************************************************************
 * Type representing a population
 ***************************************************************************/
template<typename Individual>
using Population = std::vector<std::shared_ptr<Individual>>;

/****************************************************************************
 * Interface abstracting the combination of two Individuals.
 ***************************************************************************/
template<typename Genotype>
struct CombinationStrategy
{
  virtual std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&) = 0;
  virtual ~CombinationStrategy() { }
};

/****************************************************************************
 * Strategy for evaluating the level of attraction between individuals.
 ***************************************************************************/
template<typename Individual>
struct AttractionMeter
{
  virtual float attractionBetween(const Individual&, const Individual&) = 0;

  virtual ~AttractionMeter() { };
};

/****************************************************************************
 * Fitness function.
 ***************************************************************************/
template<typename Individual>
struct FitnessFunction
{
  typedef std::map<Individual*, float> Fitness;

  virtual Fitness calculate(const Population<Individual>&) = 0;

  virtual ~FitnessFunction() { }
};

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotype.
 *****************************************************************************/
template<typename Individual, typename Genotype>
struct IndividualFactory
{
  virtual std::string description() const = 0;

  virtual std::unique_ptr<Individual> create(const Genotype&)
                                 throw(std::invalid_argument) = 0;

  virtual ~IndividualFactory() { }
};

/****************************************************************************
 * Interface abstracting a mutation in a genotipe.
 ***************************************************************************/
template<typename Genotype>
struct MutationStrategy
{
  virtual std::unique_ptr<Genotype> mutate(const Genotype&) = 0;

  virtual ~MutationStrategy() { }
};

/****************************************************************************
 * Interface abstracting the provider of mutation rate.
 ***************************************************************************/
template<typename Individual>
struct MutationRate
{
  virtual float mutationProbabilityFor(const Individual&) = 0;
  virtual ~MutationRate() { }
};

/****************************************************************************
 * Implementation of MutationRate that always returns a constant.
 ***************************************************************************/
template<typename Individual>
struct ConstantMutationRate : public MutationRate<Individual>
{
  const float mutationRate_;

  inline ConstantMutationRate(float mutationRate)
    : mutationRate_(mutationRate)
  {
    assert (mutationRate >= 0 && mutationRate <= 1);
  }

  inline float mutationProbabilityFor(const Individual&)
  {
    return mutationRate_;
  }
};

/****************************************************************************
 * Interface abstracting the criteria for 
 ***************************************************************************/
template<typename Individual>
struct SurvivalPolicy
{
  virtual Population<Individual> sift (const Population<Individual>& ancestors,
                                       const Population<Individual>& offspring) = 0;

  virtual ~SurvivalPolicy() { }
};

}
#endif
