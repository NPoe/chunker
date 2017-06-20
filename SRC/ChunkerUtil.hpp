#ifndef CHUNKER_UTIL_HPP
#define CHUNKER_UTIL_HPP




class ChunkerUtil
{
public:
	static const std::string PAUSESTRING; 
	static const std::string TAGSTART; 
	static const std::string TAGEND; 
	static const std::string SPECIALSTRING; 
	static const std::string NULLSTRING; 
	static const std::string BOOSTSTRING;
	static const std::set<char> WHITESPACES; 
	static const std::string EMPTYSTRING; 
	static const std::string KANKEY; 
	static const std::string ORTKEY; 
	static const std::string TRNKEY; 
	static const std::string SAMKEY; 
	static const std::string LBDKEY; 
	static const std::string HLSTATSMLF;
	static const std::string HLSTATSBACKOFF;
	static const std::string HLSTATSLATTICE;
	static const std::string HLSTATSWORDLIST;
	static const std::string WORDVARSOURCE;
	static const std::string WORDVARTARGET;
	static const std::string WORDVARSTEM;
	static const std::string WORDVARBOUNDARY;
	static const std::set<char> MLFCOMMENTCHARS;
	static const std::string SEP;

	static std::string getBPFFile(const std::string &);
	static std::string getHCopyConfig(const std::string &);
	static std::string getHTKDir(const std::string &);
	static std::string getMLFDir(const std::string &);
	static std::string getHTKFile(const std::string &);
	static std::string getMLFFile(const std::string &);
	static std::string getHTKInterval(const std::string &, long);
	static std::string getMLFInterval(const std::string &, long);
	static std::string getDictFile(const std::string &);
	static std::string getLMFile(const std::string &);
	static std::string getAudioFile(const std::string &);


	static const long STANDARDSAMPLERATE = 16000;
	static std::string lowercase(const std::string &);
	static std::string uppercase(const std::string &);

	static std::vector<std::string> tokenize(const std::string&, const std::set<char>&);
	static std::vector<std::string> tokenize(const std::string&, const std::string&);

	static void createDir(const std::string&);

	static void checkInStream(std::ifstream&, const std::string&);

	static void checkOutStream(std::ofstream&, const std::string&);

	static void checkProc(FILE *, const std::string&);

	static void closeProc(FILE *, const std::string&);

	static void wrap(const std::string&);

	static void wrapOut(const std::string&, std::string&);

	static long getSamplingRate(const std::string &);
	
	static long getAudioDuration(const std::string &);

	static bool isIntString(const std::string& token);

	static bool isLongString(const std::string& token);

	static bool isFloatString(const std::string& token);
	

};
#endif
