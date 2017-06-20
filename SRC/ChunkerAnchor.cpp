#ifndef CHUNKER_ANCHOR_HPP
#define CHUNKER_ANCHOR_HPP

#include "ChunkerGlobal.hpp"

std::deque<const AlignPoint *>::const_iterator Anchor::cbegin() const
{
	return _anchor.cbegin();
}

std::deque<const AlignPoint *>::const_iterator Anchor::cend() const
{
	return _anchor.cend();
}

void Anchor::addAlignPoint(const AlignPoint & alignPoint)
{
	if(alignPoint.getCost() == 0 && !(_anchor.empty()))
	{
		if(_anchor.back()->getCost() == 0 && alignPoint.getIndex() != _anchor.back()->getIndex())
		{
			long pause = alignPoint.getStartTime() - _anchor.back()->getEndTime();
			if(pause > _longestPause)
			{
				_longestPause = pause;
			}
		}
	}
	
	if(_anchor.empty() || _anchor.back()->getIndex() != alignPoint.getIndex())
	{
		if(_unigramProbs->getUnigramCount(alignPoint.getTranscriptionToken()) == 1)
		{
			_numSingletons += 1;
		}
	}

	_anchor.push_back(&alignPoint);
}

Anchor::Anchor(const UnigramProbabilities * unigramprobs)
{
	_unigramProbs = unigramprobs;
}

unsigned Anchor::size() const
{
	return _anchor.size();
}	

int Anchor::getNumSingletons() const
{
	return _numSingletons;
}	

long Anchor::getLongestPause() const
{
	return _longestPause;
}

long Anchor::getStartTime() const
{
	return _anchor.front()->getStartTime();
}	

bool Anchor::operator < (const Anchor & other) const
{

	if(size() != other.size())
	{ 
		return size() > other.size(); 
	}

	if(getLongestPause() != other.getLongestPause())
	{ 
		return getLongestPause() > other.getLongestPause();
	}
	
	return getStartTime() > other.getStartTime();
}

#endif
