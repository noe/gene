// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)


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
std::unique_ptr<Genotype> SimpleCrossover::combine(const Genotype& g1,
                                                   const Genotype& g2)
{
  std::vector<Chromosome> m1 = meiosis(g1, random_);
  std::vector<Chromosome> m2 = meiosis(g2, random_);

  m1.insert(m1.end(),
            make_move_iterator(m2.begin()),
            make_move_iterator(m2.end()));

  return std::unique_ptr<Genotype>(new Genotype(std::move(m1)));
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Genotype> BaseMutation::mutate(const Genotype& g)
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

  return std::unique_ptr<Genotype>(new Genotype(std::move(chromosomes)));
}

///////////////////////////////////////////////////////////////////////////////
BaseMutation::BaseMutation(float percentageOfBasesToMutate, uint32_t seed)
  : percentageOfBasesToMutate_(percentageOfBasesToMutate),
    random_(seed),
    distribution_(0.0, 100.0)
{
  // do nothing
}

}}}
