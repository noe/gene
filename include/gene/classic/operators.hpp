// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef OPERATORS_HEADER_SEEN_
#define OPERATORS_HEADER_SEEN_

#include <memory>
#include "individual.hpp"

namespace gene { namespace classic { namespace operators
{
  /****************************************************************************
   * Interface abstracting a mutation in an Individual.
   ***************************************************************************/
  struct Mutation
  {
    virtual std::unique_ptr<Genotipe> mutate(const Genotipe&) = 0;

    virtual ~Mutation() { }
  };

  /****************************************************************************
   * Interface abstracting the combination of two Individuals.
   ***************************************************************************/
  struct Combination
  {
    virtual std::unique_ptr<Genotipe> combine(const Genotipe&, const Genotipe&) = 0;

    virtual ~Combination() { }
  };

  /****************************************************************************
   * Implementation of mutation with a fixed probability.
   ***************************************************************************/
  struct BitFlipMutation : public Mutation, boost::noncopyable
  {
    BitFlipMutation (float percentageOfMutation);

    std::unique_ptr<Genotipe> mutate(const Genotipe&);

    private: const float percentageOfMutation_;
  };  

  /****************************************************************************
   * Implementation of Combination that performs N point crossover.
   ***************************************************************************/
  struct NPointCrossover : public Combination, boost::noncopyable
  {
    NPointCrossover (std::size_t numberOfPoints);

    std::unique_ptr<Genotipe> combine(const Genotipe&, const Genotipe&);

    private: const std::size_t numberOfPoints_;
  };
}}}
#endif
