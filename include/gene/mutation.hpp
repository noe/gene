// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef MUTATION_HEADER_SEEN__
#define MUTATION_HEADER_SEEN__

#include <memory>
#include <boost/noncopyable.hpp>

namespace gene {

/****************************************************************************
 * Interface abstracting a mutation in a genotipe.
 ***************************************************************************/
template<typename Genotype, typename Context>
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

}

#endif
