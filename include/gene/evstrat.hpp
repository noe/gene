// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)
#ifndef GENE_EVOLUTIONARY_STRATEGIES_HEADER_SEEN__
#define GENE_EVOLUTIONARY_STRATEGIES_HEADER_SEEN__

#include <array>
#include <cmath>
#include <random>

#include "gene/policies.hpp"
#include "gene/selection.hpp"
#include "gene/mating.hpp"

namespace gene{ namespace evstrat {

struct Void{ };

///////////////////////////////////////////////////////////////////////////////
struct EvolutionParams
{
  std::vector<double> value;
  std::vector<double> sigma;
};

using Individual = gene::Individual<Void, EvolutionParams>;
using Codec = gene::Codec<Void, EvolutionParams>;
using Population = gene::Population<Void, EvolutionParams>;
using FitnessFunction = gene::FitnessFunction<Void, EvolutionParams>;
using MutationStrategy = gene::MutationStrategy<Void, EvolutionParams>;
using CombinationStrategy = gene::CombinationStrategy<Void, EvolutionParams>;

///////////////////////////////////////////////////////////////////////////////
struct UncorrelatedOneStep : public MutationStrategy
{
  const double n_; 
  const double min_;
  const double max_;
  const double epsilon0_;
  const double tau_;
  std::mt19937 g_;
  std::normal_distribution<> normal_;

  UncorrelatedOneStep(std::size_t n,
                      double minValue = std::numeric_limits<double>::min(),
                      double maxValue = std::numeric_limits<double>::max(),
                      double epsilon0 = 0.1,
                      double tauProportionality = 1.0)
    : n_(n),
      min_(minValue),
      max_(maxValue),
      epsilon0_(epsilon0),
      tau_(tauProportionality / std::sqrt(n_)),
      g_ (std::random_device{}()),
      normal_ (0.0, 1.0)
  {
    // do nothing
  }

  Individual mutate(Individual individual, const Codec& codec) override
  {
    EvolutionParams& evParams = individual.second;    
    double& oldSigma = evParams.sigma[0];
    double newSigma = std::max(epsilon0_, oldSigma * exp(tau_ * normal_(g_)));
    newSigma = std::min(newSigma, (max_ - min_) / 2);

    for (std::size_t i = 0; i < n_; ++i)
    {
      double& oldValue = evParams.value[i];
      double newValue = std::max(std::min(oldValue + newSigma * normal_(g_), max_), min_);
      evParams.value[i] = newValue;
    }
    evParams.sigma[0] = newSigma;
    return std::move(individual);
  }
};

///////////////////////////////////////////////////////////////////////////////
struct UncorrelatedNSteps : public MutationStrategy
{
  const double n_; 
  const double min_; 
  const double max_; 
  const double epsilon0_;
  const double tau_;
  const double tauPrime_;
  std::mt19937 g_;
  std::normal_distribution<> normal_;

  UncorrelatedNSteps(std::size_t n,
                     double minValue = std::numeric_limits<double>::min(),
                     double maxValue = std::numeric_limits<double>::max(),
                     double epsilon0 = 0.1,
                     double tauProportionality = 1.0)
    : n_(n),
      min_(minValue),
      max_(maxValue),
      epsilon0_(epsilon0),
      tau_(tauProportionality / std::sqrt(2*n_)),
      tauPrime_(tauProportionality / std::sqrt(2*std::sqrt(n_))),
      g_ (std::random_device{}()),
      normal_ (0.0, 1.0)
  {
    // do nothing
  }

  Individual mutate(Individual individual, const Codec& codec) override
  {
    EvolutionParams& evParams = individual.second;    

    double baseMutation = tauPrime_ * normal_(g_);

    for (std::size_t i = 0; i < n_; ++i)
    {
      double& oldSigma = evParams.sigma[i];
      double& oldValue = evParams.value[i];
      double coordinateSpecificMutation = tau_* normal_(g_);
      double newSigma = oldSigma * exp(baseMutation + coordinateSpecificMutation);
      newSigma = std::min(std::max(epsilon0_, newSigma), (max_ - min_)/2);
      double newValue = std::max(std::min(oldValue + newSigma * normal_(g_), max_), min_);
      evParams.value[i] = newValue;
      evParams.sigma[i] = newSigma;
    }
    return std::move(individual);
  }
};

///////////////////////////////////////////////////////////////////////////////
struct LocalRecombination : public CombinationStrategy

{
  std::mt19937 g_;
  std::bernoulli_distribution dist_;

