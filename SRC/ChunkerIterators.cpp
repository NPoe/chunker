#include "ChunkerGlobal.hpp"

void TranscriptionIterator::useOrt()
{CHUNKER_FLOG
	_useOrt = true;
}

TranscriptionIterator::TranscriptionIterator(const std::map<long int, TranscriptionToken>::const_iterator begin,
		const std::map<long int, TranscriptionToken>::const_iterator end)
{CHUNKER_FLOG
	_iterOuter = begin;
	_done = end;
}

void BaseIterator::deactivateSilentSegments()
{
	_showInactive = false;
	if(!show())
	{
		++(*this);
	}
}

void BaseIterator::activateSilentSegments()
{
	_showInactive = true;
}

TranscriptionIterator::TranscriptionIterator(const std::map<long int, TranscriptionToken> & anchor) : 
	TranscriptionIterator(anchor.cbegin(), anchor.cend())
{CHUNKER_FLOG}

TranscriptionIterator::TranscriptionIterator(const TranscriptionIterator & start, const TranscriptionIterator & end) : 
	TranscriptionIterator(start._iterOuter, end._iterOuter)
{CHUNKER_FLOG}

TranscriptionIterator::TranscriptionIterator(const TranscriptionIterator & other) : TranscriptionIterator(other._iterOuter, other._done)
{CHUNKER_FLOG
	_showInactive = other._showInactive;
	_useOrt = other._useOrt;
}

long int TranscriptionIterator::getIndex() const 
{CHUNKER_FLOG
	return _iterOuter->second.getIndex(); 
}

const std::string & TranscriptionTokenIterator::getOrt() const
{CHUNKER_FLOG
	return _iterOuter->second.getOrt();
}

std::string TranscriptionTokenIterator::getLMRepresentation() const
{CHUNKER_FLOG
	return ChunkerUtil::SPECIALSTRING + getUnit() + ChunkerUtil::SPECIALSTRING + getOrt() + 
		ChunkerUtil::SPECIALSTRING + std::to_string(getIndex());
}

std::string TranscriptionPhoneIterator::getLMRepresentation() const
{CHUNKER_FLOG
	
	return ChunkerUtil::SPECIALSTRING + getUnit() + ChunkerUtil::SPECIALSTRING + getUnit() + 
		ChunkerUtil::SPECIALSTRING + std::to_string(getIndex());
}

TranscriptionTokenIterator::TranscriptionTokenIterator(const TranscriptionTokenIterator & start, const TranscriptionTokenIterator & end) : 
	TranscriptionIterator(start, end)
{CHUNKER_FLOG}
TranscriptionTokenIterator::TranscriptionTokenIterator(const std::map<long int, TranscriptionToken> & anchor) : TranscriptionIterator(anchor) 
{CHUNKER_FLOG}

TranscriptionTokenIterator::TranscriptionTokenIterator(const TranscriptionTokenIterator& other) : TranscriptionIterator(other) 
{CHUNKER_FLOG}

std::deque<TranscriptionPhoneme *> TranscriptionTokenIterator::getPhonemes() const
{CHUNKER_FLOG
	return _iterOuter->second.getPhonemes();
}

bool TranscriptionIterator::show() const
{CHUNKER_FLOG
	if(done()) return true;
	if(_iterOuter->second.active()) return true;
	if(_showInactive) return true;
	return false;
}

bool RecognitionIterator::show() const
{CHUNKER_FLOG
	if(done()) return true;
	if(_iterOuter->active()) return true;
	if(_showInactive) return true;
	return false;
}

BaseIterator& TranscriptionTokenIterator::operator ++ ()
{CHUNKER_FLOG

	_iterOuter++;

	if(show()) return *this;
	return ++(*this);
}
	
const std::string& TranscriptionIterator::getToken() const
{CHUNKER_FLOG
	if(_useOrt) return _iterOuter->second.getOrt();
	return _iterOuter->second.getLabel();
}

const std::string& TranscriptionTokenIterator::getUnit() const 
{CHUNKER_FLOG
	if(_useOrt) return _iterOuter->second.getOrt();
	return _iterOuter->second.getLabel(); 
}

std::unique_ptr<BaseIterator> TranscriptionTokenIterator::copyAsBaseIterator() const 
{CHUNKER_FLOG
	return std::unique_ptr<BaseIterator>(new TranscriptionTokenIterator(*this)); 
}

std::shared_ptr<BaseIterator> TranscriptionTokenIterator::copyAsBaseIteratorShared() const 
{CHUNKER_FLOG
	return std::shared_ptr<BaseIterator>(new TranscriptionTokenIterator(*this)); 
}

std::unique_ptr<TranscriptionIterator> TranscriptionTokenIterator::copyAsTranscriptionIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<TranscriptionIterator>(new TranscriptionTokenIterator(*this));
}

std::shared_ptr<TranscriptionIterator> TranscriptionTokenIterator::copyAsTranscriptionIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<TranscriptionIterator>(new TranscriptionTokenIterator(*this));
}


