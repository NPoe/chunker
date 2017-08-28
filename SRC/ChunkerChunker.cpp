	#include "ChunkerGlobal.hpp"


/////////////////////////////////////////////////////////////////////
// CONSTRUCTORS /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


Chunker::Chunker() : _transcription(), _recognition() // Build transcript from original BPF
{CHUNKER_FLOG

	_prefix = ChunkerManager::getOptionString(ChunkerManager::WORKDIR) + ChunkerUtil::SEP + 
		std::to_string(time(NULL))+"_"+std::to_string(getpid()); // Unique working directory

	ChunkerUtil::createDir(_prefix);        
	
	_parent = NULL; 
	_depth = 0; 
	_offset = 0; 
	_onlyChild = false;

	resample();
	saveBPFFile();	
	
	_duration = ChunkerUtil::getAudioDuration(ChunkerManager::getOptionString(ChunkerManager::AUDIOFILE));
	
	_progressBarPointer.reset(new ProgressBar(_duration));
	_silenceOnlyPointer.reset(new std::set<Boundary, Boundary::orderPriorityPauseDuration>);

	_silenceOnlyPointer->emplace(Boundary(_offset, _transcription.minIndex(), -1, -1, true));
	_silenceOnlyPointer->emplace(Boundary(_offset + _duration - 1, _transcription.maxIndex() + 1, -1, -1, true));
}

void Chunker::saveBPFFile() const
{
	std::ofstream outstream(ChunkerUtil::getBPFFile(_prefix));
	ChunkerUtil::checkOutStream(outstream, ChunkerUtil::getBPFFile(_prefix));

	std::ifstream instream(ChunkerManager::getOptionString(ChunkerManager::BPFFILE));
	ChunkerUtil::checkInStream(instream, ChunkerManager::getOptionString(ChunkerManager::BPFFILE));

	std::string buffer;
	while(std::getline(instream, buffer))
	{
		if(buffer.find(ChunkerUtil::TRNKEY) != 0)
		{
			outstream << buffer;
			if(buffer.back() != '\n') outstream << std::endl;
		}
	}

	instream.close();
	outstream.close();
}

ChunkerClassic::ChunkerClassic() : Chunker() 
{CHUNKER_FLOG 
	
	_stage = 1;
	if(ChunkerManager::isBoosted())
	{
		_stage = 0;
	}
}

ChunkerForced::ChunkerForced() : Chunker()
{CHUNKER_FLOG 
	_stage = 0;
}

ChunkerClassic::ChunkerClassic(ChunkerClassic * parent, const Boundary & start, const Boundary & end, int childID, bool onlyChild) :
	Chunker(parent, start, end, childID, onlyChild) { CHUNKER_FLOG }

ChunkerForced::ChunkerForced(ChunkerForced * parent, const Boundary & start, const Boundary & end, int childID, bool onlyChild) :
	Chunker(parent, start, end, childID, onlyChild) { CHUNKER_FLOG }


Chunker::Chunker(Chunker * parent, const Boundary & start, const Boundary & end, int childID, bool onlyChild) 
	: _transcription(parent->getTranscription(), start.getNextIndex(), end.getNextIndex()), _recognition() // Build transcript by cutting a slice from the parent transcript
{CHUNKER_FLOG


	_parent = parent;
	_onlyChild = onlyChild;
	_stage = _parent->getStage();

	_silenceOnlyPointer = _parent->getSilenceOnlyPointer(); // Help filling the top chunker's silence-only set
	 
	_depth = parent->getDepth() + 1;
	
	_offset = start.getBoundaryTime();
	_duration = end.getBoundaryTime() - start.getBoundaryTime();

	_progressBarPointer = _parent->getProgressBarPointer();	
	_prefix = _parent->getPrefix() + ChunkerUtil::SEP + std::to_string(childID); // Working directory inside parent's working directory
	ChunkerUtil::createDir(_prefix);        
}



/////////////////////////////////////////////////////////////////////
// RUN //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


