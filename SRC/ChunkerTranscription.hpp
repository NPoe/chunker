#ifndef CHUNKER_TRANSCRIPTION_HPP
#define CHUNKER_TRANSCRIPTION_HPP

/**	\class Transcription
 * 	\brief Class containing the transcription for a specific chunker
 *
 *	In the root chunker, this object holds the full original transcription.
 *	In child chunkers, it holds a subsequence of the transcription.
 */
class Transcription
{
private:

	/** \brief The actual transcription 
	 * 
	 * Maps token IDs (from BPF) onto token objects.
	 */
	std::map<long int, TranscriptionToken> _transcript;

	/** Needed to order labels by length in _labelSet and _exceptionSet */
	struct orderByLength { bool operator() (const std::string &, const std::string &); }; 

	std::set<std::string, orderByLength> _labelSet; /** set of all known phonemes */
	std::set<std::string, orderByLength> _exceptionSet; /** set of symbols that should be ignored (e.g. '#') */
	std::map<std::string, TranscriptionTerm> _terms; /** maps the strings (names) of unique word types to their objects */
	std::map<std::string, TranscriptionPhoneme> _phonemes; /** maps the strings (names) of unique phonemes to their objects */
	std::map<long, std::string> _ortMap; /** if an ORT tier is present, this map maps token IDs to their orthographical form*/

	/** \brief Extract ORT (orthography) tier from BPF
	 *
	 * The ORT tier's sole purpose is a pretty output (i.e. orthographical forms in the TRN tier).
	 * If it is not present canonical forms are used instead.
	 */
	void extractORTTier();
	void parseORTLine(const std::string&, const std::string&);

	std::string escapePhoneme(const std::string&);
	/** \brief Extract KAN (canonical) tier from BPF */
	void extractKANTier();
	void parseKANLine(const std::string&, const std::string&);

	void segmentWithBlanks(const std::vector<std::string> &, std::string *, std::deque<TranscriptionPhoneme *> *);
	void segmentWithoutBlanks(const std::string &, std::deque<TranscriptionPhoneme * > *);

public:
	/** \brief Returns an interator pointing to the transcription's first token */
	TranscriptionTokenIterator getTokenIterator() const;
	
	/** \brief Returns an interator pointing to the transcription's first phone */
	TranscriptionPhoneIterator getPhoneIterator() const;
	
	long maxIndex() const;	
	long minIndex() const;	
	
	/** \brief Returns an interator for the language model (LM)
	 *
	 * This may be a phone or token iterator, depending on whether the current chunker has a word or phoneme LM.
	 */
	std::unique_ptr<TranscriptionIterator> languageModelIterator(bool) const;
	
	/** \brief Returns an interator for the symbolic alignment process
	 *
	 * This may be a phone or token iterator, depending on whether the current chunker aligns words or phonemes.
	 */
	std::unique_ptr<TranscriptionIterator> alignmentIterator(bool) const;
	
	/** \brief Constructor for empty transcription (root transcription only) */
	Transcription ();

	/** \brief Constructor for transcription created from a slice of another transcription */
	Transcription (const Transcription &, long int, long int);

	/** \brief Print the dictionary mapping phonemes to HMM models to stream */
	void printDictionary(std::ostream &, unsigned) const;		
	
	/** \brief Print a list of all necessary HMM models to stream */
	void printHMMList(std::ostream &, unsigned) const;		
};

#endif
