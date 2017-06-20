#ifndef CHUNKER_LANGUAGEMODEL_HPP
#define CHUNKER_LANGUAGEMODEL_HPP


class LanguageModel
{
protected:
	std::deque<std::tuple<unsigned, std::string> > _nodes;
	std::deque<std::tuple<unsigned, unsigned, std::string> > _links; 

	std::unique_ptr<UnitModeller> _modeller;
public:
	void addNode(unsigned, const std::string &);
	void addLink(unsigned, unsigned, std::string);

	void print(std::ostream &) const;

	LanguageModel(std::unique_ptr<UnitModeller>);
};

class LMHTK : public LanguageModel
{
public:
	LMHTK(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>, const std::string &);
};

class LMZerogram : public LanguageModel
{
public:
	LMZerogram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMUnigram : public LanguageModel
{
public:
	LMUnigram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMBigram : public LanguageModel
{
public:
	LMBigram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMPair : public LanguageModel
{
public:
	LMPair(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMTriple : public LanguageModel
{
public:
	LMTriple(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMFactorAutomaton : public LanguageModel
{
public:
	LMFactorAutomaton(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

class LMForcedAlign : public LanguageModel
{
public:
	LMForcedAlign(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

std::ostream & operator << (std::ostream &, const LanguageModel &);
#endif
