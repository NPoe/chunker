#ifndef CHUNKER_UNITMODELLER_HPP
#define CHUNKER_UNITMODELLER_HPP

class LanguageModel;

/**
 * \class UnitModeller
 * \brief Abstract base class for modelling units (i.e. terms or phonemes) in the language model.
 *
 * Can be realized as a classic (linear) modeller or MAUS modeller.
 * If the language model is phoneme-based, there is no difference between the two possibilities.
 */
class UnitModeller
{
protected:
	LanguageModel * _lm; /** Pointer to the language model that is using this modeller */

	/**
	 * \brief abstract version of the inner modelling function, implemented by child classes
	 * 
	 * \abstract
	 * \param in Index where the current unit begins in the HTK lattice
	 * \param unit LM representation (with leading and trailing ___) of current unit (phoneme / type)
	 * \param segmentation Linear sequence of phonemes (or one phoneme) corresponding to current unit
	 * \return Index where the current unit exits in the HTK lattice
	 */
	virtual unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const = 0;
public:
	/**
	 * \brief set pointer to the calling language model
	 * 
	 * \param lm Language model
	 */
	void setLM(LanguageModel *);

	/**
	 * \brief Interface used for cyclical language model
	 *
	 * \param in Index where the current unit begins in the HTK lattice
	 * \param iter Iterator pointing to next unique unit (type, phoneme) in the transcription
	 * \return Index where the current unit exits in the HTK lattice
	 */
	unsigned modelUnit(unsigned, const UniqueIterator &) const;
	
	/**
	 * \brief Interface used for factor automaton language model
	 *
	 * \param in Index where the current unit begins in the HTK lattice
	 * \param iter Iterator pointing to next unit (token, phone) in the transcription
	 * \return Index where the current unit exits in the HTK lattice
	 */
	unsigned modelUnit(unsigned, const std::unique_ptr<TranscriptionIterator> &) const;
};

/**
 * \class UnitModellerClassic
 * \brief Models units as a linear sequence of phonemes.
 *
 * This unit modeller models terms as linear sequences of phonemes. It is equivalent to the mode=align parameter in MAUS.
 */
class UnitModellerClassic : public UnitModeller
{
private:
	/**
	 * \brief linear implementation of inner modelling function
	 *
	 * Models units (types, phonemes) as linear sequences of phoneme(s). 
	 *
	 * \param in Index where the current unit begins in the HTK lattice
	 * \param unit LM representation (with leading and trailing ___) of current unit (phoneme / type)
	 * \param segmentation Linear sequence of phonemes (or one phoneme) corresponding to current unit
	 * \return Index where the current unit exits in the HTK lattice
	 */
	unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const;
};

/**
 * \class UnitModellerMaus
 * \brief Models units as a MAUS acyclic pheneme graph (used with parameter --maus true).
 *
 * This unit modeller calls wordvar (MAUS) to determine the correct graph for a given unit.
 * As a result, terms are modelled as acyclic graphs with forks and junctions.
 * If the basic units are phonemes, there is no difference to the linear (classic) unit modeller.
 * So far, I have not found that MAUS modelling helps the chunker, therefore I do not recommend using it.
 */
class UnitModellerMaus : public UnitModeller
{
private:
	std::string _prefix; /**< File system prefix, same as for the chunker object that owns this modeller */
	
	/**
	 * \brief linear implementation of inner modelling function
	 *
	 * Models units (types, phonemes) as acyclical MAUS graph by calling wordvar 
	 *
	 * \param in Index where the current unit begins in the HTK lattice
	 * \param unit LM representation (with leading and trailing ___) of current unit (phoneme / type)
	 * \param segmentation Linear sequence of phonemes (or one phoneme) corresponding to current unit
	 * \return Index where the current unit exits in the HTK lattice
	 */
	unsigned modelUnit(unsigned, const std::string &, const std::deque<TranscriptionPhoneme *> &) const;

	/**
	 * \brief Run MAUS modelling exe for a particular sequence of phonemes
	 *
	 * \param segmentation Sequence of phonemes (corresponds to canonical pronunciation of term)
	 * Important: If the unit modeller models phonemes, the sequence contains a single phoneme.
	 */
	void runMaus(const std::deque<TranscriptionPhoneme *> &) const;

public:
	/**
	 * \brief Constructor.
	 *
	 * \param prefix File system prefix where temporary files can be stored
	 * */
	UnitModellerMaus(const std::string);

};
#endif
