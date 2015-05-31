// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)
#ifndef GENE_COMBINATION_HEADER_SEEN__
#define GENE_COMBINATION_HEADER_SEEN__

namespace gene
{

///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
struct CombinationMix : public CombinationStrategy<Phenotype, Genotype>
{
  using CombinationPtr = CombinationStrategy<Phenotype, Genotype>*;

  std::map<float, CombinationPtr> combinations_;
  std::mt19937 generator_;
  std::uniform_real_distribution<float> distribution_;

  CombinationMix(std::multimap<float, CombinationPtr>&& combinations)
    : distribution_(0.0, 1.0)
  {
    float accumulatedProb = 0.0;
    for (auto& entry : combinations)
    {
      accumulatedProb += entry.first;
      if (accumulatedProb>1.0) throw std::exception();
      if (accumulatedProb>.99) accumulatedProb = 1.0;
      combinations_[accumulatedProb] = entry.second;
    }
  }

  Individual<Phenotype, Genotype> combine(const Individual<Phenotype, Genotype>& i1,
                                          const Individual<Phenotype, Genotype>& i2,
                                          const Codec<Phenotype, Genotype>& codec) override
  {
    float p = distribution_(generator_);
    auto it = combinations_.lower_bound(p);
    if (it == combinations_.end()){
      it = combinations_.begin();
    }
    CombinationPtr& combination = it->second;
    return std::move(combination->combine(i1, i2, codec));
  }
};

///////////////////////////////////////////////////////////////////////////////
template<typename Phenotype, typename Genotype>
struct LocalSearchCombination : public CombinationStrategy<Phenotype, Genotype>
{
  using Combination = CombinationStrategy<Phenotype, Genotype>;
  using CombinationPtr = std::unique_ptr<Combination>;
  using Fitness = FitnessFunction<Phenotype, Genotype>;
  using FitnessPtr = std::unique_ptr<Fitness>;

  const CombinationPtr combination_;
  const FitnessPtr fitness_;
  const std::size_t numChildren_;

  LocalSearchCombination(Combination* combination,
                         Fitness* fitness,
                         std::size_t numChildren)
     : combination_(combination), fitness_(fitness), numChildren_(numChildren) { }

  Individual<Phenotype, Genotype> combine(const Individual<Phenotype, Genotype>& i1,
                                          const Individual<Phenotype, Genotype>& i2,
                                          const Codec<Phenotype, Genotype>& codec) override
  {
    Population<Phenotype, Genotype> children;
    children.reserve(numChildren_);

    for (std::size_t k = 0; k < numChildren_; ++k)
    {
      children.emplace_back(combination_->combine(i1, i2, codec));
    }

    PopulationFitness f = fitness_->calculate(children);
    auto maxFitnessIt = std::max_element(f.begin(), f.end());
    Individual<Phenotype, Genotype>& best = children[maxFitnessIt - f.begin()];
    return std::move(best);
  }
};

}
#endif
