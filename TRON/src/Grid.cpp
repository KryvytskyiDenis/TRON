#include "Grid.h"

Grid::Grid(const unsigned int& width, const unsigned int& heigth)
	:m_Width(width), m_Height(heigth), m_Stride(width / 100), m_LinesMode(true)
{}


Grid::~Grid()
{
	m_Positions.clear();
}

void Grid::SetUpPositions()
{
	// Set up horizontal lines
	SetLines(m_Width, m_Height);

	// Set up vertical lines
	m_LinesMode = false;
	SetLines(m_Height, m_Width);
	PushBackToPositionsVector(m_Height);
}

void Grid::SetLines(const unsigned int& lineLength, const unsigned int& loopLength)
{
	float step = m_Stride;
	short coordLineBegin;
	short firstCoordLineEnd;
	short secondCoordLineEnd;

	if (m_LinesMode == true)
	{
		coordLineBegin = 1;
		firstCoordLineEnd = 3;
		secondCoordLineEnd = 0;
	}
	else 
	{
		coordLineBegin = 0;
		firstCoordLineEnd = 2;
		secondCoordLineEnd = 1;
	}

	for (int i = 0; step < loopLength; ++i %= 4)
	{
		if (i == coordLineBegin || i == firstCoordLineEnd)
		{
			PushBackToPositionsVector(step);
			if (i == firstCoordLineEnd)
				step += m_Stride;
		}
		else if (i == secondCoordLineEnd)
		{
			PushBackToPositionsVector(0.0f);
		}
		else
		{
			PushBackToPositionsVector(lineLength);
		}
	}
}

void Grid::PushBackToPositionsVector(const float & value)
{
	m_Positions.push_back(value);
}