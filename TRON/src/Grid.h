#pragma once

#include <vector>

class Grid
{
private:
	std::vector<float> m_Positions;
	float m_Stride;			// one cell's size of the grid by height
	unsigned int m_Width;	// The width of the window
	unsigned int m_Height;  // The height of the window
	bool m_LinesMode;			// Mode of lines positions generation: true - horizontal, false - vertical
public:
	Grid(const unsigned int& width, const unsigned int& heigth);
	Grid() {};
	~Grid();

	void SetUpPositions();
	void SetLines(const unsigned int& lineLength, const unsigned int& loopLength);
	void PushBackToPositionsVector(const float& value);

	inline unsigned int GetStride() const { return m_Stride; }
	inline float* GetPositions() const { return (float *) &m_Positions[0]; }
	inline unsigned int GetPositionsSize() const { return m_Positions.size(); }
};
