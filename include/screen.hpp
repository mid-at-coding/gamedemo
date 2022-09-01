#ifndef screen_h
#define screen_h
#include "engine.hpp"
#include "entity.hpp"
#include <fstream>
class Screen : public BackgroundScreen {
public : 
	std::vector<EntityContainer> entities;
	std::vector<EntityContainer> barrier;
	double tileSizeX = 1;
	double tileSizeY = 1;
	int size = 1;
	Screen() {}
	Screen(Texture2D backin, std::vector<EntityContainer> entin) {
		background = backin;
		entities = entin;
	}
	void Load() {
		clearEntities();
		if (DEBUG)
			std::cout << "Screen Cleared \n";

		for (int i = 0; i < entities.size(); i++) {
			LOADED_ENTITIES[i] = entities[i];
		}
		LOADED_ENTITIES_HEAD = entities.size();
		if (DEBUG) {
			std::cout << "Entities Added to Entity Array \n";
			std::cout << "EArr Head : " << LOADED_ENTITIES_HEAD << std::endl;
		}
	}
	void ReadFromFile(std::string fileName, DoublePoint player = {-1,-1}) {
		std::ifstream file;
		file.open(fileName);
		std::vector<std::string> data;
		static std::vector<Texture2D> texts;
		static std::vector<std::string> loadedTexts;
		std::vector<Texture2D> expectedTexts;
		bool flag = false;
		std::string currentLine;
		std::string currentNumber;
		int xcounter;
		barrier.clear();
		EntityContainer empty;
		currentNumber = "0";
		int size;
		if (file.is_open()) {
			while (getline(file, currentLine)) {
				while (currentLine.find("\r") != std::string::npos) {
					currentLine.erase(currentLine.find("\r"), 1);
				}
				data.push_back(currentLine);
			}
		}
		file.close();
		size = std::stoi(data[0]); // first line is the size of the grid
		this->size = size;
		tileSizeX = (float)SCREENX / size;
		tileSizeY = (float)SCREENY / size;
		// 1 to (data.size() - (size + 1)) is all textures
		for (int i = 1; i <= (data.size() - (size + 1)); i++) {
			for (int k = 0; k < loadedTexts.size(); k++) {
				if (loadedTexts[k] == data[i]) {
//					std::cout << loadedTexts[k] << " = " << data[i] << std::endl;
					flag = true;
					break;
				}
			}
			if (!flag) {
				loadedTexts.push_back(data[i]);
				texts.push_back(LoadTexture(data[i].c_str()));
			}
			flag = false;
		}
		for (int i = 1; i <= (data.size() - (size + 1)); i++) {
			for (int k = 0; k < loadedTexts.size(); k++) {
				if (loadedTexts[k] == data[i]) {
					expectedTexts.push_back(texts[k]);
				}
			}
		}
		//(data.size() - (size + 1)) to data.size() - 2 is all tile data
		for (int y = (data.size() - (size)); y < data.size() - 1; y++) {
			xcounter = 0;
			for (int x = 0; x < data[y].size(); x++) {
				if (data[y][x] == 'p') {
					if (entities.size()) {
						if (entities[0].hitboxes.size()) {
							if (player.x == -1 && player.y == -1) {
								entities[0].hitboxes[0].pos = { xcounter * SCREENX / size, (y - (data.size() - (size))) * SCREENY / size };
							}
							else {
								entities[0].hitboxes[0].pos = { player.x * SCREENX / size, player.y * SCREENY / size };
							}
						}
					}
				}
				else if (data[y][x] == '[') { // this is an abomonation upon mankind, truly disgusting
					currentNumber = "0";
					x++;
					while (data[y][x] != ']') {
						currentNumber += data[y][x];
						x++;
					}
					empty.hitboxTexts.clear();
					empty.hitboxes.clear();
					empty.isTrigger = true;
					empty.triggerID = std::stoi(currentNumber);
					currentNumber = "0";
					empty.hitboxes.push_back(EntityHitbox((DoublePoint) { xcounter* SCREENX / size, (y - (data.size() - (size)))* SCREENY / size }, SCREENX / size, SCREENY / size));
					entities.push_back(empty); 
				}
				else if (data[y][x] == 'b') {
					empty.hitboxTexts.clear();
					empty.hitboxes.clear();
					empty.triggerID = 0;
					empty.hitboxes.push_back(EntityHitbox((DoublePoint) { xcounter* SCREENX / size, (y - (data.size() - (size)))* SCREENY / size }, SCREENX / size, SCREENY / size));
					empty.isTrigger = true;
					barrier.push_back(empty);
				}
				else if (data[y][x] == ',') {
					if (stoi(currentNumber)) {
						empty.hitboxTexts.clear();
						empty.hitboxes.clear();
						empty.isTrigger = false;
						empty.triggerID = 0;
						empty.hitboxTexts.push_back(expectedTexts[stoi(currentNumber) - 1]);
						empty.hitboxes.push_back(EntityHitbox((DoublePoint) { xcounter * SCREENX / size, (y - (data.size() - (size)))* SCREENY / size }, SCREENX / size, SCREENY / size));
						entities.push_back(empty);
					}
					currentNumber = " ";
					xcounter++;
				}
				else {
					currentNumber += data[y][x];
				}
			}
		}
	}
	bool CheckMove(Point to) {
		for (int i = 0; i < barrier.size(); i++) {
			if (barrier[i].Colliding(to)) {
				return false;
			}
		}
		return true;
	}
	bool CheckMove(EntityContainer to) {
		for (int i = 0; i < barrier.size(); i++) {
			if (barrier[i].Colliding(to)) {
				return false;
			}
		}
		return true;
	}
	bool CheckMove(EntityHitbox to) {
		for (int i = 0; i < barrier.size(); i++) {
			if (barrier[i].Colliding(to)) {
				return false;
			}
		}
		return true;
	}
};
#endif