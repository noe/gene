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
 * Type representing a Individual
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
using Individual = std::pair<Phenotype, Genotype>;

/****************************************************************************
 * Type representing a population
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
using Population = std::vector<Individual<Phenotype, Genotype>>;

/****************************************************************************
 * Type representing the fitness of a population
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct Fitness
{
  std::map<const Individual<Phenotype, Genotype>*, float> individualToFitness;
  std::multimap<float, const Individual<Phenotype, Genotype>*> fitnessToIndividual;
};

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
  typedef std::vector<std::tuple<const Individual<Phenotype, Genotype>*,
                                 const Individual<Phenotype, Genotype>*,
                                 std::size_t>> Mating;

  virtual Mating mating(const Population<Phenotype, Genotype>&,
                        const Fitness<Phenotype, Genotype>&) = 0;
  virtual ~MatingStrategy() { }
};

/****************************************************************************
 * Fitness function.
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct FitnessFunction
{
  virtual Fitness<Phenotype, Genotype> calculate(const Population<Phenotype, Genotype>&) = 0;
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
  virtual std::map<const Individual<Phenotype, Genotype>*, float>
            mutationProbability(const Population<Phenotype, Genotype>&) = 0;
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

  std::map<const Individual<Phenotype, Genotype>*, float>
            mutationProbability(const Population<Phenotype, Genotype>& population) override
  {
    std::map<const Individual<Phenotype, Genotype>*, float> result;
    for (const auto& i : population)
    {
      result[&i] = mutationRate_;
    } 
    return result;
  }
};

/****************************************************************************
 * Interface abstracting the criteria for 
 ***************************************************************************/
template<typename Phenotype, typename Genotype>
struct SurvivalPolicy
{
  virtual Population<Phenotype, Genotype>
          selectSurvivors (Population<Phenotype, Genotype>&& ancestors,
                           Population<Phenotype, Genotype>&& offspring,
                           const Fitness<Phenotype, Genotype>& ancestorsFitness) = 0;
  virtual ~SurvivalPolicy() { }
};

}
#endif
