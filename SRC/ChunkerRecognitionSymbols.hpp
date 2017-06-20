#ifndef CHUNKER_RECOGNITIONSYMBOLS_HPP
#define CHUNKER_RECOGNITIONSYMBOLS_HPP


class RecognitionPhone
{
private:
	long int _start;
	long int _end;
	std::string _label;

public:
	const std::string & getLabel() const;
	long int getStart() const;
	long int getEndTime() const;

	RecognitionPhone(long int, long int, std::string);
};

class RecognitionToken
{
private:
	std::string _label;
	std::string _ort;
	std::deque<RecognitionPhone> _segmentation;
	long _index;

public:
	bool active() const;
	const std::string & getLabel() const;
	const std::string & getOrt() const;
	long getStart() const; 
	long getEndTime() const;
	long getIndex() const;
	std::deque<RecognitionPhone>::const_iterator cbegin() const;
	std::deque<RecognitionPhone>::const_iterator cend() const;

	RecognitionToken(std::string, std::string, std::deque<RecognitionPhone>, long);
};


#endif
