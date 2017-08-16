#include "ChunkerGlobal.hpp"

const unsigned short ChunkerManager::VERBOSE = 0;
const unsigned short ChunkerManager::LANGUAGE = 1;
const unsigned short ChunkerManager::CLEAR = 2;

const unsigned short ChunkerManager::CHUNKERTYPE = 10;
const unsigned short ChunkerManager::LANGUAGEMODEL = 11;
const unsigned short ChunkerManager::LANGUAGEMODELWEIGHT = 12;
const unsigned short ChunkerManager::BIGRAMWEIGHT = 13;
const unsigned short ChunkerManager::RECURSION = 14;
const unsigned short ChunkerManager::ALIGNER = 15;
const unsigned short ChunkerManager::WINDOWSIZE = 16;
const unsigned short ChunkerManager::MAUS = 17;
const unsigned short ChunkerManager::BOOST = 18;
const unsigned short ChunkerManager::FORCE = 19;

const unsigned short ChunkerManager::MINCHUNKDURATION = 25;
const unsigned short ChunkerManager::MAXCHUNKDURATION = 26;
const unsigned short ChunkerManager::SILENCEONLY = 27;
const unsigned short ChunkerManager::WARNCHUNKDURATION = 29;	

const unsigned short ChunkerManager::MINANCHORLENGTH = 30;
const unsigned short ChunkerManager::MAXANCHORCOST = 33;
const unsigned short ChunkerManager::MINANCHORSINGLETONS = 34;
	
const unsigned short ChunkerManager::MAXRECURSIONDEPTH = 40;
const unsigned short ChunkerManager::SPLITDURATION = 41;

const unsigned short ChunkerManager::AUDIOFILE = 50;
const unsigned short ChunkerManager::BPFFILE = 51;
const unsigned short ChunkerManager::OUTFILE = 52;
const unsigned short ChunkerManager::OUTTYPE = 53;
const unsigned short ChunkerManager::MMFFILE = 55;
const unsigned short ChunkerManager::EXCEPTIONFILE = 59;
const unsigned short ChunkerManager::WORKDIR = 62;
const unsigned short ChunkerManager::LOGFILE = 63;
const unsigned short ChunkerManager::CONFIG = 66;
const unsigned short ChunkerManager::INSYMBOLS = 67;

const unsigned short ChunkerManager::MAXNUMTHREADS = 70;
const unsigned short ChunkerManager::HVITEBEAMWIDTH = 71;
const unsigned short ChunkerManager::HTKDURATIONFACTOR = 72;

const unsigned short ChunkerManager::WORDVAREXE = 80;
const unsigned short ChunkerManager::GRAPHINVENTAR = 81;
const unsigned short ChunkerManager::RULESET = 82;
const unsigned short ChunkerManager::MAUSDICT = 83;
const unsigned short ChunkerManager::MAUSHMMINVENTAR = 84;
const unsigned short ChunkerManager::KANINVENTAR = 85;

const unsigned short ChunkerManager::BOOLTYPE = 100;
const unsigned short ChunkerManager::INTEGERTYPE = 101;
const unsigned short ChunkerManager::PROBABILITYTYPE = 102;
const unsigned short ChunkerManager::INTEGERGREATERTWOTYPE = 103;
const unsigned short ChunkerManager::FILEREADTYPE = 104;
const unsigned short ChunkerManager::FILEWRITETYPE = 105;
const unsigned short ChunkerManager::DIRWRITETYPE = 106;
const unsigned short ChunkerManager::INTEGERGREATERONETYPE = 107;
const unsigned short ChunkerManager::STRINGTYPE = 108;

