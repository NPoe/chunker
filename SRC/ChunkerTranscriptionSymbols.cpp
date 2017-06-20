#include "ChunkerGlobal.hpp"

const std::string & TranscriptionPhoneme::getLabel() const 
{CHUNKER_FLOG
	return _label; 
}

TranscriptionPhoneme::TranscriptionPhoneme(const std::string & label)
{CHUNKER_FLOG
	_label = label;
}

const std::string & TranscriptionTerm::getLabel() const 
{CHUNKER_FLOG 
	return _label;
}

std::deque<TranscriptionPhoneme *>::const_iterator TranscriptionTerm::cbegin() 
{CHUNKER_FLOG
	return _segmentation.cbegin();
}

std::deque<TranscriptionPhoneme *>::const_iterator TranscriptionTerm::cend() 
{CHUNKER_FLOG 
	return _segmentation.cend();
}

bool TranscriptionTerm::active() const
{CHUNKER_FLOG
	if(_segmentation.empty()) return false;
	if(_label.empty()) return false;
	if(_label == ChunkerUtil::PAUSESTRING) return false;
	if(_label.find(ChunkerUtil::TAGSTART) == 0 &&
			_label.rfind(ChunkerUtil::TAGEND) == (_label.length() - ChunkerUtil::TAGEND.length())) return false;
	return true;
}

bool TranscriptionToken::active() const
{CHUNKER_FLOG
	return _term->active();
}

TranscriptionTerm::TranscriptionTerm(std::string label, std::deque<TranscriptionPhoneme *> segmentation)
{CHUNKER_FLOG
	_label = label;
	_segmentation = segmentation;
}

std::deque<TranscriptionPhoneme *> TranscriptionTerm::getPhonemes() const 
{CHUNKER_FLOG
	return _segmentation;
}

std::deque<TranscriptionPhoneme *> TranscriptionToken::getPhonemes() const 
{CHUNKER_FLOG
	return _term->getPhonemes(); 
}

long int TranscriptionToken::getIndex() const 
{CHUNKER_FLOG 
	return _index; 
}

const std::string & TranscriptionToken::getOrt() const
{CHUNKER_FLOG
	return _ort;
}

const std::string & TranscriptionToken::getLabel() const 
{CHUNKER_FLOG 
	return _term->getLabel(); 
}

std::deque<TranscriptionPhoneme *>::const_iterator TranscriptionToken::cbegin() const 
{CHUNKER_FLOG 
	return _term->cbegin(); 
}
	
std::deque<TranscriptionPhoneme *>::const_iterator TranscriptionToken::cend() const 
{CHUNKER_FLOG
	return _term->cend();
}

TranscriptionToken::TranscriptionToken(long int index, const std::string & ort, TranscriptionTerm * term)
{CHUNKER_FLOG
	_index = index;
	_term = term;
	_ort = ort;
}
