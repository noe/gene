#include "gene/coding/dna.hpp"
#include "gene/policies.hpp"
#include "gene/algorithm.hpp"

#include <utility>

using namespace gene;
using namespace gene::coding::dna;

struct Individual
{
  //TODO
};

struct MyFactory : public IndividualFactory<Individual, Genotype>
{
  std::string description() const
  {
    //TODO
  }

  std::unique_ptr<Individual> create(const Genotype&)
                                 throw(std::invalid_argument)
  {
    //TODO
  }
};

struct MyFitness : FitnessFunction<Individual>
{
  Fitness calculate(const Population<Individual>&)
  {
    //TODO
  }
};

struct Attraction : public AttractionMeter<Individual>
{
  float attractionBetween(const Individual&, const Individual&)
  {
    //TODO
  }
};

struct Survival : public SurvivalPolicy<Individual>
{
  Population<Individual> sift (const Population<Individual>& ancestors,
                               const Population<Individual>& offspring)
  {
    //TODO
  }
};

int main(void)
{
  std::vector<gene::coding::dna::Chromosome> chromosomes;
  gene::coding::dna::Genotype g(std::move(chromosomes));

  BaseMutation mutation(0.10, 423);
  ConstantMutationRate<Individual> mutationRate(0.20);
  SimpleCrossover crossover(1245);

  MyFactory factory;
  MyFitness fitness;
  Attraction attraction;
  Survival survival;

  GeneticAlgorithm<Individual, Genotype> (factory,
                                          fitness,
                                          mutation,
                                          mutationRate,
                                          attraction,
                                          crossover,
                                          survival);
  return 0;
}
