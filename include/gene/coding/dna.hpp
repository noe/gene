#ifndef DNA_CODING_HEADER_SEEN__
#define DNA_CODING_HEADER_SEEN__

#include <vector>
#include <array>
#include <memory>
#include <random>
#include <boost/noncopyable.hpp>

#include "gene/policies.hpp"

namespace gene { namespace coding { namespace dna {

enum class Base : char {G, A, T, C};

typedef std::array<Base, 3> Codon;

const std::vector<Codon> START_CODONS = {Codon{{ Base::A, Base::T, Base::G }}};

const std::vector<Codon> STOP_CODONS = {Codon{{ Base::T, Base::A, Base::A }},
                                        Codon{{ Base::T, Base::A, Base::G }},
                                        Codon{{ Base::T, Base::G, Base::A }}};

/******************************************************************************
 * 
 *****************************************************************************/
struct Chromosome : boost::noncopyable
{
  std::vector<Base> bases;

  Chromosome(Chromosome&& other) : bases(other.bases) { }

  Chromosome(std::vector<Base>&& b) : bases(std::move(b)) { }

  Chromosome& operator=(Chromosome&& rhs) { bases = rhs.bases; return *this;}

};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotype : boost::noncopyable
{
  const std::vector<Chromosome> chromosomes;

  Genotype(std::vector<Chromosome>&& c) : chromosomes(std::move(c)) { }
};

/****************************************************************************
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct SimpleCrossover : public CombinationStrategy<Genotype>,
                         boost::noncopyable
{
  std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&);

  SimpleCrossover(std::size_t seed);

  private: std::mt19937 random_;
};

}}}

#include "gene/coding/dna_impl.hpp"

#endif
