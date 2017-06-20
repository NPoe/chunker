#ifndef CHUNKER_PROBABILITIES_HPP
#define CHUNKER_PROBABILITIES_HPP


class UnigramProbabilities
{
private:
	std::map<std::string, unsigned> _unigramCounts;
	std::map<std::string, float> _unigramProbs;
	unsigned _total;

public:
	
	UnigramProbabilities(std::unique_ptr<TranscriptionIterator>);

	float getUnigramProb(const std::string & ) const;
	
	int getUnigramCount(const std::string &) const;
};

class BigramProbabilities
{
private:
	std::map<std::string, unsigned> _unigramCounts;
	std::map<std::string, std::map<std::string, unsigned> > _bigramCounts;
	std::map<std::string, std::map<std::string, float> > _bigramProbs;

public:
	BigramProbabilities(std::unique_ptr<TranscriptionIterator>);
	
	float getBigramProb(const std::string &, const std::string &) const;
};

#endif
