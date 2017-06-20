#include "ChunkerGlobal.hpp"

ProgressBar::ProgressBar(long maximum)
{
	_maximum = maximum;
}

void ProgressBar::set(long value)
{
	int length = value * (BARLENGTH - 1) / _maximum + 1;
	CHUNKER_VERB1 << "\r" << "    [" << std::string(length, '#') << 
		std::string(BARLENGTH - length, '-') << "]    " CHUNKER_ENDVERB
}
