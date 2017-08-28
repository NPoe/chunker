#ifndef CHUNKER_HPP
#define CHUNKER_HPP

/**	\class Chunker
 * 	\brief Class responsible for chunking a specific audio -- transcription pair.
 *
 *	A chunker object is responsible for a specific part of the audio and a specific part of the transcription.
 *	In all cases, there will be a root chunker responsible for the full audio and the full transcription.
 *	The "root chunker" may spawn child chunkers, which are responsible for subsequences, and they may spawn
 *	child chunkers themselves, resulting in a recursive tree structure.
 */
class Chunker
{
private:
	bool BPFcontainsKey(const std::string &) const;
	void saveBPFFile() const;

protected:
	
        std::set<Anchor> _anchors;

	Transcription _transcription;
	Recognition _recognition;
	Chunker * _parent = NULL;

	bool _stage;
	bool _onlyChild = false;

	std::shared_ptr<std::set<Boundary, Boundary::orderPriorityPauseDuration> > _silenceOnlyPointer;
	std::shared_ptr<ProgressBar> _progressBarPointer;

	unsigned _depth;
	unsigned _childID;	
	long _offset; 
	long _duration; 
	std::string _prefix; 	
	std::set<Boundary, Boundary::orderFinal> _splitPoints; 
	std::deque<AlignPoint> _path;
	
	void resample();

	void prepare();
	virtual void findBoundaries() = 0;
	std::shared_ptr<std::set<Boundary, Boundary::orderPriorityPauseDuration> > getSilenceOnlyPointer();
	std::shared_ptr<ProgressBar> getProgressBarPointer();
	const Transcription & getTranscription() const;
	const Recognition & getRecognition() const;
	const Chunker * getRootChunker() const;
	void writeOriginalBPFHeader(std::ostream &) const;
	void writeOriginalBPFBody(std::ostream &) const;
	void obtainHTKFile();
	void recognizer();
	virtual bool recursionNeeded() const = 0;
	bool getChildStage(long) const;
	void collectModels(std::set<std::string> * const) const;
	void writeHCopyConfig();
	void recursion();

	virtual void displayFeatures() const = 0;
	virtual void writeLanguageModel() = 0;
	virtual std::vector<std::pair<long, long> > getIntervals() const = 0;
	virtual std::unique_ptr<Chunker> spawnChild(const Boundary &, const Boundary &, int) = 0;
	virtual std::unique_ptr<TurnTier> buildTurnTier() const = 0;
	
	Chunker(Chunker *, const Boundary &, const Boundary &, int, bool);	/** Private constructor for child chunkers */

public:
	const std::string & getPrefix() const;
	int getDepth() const;
	int getChildID() const;
	bool getStage() const;
	const Chunker * getParent() const;
	Chunker();
	void run();
	void writeOutput() const;
};

/**	\class ChunkerClassic
 * 	\brief Classical chunker that works with loop language or phonotactic model.
 *
 *	A classic chunker performs recognition using a word or phoneme loop or n-gram model.
 *	This chunker may refuse to find any chunks in cases of transcription errors, bad audio quality etc.
 *	It is however less likely "force" alignment errors in these cases.
 */

class ChunkerClassic : public Chunker
{
private:

	void findBoundaries();
	bool recursionNeeded() const;
	void alignment();
	
	void writeLanguageModel();
	void displayFeatures() const;
	void getCandidateAnchors();
	std::vector<std::pair<long, long> > getIntervals() const;
	
	ChunkerClassic(ChunkerClassic *, const Boundary &, const Boundary &, int, bool);
	std::unique_ptr<Chunker> spawnChild(const Boundary &, const Boundary &, int);
public:
	virtual std::unique_ptr<TurnTier> buildTurnTier() const;
	ChunkerClassic(); /** Public constructor for the root chunker */	
};

/**	\class ChunkerForced
 * 	\brief Forced chunker that works with a factor automaton language model.
 *
 *	A forced chunker performs recognition using a factor automaton language model.
 *	Since factor automata accept all infixes of the target string, this guarantees a recognition result.
 *	However, the forced method is prone to incorrectly "finding" text in places where there is background
 *	noise, music, untranscribed speech etc. It is also slower than the classical chunker.
 *	Therefore, it is run in a "logarithmic" fashion. This means that the root chunker processes a short
 *	slice (e.g. two minutes) from the middle of the signal. When the corresponding transcription has been
 *	found, the long audio file can be split in the middle. In the next step, two child chunkers are run
 *	on the second level, four on the third level, and so forth.
 */

class ChunkerForced : public Chunker
{
private:
	void findBoundaries();
	bool recursionNeeded() const;
	void writeLanguageModel();
	void displayFeatures() const;
	std::unique_ptr<Chunker> spawnChild(const Boundary &, const Boundary &, int);
	std::vector<std::pair<long, long> > getIntervals() const;
	
	ChunkerForced(ChunkerForced *, const Boundary &, const Boundary &, int, bool);	
public:
	virtual std::unique_ptr<TurnTier> buildTurnTier() const;
	ChunkerForced(); /** Public constructor for the root chunker */
};


#endif
