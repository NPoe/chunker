#ifndef CHUNKER_PROGRESSBAR_HPP
#define CHUNKER_PROGRESSBAR_HPP

class ProgressBar
{
private:
	long _maximum = 0;
	static const int BARLENGTH = 60;
public:
	ProgressBar(long);
	void set(long);
};

#endif
