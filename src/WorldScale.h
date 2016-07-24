#pragma once
#include <vector>

class WorldScale
{
public:
	WorldScale();
	void zoomIn();
	void zoomOut();
	float get() const;
	
private:
	std::vector<float> scales;
	size_t scaleIndex;
};
