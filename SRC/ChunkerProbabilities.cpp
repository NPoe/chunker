#include "ChunkerGlobal.hpp"

UnigramProbabilities::UnigramProbabilities(std::unique_ptr<TranscriptionIterator> iter)
{CHUNKER_FLOG
	unsigned total = 0;
	
	for(; ! iter->done(); ++(*iter))
	{
		_unigramCounts.emplace(iter->getUnit(), 0).first->second ++;
		total ++;
	}

	_unigramCounts.emplace(ChunkerUtil::NULLSTRING, 0).first->second ++;

	for(const auto & variable : _unigramCounts)
	{
		_unigramProbs[variable.first] = (float)(variable.second) / (float)total;
	}
}

float UnigramProbabilities::getUnigramProb(const std::string& unit) const
{CHUNKER_FLOG
	auto res = _unigramProbs.find(unit);
	if(res == _unigramProbs.end())
	{
		CHUNKER_ERR << "No unigram probability for: " << unit CHUNKER_ENDERR4
	}
	return res->second;
}

int UnigramProbabilities::getUnigramCount(const std::string& unit) const
{CHUNKER_FLOG
	auto res = _unigramCounts.find(unit);
	if(res == _unigramCounts.end())
	{
		CHUNKER_ERR << "No unigram count for: " << unit CHUNKER_ENDERR4
	}
	return res->second;

}

BigramProbabilities::BigramProbabilities(std::unique_ptr<TranscriptionIterator> iter)
{CHUNKER_FLOG
	std::string last(ChunkerUtil::NULLSTRING);
	
	for(; ! iter->done(); ++(*iter))
	{
		_unigramCounts.emplace(iter->getUnit(), 0).first->second ++;
		_bigramCounts.emplace(last, std::map<std::string, unsigned>()).first->second.
			emplace(iter->getUnit(), 0).first->second ++;
		last = iter->getUnit(); 
	}

	_unigramCounts.emplace(ChunkerUtil::NULLSTRING, 0).first->second ++;
	_bigramCounts.emplace(last, std::map<std::string, unsigned>()).first->second.
		emplace(ChunkerUtil::NULLSTRING, 0).first->second ++;
	
	for(const auto & factor : _bigramCounts)
	{
		auto res = _bigramProbs.emplace(factor.first, std::map<std::string, float>()).first;
		for(const auto & variable : factor.second)
		{
			res->second.emplace(variable.first, 
				(float)_bigramCounts.at(factor.first).at(variable.first) / (float) _unigramCounts.at(factor.first));
		}
	}
}

float BigramProbabilities::getBigramProb(const std::string& factor, const std::string& variable) const
{CHUNKER_FLOG
	auto resFactor = _bigramProbs.find(factor);
	if(resFactor == _bigramProbs.end())
	{
		return 0;
	}
	auto resVariable = resFactor->second.find(variable);
	if(resVariable == resFactor->second.end())
	{
		return 0;
	}
	return resVariable->second;
}

