// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef DNA_CODING_HEADER_SEEN__
#define DNA_CODING_HEADER_SEEN__

#include <vector>
#include <array>
#include <memory>
#include <random>

#include "gene/policies.hpp"

namespace gene { namespace coding { namespace dna {

/******************************************************************************
 * Bases forming the DNA.
 *****************************************************************************/
const size_t NUMBER_OF_BASES = 4;
enum class Base : char {G, A, T, C};

/******************************************************************************
 * PoD representing a codon.
 *****************************************************************************/
const size_t CODON_SIZE = 3;
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
struct Chromosome
{
  std::vector<Base> bases;

  //FIXME: this should not be necessary but a seemingly bug in GCC makes it so.
  //TODO: check in future versions of GCC.
  Chromosome(const Chromosome& other) = default;

  Chromosome(Chromosome&& other) = default;

  Chromosome(std::vector<Base> b) : bases{std::move(b)} { }

  //FIXME: this should not be necessary but a seemingly bug in GCC makes it so.
  //TODO: check in future versions of GCC.
  Chromosome& operator=(Chromosome&& rhs) = default;
};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotype
{
  const std::vector<Chromosome> chromosomes;

  Genotype(Genotype&& g) = default;

  Genotype(std::vector<Chromosome> c) : chromosomes(std::move(c)) { }

  Genotype& operator=(Genotype&& rhs) = delete;
};

/****************************************************************************
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct SimpleCrossover : public CombinationStrategy<Genotype>
{
  SimpleCrossover(uint32_t seed);

  SimpleCrossover(const SimpleCrossover&) = delete;

  Genotype combine(const Genotype&, const Genotype&) override;

  private: std::mt19937 random_;
};

/****************************************************************************
 * Implementation of MutationStrategy for DNA coded Genotypes.
 ***************************************************************************/
template<typename Phenotype>
struct BaseMutation : MutationStrategy<Phenotype, Genotype>
{
  BaseMutation(float percentageOfBasesToMutate, uint32_t seed);

  std::pair<Phenotype, Genotype>
          mutate(Phenotype,
                 Genotype,
                 const Codec<Phenotype, Genotype>&) override;
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
