#include "ChunkerGlobal.hpp"


RecognitionTokenIterator Recognition::getTokenIterator() const 
{CHUNKER_FLOG 
	return RecognitionTokenIterator(_recognition); 
}

RecognitionPhoneIterator Recognition::getPhoneIterator() const 
{CHUNKER_FLOG
	return RecognitionPhoneIterator(_recognition); 
}

std::unique_ptr<RecognitionIterator> Recognition::alignmentIterator(bool stage) const
{CHUNKER_FLOG
	std::string type = ChunkerManager::getOptionString(ChunkerManager::CHUNKERTYPE, stage);

	std::unique_ptr<RecognitionIterator> iter;
	
	if (type == "p" || type == "tp") { iter.reset(new RecognitionPhoneIterator(_recognition)); }
	
	if (type == "t") { iter.reset(new RecognitionTokenIterator(_recognition)); }

	iter->deactivateSilentSegments();

	return iter;
}

Boundary Recognition::getMiddle() const
{CHUNKER_FLOG

	long start =_recognition.cbegin()->getStart();
	long end = _recognition.crbegin()->getEndTime();
	long middle = start + ((end - start) / 2);

	RecognitionTokenIterator iter = getTokenIterator();
	
	long lastIndex = -1;
	long lastStart = -1;

	for(; (!iter.done()) && iter.getStartTime() < middle; ++iter)
	{
		if(iter.getIndex() != lastIndex)
		{
			lastIndex = iter.getIndex();
			lastStart = iter.getStartTime();
		}
	}
	return Boundary(lastStart, lastIndex, -1, -1, false);
}


void Recognition::build ( const std::string & MLFFile, long offset ) 
{CHUNKER_FLOG
	CHUNKER_LOG4 << MLFFile CHUNKER_ENDLOG

	std::ifstream mlfStream(MLFFile);
	ChunkerUtil::checkInStream(mlfStream, MLFFile);

	std::string buffer;

	std::deque<RecognitionPhone> currentSegmentation;
	std::string tokenLabel;
	std::string tokenOrt;
	long tokenIndex(-1);

	while(std::getline(mlfStream, buffer))
	{
		if(buffer.size() > 0 && ChunkerUtil::MLFCOMMENTCHARS.find(buffer.at(0)) == ChunkerUtil::MLFCOMMENTCHARS.cend())
		{
			auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
			if(!(tokens.size() >= 3 && ChunkerUtil::isLongString(tokens.at(0)) && ChunkerUtil::isLongString(tokens.at(1))))
			{
				CHUNKER_ERR << "Cannot parse line " << buffer << " in file " << MLFFile CHUNKER_ENDERR4
			}

			long start = std::stol(tokens.at(0)) + offset;
			long end = std::stol(tokens.at(1)) + offset;

			std::string label = tokens.at(2);

			if(label.find(ChunkerUtil::SPECIALSTRING) == 0 || label == ChunkerUtil::PAUSESTRING)
			{
				if(!(currentSegmentation.empty()))
				{
					CHUNKER_LOG4 << "Token start: " << start << " Label: " << tokenLabel CHUNKER_ENDLOG
					_recognition.push_back(RecognitionToken(tokenLabel, tokenOrt, currentSegmentation, tokenIndex));
					currentSegmentation.clear();
				}

				if(label.find(ChunkerUtil::SPECIALSTRING) == 0)
				{
					auto tokens2 = ChunkerUtil::tokenize(label, ChunkerUtil::SPECIALSTRING);
					if(tokens2.size() < 3 || !ChunkerUtil::isLongString(tokens2.at(2)))
					{
						CHUNKER_ERR << "Cannot parse recognition label " << label CHUNKER_ENDERR2
					}

					tokenLabel = tokens2.at(0);
					tokenOrt = tokens2.at(1);
					tokenIndex = std::stol(tokens2.at(2));
				}

				if(start < end)
				{
					std::deque<RecognitionPhone> pauseSeg({RecognitionPhone(start, end, ChunkerUtil::PAUSESTRING)});
					_recognition.push_back(RecognitionToken(ChunkerUtil::PAUSESTRING, ChunkerUtil::PAUSESTRING, 
								pauseSeg, -1));
				}
			}

			else
			{
				CHUNKER_LOG4 << "Phone start: " << start << " Label: " << tokens.at(2) CHUNKER_ENDLOG
				currentSegmentation.push_back(RecognitionPhone(start, end, tokens.at(2)));
			}
		}
	}

	if(!(currentSegmentation.empty()))
	{
		_recognition.push_back(RecognitionToken(tokenLabel, tokenOrt, currentSegmentation, tokenIndex));
	}
	
	mlfStream.close();
}
