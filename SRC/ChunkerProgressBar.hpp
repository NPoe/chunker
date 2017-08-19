#ifndef CHUNKER_PROGRESSBAR_HPP
#define CHUNKER_PROGRESSBAR_HPP

/**
 * \class ProgressBar
 * \brief Progress bar (used if --verbose == 1)
 */
class ProgressBar
{
private:
	long _maximum = 0; /**< Full duration of audio file */
	static const int BARLENGTH = 60; /**< Length of progress bar in characters */
public:
	/**
	 * \brief Constructor
	 * \param maximum Full duration of audio file
	 */
	ProgressBar(long);

	/**
	 * \brief Set progress bar
	 * \param value Current position in audio file
	 */
	void set(long);
};

#endif