const std::map<std::string, unsigned short> ChunkerManager::KEYTOINDEX = 
{
	{"verbose", ChunkerManager::VERBOSE},
	{"language", ChunkerManager::LANGUAGE},
	{"clear", ChunkerManager::CLEAR},
	{"chunkertype", ChunkerManager::CHUNKERTYPE},
	{"languagemodel", ChunkerManager::LANGUAGEMODEL},
	{"languagemodelweight", ChunkerManager::LANGUAGEMODELWEIGHT},
	{"bigramweight", ChunkerManager::BIGRAMWEIGHT},
	{"recursion", ChunkerManager::RECURSION},
	{"aligner", ChunkerManager::ALIGNER},
	{"windowsize", ChunkerManager::WINDOWSIZE},
	{"maus", ChunkerManager::MAUS},
	{"boost", ChunkerManager::BOOST},
	{"force", ChunkerManager::FORCE},
	{"minchunkduration", ChunkerManager::MINCHUNKDURATION},
	{"maxchunkduration", ChunkerManager::MAXCHUNKDURATION},
	{"silenceonly", ChunkerManager::SILENCEONLY},
	{"warnchunkduration", ChunkerManager::WARNCHUNKDURATION},
	{"minanchorlength", ChunkerManager::MINANCHORLENGTH},
	{"maxanchorcost", ChunkerManager::MAXANCHORCOST},
	{"minanchorsingletons", ChunkerManager::MINANCHORSINGLETONS},
	{"maxrecursiondepth", ChunkerManager::MAXRECURSIONDEPTH},
	{"splitduration", ChunkerManager::SPLITDURATION},
	{"audio", ChunkerManager::AUDIOFILE},
	{"bpf", ChunkerManager::BPFFILE},
	{"outfile", ChunkerManager::OUTFILE},
	{"outtype", ChunkerManager::OUTTYPE},
	{"config", ChunkerManager::CONFIG},
	{"insymbols", ChunkerManager::INSYMBOLS},
	{"workdir", ChunkerManager::WORKDIR},
	{"mmffile", ChunkerManager::MMFFILE},
	{"exceptionfile", ChunkerManager::EXCEPTIONFILE},
	{"logfile", ChunkerManager::LOGFILE},
	{"maxnumthreads", ChunkerManager::MAXNUMTHREADS},
	{"hvitebeamwidth", ChunkerManager::HVITEBEAMWIDTH},
	{"htkdurationfactor", ChunkerManager::HTKDURATIONFACTOR},
	{"wordvar", ChunkerManager::WORDVAREXE},
	{"ruleset", ChunkerManager::RULESET},
	{"graphinventar", ChunkerManager::GRAPHINVENTAR},
	{"kaninventar", ChunkerManager::KANINVENTAR},
	{"dict", ChunkerManager::MAUSDICT},
	{"hmminventar", ChunkerManager::MAUSHMMINVENTAR}
};

const std::map<unsigned short, unsigned short> ChunkerManager::INDEXTOTYPE =
{
	{ChunkerManager::VERBOSE, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::LANGUAGE, ChunkerManager::STRINGTYPE},
	{ChunkerManager::CLEAR, ChunkerManager::BOOLTYPE},
	{ChunkerManager::CHUNKERTYPE, ChunkerManager::CHUNKERTYPE},
	{ChunkerManager::LANGUAGEMODEL, ChunkerManager::LANGUAGEMODEL},
	{ChunkerManager::LANGUAGEMODELWEIGHT, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::BIGRAMWEIGHT, ChunkerManager::PROBABILITYTYPE},
	{ChunkerManager::RECURSION, ChunkerManager::BOOLTYPE},
	{ChunkerManager::ALIGNER, ChunkerManager::ALIGNER},
	{ChunkerManager::FORCE, ChunkerManager::FORCE},
	{ChunkerManager::WINDOWSIZE, ChunkerManager::INTEGERGREATERTWOTYPE},
	{ChunkerManager::MAUS, ChunkerManager::BOOLTYPE},
	{ChunkerManager::BOOST, ChunkerManager::BOOLTYPE},
	{ChunkerManager::MINCHUNKDURATION, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::MAXCHUNKDURATION, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::SILENCEONLY, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::WARNCHUNKDURATION, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::MINANCHORLENGTH, ChunkerManager::INTEGERGREATERTWOTYPE},
	{ChunkerManager::MAXANCHORCOST, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::MINANCHORSINGLETONS, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::MAXRECURSIONDEPTH, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::SPLITDURATION, ChunkerManager::INTEGERGREATERONETYPE},
	{ChunkerManager::AUDIOFILE, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::BPFFILE, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::OUTFILE, ChunkerManager::FILEWRITETYPE},
	{ChunkerManager::OUTTYPE, ChunkerManager::OUTTYPE},
	{ChunkerManager::CONFIG, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::INSYMBOLS, ChunkerManager::INSYMBOLS},
	{ChunkerManager::MMFFILE, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::EXCEPTIONFILE, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::WORKDIR, ChunkerManager::DIRWRITETYPE},
	{ChunkerManager::LOGFILE, ChunkerManager::FILEWRITETYPE},
	{ChunkerManager::MAXNUMTHREADS, ChunkerManager::INTEGERGREATERONETYPE},
	{ChunkerManager::HVITEBEAMWIDTH, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::HTKDURATIONFACTOR, ChunkerManager::INTEGERTYPE},
	{ChunkerManager::RULESET, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::WORDVAREXE, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::GRAPHINVENTAR, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::KANINVENTAR, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::MAUSDICT, ChunkerManager::FILEREADTYPE},
	{ChunkerManager::MAUSHMMINVENTAR, ChunkerManager::FILEREADTYPE}
};


