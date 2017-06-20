#ifndef CHUNKER_ALIGNER
#define CHUNKER_ALIGNER

class Aligner
{
public:
	virtual void run(std::deque<AlignPoint> *) = 0;
	Aligner(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<RecognitionIterator>);
	Aligner();	

protected:
	std::vector<std::shared_ptr<BaseIterator> > _transcriptPoints;
	std::vector<std::shared_ptr<BaseIterator> > _recognitionPoints;
};



class AlignFast : public Aligner
{
public:
	void run(std::deque<AlignPoint> *);
	AlignFast(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<RecognitionIterator>);

};

class AlignHirschberg : public Aligner
{
private:

	std::vector<std::shared_ptr<BaseIterator> > * _shortVec = NULL;
	std::vector<std::shared_ptr<BaseIterator> > * _longVec = NULL;

	std::vector<unsigned> getFirstHalfCosts
	(
		const std::vector<std::shared_ptr<BaseIterator> >::iterator, 
		const std::vector<std::shared_ptr<BaseIterator> >::iterator,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
	
	std::vector<unsigned> getSecondHalfCosts
	(
		const std::vector<std::shared_ptr<BaseIterator> >::iterator, 
		const std::vector<std::shared_ptr<BaseIterator> >::iterator,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
	
	std::deque<AlignPoint> trivialCaseHeight0
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
	
	std::deque<AlignPoint> trivialCaseWidth0
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
	
	std::deque<AlignPoint> trivialCaseHeight1
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
	
	std::deque<AlignPoint> trivialCaseWidth1
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);

	std::deque<AlignPoint> nonTrivialCase
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);

	std::deque<AlignPoint> hirschberg
	(
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator,
	 	const std::vector<std::shared_ptr<BaseIterator> >::iterator
	);
public:

	void run(std::deque<AlignPoint> *);
	AlignHirschberg(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<RecognitionIterator>);
};


class AlignMatrix : public Aligner
{
private:
	
	std::vector<std::vector<unsigned> >_matrix; 

public:
	void run(std::deque<AlignPoint> *);
	AlignMatrix(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<RecognitionIterator>);
	std::deque<AlignPoint> findPath(unsigned, unsigned) const;
	std::deque<AlignPoint> findPathUnrestricted() const;
	std::deque<AlignPoint> findPathRestricted() const;
	AlignMatrix(
		const std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptStart,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptEnd,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionStart,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionEnd);
	void fillMatrix();
};

#endif
