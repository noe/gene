

namespace gene { namespace coding { namespace dna {

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

SimpleCrossover::SimpleCrossover(std::size_t seed)
  : random_(seed)
{
  // do nothing
}

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

}}}
