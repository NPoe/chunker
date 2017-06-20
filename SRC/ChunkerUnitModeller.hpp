#ifndef CHUNKER_UNITMODELLER_HPP
#define CHUNKER_UNITMODELLER_HPP

class LanguageModel;

class UnitModeller
{
protected:
	LanguageModel * _lm;
	virtual unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const = 0;
public:
	void setLM(LanguageModel *);
	unsigned modelUnit(unsigned, const UniqueIterator &) const;
	unsigned modelUnit(unsigned, const std::unique_ptr<TranscriptionIterator> &) const;
};

class UnitModellerClassic : public UnitModeller
{
private:
	unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const;
};

class UnitModellerMaus : public UnitModeller
{
private:
	std::string _prefix;
	unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const;
	void runMaus(const std::deque<TranscriptionPhoneme *> &) const;

public:
	UnitModellerMaus(const std::string prefix);

};
#endif
