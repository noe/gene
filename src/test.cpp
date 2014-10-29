#include "gene/coding/dna.hpp"
#include "gene/policies.hpp"
#include "gene/algorithm.hpp"

#include <utility>
#include <boost/noncopyable.hpp>

using namespace gene;
using namespace gene::coding::dna;

///////////////////////////////////////////////////////////////////////////////
struct Neuron : boost::noncopyable
{
  uint8_t x;
  uint8_t y;
  uint8_t z;
  uint8_t distanceThreshold;
  uint8_t outputThreshold;
};

///////////////////////////////////////////////////////////////////////////////
struct Network
{
  std::vector<Network> neurons;
};

///////////////////////////////////////////////////////////////////////////////
struct MyFactory : public IndividualCodec<Network, Genotype>
{
  Network decode(const Genotype&) const throw(std::invalid_argument) override { }
  Genotype encode(const Network&) const override { };
};

///////////////////////////////////////////////////////////////////////////////
struct MyFitness : FitnessFunction<Network, Genotype>
{
  Fitness calculate(const Population<Network, Genotype>&) override { }
};


///////////////////////////////////////////////////////////////////////////////
struct Survival : public SurvivalPolicy<Network, Genotype>
{
  Population<Network, Genotype>
          sift (const Population<Network, Genotype>&& ancestors,
                const Population<Network, Genotype>&& offspring) override { }
};

///////////////////////////////////////////////////////////////////////////////
int main(void)
{
  std::vector<gene::coding::dna::Chromosome> chromosomes;
  gene::coding::dna::Genotype g(std::move(chromosomes));

  BaseMutation<Network> mutation(0.10, 423);
  ConstantMutationRate<Network> mutationRate(0.20);
  SimpleCrossover crossover(1245);

  MyFactory factory;
  MyFitness fitness;
  Survival survival;
/*
  GeneticAlgorithm<Individual, Genotype> (factory,
                                          fitness,
                                          mutation,
                                          mutationRate,
                                          attraction,
                                          crossover,
                                          survival);
*/  return 0;
}
