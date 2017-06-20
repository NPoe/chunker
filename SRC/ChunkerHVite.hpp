#ifndef CHUNKER_HVITE_HPP
#define CHUNKER_HVITE_HPP


class HViteWrapper
{
private:
	const Chunker * _chunker = NULL;
	void multithreadedHVite(const std::vector<std::pair<long, long> > &);
 	void appendMLF(long, std::ofstream &) const;
	void recognizeInterval(long, long);

public:
	HViteWrapper(const Chunker *);
	void run(const std::vector<std::pair<long, long> > &);
};


#endif
