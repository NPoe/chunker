#include "ChunkerGlobal.hpp"

Aligner::Aligner()
{CHUNKER_FLOG}

Aligner::Aligner(std::unique_ptr<TranscriptionIterator> transIter, std::unique_ptr<RecognitionIterator> recogIter)
{CHUNKER_FLOG

	for(; ! transIter->done(); ++(*transIter))
	{
		CHUNKER_LOG4 << "Pushing " << transIter->getUnit() << " to _transcriptPoints" CHUNKER_ENDLOG
		_transcriptPoints.push_back(transIter->copyAsBaseIteratorShared());
	}
	for(; ! recogIter->done(); ++(*recogIter))
	{
		CHUNKER_LOG4 << "Pushing " << recogIter->getUnit() << " to _recognitionPoints" CHUNKER_ENDLOG
		_recognitionPoints.push_back(recogIter->copyAsBaseIteratorShared());
	}
}



AlignHirschberg::AlignHirschberg(std::unique_ptr<TranscriptionIterator> iter1, std::unique_ptr<RecognitionIterator> iter2) : 
	Aligner (std::move(iter1), std::move(iter2)) 
{CHUNKER_FLOG}

void AlignHirschberg::run(std::deque<AlignPoint> * path)
{CHUNKER_FLOG
	
	// Assign _long Vec (the axis that will be split by the 'middle column' of the Hirschberg algorithm) and _shortVec
	_longVec = &_transcriptPoints;
	_shortVec = &_recognitionPoints;

	if(_transcriptPoints.size() < _recognitionPoints.size())
	{
		_longVec = &_transcriptPoints;
		_shortVec = &_recognitionPoints;
	}

	// Hirschberg algorithm from start to end in both dimensions	
	std::deque<AlignPoint> pathTmp = hirschberg(_longVec->begin(), _longVec->end(), _shortVec->begin(), _shortVec->end());
	std::swap(*path, pathTmp);
}




std::deque<AlignPoint> AlignHirschberg::hirschberg
(
 	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
)
{CHUNKER_FLOG

	CHUNKER_LOG3 << "Start X " << std::distance(_longVec->begin(), startX) << 
		" End X " << std::distance(_longVec->begin(), endX) << 
		" Start Y " << std::distance(_shortVec->begin(), startY) << 
		" End Y " << std::distance(_shortVec->begin(), endY) CHUNKER_ENDLOG


	if(startX == endX)
	{
		return trivialCaseWidth0(startY, endY);
	}
	if(startY == endY)
	{
		return trivialCaseHeight0(startX, endX);
	}
	if(startX + 1 == endX)
	{
		return trivialCaseWidth1(startX, endX, startY, endY);
	}
	if(startY + 1 == endY)
	{
		return trivialCaseHeight1(startX, endX, startY, endY);
	}

	return nonTrivialCase(startX, endX, startY, endY);
	
}

std::deque<AlignPoint> AlignHirschberg::trivialCaseWidth0
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
)
{
	std::deque<AlignPoint> path;
	CHUNKER_LOG3 << "Empty string on X side" CHUNKER_ENDLOG
	// Push back insertions
	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpY = startY; tmpY != endY; ++tmpY)
	{
		path.push_back(AlignPoint(std::shared_ptr<BaseIterator>(), (*tmpY)->copyAsBaseIteratorShared()));
	}
	return path;
}

std::deque<AlignPoint> AlignHirschberg::trivialCaseHeight0
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX
)
{
	std::deque<AlignPoint> path;
	CHUNKER_LOG3 << "Empty string on Y side" CHUNKER_ENDLOG
	// Push back deletions
	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpX = startX; tmpX != endX; ++tmpX)
	{
		path.push_back(AlignPoint((*tmpX)->copyAsBaseIteratorShared(), std::shared_ptr<BaseIterator>()));
	}
	return path;
}

