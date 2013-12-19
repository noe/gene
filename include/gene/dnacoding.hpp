#ifndef DNA_CODING_HEADER_SEEN__
#define DNA_CODING_HEADER_SEEN__

#include <vector>
#include <array>
#include <memory>
#include <boost/noncopyable.hpp>

#include "gene/policies.hpp"

namespace gene { namespace dna {

enum class Base : char {G, A, T, C};

typedef std::array<Base, 3>Codon;

const std::vector<Codon> START_CODONS = {Codon{ Base::A, Base::T, Base::G }};

const std::vector<Codon> STOP_CODONS = {Codon{ Base::T, Base::A, Base::A },
                                        Codon{ Base::T, Base::A, Base::G },
                                        Codon{ Base::T, Base::G, Base::A }};

//TODO: re-read notes on genetics in google drive

struct Chromosome : boost::noncopyable
{
  const std::vector<Base> bases;
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
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct SimpleCrossover : public CombinationStrategy<Genotype>,
                         boost::noncopyable
{
  std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&);
};

}}

#include "gene/dnacoding_impl.hpp"

#endif
