#include "ChunkerGlobal.hpp"


void HViteWrapper::run(const std::vector<std::pair<long, long> > & intervals)
{CHUNKER_FLOG
	
	multithreadedHVite(intervals);
	
	std::ofstream mlfStream(ChunkerUtil::getMLFFile(_chunker->getPrefix()));
	ChunkerUtil::checkOutStream(mlfStream, ChunkerUtil::getMLFFile(_chunker->getPrefix()));
	
	for ( auto & interval : intervals)
	{ 
		appendMLF(interval.first, mlfStream); 
	}

	CHUNKER_VERB3 << std::endl CHUNKER_ENDVERB
}


HViteWrapper::HViteWrapper(const Chunker * chunker)
{
	_chunker = chunker;
}

void HViteWrapper::multithreadedHVite(const std::vector<std::pair<long, long> > & intervals)
{CHUNKER_FLOG

	std::deque<std::thread> threads;
	std::deque<std::promise<void> > promises;
	std::deque<std::future<void> > futures;
	std::deque<bool> flags;

	promises.push_back(std::promise<void>());
	futures.push_back(promises.back().get_future());

	CHUNKER_VERB3 << std::string(4 * _chunker->getDepth(), ' ') << "* Starting " << intervals.size() << 
		" recognizer thread(s)    " CHUNKER_ENDVERB

	for (auto & interval : intervals)
	{
		long start = interval.first;
		long end = interval.second;
		promises.push_back(std::promise<void>());
		std::promise<void> * promise = &(promises.back());
		std::future<void> * future = &(futures.back());	// This thread's future is the promise of the previous thread
		futures.push_back(promises.back().get_future());
		
		flags.push_back(false);
		bool * flag = &(flags.back());
		threads.push_back
		(
			std::thread(
				[start, end, promise, future, flag, this]
				{
					future->get();	// Sleep until the previous thread has set its promise
					while(!ChunkerManager::getThread())	// Wait for permission to start
					{
						usleep(2000); 
					}
					CHUNKER_VERB3 << '|' CHUNKER_ENDVERB;
					promise->set_value();	// Set promise for the next thread
					recognizeInterval(start, end);	// Run HVite for interval
					*flag = true;	// Set flag for main thread
					ChunkerManager::returnThread();	// Return thread to manager class
				}
			)
		);
	}


	promises.front().set_value();	// Set promise for the first thread
	ChunkerManager::returnThread();	// Give up current thread
	futures.back().get();	// Wait for the last thread to set its promise

	while(!ChunkerManager::getThread())	// Wait for permission to start
	{ 
		usleep(2000); 
	}

	unsigned numJoined = 0;
	while(numJoined < threads.size())
	{
		std::deque<std::thread>::iterator threadIterator = threads.begin();
		std::deque<bool>::iterator flagIterator = flags.begin();
		
		// Iterate jointly over threads and flags
		for(;threadIterator != threads.end(); ++threadIterator, ++flagIterator)
		{
			// If this thread is done
			if(*flagIterator)
			{
				threadIterator->join();
				*flagIterator = false;
				numJoined ++;
			}
		}
		usleep(1000);
	}
}

void HViteWrapper::recognizeInterval(long start, long end)
{CHUNKER_FLOG

	const std::string & prefix = _chunker->getPrefix();

	std::string hcopycmd(
		"HCopy -s " + std::to_string(start) +
		" -e " + std::to_string(end) +
		" " + ChunkerUtil::getHTKFile(_chunker->getPrefix()) + 
		" " + ChunkerUtil::getHTKInterval(_chunker->getPrefix(), start));

	std::string hvitecmd(
		"HVite -w " + ChunkerUtil::getLMFile(prefix) + 	// Language model file
		" -T 0 " +
		" -s " + ChunkerManager::getOptionString(ChunkerManager::LANGUAGEMODELWEIGHT, _chunker->getStage()) + 
		" -H " + ChunkerManager::getOptionString(ChunkerManager::MMFFILE) +
		" -i " + ChunkerUtil::getMLFInterval(_chunker->getPrefix(), start) +
		" -t " + ChunkerManager::getOptionString(ChunkerManager::HVITEBEAMWIDTH, _chunker->getStage()) +
		" " + ChunkerUtil::getDictFile(prefix) +	// Dictionary file
		" " + ChunkerManager::getOptionString(ChunkerManager::MAUSHMMINVENTAR) +	// HMM list file
		" " + ChunkerUtil::getHTKInterval(_chunker->getPrefix(), start)); // HTK file
	

	CHUNKER_LOG4 << "Executing HCopy" CHUNKER_ENDLOG
	ChunkerUtil::wrap(hcopycmd);	// Cut interval from HTK file
	CHUNKER_LOG4 << "Executing HVite" CHUNKER_ENDLOG
	ChunkerUtil::wrap(hvitecmd);	// Run HVite
}

void HViteWrapper::appendMLF(long offset, std::ofstream & mlfStream) const
{CHUNKER_FLOG
	
	const std::string & prefix = _chunker->getPrefix();
	
	std::string mlfTmp = ChunkerUtil::getMLFInterval(_chunker->getPrefix(), offset);	
	std::ifstream instream(mlfTmp);
	ChunkerUtil::checkInStream(instream, mlfTmp);

	std::string buffer;
	
	while(std::getline(instream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() >= 3 && ChunkerUtil::isLongString(tokens.at(0)) && ChunkerUtil::isLongString(tokens.at(1)))
		{
			long newStart = std::stol(tokens.at(0)) + offset;	// Offset entry
			long newEnd = std::stol(tokens.at(1)) + offset;
			
			mlfStream << std::to_string(newStart) << " " << std::to_string(newEnd) << " ";
			
			for(auto iter = tokens.cbegin() + 2; iter != tokens.cend(); ++iter)
			{
				mlfStream << *iter << " ";
			}

			mlfStream.seekp((long)mlfStream.tellp() - 1);
			mlfStream << std::endl;
		}
	}
	instream.close();
}