std::deque<AlignPoint> AlignHirschberg::trivialCaseWidth1
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
)
{
	std::deque<AlignPoint> path;
	CHUNKER_LOG3 << "Size 1 string on X side" CHUNKER_ENDLOG
	bool flag = true;
	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpY = startY; tmpY != endY; ++tmpY)
	{
		// If there is the possibility of a zero-substitution, or this is the last candidate and we have
		// been unable to do a zero-substitution all along
		if(flag && ((*startX)->getUnit() == (*tmpY)->getUnit() || tmpY == endY - 1))
		{
			path.push_back(AlignPoint((*startX)->copyAsBaseIteratorShared(), (*tmpY)->copyAsBaseIteratorShared()));
			flag = false;
		}
		else
		{
			// push back an insertion
			path.push_back(AlignPoint(std::shared_ptr<BaseIterator>(), (*tmpY)->copyAsBaseIteratorShared()));
		}
	}
	return path;	
}

std::deque<AlignPoint> AlignHirschberg::trivialCaseHeight1
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
)
{
	std::deque<AlignPoint> path;
	CHUNKER_LOG3 << "Size 1 string on Y side" CHUNKER_ENDLOG
	bool flag = true;
	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpX = startX; tmpX != endX; ++tmpX)
	{
		if(flag && ((*tmpX)->getUnit() == (*startY)->getUnit() || tmpX == endX - 1))
		{
			path.push_back(AlignPoint((*tmpX)->copyAsBaseIteratorShared(), (*startY)->copyAsBaseIteratorShared()));
			flag = false;
		}
		else
		{
			path.push_back(AlignPoint((*tmpX)->copyAsBaseIteratorShared(), std::shared_ptr<BaseIterator>()));
		}
	}
	return path;	
}

std::deque<AlignPoint> AlignHirschberg::nonTrivialCase
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
)
{
	std::deque<AlignPoint> path;

	std::vector<std::shared_ptr<BaseIterator> >::iterator midPointX = startX + std::distance(startX, endX) / 2;
	//int midPointX = startX + (endX - startX) / 2; // X coordinate of the 'middle column'

	std::vector<unsigned> forwardpass;
	std::vector<unsigned> backwardpass;

	// If we get permission for one more thread, perform the forward pass in that thread
	// and the backward pass in the current thread
	if(ChunkerManager::getThread())
	{
		std::thread otherThread = 
			std::thread( [=, &forwardpass] 
			{ 
				forwardpass = getFirstHalfCosts(startX, midPointX, startY, endY);
				ChunkerManager::returnThread();
			} );
		backwardpass = getSecondHalfCosts(midPointX, endX, startY, endY);
		otherThread.join();
	}

	// Else, perform both passes in the current thread, sequentially
	else
	{
		forwardpass = getFirstHalfCosts(startX, midPointX, startY, endY);
		backwardpass = getSecondHalfCosts(midPointX, endX, startY, endY);
	}


	// If something has gone wrong
	if(forwardpass.size() != backwardpass.size())
	{
		CHUNKER_ERR << "Forward pass size " << forwardpass.size() << 
			" != backward pass size " << backwardpass.size() CHUNKER_ENDERR4
	}
	
	if(forwardpass.empty() || backwardpass.empty())
	{
		CHUNKER_ERR << "Zero pass sizes: " << forwardpass.size() << 
			" " << backwardpass.size() CHUNKER_ENDERR4
	}	


	int min = forwardpass.front() + backwardpass.front();

	std::vector<std::shared_ptr<BaseIterator> >::iterator midPointY = startY;

	// Get the Y coordinate of the field in the 'middle column' with the minimum sum of forward and backward costs
	for (int i = 0; i < forwardpass.size() - 1; i++)
	{
		int sum = forwardpass.at(i) + backwardpass.at(i);
		CHUNKER_LOG3 << forwardpass.at(i) << " " << backwardpass.at(i) << " " << sum CHUNKER_ENDLOG
		if(sum < min)
		{
			min = sum;
			midPointY = startY + i;
		}
	}

	CHUNKER_LOG3 << "Mid points: " << std::distance(_longVec->begin(), midPointX) << " " << 
		std::distance(_shortVec->begin(), midPointY) CHUNKER_ENDLOG

	// If somthing has gone wrong
	if(midPointX > endX || midPointY > endY || midPointX < startX || midPointY < startY)
	{
		CHUNKER_ERR << "Y midpoint issue" CHUNKER_ENDERR4
	}

	std::deque<AlignPoint> tmpleft; 
	std::deque<AlignPoint> tmpright; 
	
	// If we get permission for one more thread, perform Hirschberg algorithm on the top-left submatrix in that thread
	// and the Hirschberg algorithm on the bottom-right submatrix in the current thread
	if(ChunkerManager::getThread())
	{
		std::thread otherThread( [=, &tmpleft] 
		{ 
			tmpleft = hirschberg(startX, midPointX, startY, midPointY); 
			ChunkerManager::returnThread();
		});
		tmpright = hirschberg(midPointX, endX, midPointY, endY);
		otherThread.join();
	}

	// Else, perform sequentially
	else
	{
		tmpleft = hirschberg(startX, midPointX, startY, midPointY);
		tmpright = hirschberg(midPointX, endX, midPointY, endY);
	}

	// Top-left path -> beginning of current path
	path.insert(path.begin(), tmpleft.begin(), tmpleft.end());

	// Bottom-right path -> end of current path
	path.insert(path.end(), tmpright.begin(), tmpright.end());

	return path;
}



