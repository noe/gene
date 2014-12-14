// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_MATING_HEADER_SEEN_
#define GENE_MATING_HEADER_SEEN_

#include "gene/policies.hpp"
#include "gene/fitness.hpp"

namespace gene
{

template<typename Phenotype, typename Genotype>
struct FitnessProportionateMating : public MatingStrategy<Phenotype, Genotype>
{
  using typename MatingStrategy<Phenotype,Genotype>::Mating; 

  private: std::size_t offspringCount_;

  public:

  FitnessProportionateMating(std::size_t offspringCount)
    : offspringCount_(offspringCount) { }

  Mating mating(const Population<Phenotype, Genotype>& population,
                const PopulationFitness& fitness) override
  {
    Wheel wheel = computeWheel(fitness);
    std::mt19937 generator {std::random_device{}()};
    std::uniform_real_distribution<> distribution (0.0f, 0.999f);

    Mating result;

    for (std::size_t k = 0 ; k < offspringCount_; ++k)
    {
      PopulationIndex i1 = wheel.lower_bound(distribution(generator))->second;
      PopulationIndex i2 = wheel.lower_bound(distribution(generator))->second;
      result.emplace_back(std::make_tuple(i1, i2, 1));
    }
    return result; 
  }
};

template<typename Phenotype, typename Genotype>
struct RandomMating : public MatingStrategy<Phenotype, Genotype>
{
  using typename MatingStrategy<Phenotype,Genotype>::Mating; 

  private: std::size_t offspringCount_;

  public:

  RandomMating(std::size_t offspringCount)
    : offspringCount_(offspringCount) { }

  Mating mating(const Population<Phenotype, Genotype>& population,
                const PopulationFitness& fitness) override
  {
    std::mt19937 generator {std::random_device{}()};
    std::uniform_int_distribution<> distribution (0, population.size() - 1);

    Mating result;

    for (std::size_t k = 0 ; k < offspringCount_; ++k)
    {
      PopulationIndex i1 = distribution(generator);
      PopulationIndex i2 = distribution(generator);
      result.emplace_back(std::make_tuple(i1, i2, 1));
    }
    return result; 
  }
};

}
#endif
