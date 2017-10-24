#ifndef CHUNKER_UTIL_HPP
#define CHUNKER_UTIL_HPP


/**
 * \class ChunkerUtil
 * \brief Namespace for different utility functions and variables
 */
class ChunkerUtil
{
public:
	static const std::string PAUSESTRING; /**< Name of optional pause acoustic model */ 
	static const std::string TAGSTART; /**< Name of non-optional start pause acoustic model */
	static const std::string TAGEND; /**< Name of non-optional end pause acoustic model */
	static const std::string SPECIALSTRING; /**< String used to delimit complex units in language model */
	static const std::string NULLSTRING; /**< HTK lattice's name for non-emitting states */
	static const std::string BOOSTSTRING; /**< Prefix for parameters applicable to boost phase (e.g. boost_lm) */
	static const std::set<char> WHITESPACES; /**< Known white space delimiter characters */
	static const std::string EMPTYSTRING; /**< Empty string '' */
	static const std::string KANKEY; /**< BPF key for canonical pronunciation tier */
	static const std::string ORTKEY; /**< BPF key for orthographic tier */
	static const std::string TRNKEY; /**< BPF key for chunk / turn tier */
	static const std::string SAMKEY; /**< BPF key for sampling rate entry */
	static const std::string LBDKEY; /**< BPF key for BPF body start */
	static const std::string LHDKEY; /**< BPF key for BPF body start */
	static const std::string HLSTATSMLF; /**< /deprecated */
	static const std::string HLSTATSBACKOFF; /**< /deprecated */
	static const std::string HLSTATSLATTICE; /**< /deprecated */
	static const std::string HLSTATSWORDLIST; /**< /deprecated */
	static const std::string WORDVARSOURCE; /**< /deprecated */
	static const std::string WORDVARTARGET; /**< /deprecated */
	static const std::string WORDVARSTEM; /**< /deprecated */
	static const std::string WORDVARBOUNDARY; /**< /deprecated */
	static const std::set<char> MLFCOMMENTCHARS; /**< Characters at the beginning of label file lines that we need to ignore */
	static const std::string SEP; /**< File path separator string (e.g. /) on current machine */
	static const long STANDARDSAMPLERATE = 16000; /**< Sampling rate used for downsampling */

	/**
	 * \brief Return path to current chunker's BPF file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to BPF file
	 *
	 * Important: The BPF file only exists for the topmost chunker!
	 */
	static std::string getBPFFile(const std::string &);

	/**
	 * \brief Return path to current chunker's HCopy file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to HCopy file
	 */
	static std::string getHCopyConfig(const std::string &);
	
	/**
	 * \brief Return path to current chunker's HTK directory
	 * \param prefix Current chunker's directory prefix
	 * \returns path to HTK directory
	 *
	 * The HTK directory stores HTK intervals for multithreading.
	 */
	static std::string getHTKDir(const std::string &);
	
	/**
	 * \brief Return path to current chunker's MLF directory
	 * \param prefix Current chunker's directory prefix
	 * \returns path to MLF directory
	 *
	 * The HTK directory stores MLF intervals produced by multithreading.
	 */
	static std::string getMLFDir(const std::string &);

	/**
	 * \brief Return path to current chunker's overall HTK file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to overall HTK file
	 */
	static std::string getHTKFile(const std::string &);
	
	/**
	 * \brief Return path to current chunker's overall MLF file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to overall MLF file
	 *
	 * The overall MLF file is produced by concatenating MLF files from individual processes.
	 */
	static std::string getMLFFile(const std::string &);

	/**
	 * \brief Return path to current chunker's HTK interval with given start time
	 * \param prefix Current chunker's directory prefix
	 * \param start Interval start time
	 * \returns path to interval's HTK file
	 */
	static std::string getHTKInterval(const std::string &, long);
	
	/**
	 * \brief Return path to current chunker's MLF interval with given start time
	 * \param prefix Current chunker's directory prefix
	 * \param start Interval start time
	 * \returns path to interval's MLF file
	 */
	static std::string getMLFInterval(const std::string &, long);

	/**
	 * \brief Return path to current chunker's HTK dictionary file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to HTK dictionary file
	 */
	static std::string getDictFile(const std::string &);
	
