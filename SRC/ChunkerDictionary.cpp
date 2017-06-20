#include "ChunkerGlobal.hpp"

void Dictionary::readMAUSDictionary()
{CHUNKER_FLOG

	std::ifstream mausdictStream(ChunkerManager::getOptionString(ChunkerManager::MAUSDICT));
	ChunkerUtil::checkInStream(mausdictStream, ChunkerManager::getOptionString(ChunkerManager::MAUSDICT));
	
	std::string buffer;
	
	while(std::getline(mausdictStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() < 2) continue;
		
		_dict.emplace(tokens.at(0), tokens.at(1));
	}
	
	mausdictStream.close();
}

std::string Dictionary::getPauseModel() const
{CHUNKER_FLOG

	std::ifstream mausdictStream(ChunkerManager::getOptionString(ChunkerManager::MAUSDICT));
	std::string buffer;

	while(std::getline(mausdictStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() < 2) continue;

		if(tokens.at(0) == ChunkerUtil::PAUSESTRING)
		{
			mausdictStream.close();
			return tokens.at(1);
		}
	}
	
	mausdictStream.close();
	CHUNKER_ERR << "No pause model in MAUS DICT" CHUNKER_ENDERR2
}

Dictionary::Dictionary()
{
	readMAUSDictionary();
}

Dictionary::Dictionary(UniqueIterator iter) : Dictionary()
{
	std::string pausemodel = getPauseModel();
	for(; ! iter.done(); ++iter)
	{
		_dict.emplace(iter.getLMRepresentation(), pausemodel);
	}
}

Dictionary::Dictionary(std::unique_ptr<TranscriptionIterator> iter) : Dictionary()
{
	std::string pausemodel = getPauseModel();
	for(; ! iter->done(); ++(*iter))
	{
		_dict.emplace(iter->getLMRepresentation(), pausemodel);
	}
}

void Dictionary::print(std::ostream & stream) const
{
	for(const auto & pair : _dict)
	{
		stream << pair.first << " " << pair.second << std::endl;
		
	}
}

std::ostream & operator << (std::ostream & stream, const Dictionary & dict)
{CHUNKER_FLOG
	dict.print(stream);
}
