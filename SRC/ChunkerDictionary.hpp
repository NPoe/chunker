#ifndef CHUNKER_DICTIONARY_HPP
#define CHUNKER_DICTIONARY_HPP

class Dictionary
{
private:
	std::map<std::string, std::string> _dict;
	std::string getPauseModel() const;
	void readMAUSDictionary();
	Dictionary();

public:
	Dictionary(UniqueIterator);
	Dictionary(std::unique_ptr<TranscriptionIterator>);
	void print(std::ostream &) const;
};

std::ostream & operator << (std::ostream &, const Dictionary &);

#endif
