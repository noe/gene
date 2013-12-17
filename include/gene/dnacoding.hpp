#ifndef DNA_CODING_HEADER_SEEN__
#define DNA_CODING_HEADER_SEEN__

namespace gene { namespace dna {

//TODO
enum class Base {G, A, T, C};

struct Chromosome : boost::noncopyable
{
  const std::vector<Base> encodedGenes;
};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotype : boost::noncopyable
{
  const std::vector<std::unique_ptr<Chromosome>> chromosomes;
};

/****************************************************************************
 * Implementation of mutation with a fixed probability.
 ***************************************************************************/
struct BaseFlipMutation : public gene::MutationStrategy<Genotype>,
                          boost::noncopyable
{
  BaseFlipMutation (float bitMutationProbability);

  std::unique_ptr<Genotype> mutate(const Genotype&);

  private: const float percentageOfMutation_;
};  

/****************************************************************************
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct SimpleCrossover : public CombinationStrategy<Genotype>,
                         boost::noncopyable
{
  std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&);
};



#endif