void ChunkerForced::displayFeatures() const 
{
	std::string verboffset = std::string(4 * _depth, ' ');

	CHUNKER_VERB2 << verboffset << "* Starting forced chunker for interval " << 
		_offset / ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) << " - " << 
		(_offset + _duration) / ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) << 
		" seconds" << std::endl CHUNKER_ENDVERB

	CHUNKER_VERB2 << verboffset << "* Stage " << _stage << std::endl CHUNKER_ENDVERB
}

void ChunkerClassic::displayFeatures() const
{CHUNKER_FLOG
	
	std::string verboffset = std::string(4 * _depth, ' ');

	CHUNKER_VERB2 << verboffset << "* Starting chunker for interval " << 
		_offset / ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) << " - " << 
		(_offset + _duration) / ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) << 
		" seconds" << std::endl CHUNKER_ENDVERB

	CHUNKER_VERB2 << verboffset << "* Stage " << _stage << std::endl CHUNKER_ENDVERB

	std::string mausmsg;	
	if(ChunkerManager::getOptionBool(ChunkerManager::MAUS, _stage))
	{
		mausmsg = " (with MAUS knowledge)";
	}
	
	CHUNKER_VERB3 << verboffset << "* Chunker type: " << 
		ChunkerManager::getOptionString(ChunkerManager::CHUNKERTYPE, _stage) << mausmsg << std::endl CHUNKER_ENDVERB
}

void Chunker::run()
{CHUNKER_FLOG

	if(_onlyChild || _duration < ChunkerManager::getOptionLong(ChunkerManager::MINCHUNKDURATION, _stage) *
			ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) * 2L)
	{
		if(_stage == 0) _stage = 1;
		else return;
	}

	CHUNKER_VERB2 << std::string(4 * _depth, ' ') << std::string(75 - 4 * _depth, '-') << std::endl CHUNKER_ENDVERB
	
	_progressBarPointer->set(_offset);
	displayFeatures();

	prepare();	// Preparation phase
	
	writeLanguageModel();	

	recognizer();	// Recognition phase

	findBoundaries();

	if(recursionNeeded()) recursion();

	CHUNKER_VERB2 << std::string(4 * _depth, ' ') << std::string(75 - 4 * _depth, '-') << std::endl CHUNKER_ENDVERB
}

bool ChunkerClassic::recursionNeeded() const
{CHUNKER_FLOG
	if(_depth >= ChunkerManager::getOptionInt(ChunkerManager::MAXRECURSIONDEPTH)) return false;

	if(_splitPoints.size() <= 2 && _stage == 1) return false;

	return true;
}

bool ChunkerForced::recursionNeeded() const
{CHUNKER_FLOG
	if(_depth >= ChunkerManager::getOptionInt(ChunkerManager::MAXRECURSIONDEPTH)) return false;

	if(_stage == 1) return false;

	return true;
}


/////////////////////////////////////////////////////////////////////
// GETTERS //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

int Chunker::getDepth() const
{CHUNKER_FLOG
	return _depth;
}

int Chunker::getChildID() const
{CHUNKER_FLOG
	return _childID;
}

std::shared_ptr<ProgressBar> Chunker::getProgressBarPointer()
{CHUNKER_FLOG
	return _progressBarPointer;
}

std::shared_ptr<std::set<Boundary, Boundary::orderPriorityPauseDuration> > Chunker::getSilenceOnlyPointer()
{CHUNKER_FLOG
	return _silenceOnlyPointer;
}


const std::string & Chunker::getPrefix() const
{CHUNKER_FLOG
	return _prefix;
}

bool Chunker::getStage() const
{CHUNKER_FLOG
	return _stage;
}

const Transcription & Chunker::getTranscription() const
{CHUNKER_FLOG
	return _transcription;
}

const Recognition & Chunker::getRecognition() const
{CHUNKER_FLOG
	return _recognition;
}