  Individual combine(const Individual& individual1,
                     const Individual& individual2,
                     const Codec& codec) override
  {
    std::size_t n = individual1.second.value.size();
    std::size_t nSigma = individual1.second.sigma.size();
    Individual result;
    result.second.value.reserve(n);
    result.second.sigma.reserve(nSigma);

    // Discrete recombination for the values
    for (std::size_t k = 0; k < n; ++k)
    {
      bool useFirst = dist_(g_);
      result.second.value.push_back((useFirst? individual1 : individual2).second.value[k]);
    }

    // Intermediary recombination for strategy params
    for (std::size_t k = 0; k < nSigma; ++k)
    {
      double val = (individual1.second.sigma[k] + individual2.second.sigma[k]) / 2.0;
      result.second.sigma.push_back(val);
    }
    return std::move(result);
  }
};

///////////////////////////////////////////////////////////////////////////////
struct SurvivalPolicy
{
  virtual Population selectSurvivors (FitnessFunction& function,
                                      const Population& previousGeneration,
                                      const Population& offspring) = 0;
  virtual ~SurvivalPolicy() { }
};

///////////////////////////////////////////////////////////////////////////////
struct MuPlusLambda : public SurvivalPolicy
{
  Population selectSurvivors (FitnessFunction& function,
                              const Population& previousGeneration,
                              const Population& offspring) override
  {
    std::size_t size = previousGeneration.size();

    Population together (previousGeneration);
    together.insert(together.end(), offspring.begin(), offspring.end());
    gene::PopulationFitness fitness = function.calculate(together);

    gene::TruncationSelection<Void, EvolutionParams> truncation(size);
    gene::Survivors survivors = std::move(truncation.selectSurvivors(together, fitness));
    return std::move(truncation.select(std::move(together), survivors));
  }
};

///////////////////////////////////////////////////////////////////////////////
struct MuCommaLambda : public SurvivalPolicy
{
  Population selectSurvivors (FitnessFunction& function,
                              const Population& previousGeneration,
                              const Population& offspring) override
  {
    std::size_t size = previousGeneration.size();
    gene::TruncationSelection<Void, EvolutionParams> truncation(size);
    gene::PopulationFitness fitness = function.calculate(offspring);
    gene::Survivors survivors = std::move(truncation.selectSurvivors(offspring, fitness));
    Population offspringCopy (offspring);
    return std::move(truncation.select(std::move(offspringCopy), survivors));
  }
};

///////////////////////////////////////////////////////////////////////////////
// Void implementation of codec
struct NullCodec : public Codec
{
  Void decode(const EvolutionParams&) const throw(std::invalid_argument) { return Void();}
  EvolutionParams encode(const Void&) const override { return EvolutionParams();}
};

///////////////////////////////////////////////////////////////////////////////
// Implements the evolution strategy optimization
struct EvolutionStrategies
{
  private:

    std::size_t offspringCount_;
    gene::RandomMating<Void, EvolutionParams> matingStrategy_;
    FitnessFunction& fitnessFunction_;
    MutationStrategy& mutationStrategy_;
    CombinationStrategy& combinationStrategy_;
    SurvivalPolicy& survivalPolicy_;

  public:

    EvolutionStrategies (FitnessFunction& fitnessFunction,
                         MutationStrategy& mutationStrategy,
                         CombinationStrategy& combinationStrategy,
                         SurvivalPolicy& survivalPolicy,
                         std::size_t offspringCount)
      : offspringCount_(offspringCount),
        matingStrategy_(offspringCount),
        fitnessFunction_(fitnessFunction),
        mutationStrategy_(mutationStrategy),
        combinationStrategy_(combinationStrategy),
        survivalPolicy_(survivalPolicy) { /* do nothing */ }

    Population iterate(Population&& population)
    {
      NullCodec nullCodec;

      // Determine the mating among individuals of the population
      PopulationFitness emptyFitness;
      auto mating = matingStrategy_.mating(population, emptyFitness);

      Population offspring; offspring.reserve(offspringCount_);
      // Combine each of the pairs specified in the calculated mating
      for (const auto& entry : mating)
      {
        PopulationIndex index1 = std::get<0>(entry);
        PopulationIndex index2 = std::get<1>(entry);
        NumberOfChildren numOffspring = std::get<2>(entry);

        const Individual& i1 = population[index1];
        const Individual& i2 = population[index2];

        for (std::size_t k = 0; k < numOffspring; ++k)
        {
          offspring.push_back(combinationStrategy_.combine(i1, i2, nullCodec));
        }
      }

      // Mutate offspring with probability 1.
      for (std::size_t k = 0; k < offspringCount_; ++k)
      {
        offspring[k] = mutationStrategy_.mutate(offspring[k], nullCodec);
      }

      // Use offspring as base for the new population...
      Population newPopulation =
         survivalPolicy_.selectSurvivors(fitnessFunction_, population, offspring);

      return std::move(newPopulation);
    }
};


///////////////////////////////////////////////////////////////////////////////
// Adapts a normal function to the fitnessfunction interface
using Function = std::function<double(const std::vector<double>&)>;
struct FitnessAdapter : public gene::FitnessFunction<gene::evstrat::Void,
                                              gene::evstrat::EvolutionParams>
{
  using Population = gene::Population<gene::evstrat::Void, gene::evstrat::EvolutionParams>;

  Function function_;

  FitnessAdapter(Function f) : function_(f) { }
  
  gene::PopulationFitness calculate(const Population& population) override
  {
    gene::PopulationFitness fitness;
    fitness.reserve(population.size());
    for (auto individual : population)
    {
      const gene::evstrat::EvolutionParams& evParams = individual.second;
      fitness.push_back(-function_(evParams.value));
    }
    return std::move(fitness);
  }
};

///////////////////////////////////////////////////////////////////////////////
// generates a random population
gene::evstrat::Population randomPopulation(std::size_t numVars,
                                           std::size_t populationSize,
                                           double minValue,
                                           double maxValue,
                                           double sigma,
                                           std::size_t numSigmas)
{
  std::mt19937 g;
  std::uniform_real_distribution<double> random(minValue, maxValue);

  gene::evstrat::Population result; result.reserve(populationSize);

  for (std::size_t i = 0; i < populationSize; ++i)
  {
    gene::evstrat::Individual individual;

    for (std::size_t j = 0; j < numVars; ++j)
    {
      individual.second.value.push_back(random(g));
    }
    for (std::size_t k = 0; k < numSigmas; ++k)
    {
      individual.second.sigma.push_back(sigma);
    }
    result.push_back(std::move(individual));
  }
  return std::move(result);
}

}}
#endif
