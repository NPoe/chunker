#ifndef CHUNKER_TURNTIER_HPP
#define CHUNKER_TURNTIER_HPP


class TurnTierItem
{
private:
	long _start;
	long _duration;
	std::deque<std::pair<long, std::string> > _tokens;

public:
	TurnTierItem(long, long, const std::deque<TranscriptionTokenIterator> &);
	long getDuration() const;
	void print(std::ostream &) const;
	int getNumberOfTokens() const;
};

class TurnTier
{
private:
	long _samplingRate;
	long _duration;
	std::vector<TurnTierItem> _items;
	void addItem(long, long, const std::deque<TranscriptionTokenIterator> &);
public:
	TurnTier(long);
	void build(const std::set<Boundary, Boundary::orderFinal> &, TranscriptionTokenIterator);
	void print(std::ostream &) const;
	bool checkLength() const;
};

std::ostream & operator << (std::ostream &, const TurnTierItem &);

std::ostream & operator << (std::ostream &, const TurnTier &);

#endif
