#include "gene/coding/dna.hpp"
#include "gene/policies.hpp"
#include "gene/algorithm.hpp"

#include <utility>

using namespace gene;
using namespace gene::coding::dna;
/*
struct MyFactory : public IndividualFactory<Genotype,
{

};


struct MyFitness
{
  Fitness calculate(const Population<Individual>&)
  {

  }

};
*/

int main(void)
{
  std::vector<gene::coding::dna::Chromosome> chromosomes;
  gene::coding::dna::Genotype g(std::move(chromosomes));

  BaseMutation mutation(0.10, 423);
/*
  MyFactory factory;
  MyFitness fitness;
  ConstantMutationRate mutationRate(0.20);

  GeneticAlgorithm (factory,
                    fitness,
                    MutationStrategy<Genotype>& mutationStrategy,
                    mutationRate,
                    AttractionMeter<Individual>& attractionMeter,
                    CombinationStrategy<Genotype>& combinationStrategy,
                    SurvivalPolicy<Individual>& survivalPolicy);
*/
  return 0;
}
