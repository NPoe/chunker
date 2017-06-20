#include "ChunkerGlobal.hpp"

int main(int argc, char ** argv)
{
	
	try
	{
		ChunkerManager::init(argc, argv);

		std::unique_ptr<Chunker> chunker;
		
		if(ChunkerManager::getOptionString(ChunkerManager::FORCE) == "true")
		{
			chunker.reset(new ChunkerForced);
		}
		else
		{
			chunker.reset(new ChunkerClassic);
		}

		chunker->run();	
		chunker->writeOutput();
	}

	catch(int status)
	{
		return status;
	}

	return 0;
}

