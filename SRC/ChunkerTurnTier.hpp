#ifndef CHUNKER_TURNTIER_HPP
#define CHUNKER_TURNTIER_HPP

/** \class TurnTierItem
 *  \brief Represents a single turn tier segment
 */
class TurnTierItem
{
private:
	long _start; /**< Start in samples */
	long _duration; /**< Duration in samples */
	std::deque<std::pair<long, std::string> > _tokens; /**< Ordered list of tokens (token = BPF index-string pair) */

public:
	/**
	 * \brief Constructor
	 * \param start Start in samples
	 * \param duration Duration in samples
	 * \param tokens Orderd list of iterators on tokens
	 */
	TurnTierItem(long, long, const std::deque<TranscriptionTokenIterator> &);
	/**
	 * \brief Getter for turn duration
	 * \returns duration
	 */
	long getDuration() const;

	/**
	 * \brief Print this turn to BPF (as a single TRN line)
	 * \param stream Writeable output stream
	 */
	void print(std::ostream &) const;
	int getNumberOfTokens() const;
};

class TurnTier
{
private:
	long _samplingRate; /**< Audio sampling rate (for conversion) */
	long _duration; /**< Total duration of audio file */
	std::vector<TurnTierItem> _items; /**< List of all turn items */

	/**
	 * \brief Add a new turn
	 * \param start turn start in samples
	 * \param dur turn duration in samples
	 * \param tokens List of iterators on tokens that this turn will hold
	 *
	 * \note Turn will only be added if either maxchunkduration == 0, or if the turn is short enough
	 */
	void addItem(long, long, const std::deque<TranscriptionTokenIterator> &);
public:
	/**
	 * \brief Constructor
	 * \param duration Total audio duration
	 * \param samplingRate Original audio sampling rate
	 */
	TurnTier(long, long);

	/**
	 * \brief Derive and build turn tier after chunking
	 *
	 * \param boundaries Set of all boundaries, orderd chronologically
	 * \param tokenIter Transcription iterator
	 */
	void build(const std::set<Boundary, Boundary::orderFinal> &, TranscriptionTokenIterator);

	/**
	 * \brief Print the entire TRN tier to BPF stream
	 * \param stream Writeable BPF stream
	 */
	void print(std::ostream &) const;

	/**
	 * \brief Check if the TRN tier contains at least one segment that is longer than MAUS's limit (3000 tokens)
	 * \returns boolean indicating whether there is an overly long segment
	 */
	bool checkLength() const;
};

/**
 * \brief Print turn tier item to stream
 * \param stream BPF stream
 * \param item Reference to turn tier item
 * \returns stream
 */
std::ostream & operator << (std::ostream &, const TurnTierItem &);

/**
 * \brief Print entire turn tier to stream
 * \param stream BPF stream
 * \param item Reference to turn tier
 * \returns stream
 */
std::ostream & operator << (std::ostream &, const TurnTier &);

#endif
