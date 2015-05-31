// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_MUTATION_HEADER_SEEN_
#define GENE_MUTATION_HEADER_SEEN_

#include "gene/policies.hpp"
#include "gene/fitness.hpp"

#include <map>
#include <memory>
#include <random>
#include <stdexcept>

namespace gene
{
///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
struct MutationMix : public MutationStrategy<Phenotype, Genotype>
{
  using MutationPtr = MutationStrategy<Phenotype, Genotype>*;

  std::map<float, MutationPtr> mutations_;
  std::mt19937 generator_;
  std::uniform_real_distribution<float> distribution_;

  MutationMix(std::multimap<float, MutationPtr>&& mutations)
    : distribution_(0.0, 1.0)
  {
    float accumulatedProb = 0.0;
    for (auto& entry : mutations)
    {
      accumulatedProb += entry.first;
      if (accumulatedProb>1.0) throw std::exception();
      if (accumulatedProb>.99) accumulatedProb = 1.0;
      mutations_[accumulatedProb] = entry.second;
    }
  }

  Individual<Phenotype, Genotype> mutate(Individual<Phenotype, Genotype> i,
                                         const Codec<Phenotype, Genotype>& codec) override
  {
    float p = distribution_(generator_);
    auto it = mutations_.lower_bound(p);
    if (it == mutations_.end()){
      it = mutations_.begin();
    }
    MutationPtr& mutation = it->second;
    return std::move(mutation->mutate(std::move(i), codec));
  }
};

///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
struct LocalSearchMutation : public MutationStrategy<Phenotype, Genotype>
{
  using Mutation = MutationStrategy<Phenotype, Genotype>;
  using MutationPtr = std::unique_ptr<Mutation>;
  using Fitness = FitnessFunction<Phenotype, Genotype>;
  using FitnessPtr = std::unique_ptr<Fitness>;

  const MutationPtr mutation_;
  const FitnessPtr fitness_;
  const std::size_t numChildren_;

  LocalSearchMutation(Mutation* mutation,
              Fitness* fitness,
              std::size_t numChildren)
 
    : mutation_(mutation), fitness_(fitness), numChildren_(numChildren) { }

  Individual<Phenotype, Genotype> mutate(Individual<Phenotype, Genotype> i,
                                         const Codec<Phenotype, Genotype>& codec) override
  {
    Population<Phenotype, Genotype> children;
    children.reserve(numChildren_);

    for (std::size_t k = 0; k < numChildren_; ++k)
    {
      children.emplace_back(mutation_->mutate(i, codec));
    }

    PopulationFitness f = fitness_->calculate(children);
    auto maxFitnessIt = std::max_element(f.begin(), f.end());
    Individual<Phenotype, Genotype>& best = children[maxFitnessIt - f.begin()];
    return std::move(best);
  }
};

}
#endif