	/**
	 * \brief Return path to current chunker's HTK language model file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to language model file
	 */
	static std::string getLMFile(const std::string &);

	/**
	 * \brief Return path to current chunker's downsampled audio file
	 * \param prefix Current chunker's directory prefix
	 * \returns path to audio file
	 *
	 * Important: The audio file only exists for the topmost chunker!
	 */
	static std::string getAudioFile(const std::string &);

	/**
	 * \brief Return path to current chunker's audio file (not downsampled yet)
	 * \param prefix Current chunker's directory prefix
	 * \returns path to audio file
	 *
	 * Important: The audio file only exists for the topmost chunker!
	 */
	static std::string getAudioFileRaw(const std::string &);
	
	/**
	 * \brief Convert string into lowercase
	 * \param string
	 * \return String in lowercase
	 */
	static std::string lowercase(const std::string &);
	
	/**
	 * \brief Convert string into uppercase
	 * \param string
	 * \return String in uppercase
	 */
	static std::string uppercase(const std::string &);

	/**
	 * \brief Tokenize string by splitting at certain characters
	 * \param line String to be split
	 * \param delims Set of characters used for delimitation (e.g. all whitespace characters)
	 * \returns Vector of constituent strings
	 */
	static std::vector<std::string> tokenize(const std::string&, const std::set<char>&);
	
	/**
	 * \brief Tokenize string by splitting at a certain string
	 * \param line String to be split
	 * \param delimiter String used for delimitation (e.g. "___")
	 * \returns Vector of constituent strings
	 */
	static std::vector<std::string> tokenize(const std::string&, const std::string&);

	/**
	 * \brief Create new directory
	 * \param dir Path to new directory
	 */
	static void createDir(const std::string&);

	/**
	 * \brief Check if stream can be read from
	 * \param stream Stream to be checked
	 * \param file File name (only used for error message)
	 */
	static void checkInStream(std::ifstream&, const std::string&);

	/**
	 * \brief Check if stream can be written to
	 * \param stream Stream to be checked
	 * \param file File name (only used for error message)
	 */
	static void checkOutStream(std::ofstream&, const std::string&);

	/**
	 * \brief Check if process started without error
	 * \param proc FILE pointer to process
	 * \param cmd Command string (only used for error message)
	 */
	static void checkProc(FILE *, const std::string&);

	/**
	 * \brief Wait for and close process
	 * \param proc FILE pointer to process
	 * \param cmd Command string (only used for error message)
	 */
	static void closeProc(FILE *, const std::string&);

	/**
	 * \brief Execute command
	 * \param cmd Command to be executed
	 */
	static void wrap(const std::string&);

	/**
	 * \brief Execute command and write output to string
	 * \param cmd Command to be executed
	 * \param output String that output will be written to
	 */
	static void wrapOut(const std::string&, std::string&);

	/**
	 * \brief Derive sampling rate of audio file
	 * \param audiofile Path of audio file
	 * \returns sampling rate
	 * Uses soxi internally!
	 */
	static long getSamplingRate(const std::string &);
	
	/**
	 * \brief Derive duration of audio file
	 * \param audiofile Path of audio file
	 * \returns audio duration
	 * Uses soxi internally!
	 */
	static long getAudioDuration(const std::string &);

	/**
	 * \brief Check if string is a valid integer string
	 * \param token String
	 * \returns Whether the string is a valid integer
	 */
	static bool isIntString(const std::string&);

	/**
	 * \brief Check if string is a valid long integer string
	 * \param token String
	 * \returns Whether the string is a valid long integer
	 */
	static bool isLongString(const std::string&);

	/**
	 * \brief Check if string is a valid float string
	 * \param token String
	 * \returns Whether the string is a valid float
	 */
	static bool isFloatString(const std::string&);
	
	/**
	 * \brief Check if string ends with another string (e.g., an extension
	 * \param string long string
	 * ¸param extension the extension to be checked
	 * \returns Whether the long string ends in this extension
	 */
	static bool stringEndswith(const std::string&, const std::string&);
	
	static bool isWavExtension(const std::string&);
	static bool isNistExtension(const std::string&);
	static bool isVideoExtension(const std::string&);
};
#endif
