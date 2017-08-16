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
	static const unsigned short KANINVENTAR;
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

	static std::mutex _lock; /**< Lock that can only be owned by one thread at a time (to prevent race conditions) */
	

	static std::map<unsigned short, std::array<std::string, 2> > _indexToValue; /**< Maps argument indices to their boost value (0) and normal value (1) */


	static int _currentThreads; /**< Number of threads currently running */

	static const std::map<std::string, unsigned short> KEYTOINDEX; /**< Maps argument strings to their indices */
	static const std::map<unsigned short, unsigned short> INDEXTOTYPE; /**< Maps argument indices to their type indices (important to check argument validity) */

	static std::map<unsigned short, std::string> _indexToKey; /**< Reverse-map of KEYTOINDEX */
	
	/** \brief Private constructor
	 *
	 *  Not to be used, since this is a static class.
	 */
	ChunkerManager();

	/**
	 * \brief Read argument vector
	 * \param argv
	 */
	static void readArgv(std::vector<std::string>);

	/**
	 * \brief Read configuation file
	 * \param configfile Path to configuation file
	 *
	 */
	static void readConfig(const std::string &);

	/**
	 * \brief Check if string contains boost_ prefix and cut off that prefix
	 * \param key Pointer to argument string
	 * \returns boolean that indicates whether there was a boost_ prefix
	 */
	static bool deriveStage(std::string *);
	
	/**
	 * \brief Check whether all options are valid (e.g. valid integers), otherwise throw error
	 */
	static void checkOptions();

	/**
	 * \brief Fill option table
	 */
	static void fillOptions();
	
	/** \brief initialize manager by parsing arguments and config file
	 *
	 * \param argv vectorized and stringified argument vector
	 */
	static void init(std::vector<std::string>);

public:
	static std::ofstream LOG; /**< Output stream to log file */
	
	/**
	 * \brief Get lock before changing a variable shared by different threads
	 * \returns Pointer to lock
	 */
	static std::unique_ptr<std::lock_guard<std::mutex> > getLock();
	
	/**
	 * \brief Check if the boost phase is used (or if there is at least one boost_ parameter)
	 * \returns boolean indicating whether to start boost phase
	 */
	static bool isBoosted();

	/** \brief initialize manager by parsing arguments and config file
	 *
	 * \param argc argument count as passed to main
	 * \param argv argument vector as passed to main
	 */
	static void init(int, char**);
	
	
	/** \brief Return option value as string
	 * \param option Option index
	 * \note Throws an exception if the boost value and the normal value differ
	 */
	static const std::string& getOptionString(unsigned short);
	
	/** \brief Return option value as string
	 * \param option Option index
	 * \param boost if 0, return boost parameter value; if 1, return normal parameter value
	 */
	static const std::string& getOptionString(unsigned short, bool);

	/** \brief Check if an option is equal for all stages
	 *
	 * \param option Option index
	 * \note This function throws an error if an option with an explicit boost parameter is queried without 
	 * information on whether the boost parameter is meant
	 */
	static void checkNonBoostable(unsigned short);
	
	/** \brief Return option value as integer
	 * \param option Option index
	 * \note Throws an exception if the boost value and the normal value differ
	 * \note Throws an exception if the parameter is not a valid integer parameter
	 */
	static int getOptionInt(unsigned short);
	
	/** \brief Return option value as integer
	 * \param option Option index
	 * \param boost if 0, return boost parameter value; if 1, return normal parameter value
	 * \note Throws an exception if the parameter is not a valid integer parameter
	 */
	static int getOptionInt(unsigned short, bool);
	
	/** \brief Return option value as long integer
	 * \param option Option index
	 * \note Throws an exception if the boost value and the normal value differ
	 * \note Throws an exception if the parameter is not a valid long integer parameter
	 */
	static long getOptionLong(unsigned short);
	
	/** \brief Return option value as long integer
	 * \param option Option index
	 * \param boost if 0, return boost parameter value; if 1, return normal parameter value
	 * \note Throws an exception if the parameter is not a valid long integer parameter
	 */
	static long getOptionLong(unsigned short, bool);
	
	/** \brief Return option value as float
	 * \param option Option index
	 * \note Throws an exception if the boost value and the normal value differ
	 * \note Throws an exception if the parameter is not a valid float parameter
	 */
	static float getOptionFloat(unsigned short);
	
	/** \brief Return option value as long integer
	 * \param option Option index
	 * \param boost if 0, return boost parameter value; if 1, return normal parameter value
	 * \note Throws an exception if the parameter is not a valid float parameter
	 */
	static float getOptionFloat(unsigned short, bool);
	
	/** \brief Return option value as boolean
	 * \param option Option index
	 * \note Throws an exception if the boost value and the normal value differ
	 * \note Throws an exception if the parameter is not a valid boolean parameter
	 */
	static bool getOptionBool(unsigned short);	
	
	/** \brief Return option value as boolean
	 * \param option Option index
	 * \param boost if 0, return boost parameter value; if 1, return normal parameter value
	 * \note Throws an exception if the parameter is not a valid boolean parameter
	 */
	static bool getOptionBool(unsigned short, bool);	

	/**
	 * \brief Check whether a new thread can be started
	 * \returns true if a new thread can be started, false otherwise
	 */
	static bool getThread();
	
	/**
	 * \brief Inform manager that a thread has finished
	 */
	static void returnThread();
};

#endif
