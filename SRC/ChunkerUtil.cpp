#include "ChunkerGlobal.hpp"

const std::string ChunkerUtil::PAUSESTRING = "#";
const std::string ChunkerUtil::TAGSTART = "<";
const std::string ChunkerUtil::TAGEND = ">";
const std::string ChunkerUtil::SPECIALSTRING = "___";
const std::string ChunkerUtil::NULLSTRING = "!NULL";
const std::string ChunkerUtil::BOOSTSTRING = "boost_";
const std::set<char> ChunkerUtil::WHITESPACES = {' ', '\t', '\r', '\n'};
const std::string ChunkerUtil::EMPTYSTRING = "";
const std::string ChunkerUtil::KANKEY = "KAN"; 
const std::string ChunkerUtil::ORTKEY = "ORT";
const std::string ChunkerUtil::TRNKEY = "TRN";
const std::string ChunkerUtil::SAMKEY = "SAM";
const std::string ChunkerUtil::LBDKEY = "LBD";
const std::string ChunkerUtil::LHDKEY = "LHD";
const std::string ChunkerUtil::WORDVARSOURCE = "wordvar.mlf";
const std::string ChunkerUtil::WORDVARTARGET = "wordvar.slf";
const std::string ChunkerUtil::WORDVARSTEM = "wordvar";
const std::string ChunkerUtil::HLSTATSMLF = "hlstats_mlf";
const std::string ChunkerUtil::HLSTATSBACKOFF = "hlstats_backoff";
const std::string ChunkerUtil::HLSTATSLATTICE = "hlstats_lattice";
const std::string ChunkerUtil::HLSTATSWORDLIST = "hlstats_wordlist";
const std::string ChunkerUtil::WORDVARBOUNDARY = "#";
const std::set<char> ChunkerUtil::MLFCOMMENTCHARS = {'#', '\"', '.'};

const std::string ChunkerUtil::SEP =
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
	"\\";
#else
	"/";
#endif
	
bool ChunkerUtil::isWavExtension(const std::string& string)
{
	return stringEndswith(string, "wav");
}

bool ChunkerUtil::isNistExtension(const std::string& string)
{
	return stringEndswith(string, "nis") or stringEndswith(string, "nist") or stringEndswith(string, "sph");
}

bool ChunkerUtil::isVideoExtension(const std::string& string)
{
	return stringEndswith(string, "mp4") or stringEndswith(string, "mpeg") or stringEndswith(string, "mpg");
}

bool ChunkerUtil::stringEndswith(const std::string& string, const std::string& extension)
{
	if (string.length() < extension.length())
	{
		return false;
	}

	return std::equal(extension.rbegin(), extension.rend(), string.rbegin());
}

std::string ChunkerUtil::getBPFFile(const std::string & prefix)
{
	return prefix + SEP + "BPF.par";
}

std::string ChunkerUtil::getAudioFile(const std::string & prefix)
{
	return prefix + SEP + "AUDIO.wav";
}

std::string ChunkerUtil::getAudioFileRaw(const std::string & prefix)
{
	return prefix + SEP + "AUDIORAW.wav";
}

std::string ChunkerUtil::getHCopyConfig(const std::string & prefix)
{
	return prefix + SEP + "HCOPYCONFIG";
}

std::string ChunkerUtil::getDictFile(const std::string & prefix)
{
	return prefix + SEP + "DICT";
}

std::string ChunkerUtil::getLMFile(const std::string & prefix)
{
	return prefix + SEP + "LM";
}

std::string ChunkerUtil::getMLFFile(const std::string & prefix)
{
	return prefix + SEP + "MLF";
}

std::string ChunkerUtil::getHTKFile(const std::string & prefix)
{
	return prefix + SEP + "HTK";
}

std::string ChunkerUtil::getMLFInterval(const std::string & prefix, long start)
{
	return getMLFDir(prefix) + SEP + std::to_string(start);
}