/////////////////////////////////////////////////////////////////////
// LANGUAGE MODEL PHASE /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void Chunker::prepare()
{CHUNKER_FLOG

	ChunkerUtil::createDir(ChunkerUtil::getHTKDir(_prefix));
	ChunkerUtil::createDir(ChunkerUtil::getMLFDir(_prefix));

	// Emplace beginning and end of current chunk in _splitPoints set
	_splitPoints.emplace(Boundary(_offset, _transcription.minIndex(), -1, -1, true));
	_splitPoints.emplace(Boundary(_offset + _duration - 1, _transcription.maxIndex() + 1, -1, -1, true));
	
	// Create necessary working directories

	CHUNKER_VERB3 << std::string(4 * _depth, ' ') << "* Getting HTK file" << std::endl CHUNKER_ENDVERB
	obtainHTKFile(); // Obtain HTK file by converting the input WAVE file or cutting a slice from the parent HTK file
}

void Chunker::resample()
{
	std::string sox = "sox " + ChunkerManager::getOptionString(ChunkerManager::AUDIOFILE) + " -b 16 -c 1 -e signed-integer -r " + 
		std::to_string(ChunkerUtil::STANDARDSAMPLERATE) + " " + ChunkerUtil::getAudioFile(_prefix);
	ChunkerUtil::wrap(sox);
}

void Chunker::obtainHTKFile()
{CHUNKER_FLOG
	std::string cmd;
	if(_depth == 0) // If the current chunker is the topmost chunker
	{
		writeHCopyConfig();
		cmd = "HCopy -C " + ChunkerUtil::getHCopyConfig(_prefix) + " " + 
			ChunkerUtil::getAudioFile(_prefix) + " " + ChunkerUtil::getHTKFile(_prefix);
	}

	else // If the current chunker is a child chunker
	{
		cmd = "HCopy -s " + std::to_string(_offset) + " -e " + std::to_string(_offset + _duration) + 
			" " + ChunkerUtil::getHTKFile(getRootChunker()->getPrefix()) + " " + ChunkerUtil::getHTKFile(_prefix);
	}
		
	ChunkerUtil::wrap(cmd);
}

void Chunker::writeHCopyConfig()
{CHUNKER_FLOG

	std::ofstream HCopyStream(ChunkerUtil::getHCopyConfig(_prefix));
	ChunkerUtil::checkOutStream(HCopyStream, ChunkerUtil::getHCopyConfig(_prefix));

	HCopyStream << "SOURCEKIND = WAVE-FORM" << std::endl <<
		"SOURCEFORMAT = WAVE" << std::endl <<
		"TARGETFORMAT = HTK" << std::endl <<
		"TARGETKIND = MFCC_E_D_A" << std::endl <<
		"NUMCEPS = 12" << std::endl <<
		"TARGETRATE = 100000.0" << std::endl <<
		"WINDOWSIZE = 200000.0" << std::endl <<
		"CEPLIFTER = 22" << std::endl <<
		"NUMCHANS = 26";

	HCopyStream.close();
}

const Chunker * Chunker::getParent() const
{
	return _parent;
}

const Chunker * Chunker::getRootChunker() const
{CHUNKER_FLOG
	if(_parent == NULL)
	{
		return this;
	}
	else 
	{
		return _parent->getRootChunker();
	}
}



void ChunkerForced::writeLanguageModel()
{CHUNKER_FLOG
	
	std::unique_ptr<LanguageModel> lm;
	std::unique_ptr<TranscriptionIterator> iter(_transcription.getTokenIterator().copyAsTranscriptionIterator());
	iter->deactivateSilentSegments();

	std::unique_ptr<UnitModeller> modeller(new UnitModellerClassic());
	
	Dictionary dict(iter->copyAsTranscriptionIterator());
	
	std::ofstream dictStream(ChunkerUtil::getDictFile(_prefix));
	ChunkerUtil::checkOutStream(dictStream, ChunkerUtil::getDictFile(_prefix));
	dictStream << dict;
	dictStream.close();

	if(_stage == 0) lm.reset(new LMFactorAutomaton(std::move(iter), std::move(modeller)));
	else lm.reset(new LMForcedAlign(std::move(iter), std::move(modeller)));
	
	std::ofstream netStream(ChunkerUtil::getLMFile(_prefix));
	ChunkerUtil::checkOutStream(netStream, ChunkerUtil::getLMFile(_prefix));
	netStream << *lm;
	netStream.close();
}