std::vector<unsigned> AlignHirschberg::getFirstHalfCosts
(
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
//	const int startX, const int endX, const int startY, const int endY
)
{CHUNKER_FLOG	
	CHUNKER_LOG3 << "Start X: " << std::distance(_longVec->begin(), startX) << " End X: " << std::distance(_longVec->begin(), endX) << 
		" Start Y: " << std::distance(_shortVec->begin(), startY) << " End Y: " << std::distance(_shortVec->begin(), endY) CHUNKER_ENDLOG

	// Container for the O(n) pass over the matrix	
	std::vector<unsigned> container;
	container.resize(std::distance(startY, endY) + 1);

	for(int tmp = 0; tmp < container.size(); tmp++)
	{
		container[tmp] = tmp;
	}
	
	CHUNKER_LOG4 << "Container resized to " << container.size() << " and filled " CHUNKER_ENDLOG
 
	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpX = startX; tmpX != endX; ++tmpX)
	{
		CHUNKER_LOG5 << "Column " << std::distance(_longVec->begin(), tmpX) CHUNKER_ENDLOG
		unsigned diag = container[0];
		container[0] ++;

		for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpY = startY; tmpY != endY; ++tmpY)
		{
			CHUNKER_LOG5 << "Row " << std::distance(_shortVec->begin(), tmpY) CHUNKER_ENDLOG
			int containerPos = std::distance(startY, tmpY) + 1;
			CHUNKER_LOG5 << "Container position " << containerPos << "/" << container.size() CHUNKER_ENDLOG

			unsigned subscost = 1;
			if((*tmpX)->getUnit() == (*tmpY)->getUnit())
			{
				subscost = 0;
			}

			unsigned newDiag = container[containerPos];
			container[containerPos] = std::min
			(
				diag + subscost,
				std::min
				(
					container[containerPos-1] + 1,
					container[containerPos] + 1
				)
			);

			CHUNKER_LOG5 << "Setting " << containerPos << " to " << container[containerPos] CHUNKER_ENDLOG
			diag = newDiag;
		}
	}
	container.shrink_to_fit();
	return container;
}


