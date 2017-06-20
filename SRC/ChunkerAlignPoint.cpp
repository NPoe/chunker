#include "ChunkerGlobal.hpp"

unsigned AlignPoint::getCost() const 
{CHUNKER_FLOG
	// if this is a deletion || or an insertion || or a non-zero substitution
	if(_transIter == NULL || _recogIter == NULL || _transIter->getUnit() != _recogIter->getUnit())
	{
		return 1;
	}

	return 0;
}

const std::string & AlignPoint::getTranscriptionUnit() const 
{CHUNKER_FLOG
	if(_transIter != NULL)
	{
		return _transIter->getUnit(); 
	}
	return ChunkerUtil::EMPTYSTRING;
}

const std::string & AlignPoint::getTranscriptionToken() const
{CHUNKER_FLOG
	if(_transIter != NULL)
	{
		return _transIter->getToken();
	}
	return ChunkerUtil::EMPTYSTRING;
}

const std::string & AlignPoint::getRecognitionUnit() const 
{CHUNKER_FLOG
	if(_recogIter != NULL)
	{
		return _recogIter->getUnit();
	}
	return ChunkerUtil::EMPTYSTRING;
}

long AlignPoint::getIndex() const 
{CHUNKER_FLOG
	if(_transIter != NULL)
	{
		return _transIter->getIndex(); 
	}
	return -1;
}

long AlignPoint::getStartTime() const 
{CHUNKER_FLOG
	if(_recogIter != NULL)
	{
		return _recogIter->getStartTime(); 
	}
	return -1;
}

long AlignPoint::getEndTime() const 
{CHUNKER_FLOG
	if(_recogIter != NULL)
	{
		return _recogIter->getEndTime(); 
	}
	return -1;
}

AlignPoint::AlignPoint(std::shared_ptr<BaseIterator> iter1, std::shared_ptr<BaseIterator> iter2) 
{CHUNKER_FLOG
	// If we have through some error ended up aligning transcription segments with transcription segments
	// or recognition result segments with recognition result segments
	if
	(
		std::dynamic_pointer_cast<TranscriptionIterator>(iter1) && std::dynamic_pointer_cast<TranscriptionIterator>(iter2) ||
		std::dynamic_pointer_cast<RecognitionIterator>(iter1) && std::dynamic_pointer_cast<RecognitionIterator>(iter2)
	)
	{
		CHUNKER_ERR << "Align one transcript iterator and one recognition iterator" CHUNKER_ENDERR4
	}

	// Find out which of the base iterators is which and assign _transIter and _recogIter accordingly
	
	if(std::dynamic_pointer_cast<TranscriptionIterator>(iter1))
	{
		_transIter = std::static_pointer_cast<TranscriptionIterator>(iter1);
	}
	else if(std::dynamic_pointer_cast<RecognitionIterator>(iter1))
	{
		_recogIter = std::static_pointer_cast<RecognitionIterator>(iter1);
	}
	
	if(std::dynamic_pointer_cast<TranscriptionIterator>(iter2))
	{
		_transIter = std::static_pointer_cast<TranscriptionIterator>(iter2);
	}
	else if(std::dynamic_pointer_cast<RecognitionIterator>(iter2))
	{
		_recogIter = std::static_pointer_cast<RecognitionIterator>(iter2);
	}
	
	CHUNKER_LOG3 << "New align point: " << getTranscriptionUnit() << " " << getIndex() << " " << getRecognitionUnit() << " " << getStartTime() CHUNKER_ENDLOG
}
