// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#include <algorithm>

namespace gene { namespace coding { namespace dna {

///////////////////////////////////////////////////////////////////////////////
Base randomBase(std::mt19937& random)
{
  std::uniform_int_distribution<char> dist(0, 3);
  return static_cast<Base>(dist(random));
}

///////////////////////////////////////////////////////////////////////////////
std::vector<Chromosome> meiosis (const Genotype& g, std::mt19937& random)
{
  std::vector<Chromosome> result;
  std::size_t count = g.chromosomes.size() / 2;

  for (std::size_t k = 0; k < count; ++k)
  {
    std::uniform_int_distribution<std::size_t> order(0, 1);
    bool b = order(random) == 1;

    const Chromosome& c1 = g.chromosomes[k * (b? 1:2)];
    const Chromosome& c2 = g.chromosomes[k * (b? 2:1)];

    std::size_t chromosomeSize = std::min(c1.bases.size(), c2.bases.size());
    std::uniform_int_distribution<std::size_t> dist(0, chromosomeSize);
    std::size_t whereToCut = dist(random);

    std::vector<Base> mixed;
    mixed.insert(mixed.end(), c1.bases.begin(), c1.bases.begin() + whereToCut);
    mixed.insert(mixed.end(), c2.bases.begin() + whereToCut, c2.bases.end());

    Chromosome c{std::move(mixed)};
    result.push_back(std::move(c));
  }
  return std::move(result);
}

///////////////////////////////////////////////////////////////////////////////
SimpleCrossover::SimpleCrossover(uint32_t seed)
  : random_(seed)
{
  // do nothing
}

///////////////////////////////////////////////////////////////////////////////
Genotype SimpleCrossover::combine(const Genotype& g1,
                                  const Genotype& g2)
{
  std::vector<Chromosome> m1 = meiosis(g1, random_);
  std::vector<Chromosome> m2 = meiosis(g2, random_);

  m1.insert(m1.end(),
            make_move_iterator(m2.begin()),
            make_move_iterator(m2.end()));

  Genotype combinedGenotype{std::move(m1)};
  return std::move(combinedGenotype);
}

///////////////////////////////////////////////////////////////////////////////
template<typename Individual>
std::pair<Individual, Genotype>
BaseMutation<Individual>::mutate(const Individual& i,
                                 const Genotype& g,
                                 const IndividualCodec<Individual, Genotype>& codec)
{
  std::vector<Chromosome> chromosomes;

  for(const Chromosome& original : g.chromosomes)
  {
    std::vector<Base> mutatedBases;
    mutatedBases.reserve(original.bases.size());
    for (const Base& originalBase : original.bases)
    {
      bool doMutate = distribution_(random_) < percentageOfBasesToMutate_;
      mutatedBases.push_back(doMutate ? randomBase(random_) : originalBase);
    }

    chromosomes.push_back(std::move(mutatedBases));
  }

  Genotype mutatedGenotype{std::move(chromosomes)};
  Individual mutatedIndividual{codec.decode(mutatedGenotype)};
  return make_pair(std::move(mutatedIndividual), std::move(mutatedGenotype));
}

///////////////////////////////////////////////////////////////////////////////
template<typename Individual>
BaseMutation<Individual>::BaseMutation(float percentageOfBasesToMutate, uint32_t seed)
  : percentageOfBasesToMutate_(percentageOfBasesToMutate),
    random_(seed),
    distribution_(0.0, 100.0)
{
  // do nothing
}

///////////////////////////////////////////////////////////////////////////////
bool isCodon(std::vector<Base>::const_iterator it,
             const std::vector<Codon>& codons)
{
  auto func = [=](const Codon& codon)
                 { return codon[0] == *it
                          && codon[1] == *(it + 1)
                          && codon[2] == *(it + 2); };

  return std::find_if(std::begin(codons),
                      std::end(codons),
                      func) != std::end(codons);
}

///////////////////////////////////////////////////////////////////////////////
bool isStartCodon(std::vector<Base>::const_iterator it)
{
  return isCodon(it, START_CODONS);
}

///////////////////////////////////////////////////////////////////////////////
bool isStopCodon(std::vector<Base>::const_iterator it)
{
  return isCodon(it, STOP_CODONS);
}

///////////////////////////////////////////////////////////////////////////////
template<typename iterator>
Aminoacid decodeCodon(iterator it)
{
  return static_cast<uint8_t>(*it) * 1
         + static_cast<uint8_t>(*(it+1)) * NUMBER_OF_BASES
         + static_cast<uint8_t>(*(it+2)) * NUMBER_OF_BASES * NUMBER_OF_BASES;
}

///////////////////////////////////////////////////////////////////////////////
Codon decodeCodon(Aminoacid a)
{
  uint8_t b1 = a / (NUMBER_OF_BASES*NUMBER_OF_BASES);
  uint8_t b2 = (a % (NUMBER_OF_BASES*NUMBER_OF_BASES)) / NUMBER_OF_BASES;
  uint8_t b3 = a - b1 - b2;
  return std::move(Codon{{static_cast<Base>(b1),
                          static_cast<Base>(b2),
                          static_cast<Base>(b3)}});
}

///////////////////////////////////////////////////////////////////////////////
std::vector<DecodedGene> decodeGenes (const Chromosome& chromosome)
{
  std::vector<DecodedGene> result;
  size_t chromosomeLength = chromosome.bases.size();

  // traverse the whole chromosome looking for genes
  for (size_t pos = 0; pos < chromosomeLength - CODON_SIZE; ++pos)
  {
    std::vector<Base>::const_iterator it1 = chromosome.bases.begin() + pos;

    if (!isStartCodon(it1)) continue;  // skip until start codon

    DecodedGene gene;
    gene.push_back(decodeCodon(it1));

    // decode codons until stop mark 
    for (pos += CODON_SIZE;
         pos < chromosomeLength - CODON_SIZE;
         pos += CODON_SIZE)
    {
      std::vector<Base>::const_iterator it2 = chromosome.bases.begin() + pos;
      gene.push_back(std::move(decodeCodon(it2)));

      if (!isStopCodon(it2)) continue;

      // found a gene!
      result.push_back(std::move(gene));

      // leave 'pos' so that the next loop of the outer for points properly
      pos--;
      break;
    } 
  }
  return std::move(result);
}

}}}
