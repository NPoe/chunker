#include "ChunkerGlobal.hpp"

std::ostream & operator << (std::ostream & stream, const LanguageModel & model)
{CHUNKER_FLOG
	model.print(stream);
}

void LanguageModel::addNode(unsigned idx, const std::string& label) 
{CHUNKER_FLOG
	_nodes.push_back(std::make_tuple(idx, label));
}

void LanguageModel::addLink(unsigned from, unsigned to, std::string prob) 
{CHUNKER_FLOG
	_links.push_back(std::make_tuple(from, to, prob)); 
}


void LanguageModel::print(std::ostream & stream) const
{CHUNKER_FLOG
	stream << "V=1.0" << std::endl;
	stream << "N=" << _nodes.size() << "\tL=" << _links.size() << std::endl;

	for(const auto & node : _nodes)
	{
		stream << "I=" << std::get<0>(node) << "\tW=" << std::get<1>(node) << std::endl;
	}
		
	unsigned counter = 0;

	for(const auto & link : _links)
	{
		stream << "J=" << counter << "\tS=" << std::get<0>(link) << "\tE=" << std::get<1>(link) << "\t" <<
			std::get<2>(link) << std::endl;
		counter++;
	}
	stream.flush();
}


LanguageModel::LanguageModel(std::unique_ptr<UnitModeller> modeller)
{CHUNKER_FLOG

	_modeller = std::move(modeller);
	_modeller->setLM(this);

	addNode(0, ChunkerUtil::NULLSTRING);
	addNode(1, ChunkerUtil::NULLSTRING);
	addNode(2, ChunkerUtil::PAUSESTRING);
		
	addLink(0, 1, "");
	addLink(1, 2, "");
}

LMZerogram::LMZerogram(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG	

	for(UniqueIterator tmp = UniqueIterator(iter->copyAsTranscriptionIterator()); !tmp.done(); ++tmp)
	{
		unsigned in = _nodes.size();

		addLink(1, in, "");

		unsigned out = _modeller->modelUnit(in, tmp);

		addLink(out, 1, "");
	}
}

LMUnigram::LMUnigram(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	UnigramProbabilities uprobs(iter->copyAsTranscriptionIterator());

	for(UniqueIterator tmp = UniqueIterator(iter->copyAsTranscriptionIterator()); !tmp.done(); ++tmp)
	{
		unsigned in = _nodes.size();
		addLink(1, in, "l="+std::to_string(log(uprobs.getUnigramProb(tmp.getUnit()))));
		
		unsigned out = _modeller->modelUnit(in, tmp);
		
		addLink(out, 1, "");
	}
}



LMBigram::LMBigram(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	UnigramProbabilities uprobs(iter->copyAsTranscriptionIterator());
	BigramProbabilities bprobs(iter->copyAsTranscriptionIterator());

	std::map<std::string, std::pair<unsigned, unsigned> > unitToIdx;

	for(UniqueIterator tmp = UniqueIterator(iter->copyAsTranscriptionIterator()); !tmp.done(); ++tmp)
	{
		unsigned in = _nodes.size();
		unitToIdx[tmp.getUnit()] = std::make_pair(in, - 1);
		
		addLink(0, in, "l="+std::to_string(
			log(ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT) *
				bprobs.getBigramProb(ChunkerUtil::NULLSTRING, tmp.getUnit()) +
				(1.0 - ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT)) *
				uprobs.getUnigramProb(tmp.getUnit()))));
		
		unsigned out = _modeller->modelUnit(in, tmp);

		addLink(out , 1, "l="+std::to_string(
			log(ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT) *
				bprobs.getBigramProb(tmp.getUnit(), ChunkerUtil::NULLSTRING) +
				(1.0 - ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT)) *
				uprobs.getUnigramProb(ChunkerUtil::NULLSTRING))));

		unitToIdx[tmp.getUnit()].second = out;
	}

	for(UniqueIterator outer = UniqueIterator(iter->copyAsTranscriptionIterator()); !outer.done(); ++outer)
	{	
		for(UniqueIterator inner = UniqueIterator(iter->copyAsTranscriptionIterator()); !inner.done(); ++inner)
		{
			addLink(unitToIdx.at(outer.getUnit()).second, unitToIdx.at(inner.getUnit()).first,
				"l="+std::to_string(log(ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT) * 
					bprobs.getBigramProb(outer.getUnit(), inner.getUnit()) +
					(1.0 - ChunkerManager::getOptionFloat(ChunkerManager::BIGRAMWEIGHT)) * 
					uprobs.getUnigramProb(inner.getUnit()))
				)
			);
		}
	}	
}

