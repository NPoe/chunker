#ifndef CHUNKER_MACROS_HPP
#define CHUNKER_MACROS_HPP

#define CHUNKER_ERR	{ std::cerr << "ERROR: "
#define CHUNKER_ENDERR1 << std::endl; throw 1; } 
#define CHUNKER_ENDERR2	<< std::endl; throw 2; } 
#define CHUNKER_ENDERR3	<< std::endl; throw 3; }
#define CHUNKER_ENDERR4	<< std::endl; throw 4; } 
#define CHUNKER_ENDERR5	<< std::endl; throw 5; } 

#define CHUNKER_DEBUG 	0

#define CHUNKER_LOG1	if(CHUNKER_DEBUG >= 1) { ChunkerManager::LOG
#define CHUNKER_LOG2	if(CHUNKER_DEBUG >= 2) { ChunkerManager::LOG
#define CHUNKER_LOG3	if(CHUNKER_DEBUG >= 3) { ChunkerManager::LOG
#define CHUNKER_LOG4	if(CHUNKER_DEBUG >= 4) { ChunkerManager::LOG
#define CHUNKER_LOG5	if(CHUNKER_DEBUG >= 5) { ChunkerManager::LOG

#define CHUNKER_VERB1	if(ChunkerManager::getOptionInt(ChunkerManager::VERBOSE) == 1) { std::cout
#define CHUNKER_VERB2	if(ChunkerManager::getOptionInt(ChunkerManager::VERBOSE) >= 2) { std::cout
#define CHUNKER_VERB3	if(ChunkerManager::getOptionInt(ChunkerManager::VERBOSE) >= 3) { std::cout

#define CHUNKER_WARN	{ std::cerr << "WARNING: "
#define CHUNKER_ENDWARN	<< std::endl; std::cerr.flush(); }

#define CHUNKER_FLOG	CHUNKER_LOG4 << __func__ CHUNKER_ENDLOG
#define CHUNKER_ENDLOG	<< std::endl; ChunkerManager::LOG.flush(); }
#define CHUNKER_ENDVERB ; std::cout.flush(); }

#endif
