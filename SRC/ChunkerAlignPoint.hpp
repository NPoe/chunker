#ifndef CHUNKER_ALIGNPOINT
#define CHUNKER_ALIGNPOINT


class AlignPoint
{
private:
	std::shared_ptr<TranscriptionIterator> _transIter;
	std::shared_ptr<RecognitionIterator> _recogIter;

public:
	
	unsigned getCost() const;
	const std::string & getTranscriptionUnit() const;
	const std::string & getTranscriptionToken() const;
	const std::string & getRecognitionUnit() const;
	long getIndex() const;
	long getStartTime() const;
	long getEndTime() const;
	AlignPoint(std::shared_ptr<BaseIterator>, std::shared_ptr<BaseIterator>);
};

#endif
