#ifndef CHUNKER_GLOBAL_HPP
#define CHUNKER_GLOBAL_HPP

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>
#include <future>


#include "ChunkerMacros.hpp"
#include "ChunkerMain.hpp"
#include "ChunkerUtil.hpp"
#include "ChunkerManager.hpp"
#include "ChunkerBoundaries.hpp"
#include "ChunkerTranscriptionSymbols.hpp"
#include "ChunkerRecognitionSymbols.hpp"
#include "ChunkerIterators.hpp"
#include "ChunkerTranscription.hpp"
#include "ChunkerRecognition.hpp"
#include "ChunkerProbabilities.hpp"
#include "ChunkerUnitModeller.hpp"
#include "ChunkerLanguageModel.hpp"
#include "ChunkerDictionary.hpp"
#include "ChunkerAlignPoint.hpp"
#include "ChunkerAligner.hpp"
#include "ChunkerAnchor.hpp"
#include "ChunkerTurnTier.hpp"
#include "ChunkerProgressBar.hpp"
#include "ChunkerChunker.hpp"
#include "ChunkerHVite.hpp"

#endif