void ChunkerClassic::writeLanguageModel()
{CHUNKER_FLOG
	
	CHUNKER_VERB3 << std::string(4 * _depth, ' ') << "* Writing language model (" << 
		ChunkerManager::getOptionString(ChunkerManager::LANGUAGEMODEL, _stage) << ")" << std::endl CHUNKER_ENDVERB

	std::unique_ptr<LanguageModel> lm;
	std::unique_ptr<TranscriptionIterator> iter(_transcription.languageModelIterator(_stage));
	std::unique_ptr<UnitModeller> modeller;
	
	std::string ngram = ChunkerManager::getOptionString(ChunkerManager::LANGUAGEMODEL, _stage);

	Dictionary dict(UniqueIterator(iter->copyAsTranscriptionIterator()));
	
	std::ofstream dictStream(ChunkerUtil::getDictFile(_prefix));
	ChunkerUtil::checkOutStream(dictStream, ChunkerUtil::getDictFile(_prefix));
	dictStream << dict;
	dictStream.close();

	if(ChunkerManager::getOptionBool(ChunkerManager::MAUS, _stage)) modeller.reset(new UnitModellerMaus(_prefix));
	else modeller.reset(new UnitModellerClassic());
	
	
	if(ngram == "bigram") lm.reset(new LMBigram(std::move(iter), std::move(modeller)));
	else if(ngram == "zerogram") lm.reset(new LMZerogram(std::move(iter), std::move(modeller)));
	else if(ngram == "pair") lm.reset(new LMPair(std::move(iter), std::move(modeller)));
	
	else
	{
		CHUNKER_ERR << "Unknown language model " << ngram CHUNKER_ENDERR3
	}

	std::ofstream netStream(ChunkerUtil::getLMFile(_prefix));
	ChunkerUtil::checkOutStream(netStream, ChunkerUtil::getLMFile(_prefix));
	netStream << *lm;
	netStream.close();
}

/////////////////////////////////////////////////////////////////////
// RECOGNITION PHASE ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void Chunker::recognizer()
{CHUNKER_FLOG

	std::vector<std::pair<long, long> > intervals = getIntervals();
	HViteWrapper hvite(this);
	hvite.run(intervals);
	
	_recognition.build(ChunkerUtil::getMLFFile(_prefix), _offset);
}

std::vector<std::pair<long, long> > ChunkerForced::getIntervals() const
{
	std::vector<std::pair<long, long> > intervals;
	
	long splitlen = ChunkerManager::getOptionLong(ChunkerManager::SPLITDURATION, _stage) *
		ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR);
	
	if(_stage == 1 || splitlen > _duration)
	{
		intervals.push_back(std::make_pair(0, _duration));
	}
	else
	{
		intervals.push_back(std::make_pair(_duration / 2 - splitlen / 2, _duration / 2 + splitlen / 2));
	}

	return intervals;
}

std::vector<std::pair<long, long> > ChunkerClassic::getIntervals() const
{CHUNKER_FLOG
	
	std::vector<std::pair<long, long> > intervals;
	
	long splitlen = ChunkerManager::getOptionLong(ChunkerManager::SPLITDURATION, _stage) * 
		ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR);

	for(long i = 0; i < _duration; i += splitlen)
	{
		if(i < _duration - splitlen) 
		{ 
			intervals.push_back(std::make_pair(i, i + splitlen - 1)); 
		}
		else
		{
			if(_duration - i > ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR))
			{
				intervals.push_back(std::make_pair(i, _duration));
			}
		}
	}	

	return intervals;
	
}