std::string ChunkerUtil::getHTKInterval(const std::string & prefix, long start)
{
	return getHTKDir(prefix) + SEP + std::to_string(start);
}

std::string ChunkerUtil::getMLFDir(const std::string & prefix)
{
	return prefix + SEP + "MLFDIR";
}

std::string ChunkerUtil::getHTKDir(const std::string & prefix)
{
	return prefix + SEP + "HTKDIR";
}




std::string ChunkerUtil::lowercase(const std::string & string)
{CHUNKER_FLOG
	std::string result;
	for (char character : string)
	{
		result.push_back(std::tolower(character));
	}
	return result;
}

std::string ChunkerUtil::uppercase(const std::string & string)
{CHUNKER_FLOG
	std::string result;
	for (char character : string)
	{
		result.push_back(std::toupper(character));
	}
	return result;
}

long ChunkerUtil::getSamplingRate(const std::string & audiofile)
{CHUNKER_FLOG

	std::string output;
	std::string cmd("soxi -r " + audiofile);
	ChunkerUtil::wrapOut(cmd, output);
	if(!ChunkerUtil::isLongString(output))
	{
		CHUNKER_ERR << "Cannot parse this sample rate by soxi: " << output CHUNKER_ENDERR2
	}

	return std::stol(output);
}

long ChunkerUtil::getAudioDuration(const std::string & audiofile)
{CHUNKER_FLOG

	long samplingrate = getSamplingRate(audiofile);
	
	std::string output;
	std::string cmd("soxi -ds " + audiofile);
	ChunkerUtil::wrapOut(cmd, output);
	if(!ChunkerUtil::isLongString(output))
	{
		CHUNKER_ERR << "Cannot parse this duration by soxi: " << output CHUNKER_ENDERR2
	}

	return std::stol(output) * ChunkerManager::getOptionLong(ChunkerManager::HTKDURATIONFACTOR) / samplingrate;
}

std::vector<std::string> ChunkerUtil::tokenize(const std::string & line, const std::string & delimiter)
{
	std::string buffer;
	std::vector<std::string> tokens;

	for(auto iter = line.cbegin(); iter != line.cend(); ++iter)
	{
		if(std::string(iter, iter + delimiter.size()).find(delimiter) == 0)
		{
			if(buffer.size() > 0)
			{
				tokens.push_back(buffer);
				buffer.clear();
			}
			buffer.clear();
			iter += delimiter.size() - 1;
		}
		else
		{	
			buffer.push_back(*iter);
		}
	}
	if(buffer.size() > 0)
	{
		tokens.push_back(buffer);
	}

	return tokens;
}

std::vector<std::string> ChunkerUtil::tokenize(const std::string& line, const std::set<char>& delims)
{CHUNKER_FLOG
	CHUNKER_LOG4 << "Line: " << line CHUNKER_ENDLOG
	CHUNKER_LOG4 << "Delimiters: " << std::string(delims.begin(), delims.end()) CHUNKER_ENDLOG

	std::string buffer;
	std::vector<std::string> tokens;
	
	for(auto it = line.begin(); it != line.end(); ++it)
	{
		if (delims.find(*it) != delims.end())
		{
			if(buffer.size() > 0)
			{
				CHUNKER_LOG4 << "New token: " << buffer CHUNKER_ENDLOG
				tokens.push_back(buffer);
			}
			CHUNKER_LOG4 << "Clearing buffer" CHUNKER_ENDLOG
			buffer.clear();
		}
		else
		{
			CHUNKER_LOG4 << "Adding to buffer: " << *it CHUNKER_ENDLOG;
			buffer.push_back(*it);
		}
	}

	if (buffer.size() > 0)
	{
		CHUNKER_LOG4 << "New token: " << buffer CHUNKER_ENDLOG
		tokens.push_back(buffer);
	}

	CHUNKER_LOG4 << "Returning vector with " << tokens.size() << " tokens" CHUNKER_ENDLOG
	return tokens;
}

