#include "ChunkerGlobal.hpp"

Boundary::Boundary(long boundaryTime, long nextIndex, long pauseDuration, int anchorDuration, bool isEdge)
{CHUNKER_FLOG
	_nextIndex = nextIndex;
	_boundaryTime = boundaryTime;
	_pauseDuration = pauseDuration;
	_isEdge = isEdge;
	_anchorDuration = anchorDuration;
	CHUNKER_LOG3 << "Candidate chunk boundary at time: " << boundaryTime << " next index: " << nextIndex <<
		" pause duration: " << pauseDuration << " anchor duration: " << anchorDuration << " is edge: " << isEdge
		CHUNKER_ENDLOG
}
	

long Boundary::getBoundaryTime() const 
{CHUNKER_FLOG 
	return _boundaryTime; 
}

int Boundary::getAnchorLength() const
{CHUNKER_FLOG
	return _anchorDuration;
}

long Boundary::getNextIndex() const 
{CHUNKER_FLOG
	return _nextIndex;
}
	
bool Boundary::getEdge() const 
{CHUNKER_FLOG 
	return _isEdge; 
}

long Boundary::getPauseDuration() const
{CHUNKER_FLOG
	return _pauseDuration;
}

bool Boundary::orderFinal::operator () (const Boundary & first, const Boundary & second)
{
	if(first.getEdge() && second.getEdge())
	{
		return first.getBoundaryTime() < second.getBoundaryTime();
	}

	if(std::abs(first.getBoundaryTime() - second.getBoundaryTime()) <
			ChunkerManager::getOptionLong(ChunkerManager::MINCHUNKDURATION, 1) *
			ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR))
	{
		return 0;
	}

	return first.getBoundaryTime() < second.getBoundaryTime();
}

bool Boundary::orderPriorityAnchorLength::operator() (const Boundary & first, const Boundary & second)
{
	if (first.getEdge() != second.getEdge())
	{
		return first.getEdge();
	}
	
	if(first.getAnchorLength() != second.getAnchorLength())
	{
		return first.getAnchorLength() > second.getPauseDuration();
	}
					
	if(first.getPauseDuration() != second.getPauseDuration())
	{
		return first.getPauseDuration() > second.getPauseDuration();
	}

	return first.getBoundaryTime() < second.getBoundaryTime();
}

bool Boundary::orderPriorityPauseDuration::operator() (const Boundary & first, const Boundary & second)
{
	if (first.getEdge() != second.getEdge())
	{
		return first.getEdge();
	}
	
	if(first.getPauseDuration() != second.getPauseDuration())
	{
		return first.getPauseDuration() > second.getPauseDuration();
	}
	
	if(first.getAnchorLength() != second.getAnchorLength())
	{
		return first.getAnchorLength() > second.getPauseDuration();
	}				

	return first.getBoundaryTime() < second.getBoundaryTime();
}