/////////////////////////////////////////////////////////////////////
// ALIGNMENT PHASE //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void ChunkerForced::findBoundaries()
{
	long minPauseDuration = ChunkerManager::getOptionLong(ChunkerManager::SILENCEONLY) * 
		ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) / 1000L;
	
	if(_stage == 1) 
	{
		auto iterLeft = _recognition.getTokenIterator();
		auto iterRight = _recognition.getTokenIterator();
		
		iterLeft.deactivateSilentSegments();
		iterRight.deactivateSilentSegments();
	
		if(iterLeft.done()) return;
		
		++iterRight;

		for(; !iterRight.done(); ++iterLeft, ++iterRight)
		{
			long nextIndex = iterRight.getIndex();
			long pauseDuration = iterRight.getStartTime() - iterLeft.getEndTime();
			long boundary = iterLeft.getEndTime() + pauseDuration / 2;
			
			if(pauseDuration >= minPauseDuration)
			{
				_silenceOnlyPointer->emplace(Boundary(boundary, nextIndex, pauseDuration, -1, false));
			}
		}
	}

	else
	{
		_splitPoints.emplace(_recognition.getMiddle());
	}
}



void ChunkerClassic::alignment()
{CHUNKER_FLOG
	CHUNKER_VERB3 << std::string(4 * _depth, ' ') << "* Symbolic alignment (" << 
		ChunkerManager::getOptionString(ChunkerManager::ALIGNER, _stage) << ")" << std::endl CHUNKER_ENDVERB
	
	std::unique_ptr<Aligner> aligner;

	if(ChunkerManager::getOptionString(ChunkerManager::ALIGNER, _stage) == "fast")
	{
		aligner.reset(new AlignFast(_transcription.alignmentIterator(_stage), _recognition.alignmentIterator(_stage)));
	}
	else if(ChunkerManager::getOptionString(ChunkerManager::ALIGNER, _stage) == "hirschberg")
	{
		aligner.reset(new AlignHirschberg(_transcription.alignmentIterator(_stage), _recognition.alignmentIterator(_stage)));
	}
	else
	{
		aligner.reset(new AlignMatrix(_transcription.alignmentIterator(_stage), _recognition.alignmentIterator(_stage)));
	}

	aligner->run(&_path);
}

/////////////////////////////////////////////////////////////////////
// CHUNK BOUNDARY SELECTION PHASE ///////////////////////////////////
/////////////////////////////////////////////////////////////////////

void ChunkerClassic::getCandidateAnchors()
{CHUNKER_FLOG

	
	CHUNKER_VERB3 << std::string(4 * _depth, ' ') << "* Boundary discovery with minimum anchor length " << 
		ChunkerManager::getOptionInt(ChunkerManager::MINANCHORLENGTH, _stage) << std::endl CHUNKER_ENDVERB	
	
	int minanchorlength = ChunkerManager::getOptionInt(ChunkerManager::MINANCHORLENGTH, _stage);
	int maxanchorcost = ChunkerManager::getOptionInt(ChunkerManager::MAXANCHORCOST, _stage);
	int minanchorsingletons = ChunkerManager::getOptionInt(ChunkerManager::MINANCHORSINGLETONS, _stage);

	UnigramProbabilities unigramProbs(_transcription.getTokenIterator().copyAsTranscriptionIterator());

        for(std::deque<AlignPoint>::const_iterator pathIter = _path.cbegin(); pathIter != _path.cend(); ++pathIter)
        {
		Anchor anchor (&unigramProbs);

                unsigned costSoFar(0);
                
		for
                (
                        std::deque<AlignPoint>::const_iterator alignPointIter = pathIter;
                        alignPointIter != _path.cend() && costSoFar <= maxanchorcost;
                        ++alignPointIter
                )
                {
			const AlignPoint & alignPoint = *alignPointIter;
			costSoFar += alignPoint.getCost();

                        if ( costSoFar <= maxanchorcost )
			{
				anchor.addAlignPoint(alignPoint);
			}
                }


		if
                (
                        anchor.size() >= minanchorlength &&
			anchor.getNumSingletons() >= minanchorsingletons
                )
                {
                        CHUNKER_LOG4 << "New anchor found" CHUNKER_ENDLOG
                        _anchors.emplace(anchor);
                }
        }

        CHUNKER_LOG4 << "Found " << _anchors.size() << " anchors" CHUNKER_ENDLOG
}

