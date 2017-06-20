#ifndef CHUNKER_ITERATORS_HPP
#define CHUNKER_ITERATORS_HPP

class BaseIterator
{
protected:
	bool _showInactive = false;
public:
	virtual bool show() const = 0;
	virtual const std::string& getUnit() const = 0;
	virtual BaseIterator& operator ++() = 0;
	virtual bool done() const = 0;
	virtual std::unique_ptr<BaseIterator> copyAsBaseIterator() const = 0;
	virtual std::shared_ptr<BaseIterator> copyAsBaseIteratorShared() const = 0;
	void deactivateSilentSegments();
	void activateSilentSegments();
};


class TranscriptionIterator : public BaseIterator
{
protected:
	std::map<long int, TranscriptionToken>::const_iterator _iterOuter;
	std::map<long int, TranscriptionToken>::const_iterator _done;
	bool _useOrt = false;
public:
	virtual std::unique_ptr<TranscriptionIterator> copyAsTranscriptionIterator() const = 0;
	virtual std::shared_ptr<TranscriptionIterator> copyAsTranscriptionIteratorShared() const = 0;
	bool show() const;

	virtual std::deque<TranscriptionPhoneme *> getPhonemes() const = 0;

	TranscriptionIterator(const TranscriptionIterator &, const TranscriptionIterator &);
	TranscriptionIterator(const std::map<long int, TranscriptionToken> &);
	TranscriptionIterator(const TranscriptionIterator &);
	TranscriptionIterator(const std::map<long int, TranscriptionToken>::const_iterator,
			const std::map<long int, TranscriptionToken>::const_iterator);

	void useOrt();
	long int getIndex() const;

	virtual std::string getLMRepresentation() const = 0;
	const std::string& getToken() const;
	bool done() const;
};

class TranscriptionTokenIterator : public TranscriptionIterator
{
public:
	std::deque<TranscriptionPhoneme *> getPhonemes() const;

	std::string getLMRepresentation() const;

	TranscriptionTokenIterator(const TranscriptionTokenIterator &, const TranscriptionTokenIterator &);
	TranscriptionTokenIterator(const std::map<long int, TranscriptionToken> & anchor);
	TranscriptionTokenIterator(const TranscriptionTokenIterator& other);

	BaseIterator& operator ++ ();

	const std::string& getUnit() const;
	
	const std::string& getOrt() const;

	std::unique_ptr<BaseIterator> copyAsBaseIterator() const;
	std::shared_ptr<BaseIterator> copyAsBaseIteratorShared() const;
	
	std::unique_ptr<TranscriptionIterator> copyAsTranscriptionIterator() const;
	std::shared_ptr<TranscriptionIterator> copyAsTranscriptionIteratorShared() const;
	
};

class TranscriptionPhoneIterator : public TranscriptionIterator
{
private:
	std::deque<TranscriptionPhoneme *>::const_iterator _iterInner;

public:
	std::string getLMRepresentation() const;
	std::deque<TranscriptionPhoneme *> getPhonemes() const;

	TranscriptionPhoneIterator(const TranscriptionPhoneIterator &, const TranscriptionPhoneIterator &);
	TranscriptionPhoneIterator(const std::map<long int, TranscriptionToken> & anchor);
	TranscriptionPhoneIterator(const TranscriptionPhoneIterator& other);	

	std::unique_ptr<BaseIterator> copyAsBaseIterator() const;
	std::shared_ptr<BaseIterator> copyAsBaseIteratorShared() const;

	std::unique_ptr<TranscriptionIterator> copyAsTranscriptionIterator() const;
	std::shared_ptr<TranscriptionIterator> copyAsTranscriptionIteratorShared() const;
	

	BaseIterator& operator ++ ();

	const std::string& getUnit() const;

};

class RecognitionIterator : public BaseIterator
{
protected:
	std::deque<RecognitionToken>::const_iterator _iterOuter;
	std::deque<RecognitionToken>::const_iterator _done;
public:
	bool done() const;
	bool show() const;
	
	RecognitionIterator(const RecognitionIterator &, const RecognitionIterator &);
	RecognitionIterator(const std::deque<RecognitionToken> &);
	RecognitionIterator(const RecognitionIterator &);
	RecognitionIterator(const std::deque<RecognitionToken>::const_iterator,
			const std::deque<RecognitionToken>::const_iterator);

	virtual long int getStartTime() const = 0;
	virtual long int getEndTime() const = 0;
	long int getIndex() const;

};

class RecognitionTokenIterator : public RecognitionIterator
{
public:
	RecognitionTokenIterator(const RecognitionTokenIterator &, const RecognitionTokenIterator &);
	RecognitionTokenIterator(const std::deque<RecognitionToken> &) ;
	RecognitionTokenIterator(const RecognitionTokenIterator&);
	std::unique_ptr<RecognitionIterator> copyAsRecognitionIterator() const;
	std::shared_ptr<RecognitionIterator> copyAsRecognitionIteratorShared() const;
	std::unique_ptr<BaseIterator> copyAsBaseIterator() const;
	std::shared_ptr<BaseIterator> copyAsBaseIteratorShared() const;
	long int getStartTime() const;
	long int getEndTime() const;
	const std::string& getUnit() const;
	BaseIterator& operator ++();
	std::deque<RecognitionPhone>::const_iterator cbegin() const;
	std::deque<RecognitionPhone>::const_iterator cend() const;

};

class RecognitionPhoneIterator : public RecognitionIterator
{
private:
	std::deque<RecognitionPhone>::const_iterator _iterInner;

public:
	RecognitionPhoneIterator(const RecognitionPhoneIterator &, const RecognitionPhoneIterator &);
	RecognitionPhoneIterator(const std::deque<RecognitionToken> &) ;
	RecognitionPhoneIterator(const RecognitionPhoneIterator&);
	std::unique_ptr<RecognitionIterator> copyAsRecognitionIterator() const;
	std::shared_ptr<RecognitionIterator> copyAsRecognitionIteratorShared() const;
	std::unique_ptr<BaseIterator> copyAsBaseIterator() const;
	std::shared_ptr<BaseIterator> copyAsBaseIteratorShared() const;
	long int getStartTime() const;
	long int getEndTime() const;
	const std::string& getUnit() const;
	BaseIterator& operator ++();
};


class UniqueIterator
{
private:
	std::unique_ptr<TranscriptionIterator> _iter;
	std::set<std::string> _seenUnits;
	bool _done = false;

public:
	std::string getLMRepresentation() const;
	UniqueIterator(std::unique_ptr<TranscriptionIterator>);
	std::deque<TranscriptionPhoneme *> getPhonemes() const;
	UniqueIterator& operator ++();
	const std::string & getUnit() const;
	bool done() const;
};

#endif
