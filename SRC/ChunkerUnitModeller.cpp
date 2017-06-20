#include "ChunkerGlobal.hpp"

void UnitModeller::setLM(LanguageModel * lm)
{CHUNKER_FLOG
	_lm = lm;
}

unsigned UnitModeller::modelUnit(unsigned in, const UniqueIterator & iter) const
{CHUNKER_FLOG
	return modelUnit(in, iter.getLMRepresentation(), iter.getPhonemes());
}

unsigned UnitModeller::modelUnit(unsigned in, const std::unique_ptr<TranscriptionIterator> & iter) const
{CHUNKER_FLOG
	return modelUnit(in, iter->getLMRepresentation(), iter->getPhonemes());
}


UnitModellerMaus::UnitModellerMaus(const std::string prefix) 
{CHUNKER_FLOG 
	_prefix = prefix; 
}

void UnitModellerMaus::runMaus(const std::deque<TranscriptionPhoneme *> & segmentation) const
{CHUNKER_FLOG
	
	std::string cmd = ChunkerManager::getOptionString(ChunkerManager::WORDVAREXE) +
		" if=" + _prefix + ChunkerUtil::SEP + ChunkerUtil::WORDVARSOURCE + 
		" odir=" + _prefix + 
		" iv=" + ChunkerManager::getOptionString(ChunkerManager::GRAPHINVENTAR) + 
		" rg=" + ChunkerManager::getOptionString(ChunkerManager::RULESET) + 
		" jwk=-1";
	
	std::ofstream sourceStream(_prefix + ChunkerUtil::SEP + ChunkerUtil::WORDVARSOURCE, std::ofstream::out | std::ofstream::trunc);
	ChunkerUtil::checkOutStream(sourceStream, _prefix + ChunkerUtil::SEP + ChunkerUtil::WORDVARSOURCE);

	sourceStream << "#!MLF!#" << std::endl;
	sourceStream << "\"*/" + ChunkerUtil::WORDVARSTEM + ".lab\"" << std::endl;
	sourceStream << ChunkerUtil::WORDVARBOUNDARY << std::endl;
	
	for (const auto & phoneme : segmentation)
	{
		sourceStream << phoneme->getLabel() << std::endl;
	}

	sourceStream << ChunkerUtil::WORDVARBOUNDARY << std::endl;
	sourceStream << ".";

	sourceStream.close();

	ChunkerUtil::wrap(cmd);
}


unsigned UnitModellerMaus::modelUnit(unsigned in, const std::string & unit, const std::deque<TranscriptionPhoneme *> & segmentation) const
{CHUNKER_FLOG
	
	unsigned initialIndex = in;

	runMaus(segmentation);

	std::ifstream targetStream(_prefix + ChunkerUtil::SEP + ChunkerUtil::WORDVARTARGET);
	ChunkerUtil::checkInStream(targetStream, _prefix + ChunkerUtil::SEP + ChunkerUtil::WORDVARTARGET);

	std::string buffer;

	bool seenIn = false;
	unsigned out = -1;

	while(std::getline(targetStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() >= 2)
		{
			if(tokens.at(0).find("I=") == 0 && tokens.at(0).length() > 2 && tokens.at(1).length() > 2)
			{
				std::string indexString = tokens.at(0).substr(2);
				std::string labelString = tokens.at(1).substr(2);
				if (ChunkerUtil::isIntString(indexString))
				{
					if(labelString == ChunkerUtil::WORDVARBOUNDARY)
					{
						if(!seenIn)
						{
							_lm->addNode(initialIndex + std::stoi(indexString), unit);
							seenIn = true;
						}
						else
						{
							out = initialIndex + std::stoi(indexString);
							_lm->addNode(initialIndex + std::stoi(indexString), ChunkerUtil::NULLSTRING);
						}
					}
					else
					{
						_lm->addNode(initialIndex + std::stoi(indexString), labelString);
					}
				}
			}

			else if(tokens.at(0).find("J=") == 0 && tokens.at(1).length() > 2 && tokens.size() >= 3 && tokens.at(2).length() > 2)
			{
				std::string fromString = tokens.at(1).substr(2);
				std::string toString = tokens.at(2).substr(2);
				std::string rest;

				if(tokens.size() > 3)
				{
					rest = tokens.at(3);
					for (int i = 4; i < tokens.size(); i++)
					{
						rest.append("\t" + tokens.at(i) + " ");
					}
				}

				if (ChunkerUtil::isIntString(fromString) && ChunkerUtil::isIntString(toString))
				{
					if(std::stoi(fromString) > 0 || initialIndex + std::stoi(toString) != out)
					{
						_lm->addLink(initialIndex + std::stoi(fromString), initialIndex + std::stoi(toString), rest);
					}
				}

			}
		}
	}

	targetStream.close();

	return out;
}
	

unsigned UnitModellerClassic::modelUnit(unsigned in, const std::string & unit, const std::deque<TranscriptionPhoneme *> & segmentation) const
{CHUNKER_FLOG

	unsigned currentIdx = in;

	_lm->addNode(currentIdx, unit);
	currentIdx ++;
	
	for (const auto & phoneme : segmentation)
	{
		_lm->addNode(currentIdx, phoneme->getLabel());
		_lm->addLink(currentIdx - 1, currentIdx, "");
		currentIdx ++;
	}
		
	_lm->addNode(currentIdx, ChunkerUtil::NULLSTRING);
	_lm->addLink(currentIdx - 1, currentIdx, "");

	return currentIdx;
}