std::map<unsigned short, std::string> ChunkerManager::_indexToKey;
std::map<unsigned short, std::array<std::string, 2> > ChunkerManager::_indexToValue;

int ChunkerManager::_currentThreads = 1;
std::mutex ChunkerManager::_lock;
std::ofstream ChunkerManager::LOG;

void ChunkerManager::fillOptions()
{CHUNKER_FLOG
	for ( auto & option : _indexToValue)
	{
		std::array<std::string, 2> & values = option.second;
		if (values.at(1).empty())
		{
			CHUNKER_ERR << "Missing option value: --" << _indexToKey.at(option.first) CHUNKER_ENDERR1
		}

		if (values.at(0).empty())
		{
			values.at(0) = values.at(1);
		}
	}
}

void ChunkerManager::checkOptions()
{CHUNKER_FLOG
	for ( auto & option : _indexToValue)
	{
		unsigned short index = option.first;
		std::array<std::string, 2> & values = option.second;

		auto res = INDEXTOTYPE.find(index);
		if(res == INDEXTOTYPE.cend())
		{
			CHUNKER_ERR << "Missig type for option: " << index CHUNKER_ENDERR1
		}

		unsigned short type = res->second;

		switch(type)
		{
			case BOOLTYPE:
				for(const std::string & value : values)
				{
					if(value != "0" && value != "1" && value != "true" && value != "false") 
						CHUNKER_ERR << "Not a boolean string: " << value CHUNKER_ENDERR1
				}
				break;

			case INTEGERTYPE:
				for(const std::string & value : values)
				{
					if(!ChunkerUtil::isIntString(value)) 
						CHUNKER_ERR << "Not an interger string: " << value CHUNKER_ENDERR1
				}
				break;

			case INTEGERGREATERONETYPE:
				for(const std::string & value : values)
				{
					if(!ChunkerUtil::isIntString(value) || std::stoi(value) < 1) 
						CHUNKER_ERR << "Not an interger string > 0: " << value CHUNKER_ENDERR1
				}
				break;
			
			case INTEGERGREATERTWOTYPE:	
				for(const std::string & value : values)
				{
					if(!ChunkerUtil::isIntString(value) || std::stoi(value) < 2) 
						CHUNKER_ERR << "Not an interger string > 2: " << value CHUNKER_ENDERR1
				}
				break;
		
			case CHUNKERTYPE:
				for(const std::string & value : values)
				{
					if(value != "t" && value != "tp" && value != "p") 
						CHUNKER_ERR << "Not a valid chunker type: " << value CHUNKER_ENDERR1
				}
				break;
		
			case LANGUAGEMODEL:
				for(const std::string & value : values)
				{
					if ( value != "zerogram" && value != "bigram" && value != "pair" ) 
						CHUNKER_ERR << "Not a valid language model: " << value CHUNKER_ENDERR1
				}
				break;

			case PROBABILITYTYPE:
				for(const std::string & value : values)
				{
					if
					(
				 		!ChunkerUtil::isFloatString(value) || 
						std::stof(value) < 0.0 || 
						std::stof(value) > 1.0
					)	CHUNKER_ERR << "Not a float between 0.0 and 1.0: " << value CHUNKER_ENDERR1
				}
				break;
	
			case OUTTYPE:
				for(const std::string & value : values)
				{
					if( value != "bpf+trn" && value != "trn") 
						CHUNKER_ERR << "Not a valid output type: " << value CHUNKER_ENDERR1
				}
				break;
			
			case ALIGNER:
				for(const std::string & value : values)
				{
					if( value != "hirschberg" && value != "matrix" && value != "fast")
						CHUNKER_ERR << "Not a valid aligner: " << value CHUNKER_ENDERR1
				}
				break;
			
			case FORCE:
				for(const std::string & value : values)
				{
					if( value != "true" && value != "false" && value != "rescue")
						CHUNKER_ERR << "Not a valid force option: " << value CHUNKER_ENDERR1
				}
				break;
			
			case INSYMBOLS:
				for(const std::string & value : values)
				{
					if( value != "sampa" && value != "ipa")
						CHUNKER_ERR << "Not a valid insymbols option: " << value CHUNKER_ENDERR1
				}
				break;

			case FILEREADTYPE:
				for(const std::string & value : values)
				{
					std::ifstream stream(value);
					ChunkerUtil::checkInStream(stream, value);
					stream.close();
				}
				break;
	
			case FILEWRITETYPE:
				for(const std::string & value : values)
				{
					std::ifstream stream(value);
					if(stream.good())
					{
						stream.close();
						break;
					}
					stream.close();
					std::ofstream outStream(value);
					ChunkerUtil::checkOutStream(outStream, value);
					outStream.close();
					remove(value.c_str());
				}
				break;
		
			case DIRWRITETYPE:
				for(const std::string & value : values)
				{
					std::string tmpFile(value + ChunkerUtil::SEP + std::to_string(std::rand()));
					std::ofstream stream(tmpFile);
					ChunkerUtil::checkOutStream(stream, tmpFile);
					remove(tmpFile.c_str());
				}
				break;
			
			case STRINGTYPE:
				break;

			default:
				CHUNKER_ERR << "Unknown option type: " << type CHUNKER_ENDERR1
		}
	}
}