// like getFirstHalfCosts, only backwards
std::vector<unsigned> AlignHirschberg::getSecondHalfCosts
(
 	const std::vector<std::shared_ptr<BaseIterator> >::iterator startX,
 	const std::vector<std::shared_ptr<BaseIterator> >::iterator endX,
 	const std::vector<std::shared_ptr<BaseIterator> >::iterator startY,
 	const std::vector<std::shared_ptr<BaseIterator> >::iterator endY
//	const int startX, const int endX, const int startY, const int endY
)
{CHUNKER_FLOG
	CHUNKER_LOG3 << "Start X: " << std::distance(_longVec->begin(), startX) << " End X: " << std::distance(_longVec->begin(), endX) << 
		" Start Y: " << std::distance(_shortVec->begin(), startY) << " End Y: " << std::distance(_shortVec->begin(), endY) CHUNKER_ENDLOG
	
	std::vector<unsigned> container;
	container.resize(std::distance(startY, endY) + 1);

	for(int tmp = 0; tmp < container.size(); tmp++)
	{
		container[tmp] = (int)container.size() - tmp - 1;
	}	

	for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpX = endX - 1; tmpX != startX - 1; --tmpX)
	{
		CHUNKER_LOG5 << "Column: " << std::distance(_longVec->begin(), tmpX) CHUNKER_ENDLOG
		unsigned diag(container.back());
		container.back() ++;

		for(std::vector<std::shared_ptr<BaseIterator> >::iterator tmpY = endY - 1; tmpY != startY - 1; --tmpY)
		{
			CHUNKER_LOG5 << "Row: " << std::distance(_shortVec->begin(), tmpY) CHUNKER_ENDLOG
			int containerPos = std::distance(startY, tmpY);
			CHUNKER_LOG5 << "Container position: " << containerPos CHUNKER_ENDLOG

			unsigned subscost = 1;
			if((*tmpX)->getUnit() == (*tmpY)->getUnit())
			{
				subscost = 0;
			}


			unsigned oldDiag = container[containerPos];
			container[containerPos] = std::min(
				diag + subscost,
				std::min(
					container[containerPos+1] + 1,
					container[containerPos] + 1
				)
			);
			diag = oldDiag;

			CHUNKER_LOG5 << "Setting " << containerPos << " to " << container[containerPos] CHUNKER_ENDLOG
		}
	}
	container.shrink_to_fit();
	return container;
}


AlignFast::AlignFast(std::unique_ptr<TranscriptionIterator> iter1, std::unique_ptr<RecognitionIterator> iter2) : 
	Aligner (std::move(iter1), std::move(iter2)) 
{CHUNKER_FLOG}


void AlignFast::run(std::deque<AlignPoint> * path)
{CHUNKER_FLOG

	std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptStart = _transcriptPoints.begin();
	std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionStart = _recognitionPoints.begin();
	
	std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptEnd = _transcriptPoints.begin();
	std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionEnd = _recognitionPoints.begin();

	while(transcriptEnd != _transcriptPoints.end() && recognitionEnd != _recognitionPoints.end())
	{

		transcriptEnd = _transcriptPoints.end();
		recognitionEnd = _recognitionPoints.end();

		if(std::distance(transcriptStart, transcriptEnd) > ChunkerManager::getOptionInt(ChunkerManager::WINDOWSIZE))
		{
			transcriptEnd = transcriptStart + ChunkerManager::getOptionInt(ChunkerManager::WINDOWSIZE);
		}

		if(std::distance(recognitionStart, recognitionEnd) > ChunkerManager::getOptionInt(ChunkerManager::WINDOWSIZE))
		{
			recognitionEnd = recognitionStart + ChunkerManager::getOptionInt(ChunkerManager::WINDOWSIZE);
		}
		
		
		AlignMatrix tmp_aligner(transcriptStart, transcriptEnd, recognitionStart, recognitionEnd);
		tmp_aligner.fillMatrix();
		
		std::deque<AlignPoint> windowPath;
		std::deque<AlignPoint>::iterator endIter;
		
		if(transcriptEnd == _transcriptPoints.end() && recognitionEnd == _recognitionPoints.end())
		{
			windowPath = tmp_aligner.findPathRestricted();
			endIter = windowPath.end();
		}
		else
		{
			windowPath = tmp_aligner.findPathUnrestricted();
			endIter = windowPath.begin() + (windowPath.size() + 1) / 2;
		}

		if(windowPath.size() > 0)
		{
			CHUNKER_LOG2 << "Current window start: " << windowPath.front().getTranscriptionUnit() << 
				" (idx: " << windowPath.front().getIndex() << ") / " << windowPath.front().getRecognitionUnit() << 
				" (time: " << windowPath.front().getStartTime() << ")" CHUNKER_ENDLOG
			
			for(std::deque<AlignPoint>::iterator iter = windowPath.begin(); iter != endIter; ++iter)
			{
				CHUNKER_LOG2 << "New alignment point: " << iter->getTranscriptionUnit() << 
					" / " << iter->getRecognitionUnit() CHUNKER_ENDLOG
				
				if(iter->getIndex() >= 0 && iter->getStartTime() >= 0)
				{
					++transcriptStart;
					++recognitionStart;
				}
				else if(iter->getIndex() >= 0)
				{
					++transcriptStart;
				}
				else
				{
					++recognitionStart;
				}

				path->push_back(*iter);
			}
		}
	}
}

