class Anchor
{
private:
	std::deque<const AlignPoint * > _anchor;
	const UnigramProbabilities * _unigramProbs;
	int _numSingletons = 0;
	long _longestPause = 0;
public:
	std::deque<const AlignPoint *>::const_iterator cbegin() const;
	std::deque<const AlignPoint *>::const_iterator cend() const;
	
	void addAlignPoint(const AlignPoint &);
	Anchor(const UnigramProbabilities *);
	unsigned size() const;
	int getNumSingletons() const;
	long getLongestPause() const;
	long getStartTime() const;
	bool operator < (const Anchor & other) const;
};