LMHTK::LMHTK(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller, const std::string & prefix) : LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	std::string mlffile = prefix + ChunkerUtil::SEP + ChunkerUtil::HLSTATSMLF;
	std::string wordlistfile = prefix + ChunkerUtil::SEP + ChunkerUtil::HLSTATSWORDLIST;
	std::string backofffile = prefix + ChunkerUtil::SEP + ChunkerUtil::HLSTATSBACKOFF;
	std::string latticefile = prefix + ChunkerUtil::SEP + ChunkerUtil::HLSTATSLATTICE;
	

	std::ofstream mlfstream(mlffile);
	mlfstream << "#!MLF!#" << std::endl;
	for(std::unique_ptr<TranscriptionIterator> itercopy = iter->copyAsTranscriptionIterator(); !itercopy->done(); ++(*itercopy))
	{
		mlfstream << itercopy->getLMRepresentation() << std::endl;
	}
	mlfstream << ".";
	mlfstream.close();

	std::ofstream wordliststream(wordlistfile);
	
	for(UniqueIterator uniqueIter = UniqueIterator(iter->copyAsTranscriptionIterator()); !uniqueIter.done(); ++uniqueIter)
	{
		wordliststream << uniqueIter.getLMRepresentation() << std::endl;	
	}

	wordliststream << "!ENTER" << std::endl << "!EXIT" << std::endl;
	wordliststream.close();
	

	std::string cmdHLStats = "HLStats -b " + backofffile + " -o " + wordlistfile + " " + mlffile;
	ChunkerUtil::wrap(cmdHLStats);

	std::string cmdHBuild = "HBuild -n " + backofffile + " " + wordlistfile + " " + latticefile;
	ChunkerUtil::wrap(cmdHBuild);
	
	std::ifstream backoffstream(latticefile);
	std::string buffer;

	std::map<std::string, long> wToIdx;
	std::map<long, std::pair<long, long> > idxToIdx;

	_links.clear();
	_nodes.clear();

	while(std::getline(backoffstream, buffer))
	{
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(buffer.find("I=") == 0)
		{
			if(tokens.size() < 2 || tokens.at(0).size() < 3 || (!ChunkerUtil::isLongString(tokens.at(0).substr(2))) ||
					tokens.at(1).find("W=") != 0 || tokens.at(1).substr(2).size() < 3)
			{
				CHUNKER_ERR << "Cannot parse line " << buffer << " in " << backofffile CHUNKER_ENDERR2
			}

			std::string label = tokens.at(1).substr(2);
			if(label == "!NULL" || label == "!ENTER" || label == "!EXIT")
			{
				std::string l;
				if(label == "!NULL")
				{
					l = label;
				}
				else
				{
					l = ChunkerUtil::PAUSESTRING;
				}
				
				long node = _nodes.size();
				addNode(node, l);
				idxToIdx[std::stol(tokens.at(0).substr(2))] = std::make_pair(node, node);
			}
			else
			{
				wToIdx[tokens.at(1).substr(2)] = std::stol(tokens.at(0).substr(2));
			}
		}
	}

	backoffstream.close();


	for(UniqueIterator uniqueIter = UniqueIterator(iter->copyAsTranscriptionIterator()); !uniqueIter.done(); ++uniqueIter)
	{
		std::string w = uniqueIter.getLMRepresentation();
		long in = _nodes.size();
		long out = _modeller->modelUnit(in, uniqueIter);
		idxToIdx[wToIdx.at(w)] = std::make_pair(in, out);
	}

	backoffstream.open(latticefile);

	while(std::getline(backoffstream, buffer))
	{	
		auto tokens = ChunkerUtil::tokenize(buffer, ChunkerUtil::WHITESPACES);
		if(buffer.find("J=") == 0)
		{
			if(tokens.size() < 4 || tokens.at(1).find("S=") != 0 || tokens.at(2).find("E=") != 0 || 
					tokens.at(3).find("l=") != 0 || tokens.at(1).size() < 3 || 
					tokens.at(2).size() < 3 || tokens.at(3).size() < 3 || 
					(!ChunkerUtil::isLongString(tokens.at(1).substr(2))) ||
					(!ChunkerUtil::isLongString(tokens.at(2).substr(2))) || 
					(!ChunkerUtil::isLongString(tokens.at(3).substr(2))))
			{
				CHUNKER_ERR << "Cannot parse line " << buffer << " in " << backofffile CHUNKER_ENDERR2
			}
			long start = std::stol(tokens.at(1).substr(2));
			long end = std::stol(tokens.at(2).substr(2));
		
			addLink(idxToIdx.at(start).second, idxToIdx.at(end).first, "l=" + tokens.at(3).substr(2));
		}
	}

	backoffstream.close();
}

