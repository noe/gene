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
struct MyFactory : public IndividualFactory<Individual, Genotype>
{
  std::string description() const
  {
    return "Dummy factory";
  }

  Neuron gene2neuron(const DecodedGene& gene)
  {
    
  }

  std::unique_ptr<Individual> create(const Genotype& genotype)
                                  throw(std::invalid_argument)
  {
    std::vector<DecodedGene> genes;

    for (Chromosome& chromosome : genotype.chromosomes)
    {
      std::vector<DecodedGene> chromGenes = decodeGenes(chromosome);
      genes.insert(genes.end(), chromGenes.begin(), chromGenes.end);
    }

    

    return std::unique_ptr<Individual>();
  }
};

///////////////////////////////////////////////////////////////////////////////
struct MyFitness : FitnessFunction<Individual>
{
  Fitness calculate(const Population<Individual>&)
  {
    //TODO
  }
};

///////////////////////////////////////////////////////////////////////////////
struct Attraction : public AttractionMeter<Individual>
{
  float attractionBetween(const Individual&, const Individual&)
  {
    //TODO
  }
};

///////////////////////////////////////////////////////////////////////////////
struct Survival : public SurvivalPolicy<Individual>
{
  Population<Individual> sift (const Population<Individual>& ancestors,
                               const Population<Individual>& offspring)
  {
    //TODO
  }
};

///////////////////////////////////////////////////////////////////////////////
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
