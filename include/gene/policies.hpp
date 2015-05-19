// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_POLICIES_HEADER_SEEN_
#define GENE_POLICIES_HEADER_SEEN_

#include <memory>
#include <set>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <algorithm>

namespace gene {

/****************************************************************************
 * Type representing a Individual
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
using Individual = std::pair<Phenotype, Genotype>;

/****************************************************************************
 * Type representing a population
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
using Population = std::vector<Individual<Phenotype, Genotype>>;

using PopulationIndex = std::size_t;
using FitnessType = float;
using PopulationFitness = std::vector<FitnessType>;
using Survivors = std::set<PopulationIndex>;
using PopulationMutationRates = std::vector<float>;
using NumberOfChildren = std::size_t;

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotype.
 *****************************************************************************/
template<typename Phenotype, typename Genotype>
struct Codec
{
  virtual Phenotype decode(const Genotype&) const throw(std::invalid_argument) = 0;
  virtual Genotype encode(const Phenotype&) const = 0;
  virtual ~Codec() { }
};

/****************************************************************************
 * Interface abstracting the combination of two Individuals.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct CombinationStrategy
{
  virtual Individual<Phenotype, Genotype>
                   combine(const Individual<Phenotype, Genotype>&,
                           const Individual<Phenotype, Genotype>&,
                           const Codec<Phenotype, Genotype>&) = 0;
  virtual ~CombinationStrategy() { }
};

/****************************************************************************
 * Strategy for defining the mating among individuals.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct MatingStrategy
{
  typedef std::vector<std::tuple<PopulationIndex,
                                 PopulationIndex,
                                 NumberOfChildren>> Mating;

  virtual Mating mating(const Population<Phenotype, Genotype>&,
                        const PopulationFitness&) = 0;
  virtual ~MatingStrategy() { }
};

/****************************************************************************
 * Fitness function.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct FitnessFunction
{
  virtual PopulationFitness calculate(const Population<Phenotype, Genotype>&) = 0;
  virtual ~FitnessFunction() { }
};

/****************************************************************************
 * Interface abstracting a mutation in a genotipe.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct MutationStrategy
{
  virtual Individual<Phenotype, Genotype> mutate(Individual<Phenotype, Genotype>,
                                                 const Codec<Phenotype, Genotype>&) = 0;
  virtual ~MutationStrategy() { }
};

/****************************************************************************
 * Interface abstracting the provider of mutation rate.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct MutationRate
{
  virtual PopulationMutationRates mutationProbability(
                                 const Population<Phenotype, Genotype>&) = 0;
  virtual ~MutationRate() { }
};

/****************************************************************************
 * Implementation of MutationRate that always returns a constant.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct ConstantMutationRate : public MutationRate<Phenotype, Genotype>
{
  const float mutationRate_;

  ConstantMutationRate(float mutationRate) : mutationRate_(mutationRate)
  {
    assert (mutationRate >= 0 && mutationRate <= 1);
  }

  PopulationMutationRates mutationProbability(
                   const Population<Phenotype, Genotype>& population) override
  {
    return PopulationMutationRates(population.size(), mutationRate_);
  }
};

/****************************************************************************
 * Interface abstracting the criteria for 
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct SurvivalPolicy
{
  virtual Survivors selectSurvivors (const Population<Phenotype, Genotype>&,
                                     const PopulationFitness&) = 0;

  Population<Phenotype, Genotype> select(Population<Phenotype, Genotype>&& p,
                                         const Survivors& s)
  {
    Population<Phenotype, Genotype> result; result.reserve(s.size());
    for (std::size_t k : s)
    {
      result.emplace_back(std::move(p[k]));
    }
    return std::move(result);
  }

  PopulationFitness select(PopulationFitness && f,
                           const Survivors& s)
  {
    PopulationFitness result; result.reserve(s.size());
    for (std::size_t k : s) result.emplace_back(f[k]);
    return std::move(result);
  }

  virtual ~SurvivalPolicy() { }
};

}
#endif