LMPair::LMPair(std::unique_ptr<TranscriptionIterator> iterfirst, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	std::unique_ptr<TranscriptionIterator> itersecond = iterfirst->copyAsTranscriptionIterator();
	++(*iterfirst);

	std::set<std::pair<std::string, std::string> > seenPairs;

	for(; !iterfirst->done(); ++(*iterfirst), ++(*itersecond))
	{
		if (seenPairs.emplace(std::make_pair(itersecond->getUnit(), iterfirst->getUnit())).second)
		{
			unsigned inFirst = _nodes.size();
			addLink(1, inFirst, "");

			unsigned outFirst = _modeller->modelUnit(inFirst, itersecond);
			unsigned inSecond = _nodes.size();
			addLink(outFirst, inSecond, "");
			
			unsigned outSecond = _modeller->modelUnit(inSecond, iterfirst);
			addLink(outSecond, 1, "");
		}
	}
}

LMTriple::LMTriple(std::unique_ptr<TranscriptionIterator> iterfirst, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	std::unique_ptr<TranscriptionIterator> itersecond = iterfirst->copyAsTranscriptionIterator();
	std::unique_ptr<TranscriptionIterator> iterthird = iterfirst->copyAsTranscriptionIterator();
	++(*iterfirst);
	++(*iterfirst);
	++(*itersecond);
	
	std::set<std::tuple<std::string, std::string, std::string> > seenTriples;

	for(; !iterfirst->done(); ++(*iterfirst), ++(*itersecond), ++(*iterthird))
	{
		if (seenTriples.emplace(std::make_tuple(iterthird->getUnit(), itersecond->getUnit(), iterfirst->getUnit())).second)
		{
			unsigned inFirst = _nodes.size();
			addLink(1, inFirst, "");
			unsigned outFirst = _modeller->modelUnit(inFirst, iterthird);
			
			unsigned inSecond = _nodes.size();
			addLink(outFirst, inSecond, "");
			unsigned outSecond = _modeller->modelUnit(inSecond, itersecond);
			
			unsigned inThird = _nodes.size();
			addLink(outSecond, inThird, "");
			unsigned outThird = _modeller->modelUnit(inThird, iterfirst);

			addLink(outThird, 1, "");
		}
	}
}

LMFactorAutomaton::LMFactorAutomaton(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	_links.clear();
	_nodes.clear();

	addNode(0, ChunkerUtil::NULLSTRING);
	addNode(1, ChunkerUtil::NULLSTRING);

	for(; !iter->done(); ++(*iter))
	{
		unsigned in = _nodes.size();
		addLink(0, in, "");

		unsigned out = _modeller->modelUnit(in, iter);
		addLink(out, 1, "");
		addLink(out, _nodes.size(), "");
	}

	_links.pop_back();
}
	

LMForcedAlign::LMForcedAlign(std::unique_ptr<TranscriptionIterator> iter, std::unique_ptr<UnitModeller> modeller) : 
	LanguageModel(std::move(modeller))
{CHUNKER_FLOG

	_links.clear();
	_nodes.clear();

	for(; !iter->done(); ++(*iter))
	{
		unsigned in = _nodes.size();
		unsigned out = _modeller->modelUnit(in, iter);
		addLink(out, _nodes.size(), "");
	}

	addNode(_nodes.size(), ChunkerUtil::PAUSESTRING);
}