AlignMatrix::AlignMatrix(std::unique_ptr<TranscriptionIterator> iter1, std::unique_ptr<RecognitionIterator> iter2) : 
	Aligner (std::move(iter1), std::move(iter2)) 
{CHUNKER_FLOG}

AlignMatrix::AlignMatrix(
		const std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptStart,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptEnd,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionStart,
		const std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionEnd)
{CHUNKER_FLOG
	std::vector<std::shared_ptr<BaseIterator> >::iterator transcriptIter(transcriptStart);
	std::vector<std::shared_ptr<BaseIterator> >::iterator recognitionIter(recognitionStart);
	
	for(; transcriptIter != transcriptEnd; ++transcriptIter)
	{
		CHUNKER_LOG2 << "Pushing " << (*transcriptIter)->getUnit() << " to transcript points" CHUNKER_ENDLOG;
		_transcriptPoints.push_back(*transcriptIter);
	}
	for(; recognitionIter != recognitionEnd; ++recognitionIter)
	{	
		CHUNKER_LOG2 << "Pushing " << (*recognitionIter)->getUnit() << " to recognition points" CHUNKER_ENDLOG;
		_recognitionPoints.push_back(*recognitionIter);
	}

}

void AlignMatrix::run(std::deque<AlignPoint> * path)
{CHUNKER_FLOG
	
	fillMatrix();
	std::deque<AlignPoint> pathTmp = findPathRestricted();
	std::swap(*path, pathTmp);
}


// Wagner-Fischer algorithm
void AlignMatrix::fillMatrix()
{CHUNKER_FLOG

	_matrix.resize(_transcriptPoints.size() + 1);
		
	// Fill the first row
	for(unsigned i = 0; i < _transcriptPoints.size() + 1; i++)
	{
		_matrix.at(i).resize(_recognitionPoints.size() + 1);
		_matrix.at(i).at(0) = i;
	}
	
	// Fill the first column
	for(unsigned j = 0; j < _recognitionPoints.size() + 1; j++)
	{
		_matrix.at(0).at(j) = j;
	}


	// Fill everything else
	auto iteratorTranscriptionPoints = _transcriptPoints.cbegin();
	for(auto iteratorMatrixOuter = _matrix.begin() + 1; iteratorMatrixOuter != _matrix.end(); 
		++iteratorMatrixOuter, ++iteratorTranscriptionPoints)
	{
		auto iteratorRecognitionPoints = _recognitionPoints.cbegin();
		auto iteratorMatrixInnerBefore = (iteratorMatrixOuter - 1)->cbegin() + 1;

		for(auto iteratorMatrixInner = iteratorMatrixOuter->begin() + 1; iteratorMatrixInner != iteratorMatrixOuter->end();
			++iteratorMatrixInner, ++iteratorRecognitionPoints, ++iteratorMatrixInnerBefore)
		{	
			// Is this a zero or non-zero substitution
			unsigned subscost = 1;
			if((*iteratorTranscriptionPoints)->getUnit() == (*iteratorRecognitionPoints)->getUnit())
			{
				subscost = 0;
			}
			
			(*iteratorMatrixInner) = std::min
			(
				*(iteratorMatrixInnerBefore - 1) + subscost,
				std::min(*(iteratorMatrixInnerBefore) + 1, *(iteratorMatrixInner - 1) + 1)
			);
		}
	}
}


