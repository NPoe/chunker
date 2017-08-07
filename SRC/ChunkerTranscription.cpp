#include "ChunkerGlobal.hpp"


bool Transcription::orderByLength::operator() (const std::string & first, const std::string & second) 
{CHUNKER_FLOG
	
	if(first.size() != second.size())
	{ 
		return first.size() > second.size(); 
	}

	return first < second;
}


std::unique_ptr<TranscriptionIterator> Transcription::languageModelIterator(bool stage) const
{CHUNKER_FLOG
	std::string type = ChunkerManager::getOptionString(ChunkerManager::CHUNKERTYPE, stage);

	std::unique_ptr<TranscriptionIterator> iter;

	if(type == "p") { iter.reset(new TranscriptionPhoneIterator(_transcript)); }

	if(type == "t" || type == "tp") { iter.reset(new TranscriptionTokenIterator(_transcript)); }

	iter->deactivateSilentSegments();

	return iter;
}
	
std::unique_ptr<TranscriptionIterator> Transcription::alignmentIterator(bool stage) const
{CHUNKER_FLOG
	std::string type = ChunkerManager::getOptionString(ChunkerManager::CHUNKERTYPE, stage);
	
	std::unique_ptr<TranscriptionIterator> iter;

	if(type == "p" || type == "tp") { iter.reset(new TranscriptionPhoneIterator(_transcript)); }

	if(type == "t") { iter.reset(new TranscriptionTokenIterator(_transcript)); }

	iter->deactivateSilentSegments();

	return iter;
}


long Transcription::minIndex() const
{CHUNKER_FLOG
	return _transcript.cbegin()->second.getIndex();
}

long Transcription::maxIndex() const
{CHUNKER_FLOG
	return _transcript.crbegin()->second.getIndex();
}

void Transcription::extractORTTier()
{CHUNKER_FLOG
	const std::string & bpfFile = ChunkerManager::getOptionString(ChunkerManager::BPFFILE);
	std::ifstream bpfStream(bpfFile);
	ChunkerUtil::checkInStream(bpfStream, bpfFile);
	CHUNKER_LOG4 << "BPF: " << bpfFile CHUNKER_ENDLOG
	
	std::string buffer;
	
	while(getline(bpfStream, buffer))
	{
		CHUNKER_LOG4 << "Line: " << buffer CHUNKER_ENDLOG
		if(buffer.find(ChunkerUtil::ORTKEY) == 0) { parseORTLine(buffer, bpfFile); }
	}
}

void Transcription::parseORTLine(const std::string & line, const std::string & bpfFile)
{CHUNKER_FLOG
	const auto & tokens = ChunkerUtil::tokenize(line, ChunkerUtil::WHITESPACES);
	if(!(tokens.size() >= 3 && ChunkerUtil::isLongString(tokens.at(1))))
	{
		CHUNKER_LOG4 << "Cannot parse line " << line << " in file " << bpfFile CHUNKER_ENDLOG
	}
	else
	{
		_ortMap.emplace(std::stol(tokens.at(1)), tokens.at(2));	
	}
}

void Transcription::extractKANTier()
{CHUNKER_FLOG
	const std::string & bpfFile = ChunkerManager::getOptionString(ChunkerManager::BPFFILE);
	std::ifstream bpfStream(bpfFile);
	ChunkerUtil::checkInStream(bpfStream, bpfFile);
	CHUNKER_LOG4 << "BPF: " << bpfFile CHUNKER_ENDLOG

	bool seenKAN(false);
	std::string buffer;
	while(getline(bpfStream, buffer))
	{
		CHUNKER_LOG4 << "Line: " << buffer CHUNKER_ENDLOG
		if(buffer.find(ChunkerUtil::KANKEY) == 0)
		{ 
			parseKANLine(buffer, bpfFile); 
			seenKAN = true;
		}
	}
	
	bpfStream.close();

	if(!seenKAN)
	{
		CHUNKER_ERR << "BPF file does not contain a KAN tier" CHUNKER_ENDERR1
	}
}

void Transcription::parseKANLine(const std::string & line, const std::string & bpfFile)
{CHUNKER_FLOG

	const auto & tokens = ChunkerUtil::tokenize(line, ChunkerUtil::WHITESPACES);
	if(!(tokens.size() >= 3 && ChunkerUtil::isLongString(tokens.at(1))))
	{
		CHUNKER_ERR << "Cannot parse line " << line << " in file " << bpfFile CHUNKER_ENDERR1
	}
	
	const long idx = std::stol(tokens.at(1));
	std::string label;
	std::deque<TranscriptionPhoneme *> segmentation;

	segmentWithBlanks(tokens, &label, &segmentation);
		
	if(_terms.find(label) == _terms.end())
	{
		{
			_terms.emplace(label, TranscriptionTerm(label, segmentation));
		}
	}
	
	std::map<std::string, TranscriptionTerm>::iterator res = _terms.find(label);

	if(res != _terms.end())
	{
		std::string ort = label;
		std::map<long, std::string>::iterator ortres = _ortMap.find(idx);
		if(ortres != _ortMap.end())
		{
			ort = ortres->second;
		}
			

		_transcript.emplace(idx, TranscriptionToken(idx, ort, &(res->second)));
	}
}
			

void Transcription::segmentWithBlanks
(
 	const std::vector<std::string> & tokens, 
	std::string * label,
	std::deque<TranscriptionPhoneme *> * segmentation
)
{CHUNKER_FLOG
	std::vector<std::string>::const_iterator iter = tokens.cbegin() + 2;
	for(auto iter = tokens.cbegin() + 2; iter != tokens.cend(); ++iter)
	{
		segmentWithoutBlanks(*iter, segmentation);
		label->append(*iter);
	}
}

