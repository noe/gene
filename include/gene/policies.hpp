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
#include <functional>

namespace gene {

/****************************************************************************
 * Type representing a XXX
 ***************************************************************************/
template<typename Individual, typename Genotype>
using XXX = std::pair<Individual, Genotype>;

/****************************************************************************
 * Type representing a population
 ***************************************************************************/
template<typename Individual, typename Genotype>
using Population = std::vector<XXX<Individual, Genotype>>;

/****************************************************************************
 * Interface abstracting the combination of two Individuals.
 ***************************************************************************/
template<typename Genotype>
struct CombinationStrategy
{
  virtual Genotype combine(const Genotype&, const Genotype&) = 0;
  virtual ~CombinationStrategy() { }
};

/****************************************************************************
 * Strategy for defining the mating among individuals.
 ***************************************************************************/
template<typename Individual, typename Genotype>
struct MatingStrategy
{
  typedef std::vector<std::tuple<const XXX<Individual, Genotype>*, const XXX<Individual, Genotype>*, std::size_t>> Mating;

  virtual Mating mating(const Population<Individual, Genotype>&) = 0;
  virtual ~MatingStrategy() { }
};

/****************************************************************************
 * Fitness function.
 ***************************************************************************/
template<typename Individual, typename Genotype>
struct FitnessFunction
{
  typedef std::map<const Individual*, float> Fitness;

  virtual Fitness calculate(const Population<Individual, Genotype>&) = 0;
  virtual ~FitnessFunction() { }
};

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotype.
 *****************************************************************************/
template<typename Individual, typename Genotype>
struct IndividualCodec
{
  virtual Individual decode(const Genotype&) const throw(std::invalid_argument) = 0;
  virtual Genotype encode(const Individual&) const = 0;
  virtual ~IndividualCodec() { }
};

/****************************************************************************
 * Interface abstracting a mutation in a genotipe.
 ***************************************************************************/
template<typename Individual, typename Genotype>
struct MutationStrategy
{
  virtual XXX<Individual, Genotype> mutate(Individual,
                                           Genotype,
                                           const IndividualCodec<Individual, Genotype>&) = 0;
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

  ConstantMutationRate(float mutationRate) : mutationRate_(mutationRate)
  {
    assert (mutationRate >= 0 && mutationRate <= 1);
  }

  float mutationProbabilityFor(const Individual&) override
  {
    return mutationRate_;
  }
};

/****************************************************************************
 * Interface abstracting the criteria for 
 ***************************************************************************/
template<typename Individual, typename Genotype>
struct SurvivalPolicy
{
  virtual Population<Individual, Genotype>
          selectSurvivors (Population<Individual, Genotype>&& ancestors,
                           Population<Individual, Genotype>&& offspring) = 0;
  virtual ~SurvivalPolicy() { }
};

}
#endif