bool ChunkerManager::isBoosted()
{CHUNKER_FLOG
	if(!getOptionBool(BOOST))
	{
		return false;
	}

	for (const auto & entry : _indexToValue)
	{
		if (getOptionString(entry.first, 1) != getOptionString(entry.first, 0))
		{
			return true;
		}
	}
	CHUNKER_WARN << "Boost requested but no boost parameters provided" CHUNKER_ENDWARN
	return false;
}


void ChunkerManager::checkNonBoostable(unsigned short option)
{CHUNKER_FLOG
	if (getOptionString(option, 1) != getOptionString(option, 0))
	{
		CHUNKER_ERR << "Parameter cannot be requested without stage information " << option CHUNKER_ENDERR1
	}	
}

const std::string& ChunkerManager::getOptionString(unsigned short option)
{CHUNKER_FLOG
	checkNonBoostable(option);
	return getOptionString(option, 1);
}

const std::string& ChunkerManager::getOptionString(unsigned short option, bool stage)
{CHUNKER_FLOG
	auto result = _indexToValue.find(option);
	if( result == _indexToValue.cend() ) CHUNKER_ERR << "Unknown option: " << option CHUNKER_ENDERR1

	return result->second.at(stage);
}

int ChunkerManager::getOptionInt(unsigned short option)
{CHUNKER_FLOG
	checkNonBoostable(option);
	return getOptionInt(option, 1);
}