void ChunkerClassic::findBoundaries()
{CHUNKER_FLOG	

	alignment();	
	getCandidateAnchors();
	
	std::set<Boundary, Boundary::orderPriorityAnchorLength> candidateBoundaries;

	long minPauseDuration = ChunkerManager::getOptionLong(ChunkerManager::SILENCEONLY) *
		(ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) / 1000L);

	for (const auto & anchor : _anchors)
        {
                if(anchor.size() <= 1) continue;
		
		for
                (
                        std::deque<const AlignPoint *>::const_iterator candidateIter = anchor.cbegin() + 1;
                        candidateIter != anchor.cend();
                        ++candidateIter
                )
                {
			const AlignPoint * candidateRight = *candidateIter;
			const AlignPoint * candidateLeft = *(candidateIter-1);

                        // We want a boundary between two zero substitutions (cost 0), which belong to different tokens
                        if
                        (
                                candidateLeft->getIndex() != candidateRight->getIndex() &&
                                candidateLeft->getCost() == 0 && candidateRight->getCost() == 0
                        )
                        {
				long index = candidateRight->getIndex();
				long pauseDuration = candidateRight->getStartTime() - candidateLeft->getEndTime();
				long boundary = candidateLeft->getEndTime() + pauseDuration / 2;
                                
				candidateBoundaries.emplace(Boundary(boundary, index, pauseDuration, anchor.size(), false));

				if(pauseDuration >= minPauseDuration)
				{
					_silenceOnlyPointer->emplace(Boundary(boundary, index, pauseDuration, anchor.size(), false));
				}
                        }
                }
	}

	_splitPoints.insert(candidateBoundaries.cbegin(), candidateBoundaries.cend());

	CHUNKER_LOG3 << "Found " << _splitPoints.size() << " split points" CHUNKER_ENDLOG
	CHUNKER_VERB3 << std::string(4 * _depth, ' ') << "* Found " << _splitPoints.size() - 2 << 
		" temporary boundary(s)" << std::endl CHUNKER_ENDVERB	
}

/////////////////////////////////////////////////////////////////////
// RECURSION PHASE //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


std::unique_ptr<Chunker> ChunkerClassic::spawnChild(const Boundary & start, const Boundary & end, int childNum)
{
	return std::unique_ptr<Chunker>(new ChunkerClassic(this, start, end, childNum, _splitPoints.size() <= 2));
}

std::unique_ptr<Chunker> ChunkerForced::spawnChild(const Boundary & start, const Boundary & end, int childNum)
{
	return std::unique_ptr<Chunker>(new ChunkerForced(this, start, end, childNum, _splitPoints.size() <= 2));
}

void Chunker::recursion()
{CHUNKER_FLOG

	auto startIter = _splitPoints.cbegin();
	auto endIter = startIter;
	endIter ++;
	
	int childNum = 0;

	std::vector<std::unique_ptr<Chunker> > children;
	
	for(; endIter != _splitPoints.cend(); startIter++, endIter++)
	{
		children.push_back(spawnChild(*startIter, *endIter, childNum));
		childNum ++;
	}
	
	for(std::unique_ptr<Chunker> & child : children)
	{
		child->run();
	}
}


/////////////////////////////////////////////////////////////////////
// OUTPUT PHASE /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


std::unique_ptr<TurnTier> ChunkerForced::buildTurnTier() const
{
	std::unique_ptr<TurnTier> turnTier(new TurnTier(_duration));
	std::set<Boundary, Boundary::orderFinal> results(_silenceOnlyPointer->cbegin(), _silenceOnlyPointer->cend());
	
	turnTier->build(results, _transcription.getTokenIterator());
	return turnTier;
}

std::unique_ptr<TurnTier> ChunkerClassic::buildTurnTier() const
{
	
	std::unique_ptr<TurnTier> turnTier(new TurnTier(_duration));
	std::set<Boundary, Boundary::orderFinal> results(_silenceOnlyPointer->cbegin(), _silenceOnlyPointer->cend());
	
	turnTier->build(results, _transcription.getTokenIterator());
	
	if(!turnTier->checkLength())
	{
		if(ChunkerManager::getOptionString(ChunkerManager::FORCE) == "rescue")
		{
			CHUNKER_WARN << "Result TRN tier contains a turn with a length beyond " << 
				ChunkerManager::getOptionInt(ChunkerManager::WARNCHUNKDURATION) << " words. " <<
				"Resorting to forced chunking." CHUNKER_ENDWARN

			ChunkerForced chunker;
			chunker.run();
			return chunker.buildTurnTier();
		}
	}

	return turnTier;
}


