#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TEXTMAKER_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TEXTMAKER_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"
#include "../engine/objects/Camera.hpp"

struct TextVertex {
	glm::vec2 pos;
	glm::vec2 texCoord;
};

struct SingleText {
	int usedLines;
	const char* l[4];
	int start;
	int len;
	bool xCentered;
	bool yCentered;
};

struct CharData {
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};

struct FontDef {
	int lineHeight;
	std::vector<CharData> P;
};

class TextMaker : public MeshObject<TextVertex> {
private:
	std::vector<SingleText*> demoText = {};
	std::vector<TextVertex> vertices;
	std::vector<uint32_t> indices;
	int totLen = 0;
	float PtoTdx[4] = { 0.0, 0.0, 0.0, 0.0 };
	float PtoTdy = 0.0;
	float PtoTsx = 0.0;
	float PtoTsy = 0.0;
	float dim = 0.0;
	int minChar = 32;
	int maxChar = 127;
	int tpx = 0;
	int tpxi = 0;
	int tpxf = 0;
	int tpy = 0;
	int ci = 0;
	int cf = 0;
	CharData di;
	CharData df;
	float xi = -1.0;
	float xf = 1.0;
	float yh = 0.0;
	float yl = 0.0;
	uint32_t width;
	uint32_t height;

public:
	TextMaker(const char* text, bool x, bool y);

	bool needResize = false;
	void Instantiate();
	void Update();
	void createTextMesh();
	void SetDimensions(uint32_t windowWidth, uint32_t windowHeight, float d) {
		width = windowWidth;
		height = windowHeight;
		dim = d;
	}
};

#endif  // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TEXTMAKER_HPP_
