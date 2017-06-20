#ifndef CHUNKER_TRANSCRIPTIONSYMBOLS_HPP
#define CHUNKER_TRANSCRIPTIONSYMBOLS_HPP


class TranscriptionPhoneme
{
private:
	std::string _label;
public:
	const std::string & getLabel() const;
	TranscriptionPhoneme(const std::string &);
};

class TranscriptionTerm
{
private:
	std::string _label;
	std::deque<TranscriptionPhoneme *> _segmentation;

public:
	const std::string & getLabel() const;
	std::deque<TranscriptionPhoneme *>::const_iterator cbegin();
	std::deque<TranscriptionPhoneme *>::const_iterator cend();

	bool active() const;

	TranscriptionTerm(std::string, std::deque<TranscriptionPhoneme *>);
	std::deque<TranscriptionPhoneme *> getPhonemes() const;
};


class TranscriptionToken
{
private:
	long int _index;
	std::string _ort;
	TranscriptionTerm * _term;

public:
	std::deque<TranscriptionPhoneme *> getPhonemes() const;
	long int getIndex() const;
	const std::string & getLabel() const;
	const std::string & getOrt() const;
	bool active() const;
	std::deque<TranscriptionPhoneme *>::const_iterator cbegin() const;
	std::deque<TranscriptionPhoneme *>::const_iterator cend() const;
	TranscriptionToken(long int, const std::string &, TranscriptionTerm *);
};

#endif