std::deque<TranscriptionPhoneme *> TranscriptionPhoneIterator::getPhonemes() const
{CHUNKER_FLOG
	std::deque<TranscriptionPhoneme *> result;
	result.push_back(*_iterInner);
	return result;
}

TranscriptionPhoneIterator::TranscriptionPhoneIterator(const TranscriptionPhoneIterator & start, const TranscriptionPhoneIterator & end) : 
	TranscriptionIterator(start, end)
{CHUNKER_FLOG
	_iterInner = _iterOuter->second.cbegin();
}

TranscriptionPhoneIterator::TranscriptionPhoneIterator(const std::map<long int, TranscriptionToken> & anchor) : TranscriptionIterator(anchor) 
{CHUNKER_FLOG 
	_iterInner = _iterOuter->second.cbegin(); 
}

TranscriptionPhoneIterator::TranscriptionPhoneIterator(const TranscriptionPhoneIterator& other) : TranscriptionIterator(other)
{CHUNKER_FLOG 
	_iterInner = other._iterInner; 
}

std::unique_ptr<BaseIterator> TranscriptionPhoneIterator::copyAsBaseIterator() const 
{CHUNKER_FLOG
	return std::unique_ptr<BaseIterator>(new TranscriptionPhoneIterator(*this)); 
}

std::shared_ptr<BaseIterator> TranscriptionPhoneIterator::copyAsBaseIteratorShared() const 
{CHUNKER_FLOG
	return std::shared_ptr<BaseIterator>(new TranscriptionPhoneIterator(*this)); 
}

std::unique_ptr<TranscriptionIterator> TranscriptionPhoneIterator::copyAsTranscriptionIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<TranscriptionIterator>(new TranscriptionPhoneIterator(*this));
}

std::shared_ptr<TranscriptionIterator> TranscriptionPhoneIterator::copyAsTranscriptionIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<TranscriptionIterator>(new TranscriptionPhoneIterator(*this));
}

BaseIterator& TranscriptionPhoneIterator::operator ++ ()
{CHUNKER_FLOG
	_iterInner++;
	if(_iterInner == _iterOuter->second.cend())
	{
		_iterOuter++;
		if(! done())
		{
			_iterInner = _iterOuter->second.cbegin();
		}
	}
	
	if(show()) return *this;
	return ++(*this);

}

const std::string& TranscriptionPhoneIterator::getUnit() const 
{CHUNKER_FLOG
	return (*_iterInner)->getLabel(); 
}

bool TranscriptionIterator::done() const 
{CHUNKER_FLOG
	return _iterOuter == _done; 
}



std::deque<RecognitionPhone>::const_iterator RecognitionTokenIterator::cbegin() const 
{CHUNKER_FLOG
	return _iterOuter->cbegin(); 
}

std::deque<RecognitionPhone>::const_iterator RecognitionTokenIterator::cend() const 
{CHUNKER_FLOG 
	return _iterOuter->cend(); 
}

RecognitionTokenIterator::RecognitionTokenIterator(const RecognitionTokenIterator & start, const RecognitionTokenIterator & end) : 
	RecognitionIterator(start, end)
{CHUNKER_FLOG}

RecognitionTokenIterator::RecognitionTokenIterator(const std::deque<RecognitionToken> & anchor) : RecognitionIterator(anchor)
{CHUNKER_FLOG}

RecognitionTokenIterator::RecognitionTokenIterator(const RecognitionTokenIterator& other) : RecognitionIterator(other)
{CHUNKER_FLOG}

RecognitionPhoneIterator::RecognitionPhoneIterator(const RecognitionPhoneIterator & start, const RecognitionPhoneIterator & end) : 
	RecognitionIterator(start, end)
{CHUNKER_FLOG
	_iterInner = _iterOuter->cbegin();
}

RecognitionPhoneIterator::RecognitionPhoneIterator(const std::deque<RecognitionToken> & anchor) : RecognitionIterator(anchor)
{CHUNKER_FLOG
	_iterInner = _iterOuter->cbegin();
}

RecognitionPhoneIterator::RecognitionPhoneIterator(const RecognitionPhoneIterator& other) : RecognitionIterator(other)
{CHUNKER_FLOG
	_iterInner = other._iterInner;
}

long RecognitionTokenIterator::getStartTime() const 
{CHUNKER_FLOG
	return _iterOuter->getStart(); 
}


long RecognitionPhoneIterator::getStartTime() const 
{CHUNKER_FLOG
	return _iterInner->getStart(); 
}

const std::string& RecognitionTokenIterator::getUnit() const 
{CHUNKER_FLOG
	return _iterOuter->getLabel(); 
}

const std::string& RecognitionPhoneIterator::getUnit() const 
{CHUNKER_FLOG
	return _iterInner->getLabel(); 
}

long RecognitionTokenIterator::getEndTime() const 
{CHUNKER_FLOG
	return _iterOuter->getEndTime(); 
}

