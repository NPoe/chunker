#ifndef CHUNKER_RECOGNITION_HPP
#define CHUNKER_RECOGNITION_HPP

/**	\class Recognition
 * 	\brief Class containing the recognition result for a specific chunker
 *
 *	In the root chunker, this object holds the recognition result for the entire recording.
 *	In child chunkers, it holds the recognition result for a subsequence of the recording.
 */
class Recognition
{
private:
	std::deque<RecognitionToken> _recognition;	

public:
	std::unique_ptr<RecognitionIterator> alignmentIterator(bool) const;
	Boundary getMiddle() const;

	RecognitionTokenIterator getTokenIterator() const;
	RecognitionPhoneIterator getPhoneIterator() const;
	void build (const std::string &, long);
};



#endif
