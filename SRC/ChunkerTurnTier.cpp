#include "ChunkerGlobal.hpp"

std::ostream & operator << (std::ostream & stream, const TurnTierItem & item)
{CHUNKER_FLOG
	item.print(stream);
}

std::ostream & operator << (std::ostream & stream, const TurnTier & tier)
{CHUNKER_FLOG
	tier.print(stream);
}

TurnTierItem::TurnTierItem(long start, long duration, const std::deque<TranscriptionTokenIterator> & tokens)
{CHUNKER_FLOG
	
	_start = start;
	_duration = duration;
	for (const auto & token : tokens )
	{
		_tokens.push_back(std::make_pair(token.getIndex(), token.getToken()));
	}
}

int TurnTierItem::getNumberOfTokens() const
{CHUNKER_FLOG

	int size(_tokens.size());
	return size;
}

void TurnTierItem::print(std::ostream & stream) const
{CHUNKER_FLOG
	
	stream << ChunkerUtil::TRNKEY + ": " << _start << " " << _duration << " ";

	for ( const auto & pair : _tokens )
	{
		stream << pair.first << ",";
	}

	stream.seekp((long)stream.tellp() - 1);
	stream << " ";

	for ( const auto & pair : _tokens )
	{
		stream << pair.second << " ";
	}
	
	stream.seekp((long)stream.tellp() - 1);
	
	stream << std::endl;
}

long TurnTierItem::getDuration() const
{CHUNKER_FLOG
	return _duration;
}

	

TurnTier::TurnTier(long duration)
{CHUNKER_FLOG

	_duration = duration;
	_samplingRate = ChunkerUtil::getSamplingRate(ChunkerManager::getOptionString(ChunkerManager::AUDIOFILE));
}

bool TurnTier::checkLength() const
{
	for(const auto & item : _items)
	{
		if(item.getNumberOfTokens() > ChunkerManager::getOptionInt(ChunkerManager::WARNCHUNKDURATION))
		{
			return false;
		}
	}
	return true;
}


void TurnTier::addItem(long start, long dur, const std::deque<TranscriptionTokenIterator> & tokens)
{
	if(ChunkerManager::getOptionInt(ChunkerManager::MAXCHUNKDURATION) == 0 ||
			dur < ChunkerManager::getOptionLong(ChunkerManager::MAXCHUNKDURATION) *
			ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR))
	{
		_items.push_back(TurnTierItem(start, dur, tokens));
	}
}

void TurnTier::build(const std::set<Boundary, Boundary::orderFinal> & boundaries, TranscriptionTokenIterator tokenIter)
{CHUNKER_FLOG

	
	tokenIter.activateSilentSegments();
	tokenIter.useOrt();

	std::set<Boundary, Boundary::orderFinal>::const_iterator startIter = boundaries.cbegin();
	std::set<Boundary, Boundary::orderFinal>::const_iterator endIter = boundaries.cbegin();
	++endIter;
	std::set<Boundary, Boundary::orderFinal>::const_iterator finalIter = boundaries.cend();

	std::deque<TranscriptionTokenIterator> tokens;
	
	for(; ! tokenIter.done(); ++tokenIter)
	{
		if(endIter != finalIter && tokenIter.getIndex() >= endIter->getNextIndex())
		{
			if(!tokens.empty())
			{
				long start = (startIter->getBoundaryTime() * _samplingRate) /
					ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR);
				long dur = (((endIter->getBoundaryTime() - startIter->getBoundaryTime()) *
					_samplingRate) / ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR)) - 1;
				addItem(start, dur, tokens);	
			}
			++ startIter;
			++ endIter;
			tokens.clear();
		}
		tokens.push_back(TranscriptionTokenIterator(tokenIter));
	}

	if(!tokens.empty())
	{
		long start = startIter->getBoundaryTime() * _samplingRate / 
				ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR);
		long dur = ((_duration - startIter->getBoundaryTime()) * _samplingRate / 
				ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR)) - 1;
		addItem(start, dur, tokens);	
	}
}


void TurnTier::print(std::ostream & stream) const
{CHUNKER_FLOG

	for ( const auto & item : _items )
	{
		stream << item;
	}
}
