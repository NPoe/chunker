#ifndef CHUNKER_LANGUAGEMODEL_HPP
#define CHUNKER_LANGUAGEMODEL_HPP

/**
 * \class LanguageModel
 *
 * \brief Base class for all language models
 */
class LanguageModel
{
protected:
	std::deque<std::tuple<unsigned, std::string> > _nodes; /**< List of all nodes */
	std::deque<std::tuple<unsigned, unsigned, std::string> > _links; /**< List of all links beteen nodes */

	std::unique_ptr<UnitModeller> _modeller; /**< Modeller being used to model types / phonemes */
public:
	/**
	 * \brief Add new node to language model
	 *
	 * \param idx Node index in language model
	 * \param label Node's label (must be in left column of DICT)
	 */
	void addNode(unsigned, const std::string &); 
	
	/**
	 * \brief Add new link to language model
	 *
	 * \param from Source node index
	 * \param to Target node index
	 * \param prob Link probability, as returned by wordvar (or empty)
	 */
	void addLink(unsigned, unsigned, std::string);

	/**
	 * \brief Print language model as HTK lattice
	 *
	 * \param stream Write-handle to lattice file
	 */
	void print(std::ostream &) const;

	/**
	 * \brief Constructor
	 *
	 * \param modeller Unit modeller that will be used to model types / phonemes
	 */
	LanguageModel(std::unique_ptr<UnitModeller>);
};

/**
 * \class LMHTK
 * \brief Language model that uses HTK's inbuilt HLStats and HBuild functions
 * \deprecated
 */
class LMHTK : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 * \param prefix File system prefix for temporary files (same as that of the calling chunker object)
	 */
	LMHTK(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>, const std::string &);
};

/**
 * \class LMZerogram
 * \brief Simple cyclical language model that gives equal probability to all units (types / phonemes)
 *
 * Currently used as default for phoneme-based language models
 */
class LMZerogram : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMZerogram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMUnigram
 * \brief Simple cyclical language model that gives units a probability that is proportionat to their unigram probabilities
 * \deprecated
 */
class LMUnigram : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMUnigram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMBigram
 * \brief Cyclical language model that gives units a probability that is proportionat to their smoothed bigram probabilities
 *
 * Currently used as default for type-based language models
 */
class LMBigram : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMBigram(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMPair
 * \brief Cyclical language model featuring unit pairs
 * \deprecated
 */
class LMPair : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMPair(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMTriple
 * \brief Cyclical language model featuring unit triples
 * \deprecated
 *
 * Slow!
 */
class LMTriple : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMTriple(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMFactorAutomaton
 * \brief Factor automaton language model. Used for forced chunking.
 */
class LMFactorAutomaton : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMFactorAutomaton(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \class LMForcedAlign
 * \brief Alignment language model (equivalent to what MAUS is doing). Used in the last phase of forced chunking.
 */
class LMForcedAlign : public LanguageModel
{
public:
	/**
	 * \brief Constructor
	 * \param iter Iterator over all units (tokens / phones) in transcription
	 * \param modeller Responsible for modelling tokens / phones internally
	 */
	LMForcedAlign(std::unique_ptr<TranscriptionIterator>, std::unique_ptr<UnitModeller>);
};

/**
 * \brief Print a given language model as HTK lattice.
 *
 * \param stream Write-stream to HTK lattice file
 * \param lm Language model to be printed
 */
std::ostream & operator << (std::ostream &, const LanguageModel &);
#endif