int ChunkerManager::getOptionInt(unsigned short option, bool stage)
{CHUNKER_FLOG
	
	auto res = INDEXTOTYPE.find(option);
	if(res == INDEXTOTYPE.cend())
	{
		CHUNKER_ERR << "No type for option: " << option CHUNKER_ENDERR1
	}
	unsigned short type = res->second;

	if
	( 
		type != INTEGERTYPE && 
		type != INTEGERGREATERONETYPE && 
		type != INTEGERGREATERTWOTYPE
	)	
	{	
		CHUNKER_ERR << "Not an integer option: " << option CHUNKER_ENDERR1
	}

	return std::stoi(getOptionString(option, stage));
}

long ChunkerManager::getOptionLong(unsigned short option)
{CHUNKER_FLOG
	checkNonBoostable(option);
	return getOptionLong(option, 1);
}
long ChunkerManager::getOptionLong(unsigned short option, bool stage)
{CHUNKER_FLOG
	
	auto res = INDEXTOTYPE.find(option);
	if(res == INDEXTOTYPE.cend())
	{
		CHUNKER_ERR << "No type for option: " << option CHUNKER_ENDERR1
	}
	unsigned short type = res->second;

	if
	(
		type != ChunkerManager::INTEGERTYPE && 
		type != ChunkerManager::INTEGERGREATERONETYPE && 
		type != ChunkerManager::INTEGERGREATERTWOTYPE
	)	
	{	
		CHUNKER_ERR << "Not a long integer option: " << option CHUNKER_ENDERR1
	}

	return std::stol(getOptionString(option, stage));
}

float ChunkerManager::getOptionFloat(unsigned short option)
{CHUNKER_FLOG
	checkNonBoostable(option);
	return getOptionFloat(option, 1);
}
float ChunkerManager::getOptionFloat(unsigned short option, bool stage)
{CHUNKER_FLOG
	auto res = INDEXTOTYPE.find(option);
	if(res == INDEXTOTYPE.cend())
	{
		CHUNKER_ERR << "No type for option: " << option CHUNKER_ENDERR1
	}
	unsigned short type = res->second;
	
	if 
	(
		type != PROBABILITYTYPE && 
		type != INTEGERTYPE && 
		type != INTEGERGREATERONETYPE && 
		type != INTEGERGREATERTWOTYPE
	)	
	{	
		CHUNKER_ERR << "Not a float option: " << option CHUNKER_ENDERR1
	}

	return std::stof(getOptionString(option, stage));
}

bool ChunkerManager::getOptionBool(unsigned short option)
{CHUNKER_FLOG
	checkNonBoostable(option);
	return getOptionBool(option, 1);
}
bool ChunkerManager::getOptionBool(unsigned short option, bool stage)
{CHUNKER_FLOG
	auto res = INDEXTOTYPE.find(option);
	if(res == INDEXTOTYPE.cend())
	{
		CHUNKER_ERR << "No type for option: " << option CHUNKER_ENDERR1
	}
	unsigned short type = res->second;
	
	if(type != BOOLTYPE)
	{
		CHUNKER_ERR << "Not a bool option: " << option CHUNKER_ENDERR1
	}

	std::string value = getOptionString(option, stage);
	if(value == "1" || value == "true")
	{
		return true;
	}
	return false;	
}


ChunkerManager::ChunkerManager() 
{CHUNKER_FLOG
	CHUNKER_ERR << "Do not use the ChunkerManager constructor" CHUNKER_ENDERR4
}


void ChunkerManager::init(int argc, char** argv)
{CHUNKER_FLOG
	LOG.open("/dev/null");

	std::vector<std::string> newArgv;

	for (int i = 0; i < argc; i++)
	{
		newArgv.push_back(std::string(argv[i]));
	}
	
	init(newArgv);
}

