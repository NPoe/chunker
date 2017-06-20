#include "ChunkerGlobal.hpp"


bool RecognitionToken::active() const
{CHUNKER_FLOG
	if(_segmentation.empty()) return false;
	if(_label.empty()) return false;
	if(_label == ChunkerUtil::PAUSESTRING) return false;
	if(_label.find(ChunkerUtil::TAGSTART) == 0 &&
			_label.rfind(ChunkerUtil::TAGEND) == (_label.length() - ChunkerUtil::TAGEND.length())) return false;
	return true;
}


const std::string & RecognitionPhone::getLabel() const 
{CHUNKER_FLOG
	return _label; 
}

long int RecognitionPhone::getStart() const 
{CHUNKER_FLOG
	return _start; 
}

long int RecognitionPhone::getEndTime() const 
{CHUNKER_FLOG 
	return _end; 
}

RecognitionPhone::RecognitionPhone(long int start, long int end, std::string label)
{CHUNKER_FLOG
	_start = start;
	_end = end;
	_label = label;
}

long RecognitionToken::getIndex() const
{CHUNKER_FLOG
	return _index;
}

const std::string & RecognitionToken::getLabel() const
{CHUNKER_FLOG
	return _label; 
}

const std::string & RecognitionToken::getOrt() const
{CHUNKER_FLOG
	return _ort; 
}

long int RecognitionToken::getStart() const 
{CHUNKER_FLOG 
	return _segmentation.cbegin()->getStart(); 
}

long int RecognitionToken::getEndTime() const 
{CHUNKER_FLOG 
	return _segmentation.crbegin()->getEndTime();
}

std::deque<RecognitionPhone>::const_iterator RecognitionToken::cbegin() const 
{CHUNKER_FLOG 
	return _segmentation.cbegin(); 
}

std::deque<RecognitionPhone>::const_iterator RecognitionToken::cend() const 
{CHUNKER_FLOG 
	return _segmentation.cend();
}

RecognitionToken::RecognitionToken(std::string label, std::string ort, std::deque<RecognitionPhone> segmentation, long index)
{CHUNKER_FLOG
	_label = label;
	_ort = ort;
	_segmentation = segmentation;
	_index = index;
}

