#ifndef CHUNKER_MANAGER_HPP
#define CHUNKER_MANAGER_HPP


class ChunkerManager
{
public:
	
	static const unsigned short VERBOSE; 
	static const unsigned short LANGUAGE;
	static const unsigned short CLEAR;

	static const unsigned short CHUNKERTYPE;
	static const unsigned short LANGUAGEMODEL;
	static const unsigned short LANGUAGEMODELWEIGHT;
	static const unsigned short BIGRAMWEIGHT;
	static const unsigned short RECURSION;
	static const unsigned short ALIGNER;
	static const unsigned short WINDOWSIZE;
	static const unsigned short MAUS;
	static const unsigned short BOOST;
	static const unsigned short ALIGNASYM;
	static const unsigned short FORCE;

	static const unsigned short MINCHUNKDURATION; 
	static const unsigned short MAXCHUNKDURATION;
	static const unsigned short FORCECHUNKDURATION;
	static const unsigned short SILENCEONLY;
	static const unsigned short WARNCHUNKDURATION;
	
	static const unsigned short MINANCHORLENGTH;
	static const unsigned short MAXANCHORCOST;
	static const unsigned short MINANCHORSINGLETONS;
	
	static const unsigned short MAXRECURSIONDEPTH;
	static const unsigned short SPLITDURATION;

	static const unsigned short AUDIOFILE;
	static const unsigned short BPFFILE;
	static const unsigned short OUTFILE;
	static const unsigned short OUTTYPE;
	static const unsigned short MMFFILE;
	static const unsigned short EXCEPTIONFILE; 
	static const unsigned short WORKDIR;
	static const unsigned short LOGFILE;
	static const unsigned short CONFIG;
	static const unsigned short INSYMBOLS;

	static const unsigned short MAXNUMTHREADS;
	static const unsigned short HVITEBEAMWIDTH;
	static const unsigned short HTKDURATIONFACTOR;

	static const unsigned short WORDVAREXE;
	static const unsigned short GRAPHINVENTAR;
	static const unsigned short RULESET;
	static const unsigned short MAUSDICT;
	static const unsigned short MAUSHMMINVENTAR;

	static const unsigned short BOOLTYPE;
	static const unsigned short INTEGERTYPE;
	static const unsigned short PROBABILITYTYPE;
	static const unsigned short INTEGERGREATERTWOTYPE;
	static const unsigned short FILEREADTYPE;
	static const unsigned short FILEWRITETYPE;
	static const unsigned short DIRWRITETYPE;
	static const unsigned short INTEGERGREATERONETYPE;
	static const unsigned short STRINGTYPE;

private:

	static std::mutex _lock;
	ChunkerManager();

	static std::map<unsigned short, std::array<std::string, 2> > _indexToValue;


	static int _currentThreads;

	static const std::map<unsigned short, std::string> INDEXTODEFAULT;	
	static const std::map<std::string, unsigned short> KEYTOINDEX; 
	static const std::map<unsigned short, unsigned short> INDEXTOTYPE;

	static std::map<unsigned short, std::string> _indexToKey;

	static void readArgv(int, std::vector<std::string>);
	
	static void readConfig(const std::string &);

	static bool deriveStage(std::string *);
	static void checkOptions();
	static void fillOptions();
public:
	static std::unique_ptr<std::lock_guard<std::mutex> >getLock();
	static bool isBoosted();
	static bool isForced();
	static std::ofstream LOG;
	
	static void init(int, char**);
	static void init(std::vector<std::string>);

	
	static const std::string& getOptionString(unsigned short);
	static const std::string& getOptionString(unsigned short, bool);

	static void checkNonBoostable(unsigned short);
	
	static int getOptionInt(unsigned short);
	static int getOptionInt(unsigned short, bool);
	
	static long getOptionLong(unsigned short);
	static long getOptionLong(unsigned short, bool);
	
	static float getOptionFloat(unsigned short);
	static float getOptionFloat(unsigned short, bool);
	
	static bool getOptionBool(unsigned short);	
	static bool getOptionBool(unsigned short, bool);	

	static bool getThread();
	
	static void returnThread();
};

#endif