void Chunker::writeOutput() const
{CHUNKER_FLOG
	
	_progressBarPointer->set(_duration);
	CHUNKER_VERB1 << std::endl CHUNKER_ENDVERB
	
	std::unique_ptr<TurnTier> turnTier = buildTurnTier();

	if(!turnTier->checkLength())
	{
		CHUNKER_ERR << "Result TRN tier contains a turn with a length beyond " << 
			ChunkerManager::getOptionInt(ChunkerManager::WARNCHUNKDURATION) << " words. " <<
			"This means that MAUS (or other forced aligners) might still have trouble segmenting " <<
			"the material in real time. Consider lowering the minanchorlength parameter or " <<
			"running the chunker with the force option set to true." 
			CHUNKER_ENDERR2
	}

	CHUNKER_VERB2 << "* Writing output to: " << 
		ChunkerManager::getOptionString(ChunkerManager::OUTFILE) << std::endl CHUNKER_ENDVERB


	std::ofstream outStream(ChunkerManager::getOptionString(ChunkerManager::OUTFILE));
	ChunkerUtil::checkOutStream(outStream, ChunkerManager::getOptionString(ChunkerManager::OUTFILE));	
	
	std::string outtype = ChunkerManager::getOptionString(ChunkerManager::OUTTYPE);

	if(outtype == "bpf+trn") 
	{
		writeOriginalBPFHeader(outStream); 
		writeOriginalBPFBody(outStream); 
	}

	outStream << (*turnTier);
	outStream.close();
}

void Chunker::writeOriginalBPFHeader(std::ostream & stream) const
{CHUNKER_FLOG
	
	std::ifstream instream(ChunkerUtil::getBPFFile(_prefix));
	ChunkerUtil::checkInStream(instream, ChunkerUtil::getBPFFile(_prefix));
	std::string buffer;

	if(!BPFcontainsKey(ChunkerUtil::LHDKEY))
	{
		stream << ChunkerUtil::LHDKEY << ": Partitur 1.3.3" << std::endl;
	}

	if(BPFcontainsKey(ChunkerUtil::LBDKEY))
	{
		while(std::getline(instream, buffer))
		{
			if(buffer.substr(0,3) == ChunkerUtil::LBDKEY)
			{
				break;
			}

			stream << buffer;
			if(buffer.back() != '\n') stream << std::endl;
		}
	}

	if(!BPFcontainsKey(ChunkerUtil::SAMKEY))
	{
		stream << ChunkerUtil::SAMKEY << ": " << 
			ChunkerUtil::getSamplingRate(ChunkerManager::getOptionString(ChunkerManager::AUDIOFILE)) << std::endl;
	}
	
	stream << ChunkerUtil::LBDKEY << ":" << std::endl;
}


void Chunker::writeOriginalBPFBody(std::ostream & stream) const
{CHUNKER_FLOG
	std::ifstream instream(ChunkerUtil::getBPFFile(_prefix));
	ChunkerUtil::checkInStream(instream, ChunkerUtil::getBPFFile(_prefix));
	std::string buffer;

	bool seenLBD = (!BPFcontainsKey(ChunkerUtil::LBDKEY));
	while(std::getline(instream, buffer))
	{
		if(seenLBD)
		{
			stream << buffer;
			if(buffer.back() != '\n') stream << std::endl;

		}
		if(buffer.substr(0,3) == ChunkerUtil::LBDKEY)
		{
			seenLBD = true;
		}
	}
}

bool Chunker::BPFcontainsKey(const std::string & key) const
{CHUNKER_FLOG
	std::ifstream instream(ChunkerUtil::getBPFFile(_prefix));
	ChunkerUtil::checkInStream(instream, ChunkerUtil::getBPFFile(_prefix));
	std::string buffer;

	while(std::getline(instream, buffer))
	{
		if(buffer.find(key) == 0) return true;
	}

	return false;
}
