#ifndef CHUNKER_BOUNDARIES_HPP
#define CHUNKER_BOUNDARIES_HPP

/**
 * \class Boundary
 *
 * \brief Represents a boundary between adjacent chunks
 */
class Boundary
{
private:
	long _nextIndex; /**< Index of next token according to BPF */
	long _boundaryTime; /**< Time of boundary in 0.0000001 seconds (HTK) */
	long _pauseDuration; /**< Duration of boundary silence in 0.0000001 seconds (HTK) */
	int _anchorDuration; /**< Length of original anchor in tokens (T chunker) or phones (P chunker) */
	bool _isEdge; /**< True if this boundary is either the beginning or the end of the full signal */

public:
	/**
	 * \brief Constructor
	 *
	 * \param boundaryTime time ouf boundary in 0.0000001 seconds
	 * \param nextIndex index of next token according to BPF
	 * \param pauseDuration duration of boundary silence in 0.0000001 seconds
	 * \param anchorDuration length of original anchor in tokens (T chunker) or phones (P chunker)
	 * \param isEdge this bool is true if the boundary is either the beginning or the end of the full signal
	 */
	Boundary(long, long, long, int, bool);
	
	/**
	 * \brief final chronological order of boundaries
	 *
	 * \note the operator returns 0 if two boundaries are too close together, hence only one of them is kept
	 */
	struct orderFinal { bool operator () (const Boundary &, const Boundary &); };
	
	/**
	 * \brief order of boundaries that prioritizes silence duration
	 */
	struct orderPriorityPauseDuration { bool operator() (const Boundary &, const Boundary &); };
	
	/**
	 * \brief order of boundaries that prioritizes anchor length
	 */
	struct orderPriorityAnchorLength { bool operator() (const Boundary &, const Boundary &); };
	
	/**
	 * \brief getter for anchor length
	 * \returns length of original anchor, in tokens or phones
	 */
	int getAnchorLength() const;

	/**
	 * \brief getter for pause duration
	 * \returns duration of boundary silence in 0.0000001 seconds
	 */
	long getPauseDuration() const;

	/**
	 * \brief getter for boundary time
	 * \returns time of boundary (mid-point of silence) in 0.0000001 seconds
	 */
	long getBoundaryTime() const;

	/**
	 * \brief getter for next BPF index
	 * \returns index of next BPF token
	 */
	long getNextIndex() const;

	/**
	 * \brief returns true if this boundary is the beginning or end of the signal, else false
	 * \returns bool
	 */
	bool getEdge() const;
	
};


#endif