std::string Transcription::escapePhoneme
(
	const std::string & phoneme
)
{CHUNKER_FLOG
	std::string escaped(phoneme);

	if(ChunkerUtil::isIntString(phoneme.substr(0,1)))
	{
		escaped.insert(escaped.begin(), 'P');
	}

	if(phoneme.back() == '\\')
	{
		escaped.pop_back();
		escaped.push_back('-');
	}

	return escaped;
}

void Transcription::segmentWithoutBlanks
(
 	const std::string & token,
	std::deque<TranscriptionPhoneme * > * segmentation
)
{CHUNKER_FLOG
	auto iter = token.cbegin();
	while(iter != token.cend())
	{
		bool flag(false);
		std::string remains(iter, token.cend());
	
		CHUNKER_LOG4 << "Remains: " << remains CHUNKER_ENDLOG

		if(remains.find('<') == 0 and remains.find('>') < remains.size())
		{
			std::string tag(remains, 0, remains.find('>') + 1);
			if(_labelSet.find(tag) == _labelSet.end())
			{
				iter += tag.size();
				if(tag.size() == token.size())
				{
					auto finder = _phonemes.emplace(tag, TranscriptionPhoneme(tag)).first;
					segmentation->push_back(&(finder->second));
				}
				continue;
			}
		}
		
		for (auto phoneme : _labelSet)
		{
			CHUNKER_LOG4 << "Pattern: " << phoneme CHUNKER_ENDLOG
			if (phoneme.size() <= remains.size() && phoneme == remains.substr(0, phoneme.size()))
			{
				CHUNKER_LOG4 << "Match in labels" CHUNKER_ENDLOG
				iter += phoneme.size();
				CHUNKER_LOG4 << "Advancing by: " << phoneme.size() CHUNKER_ENDLOG

				phoneme = escapePhoneme(phoneme);
				
				auto finder = _phonemes.emplace(phoneme, TranscriptionPhoneme(phoneme)).first;
				segmentation->push_back(&(finder->second));
				flag = true;
				break;
			}
		}

		if(flag) continue;
		
		for (const auto & exception : _exceptionSet)
		{
			CHUNKER_LOG4 << "Pattern: " << exception CHUNKER_ENDLOG
			if (exception.size() <= remains.size() && exception == remains.substr(0, exception.size()))
			{
				CHUNKER_LOG4 << "Match in exceptions" CHUNKER_ENDLOG
				iter += exception.size();
				CHUNKER_LOG4 << "Advancing by: " << exception.size() CHUNKER_ENDLOG
				flag = true;
				break;
			}
		}

		if(flag) continue;


		CHUNKER_ERR << "Unknown pattern in string " << token << " Suffix: " << remains CHUNKER_ENDERR1
	}
}	
	

TranscriptionTokenIterator Transcription::getTokenIterator() const 
{CHUNKER_FLOG
	return TranscriptionTokenIterator(_transcript); 
}

TranscriptionPhoneIterator Transcription::getPhoneIterator() const 
{CHUNKER_FLOG
	return TranscriptionPhoneIterator(_transcript); 
}


Transcription::Transcription()
{CHUNKER_FLOG
	std::string buffer;

	std::ifstream labelSetStream(ChunkerManager::getOptionString(ChunkerManager::KANINVENTAR));
	ChunkerUtil::checkInStream(labelSetStream, ChunkerManager::getOptionString(ChunkerManager::KANINVENTAR));

	while(std::getline(labelSetStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() > 1)
		{
			CHUNKER_ERR << "Cannot parse line " << buffer << " in file " <<
				ChunkerManager::getOptionString(ChunkerManager::KANINVENTAR) CHUNKER_ENDERR1
		}

		if(tokens.size() == 1)
		{
			std::string label(tokens.at(0));

			if(label != ChunkerUtil::PAUSESTRING)
			{
				_labelSet.emplace(label);
			}
		}
	}

	labelSetStream.close();
	buffer.clear();

	std::ifstream exceptionStream(ChunkerManager::getOptionString(ChunkerManager::EXCEPTIONFILE));
	ChunkerUtil::checkInStream(exceptionStream, ChunkerManager::getOptionString(ChunkerManager::EXCEPTIONFILE));

	while(std::getline(exceptionStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(tokens.size() > 1)
		{
			CHUNKER_ERR << "Cannot parse line " << buffer << " in file " <<
				ChunkerManager::getOptionString(ChunkerManager::EXCEPTIONFILE) CHUNKER_ENDERR1
		}
		if(tokens.size() == 1)
		{
			_exceptionSet.emplace(tokens.at(0));
		}
	}

	CHUNKER_LOG3 << "Label Set:" CHUNKER_ENDLOG
	for (const auto & label : _labelSet)
	{
		CHUNKER_LOG3 << label << " " << label CHUNKER_ENDLOG
	}

	CHUNKER_LOG3 << "Exception Set:" CHUNKER_ENDLOG
	for(const auto & exception : _exceptionSet)
	{
		CHUNKER_LOG3 << exception CHUNKER_ENDLOG
	}

	exceptionStream.close();
	
	extractORTTier();
	extractKANTier();
}



Transcription::Transcription(const Transcription & original, long start, long end)
{CHUNKER_FLOG
	_transcript.insert(original._transcript.find(start), original._transcript.find(end));
}