void ChunkerManager::init(std::vector<std::string> argv)
{CHUNKER_FLOG
	
	_currentThreads = 1;

	int argc = argv.size();

	for(const auto & pair : INDEXTOTYPE)
	{
		unsigned short index = pair.first;
		_indexToValue.emplace(index, std::array<std::string, 2>());
	}
	
	for(const auto & pair : KEYTOINDEX)
	{
		if(!_indexToKey.emplace(pair.second, pair.first).second)
		{
			CHUNKER_ERR << "Double index: " << pair.second CHUNKER_ENDERR2
		}
	}

	readArgv(argv);


	if(_indexToValue.at(CONFIG).at(1).empty())
	{
		CHUNKER_ERR << "Missing command line argument --config" CHUNKER_ENDERR1
	}
			
	readConfig(_indexToValue.at(CONFIG).at(1));		

	fillOptions();	
	checkOptions();
	
	checkOptions();


	LOG.close();
	LOG.open(getOptionString(LOGFILE));
}

bool ChunkerManager::getThread()
{CHUNKER_FLOG
	auto guard = getLock();
	bool success = false;
	if(_currentThreads < getOptionInt(MAXNUMTHREADS))
	{
		_currentThreads ++;
		success = true;
	}
	CHUNKER_LOG3 << " Threading success: " << success << " Current threads: " << _currentThreads CHUNKER_ENDLOG
	return success;
}

std::unique_ptr<std::lock_guard<std::mutex> > ChunkerManager::getLock()
{CHUNKER_FLOG
	return std::unique_ptr<std::lock_guard<std::mutex> >(new std::lock_guard<std::mutex>(_lock));
}

void ChunkerManager::returnThread()
{CHUNKER_FLOG
	auto guard = getLock();
	_currentThreads --;
	
	CHUNKER_LOG3 << " Current threads: " << _currentThreads CHUNKER_ENDLOG
}


void ChunkerManager::readArgv(std::vector<std::string> argv)
{
	for (int i = 0; i < argv.size(); i++)
	{
		std::string option(argv[i]);

		if(option.substr(0, 2) == "--" && i + 1 < argv.size())
		{
			option = ChunkerUtil::lowercase(option).substr(2);
	
			bool stage = deriveStage(&option);
			
			auto res = KEYTOINDEX.find(option);
			
			if(res == KEYTOINDEX.end())
			{
				CHUNKER_ERR << "Unknown option in command line: " << argv[i] CHUNKER_ENDERR1
			}
		
			auto tmp = _indexToValue.find(res->second);
			if(tmp->second.at(stage).empty())
			{
				tmp->second.at(stage) = argv.at(i+1);
			}
		}
	}
}

bool ChunkerManager::deriveStage(std::string * key)
{
	if(key->find(ChunkerUtil::BOOSTSTRING) == 0)
	{
		*key = key->substr(ChunkerUtil::BOOSTSTRING.length());
		return 0;
	}
	
	return 1;
}


void ChunkerManager::readConfig(const std::string & configfile)
{CHUNKER_FLOG

	std::ifstream configStream(configfile);
	ChunkerUtil::checkInStream(configStream, configfile);
	
	std::string buffer;
	while (std::getline(configStream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(!tokens.empty())
		{
			if(tokens.size() != 2)
			{
				CHUNKER_ERR << "Could not parse line " << buffer << 
					" in file " << configfile CHUNKER_ENDERR1
			}
			
			std::string key = tokens.at(0);
			std::string value = tokens.at(1);

			key = ChunkerUtil::lowercase(key);
			
			bool stage = deriveStage(&key);

			auto res = KEYTOINDEX.find(key);
			if(res == KEYTOINDEX.end())
			{
				CHUNKER_ERR << "Unknown option in " << configfile << ": " << key CHUNKER_ENDERR1
			}
		
			auto tmp = _indexToValue.find(res->second);
			if(tmp->second.at(stage).empty())
			{
				tmp->second.at(stage) = value;
			}
		}
	}
}