void ChunkerUtil::createDir(const std::string& dir)
{CHUNKER_FLOG

	if(mkdir(dir.c_str(), S_IRWXU))
	{
		CHUNKER_ERR << "Could not create working directory " << dir CHUNKER_ENDERR4
	}
	
	CHUNKER_LOG4 << "Directory " << dir << " created" CHUNKER_ENDLOG
}



void ChunkerUtil::checkInStream(std::ifstream& stream, const std::string& file)
{CHUNKER_FLOG
	if(!stream.good())
	{
		stream.close();
		CHUNKER_ERR << "Cannot read from file " << file CHUNKER_ENDERR4
	}
}

void ChunkerUtil::checkOutStream(std::ofstream& stream, const std::string& file)
{CHUNKER_FLOG
	if(!stream.good())
	{
		stream.close();
		CHUNKER_ERR << "Cannot write to file " << file CHUNKER_ENDERR4
	}
}

void ChunkerUtil::checkProc(FILE * proc, const std::string& cmd)
{CHUNKER_FLOG
	if ( proc == NULL ) 
	{        
		CHUNKER_ERR << "Could not execute " << cmd CHUNKER_ENDERR4
	}
}

void ChunkerUtil::closeProc(FILE * proc, const std::string& cmd)
{CHUNKER_FLOG
	int status = pclose(proc);
	CHUNKER_LOG4 << "Status: " << status CHUNKER_ENDLOG
	if(status)
	{
		CHUNKER_ERR << "Error when executing " << cmd << " Status: " << status CHUNKER_ENDERR4
	}
}

void ChunkerUtil::wrap(const std::string& cmd)
{CHUNKER_FLOG
	FILE * proc = popen((cmd + " 2>&1").c_str(), "r");
	checkProc(proc, cmd);
	
	std::string output;

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), proc) != NULL)
  	{
		output.append(buffer);
  	}

	CHUNKER_LOG4 << output CHUNKER_ENDLOG
	
	closeProc(proc, cmd);
}

void ChunkerUtil::wrapOut(const std::string& cmd, std::string& output)
{CHUNKER_FLOG

	FILE* proc = popen(cmd.c_str(), "r");
	checkProc(proc, cmd);

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), proc) != NULL)
  	{
		output.append(buffer);
  	}
	closeProc(proc, cmd);
}


bool ChunkerUtil::isIntString(const std::string& token)
{CHUNKER_FLOG
	CHUNKER_LOG4 << "Token: " << token CHUNKER_ENDLOG

	try
	{
		int res = std::stoi(token);
	}
	catch(std::invalid_argument)
	{
		CHUNKER_LOG4 << "Returning false" CHUNKER_ENDLOG
		return false;	
	}
	CHUNKER_LOG4 << "Returning true" CHUNKER_ENDLOG	
	return true;
}

bool ChunkerUtil::isLongString(const std::string& token)
{CHUNKER_FLOG
	CHUNKER_LOG4 << "Token: " << token CHUNKER_ENDLOG

	try
	{
		long res = std::stod(token);
	}
	catch(std::invalid_argument)
	{
		CHUNKER_LOG4 << "Returning false" CHUNKER_ENDLOG
		return false;	
	}
	CHUNKER_LOG4 << "Returning true" CHUNKER_ENDLOG	
	return true;
}

bool ChunkerUtil::isFloatString(const std::string& token)
{CHUNKER_FLOG
	CHUNKER_LOG4 << "Token: " << token CHUNKER_ENDLOG

	try
	{
		float res = std::stof(token);
	}
	catch(std::invalid_argument)
	{
		CHUNKER_LOG4 << "Returning false" CHUNKER_ENDLOG
		return false;	
	}
	CHUNKER_LOG4 << "Returning true" CHUNKER_ENDLOG	
	return true;
}