long RecognitionPhoneIterator::getEndTime() const 
{CHUNKER_FLOG
	return _iterInner->getEndTime(); 
}

BaseIterator& RecognitionTokenIterator::operator ++() 
{CHUNKER_FLOG
	++ _iterOuter; 
	if(show()) return *this;
	return ++(*this);
}

BaseIterator& RecognitionPhoneIterator::operator ++ ()
{CHUNKER_FLOG
	_iterInner++;
	if(_iterInner == _iterOuter->cend())
	{
		_iterOuter++;
		if(! done())
		{
			_iterInner = _iterOuter->cbegin();
		}
	}
	if(show()) return *this;
	return ++(*this);
}
	
RecognitionIterator::RecognitionIterator(const RecognitionIterator & start, const RecognitionIterator & end) : 
	RecognitionIterator(start._iterOuter, end._iterOuter)
{CHUNKER_FLOG}

RecognitionIterator::RecognitionIterator(const std::deque<RecognitionToken> & anchor) :
	RecognitionIterator(anchor.cbegin(), anchor.cend())
{CHUNKER_FLOG}

RecognitionIterator::RecognitionIterator(const RecognitionIterator & other) :
	RecognitionIterator(other._iterOuter, other._done)
{CHUNKER_FLOG
	_showInactive = other._showInactive;
}

RecognitionIterator::RecognitionIterator(const std::deque<RecognitionToken>::const_iterator begin,
		const std::deque<RecognitionToken>::const_iterator end)
{CHUNKER_FLOG
	_iterOuter = begin;
	_done = end;
}

long RecognitionIterator::getIndex() const 
{CHUNKER_FLOG
	return _iterOuter->getIndex(); 
}

bool RecognitionIterator::done() const 
{CHUNKER_FLOG
	return _iterOuter == _done; 
}

std::unique_ptr<BaseIterator> RecognitionTokenIterator::copyAsBaseIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<BaseIterator>(new RecognitionTokenIterator(*this));
}

std::unique_ptr<BaseIterator> RecognitionPhoneIterator::copyAsBaseIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<BaseIterator>(new RecognitionPhoneIterator(*this));
}

std::shared_ptr<BaseIterator> RecognitionTokenIterator::copyAsBaseIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<BaseIterator>(new RecognitionTokenIterator(*this));
}

std::shared_ptr<BaseIterator> RecognitionPhoneIterator::copyAsBaseIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<BaseIterator>(new RecognitionPhoneIterator(*this));
}

std::unique_ptr<RecognitionIterator> RecognitionTokenIterator::copyAsRecognitionIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<RecognitionIterator>(new RecognitionTokenIterator(*this));
}

std::unique_ptr<RecognitionIterator> RecognitionPhoneIterator::copyAsRecognitionIterator() const
{CHUNKER_FLOG
	return std::unique_ptr<RecognitionIterator>(new RecognitionPhoneIterator(*this));
}

std::shared_ptr<RecognitionIterator> RecognitionTokenIterator::copyAsRecognitionIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<RecognitionIterator>(new RecognitionTokenIterator(*this));
}

std::shared_ptr<RecognitionIterator> RecognitionPhoneIterator::copyAsRecognitionIteratorShared() const
{CHUNKER_FLOG
	return std::shared_ptr<RecognitionIterator>(new RecognitionPhoneIterator(*this));
}

UniqueIterator::UniqueIterator(std::unique_ptr<TranscriptionIterator> iter)
{CHUNKER_FLOG
	_iter = std::move(iter);
	_seenUnits.emplace(_iter->getUnit());
}
	
std::deque<TranscriptionPhoneme *> UniqueIterator::getPhonemes() const 
{CHUNKER_FLOG
	return _iter->getPhonemes();
}

std::string UniqueIterator::getLMRepresentation() const
{CHUNKER_FLOG
	return ChunkerUtil::SPECIALSTRING + _iter->getUnit() + ChunkerUtil::SPECIALSTRING + 
		_iter->getUnit() + ChunkerUtil::SPECIALSTRING + "-1";
}

UniqueIterator& UniqueIterator::operator ++()
{CHUNKER_FLOG
	while(true)
	{
		++(*_iter);
		
		if(_iter->done())
		{
			CHUNKER_LOG4 << "Iter is done" CHUNKER_ENDLOG
			_done = true;
			break;
		}
		if(_seenUnits.emplace(_iter->getUnit()).second)
		{
			CHUNKER_LOG4 << "Unknown unit" CHUNKER_ENDLOG
			break;
		}
		CHUNKER_LOG4 << "Iter at " CHUNKER_ENDLOG
		CHUNKER_LOG4 << _iter->getUnit() << " " << _iter->getIndex() CHUNKER_ENDLOG
	}
}

const std::string & UniqueIterator::getUnit() const
{CHUNKER_FLOG
	return _iter->getUnit();
}

bool UniqueIterator::done() const
{CHUNKER_FLOG 
	return _done; 
}
