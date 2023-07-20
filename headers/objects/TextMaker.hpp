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
	std::vector<TextVertex> vertices;
	std::vector<uint32_t> indices;

public:
	void Instantiate();
	void Update();
	void createTextMesh();// std::vector<TextVertex> verts, std::vector<uint32_t> indices);
};

#endif  // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TEXTMAKER_HPP_