std::deque<AlignPoint> AlignMatrix::findPathUnrestricted() const
{CHUNKER_FLOG
	unsigned i = _transcriptPoints.size();
	unsigned j = _recognitionPoints.size();
	unsigned minCost = _matrix.at(i).at(j);

	for(unsigned k = std::max(_transcriptPoints.size(), _recognitionPoints.size()); k > 0; k--)
	{
		if(k < _transcriptPoints.size() && _matrix.at(k).at(_recognitionPoints.size()) < minCost)
		{
			minCost = _matrix.at(k).at(_recognitionPoints.size());
			i = k;
			j = _recognitionPoints.size();
		}
		
		if(k < _recognitionPoints.size() && _matrix.at(_transcriptPoints.size()).at(k) < minCost)
		{
			minCost = _matrix.at(_transcriptPoints.size()).at(k);
			j = k;
			i = _transcriptPoints.size();
		}
	}

	return findPath(i, j);
}


// Retracing the alignment path Wagner-Fischer style
std::deque<AlignPoint> AlignMatrix::findPathRestricted() const
{CHUNKER_FLOG

	return findPath(_transcriptPoints.size(), _recognitionPoints.size());
}

std::deque<AlignPoint> AlignMatrix::findPath(unsigned i, unsigned j) const
{CHUNKER_FLOG
	std::deque<AlignPoint> path;

	if (i == 0 || j == 0)
	{
		CHUNKER_LOG3 << "Returning because of zero matrix" CHUNKER_ENDLOG
		return path;
	}
			
	while(i > 1 || j > 1)
	{
		CHUNKER_LOG4 << "Old matrix field: " << i << ";" << j CHUNKER_ENDLOG

		std::shared_ptr<BaseIterator> iter1;
		std::shared_ptr<BaseIterator> iter2;


		if(j == 1) // If we have already reached the first row
		{
			i --;
			iter1 = _transcriptPoints.at(i-1)->copyAsBaseIteratorShared();
			CHUNKER_LOG3 << "Deletion " << _transcriptPoints.at(i-1)->getUnit() CHUNKER_ENDLOG
		}
		
		else if(i == 1) // If we have already reached the first column
		{
			j --;
			iter2 = _recognitionPoints.at(j-1)->copyAsBaseIteratorShared();
			CHUNKER_LOG3 << "Insertion " << _recognitionPoints.at(j-1)->getUnit() CHUNKER_ENDLOG
		}

		else
		{
			unsigned diag = _matrix.at(i-1).at(j-1);
			unsigned idir = _matrix.at(i-1).at(j);
			unsigned jdir = _matrix.at(i).at(j-1);
	
			if(diag <= idir && diag <= jdir) // If the diagonal option is the best option
			{
				j--;
				i--;
				iter1 = _transcriptPoints.at(i-1)->copyAsBaseIteratorShared();
				iter2 = _recognitionPoints.at(j-1)->copyAsBaseIteratorShared();
				CHUNKER_LOG3 << "Substitution " << _transcriptPoints.at(i-1)->getUnit() <<
				       _recognitionPoints.at(j-1)->getUnit() CHUNKER_ENDLOG
			}

			else if(idir <= jdir)
			{
				i--;
				iter1 = _transcriptPoints.at(i-1)->copyAsBaseIteratorShared();
				CHUNKER_LOG3 << "Deletion " << _transcriptPoints.at(i-1)->getUnit() CHUNKER_ENDLOG
			}

			else
			{
				j--;
				iter2 = _recognitionPoints.at(j-1)->copyAsBaseIteratorShared();
				CHUNKER_LOG3 << "Insertion " << _recognitionPoints.at(j-1)->getUnit() CHUNKER_ENDLOG
			}
		}
		
		path.push_front(AlignPoint(iter1, iter2));
		CHUNKER_LOG4 << "New matrix field: " << i << ";" << j CHUNKER_ENDLOG

	}

	for(auto & elmt : path)
	{
		CHUNKER_LOG4 << elmt.getIndex() << " " << elmt.getStartTime() CHUNKER_ENDLOG
	}

	return path;
}
