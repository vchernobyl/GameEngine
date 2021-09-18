#include "Level.h"
#include "Errors.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include <fstream>
#include <iostream>

Level::Level(const std::string& fileName) {
    std::ifstream file;
    file.open(fileName);

    if (file.fail()) {
	FatalError("Failed to open level file: " + fileName);
    }

    std::string line;
    file >> line >> numHumans;

    std::getline(file, line);

    while (std::getline(file, line)) {
	levelData.push_back(line);
    }

    spriteBatch.Init();
    spriteBatch.Begin();

    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    Color whiteColor;
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;
    whiteColor.a = 255;
    
    for (int y = 0; y < levelData.size(); y++) {
	for (int x = 0; x < levelData[y].size(); x++) {
	    const char tile = levelData[y][x];

	    glm::vec4 destRect(x * TileSize, y * TileSize, TileSize, TileSize); 

	    switch (tile) {
	    case 'R':
		spriteBatch.Draw(destRect, uvRect,
				 ResourceManager::GetTexture("data/textures/red_bricks.png").id,
				 0.0f,
				 whiteColor);
		break;
	    case 'G':
		spriteBatch.Draw(destRect, uvRect,
				 ResourceManager::GetTexture("data/textures/glass.png").id,
				 0.0f,
				 whiteColor);
		break;
	    case 'L':
		spriteBatch.Draw(destRect, uvRect,
				 ResourceManager::GetTexture("data/textures/light_bricks.png").id,
				 0.0f,
				 whiteColor);
		break;
	    case '@':
		levelData[y][x] = '.';
		playerStartPosition.x = x * TileSize;
		playerStartPosition.y = y * TileSize;
		break;
	    case 'Z':
		levelData[y][x] = '.';
		zombieStartPositions.emplace_back(x * TileSize, y * TileSize);
		break;
	    case '.':
		break;
	    default:
		std::printf("Unexpected symbol %c at (%d, %d)\n", tile, x, y);
		break;
	    }
	}
    }

    spriteBatch.End();
}

void Level::Draw() {
    spriteBatch.DrawBatch();
}
