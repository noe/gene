// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef DNA_CODING_HEADER_SEEN__
#define DNA_CODING_HEADER_SEEN__

#include <vector>
#include <array>
#include <memory>
#include <random>
#include <boost/noncopyable.hpp>

#include "gene/policies.hpp"

namespace gene { namespace coding { namespace dna {

/******************************************************************************
 * Bases forming the DNA.
 *****************************************************************************/
enum class Base : char {G, A, T, C};

const size_t NUMBER_OF_BASES = 4;

const size_t CODON_SIZE = 3;

/******************************************************************************
 * PoD representing a codon.
 *****************************************************************************/
typedef std::array<Base, CODON_SIZE> Codon;

/******************************************************************************
 * Codons marking the start sequence of a gene.
 *****************************************************************************/
const std::vector<Codon> START_CODONS = {Codon{{ Base::A, Base::T, Base::G }}};

/******************************************************************************
 * Codons marking the stop sequence of a gene.
 *****************************************************************************/
const std::vector<Codon> STOP_CODONS = {Codon{{ Base::T, Base::A, Base::A }},
                                        Codon{{ Base::T, Base::A, Base::G }},
                                        Codon{{ Base::T, Base::G, Base::A }}};

/******************************************************************************
 * 
 *****************************************************************************/
typedef uint8_t Aminoacid; // from 0 to 63 (i.e. 4^3 - 1)
typedef std::vector<Aminoacid> DecodedGene;

/******************************************************************************
 * PoD representing a chromosome.
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

  SimpleCrossover(uint32_t seed);

  private: std::mt19937 random_;
};

/****************************************************************************
 * Implementation of MutationStrategy for DNA coded Genotypes.
 ***************************************************************************/
struct BaseMutation : MutationStrategy<Genotype>
{
  std::unique_ptr<Genotype> mutate(const Genotype&);

  BaseMutation(float percentageOfBasesToMutate, uint32_t seed);

  private:
    const float percentageOfBasesToMutate_;
    std::mt19937 random_;
    std::uniform_real_distribution<> distribution_;
};

/****************************************************************************
 * 
 ***************************************************************************/
std::vector<DecodedGene> decodeGenes (const Chromosome& chromosome);

}}}

#include "gene/coding/dna_impl.hpp"

#endif
