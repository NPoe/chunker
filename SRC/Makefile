CPPFLAGS=-pthread -std=c++11


all: \
	ChunkerMain.o \
	ChunkerUtil.o \
	ChunkerTurnTier.o \
	ChunkerChunker.o \
	ChunkerAligner.o \
	ChunkerTranscription.o \
	ChunkerRecognition.o \
	ChunkerTranscriptionSymbols.o \
	ChunkerRecognitionSymbols.o \
	ChunkerIterators.o \
	ChunkerProbabilities.o \
	ChunkerUnitModeller.o \
	ChunkerLanguageModel.o \
	ChunkerDictionary.o \
	ChunkerManager.o \
	ChunkerBoundaries.o \
	ChunkerAlignPoint.o \
	ChunkerAnchor.o \
	ChunkerHVite.o \
	ChunkerProgressBar.o

	g++ ${CPPFLAGS} \
	-o ../MAUSROOT/Chunker/chunker.binary \
	ChunkerUtil.o \
	ChunkerManager.o \
	ChunkerMain.o \
	ChunkerTurnTier.o \
	ChunkerChunker.o \
	ChunkerAligner.o \
	ChunkerTranscription.o \
	ChunkerRecognition.o \
	ChunkerTranscriptionSymbols.o \
	ChunkerRecognitionSymbols.o \
	ChunkerIterators.o \
	ChunkerProbabilities.o \
	ChunkerUnitModeller.o \
	ChunkerLanguageModel.o \
	ChunkerDictionary.o \
	ChunkerBoundaries.o \
	ChunkerAlignPoint.o \
	ChunkerAnchor.o \
	ChunkerHVite.o \
	ChunkerProgressBar.o

ChunkerMain.o: ChunkerMain.cpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerMain.cpp -o ChunkerMain.o

ChunkerChunker.o: ChunkerChunker.cpp ChunkerChunker.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerChunker.cpp -o ChunkerChunker.o

ChunkerTurnTier.o: ChunkerTurnTier.cpp ChunkerTurnTier.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerTurnTier.cpp -o ChunkerTurnTier.o

ChunkerAligner.o: ChunkerAligner.cpp ChunkerAligner.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerAligner.cpp -o ChunkerAligner.o

ChunkerTranscriptionSymbols.o: ChunkerTranscriptionSymbols.cpp ChunkerTranscriptionSymbols.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerTranscriptionSymbols.cpp -o ChunkerTranscriptionSymbols.o

ChunkerRecognitionSymbols.o: ChunkerRecognitionSymbols.cpp ChunkerRecognitionSymbols.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerRecognitionSymbols.cpp -o ChunkerRecognitionSymbols.o

ChunkerTranscription.o: ChunkerTranscription.cpp ChunkerTranscription.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerTranscription.cpp -o ChunkerTranscription.o

ChunkerRecognition.o: ChunkerRecognition.cpp ChunkerRecognition.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerRecognition.cpp -o ChunkerRecognition.o

ChunkerIterators.o: ChunkerIterators.cpp ChunkerIterators.hpp ChunkerIterators.hpp
	g++ ${CPPFLAGS} -c ChunkerIterators.cpp -o ChunkerIterators.o

ChunkerProbabilities.o: ChunkerProbabilities.cpp ChunkerProbabilities.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerProbabilities.cpp -o ChunkerProbabilities.o

ChunkerUnitModeller.o: ChunkerUnitModeller.cpp ChunkerUnitModeller.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerUnitModeller.cpp -o ChunkerUnitModeller.o

ChunkerLanguageModel.o: ChunkerLanguageModel.cpp ChunkerLanguageModel.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerLanguageModel.cpp -o ChunkerLanguageModel.o

ChunkerManager.o: ChunkerManager.cpp ChunkerManager.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerManager.cpp -o ChunkerManager.o

ChunkerUtil.o: ChunkerUtil.cpp ChunkerUtil.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerUtil.cpp -o ChunkerUtil.o

ChunkerBoundaries.o: ChunkerBoundaries.cpp ChunkerBoundaries.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerBoundaries.cpp -o ChunkerBoundaries.o

ChunkerAlignPoint.o: ChunkerAlignPoint.cpp ChunkerAlignPoint.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerAlignPoint.cpp -o ChunkerAlignPoint.o

ChunkerDictionary.o: ChunkerDictionary.cpp ChunkerDictionary.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerDictionary.cpp -o ChunkerDictionary.o

ChunkerAnchor.o: ChunkerAnchor.cpp ChunkerAnchor.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerAnchor.cpp -o ChunkerAnchor.o

ChunkerProgressBar.o: ChunkerProgressBar.cpp ChunkerProgressBar.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerProgressBar.cpp -o ChunkerProgressBar.o

ChunkerHVite.o: ChunkerHVite.cpp ChunkerHVite.hpp ChunkerGlobal.hpp
	g++ ${CPPFLAGS} -c ChunkerHVite.cpp -o ChunkerHVite.o

clean:
	rm \
	ChunkerMain.o \
	ChunkerManager.o \
	ChunkerTurnTier.o \
	ChunkerChunker.o \
	ChunkerAligner.o \
	ChunkerTranscription.o \
	ChunkerRecognition.o \
	ChunkerTranscriptionSymbols.o \
	ChunkerRecognitionSymbols.o \
	ChunkerIterators.o \
	ChunkerProbabilities.o \
	ChunkerUnitModeller.o \
	ChunkerLanguageModel.o \
	ChunkerDictionary.o \
	ChunkerUtil.o \
	ChunkerBoundaries.o \
	ChunkerAlignPoint.o \
	ChunkerAnchor.o \
	ChunkerHVite.o \
	ChunkerProgressBar.o
