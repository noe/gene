// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_TOURNAMENT_HEADER_SEEN__
#define GENE_TOURNAMENT_HEADER_SEEN__

#include <memory>

#include "gene/classic/individual.hpp"

namespace gene { namespace classic {

struct EconomyStrategy
{
  typedef float Money;
  virtual Money rewardForVictory() = 0;
  virtual Money priceForSpawningChild() = 0;
  virtual Money priceForMaintainingSelf() = 0;
  virtual Money priceForMaintainingChild() = 0;
  virtual ~EconomyStrategy() { }
};

struct ReproductionStrategy
{
  virtual float mutationProbability
};

template<typename Individual>
struct Tournament
{
  Tournament (std::unique_ptr<operators::Mutation> mutation,
              std::unique_ptr<operators::Combination> combination,
              std::unique_ptr<>);

  
};

}}
#endif
