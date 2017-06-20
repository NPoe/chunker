#ifndef CHUNKER_BOUNDARIES_HPP
#define CHUNKER_BOUNDARIES_HPP


class Boundary
{
private:
	long _nextIndex; 
	long _boundaryTime;
	long _pauseDuration;
	int _anchorDuration;
	bool _isEdge;

public:
	Boundary(long, long, long, int, bool);
	
	struct orderFinal { bool operator () (const Boundary &, const Boundary &); };
	struct orderPriorityPauseDuration { bool operator() (const Boundary &, const Boundary &); };
	struct orderPriorityAnchorLength { bool operator() (const Boundary &, const Boundary &); };
	
	int getAnchorLength() const;
	long getPauseDuration() const;
	long getBoundaryTime() const;
	long getNextIndex() const;
	bool getEdge() const;
	
};


#endif
