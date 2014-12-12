// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef GENE_SELECTION_HEADER_SEEN_
#define GENE_SELECTION_HEADER_SEEN_

#include "gene/fitness.hpp"
#include <random>

namespace gene
{

template<typename Phenotype, typename Genotype>
Survivors internalWheelSelection (const Population<Phenotype, Genotype>& population,
                                  const PopulationFitness& fitness,
                                  std::multiset<float> points)
{
  Wheel wheel = computeWheel(fitness);

  Survivors result;

  for (float point : points)
  {
    auto it = wheel.lower_bound(point);
    result.insert(it->second);
  }
  return result;
}

template<typename Phenotype, typename Genotype>
struct FitnessProportionateSelection : public SurvivalPolicy<Phenotype, Genotype>
{
  private: const std::size_t size_;

  public:
  
  FitnessProportionateSelection(std::size_t size) : size_(size) { }

  Survivors selectSurvivors (const Population<Phenotype, Genotype>& population,
                             const PopulationFitness& fitness) override
  {
    Wheel wheel = computeWheel(fitness);
    std::mt19937 generator {std::random_device{}()};
    std::uniform_real_distribution<> distribution (0.0f, 1.0f);
    std::multiset<float> points;
    for (std::size_t k = 0; k < size_; ++k)
    {
      points.insert(distribution(generator));
    }
    return internalWheelSelection(population, fitness, points);
  }
};


template<typename Phenotype, typename Genotype>
struct StochasticUniversalSampling : public SurvivalPolicy<Phenotype, Genotype>
{
  private: const std::size_t size_;

  public:
  
  StochasticUniversalSampling(std::size_t size) : size_(size) { }

  Survivors selectSurvivors (const Population<Phenotype, Genotype>& population,
                             const PopulationFitness& fitness) override
  {
    float totalFitness = 1.0f;
    float distanceBetweenPoints = totalFitness / size_;
    std::mt19937 generator {std::random_device{}()};
    std::uniform_real_distribution<> distribution (0.0f, distanceBetweenPoints);
    float start = distribution(generator);
    std::multiset<float> points;
    for (std::size_t k = 0; k < size_; ++k)
    {
      points.insert(start + k * distanceBetweenPoints); 
    }
    return internalWheelSelection(population, fitness, points);
  }
};

template<typename Phenotype, typename Genotype>
struct TruncationSelection: public SurvivalPolicy<Phenotype, Genotype>
{
  private: const std::size_t size_;

  public:
  
  TruncationSelection (std::size_t size) : size_(size) { }

  Survivors selectSurvivors (const Population<Phenotype, Genotype>& population,
                             const PopulationFitness& fitness) override
  {
    std::size_t populationSize = population.size();
    std::multimap<FitnessType, PopulationIndex> fitnessMap;
    for (PopulationIndex k = 0; k < populationSize; ++k)
    {
      fitnessMap.insert(std::make_pair(fitness[k], k));
    }

    // select survivors from higher to lower fitness
    Survivors result;
    auto it = fitnessMap.crbegin();
    auto end = fitnessMap.crend();
    for (std::size_t i = 0; it != end && i < size_; ++i, ++it)
    {
      result.insert(it->second);
    }
    return result;
  }
};

template<typename Phenotype, typename Genotype>
struct TournamentSelection : public SurvivalPolicy<Phenotype, Genotype>
{
  private: const std::size_t survivorsNumber_;
           const std::size_t tournamentSize_;

  public:
  
  TournamentSelection(std::size_t survivorsNumber, std::size_t tournamentSize)
    : survivorsNumber_(survivorsNumber), tournamentSize_(tournamentSize) { }

  Survivors selectSurvivors (const Population<Phenotype, Genotype>& population,
                             const PopulationFitness& fitness) override
  {
    std::size_t populationSize = population.size();
    std::mt19937 generator {std::random_device{}()};
    std::uniform_int_distribution<> distribution (0, populationSize - 1);
    std::set<PopulationIndex> alreadyUsed;
    std::multimap<FitnessType, PopulationIndex> participants;
    for (std::size_t k = 0 ; k < tournamentSize_; ++k)
    {
      PopulationIndex index = distribution(generator);
      while (alreadyUsed.find(index) != alreadyUsed.end()) index = distribution(generator);

      FitnessType f = fitness[index];
      participants.insert(std::make_pair(f, index));
      alreadyUsed.insert(index);
    }

    Survivors result;

    // select survivors from higher to lower fitness
    auto it = participants.crbegin();
    auto end = participants.crend();
    for (std::size_t i = 0; it != end && i < survivorsNumber_; ++i, ++it)
    {
      result.insert(it->second);
    }
    return result;
  }
};

}
#endif
