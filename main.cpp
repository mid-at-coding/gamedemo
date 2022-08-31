#include <iostream>
#include <thread>
#include "include/raylib.h"
#include "include/engine.hpp"
#include "include/entity.hpp"
#include "include/screen.hpp"
#define SPEED 50
bool isAllowed(Entity& Player, Screen& screen) {
	return screen.CheckMove(Player.ent->hitboxes[0]);
}
std::string CheckMovement(Entity& Player, Screen& screen) {
	bool moveUp = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
	bool moveDown = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
	bool moveLeft = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
	bool moveRight = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
	if (moveUp) {
		if (moveRight || moveLeft) {
			Player.hitboxes[0].pos.y -= DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes[0].pos.y -= DT * SPEED;
		}
		if (!isAllowed(Player, screen)) {
			moveUp = false;
			if (moveRight || moveLeft) {
				Player.hitboxes[0].pos.y += DT * SPEED * (sin(45));
			}
			else {
				Player.hitboxes[0].pos.y += DT * SPEED;
			}
		}
	}
	if (moveDown) {
		if (moveRight || moveLeft) {
			Player.hitboxes[0].pos.y += DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes[0].pos.y += DT * SPEED;
		}
		if (!isAllowed(Player, screen)) {
			moveDown = false;
			if (moveRight || moveLeft) {
				Player.hitboxes[0].pos.y -= DT * SPEED * (sin(45));
			}
			else {
				Player.hitboxes[0].pos.y -= DT * SPEED;
			}
		}
	}
	if (moveLeft) {
		if (moveUp || moveDown) {
			Player.hitboxes[0].pos.x -= DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes[0].pos.x -= DT * SPEED;
		}
		if (!isAllowed(Player, screen)) {
			moveLeft = false;
			if (moveUp || moveDown) {
				Player.hitboxes[0].pos.x += DT * SPEED * (sin(45));
			}
			else {
				Player.hitboxes[0].pos.x += DT * SPEED;
			}
		}
	}
	if (moveRight) {
		if (moveUp || moveDown) {
			Player.hitboxes[0].pos.x += DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes[0].pos.x += DT * SPEED;
		}
		if (!isAllowed(Player, screen)) {
			moveRight = false;
			if (moveUp || moveDown) {
				Player.hitboxes[0].pos.x -= DT * SPEED * (sin(45));
			}
			else {
				Player.hitboxes[0].pos.x -= DT * SPEED;
			}
		}
	}
	if (moveUp || moveDown || moveLeft || moveRight) {
		if (moveLeft) {
			return "left";
		}
		if (moveRight) {
			return "right";
		}
		if (moveUp) {
			return "up";
		}
		if (moveDown) {
			return "down";
		}
	}
	return " ";
}
int TriggerCollision(Entity& Player) {
	for (int i = 0; i < LOADED_ENTITIES_HEAD; i++) {
		if (LOADED_ENTITIES[i].Colliding(Player.hitboxes[0]) && LOADED_ENTITIES[i].triggerID != 0) {
			return LOADED_ENTITIES[i].triggerID;
		}
	}
	return 0;
}
Point FindTrigger(Screen & screen, int find) {
	Entity empty;
	empty.addBox(EntityHitbox(0, 0, screen.tileSizeX, screen.tileSizeY));
	for (int x = 0; x < SCREENX; x += screen.tileSizeX) {
		for (int y = 0; y < SCREENY; y += screen.tileSizeY) {
			if (TriggerCollision(empty) == find) {
				return { x,y };
			}
		}
	}
	return { 0,0 };
}
int DtoI(std::string in) { // direction to int
	if (in == " ")
		return -1;
	if (in == "left")
		return 2;
	if (in == "up")
		return 1;
	if (in == "down")
		return 0;
	if (in == "right")
		return 3;

	return -1;
}
struct CharacterTexts{
	Texture2D bottom;
	Texture2D top;
};
int main(){

	Screen Home; // creates screen
	initEntityArr(); // empties array; clears garbage
	Entity Player;
	EntityContainer textBox;
	std::string textBoxText = " ";
	std::string currentTextBoxText = " "; // for animation
	bool textBoxOpen = false;
	int textSize = 50;
	std::string currentLocation = "Bedroom";
	double textAnimCounter = 0;
	double charAnimCounter = 0;
	int charAnimIndex = 0;
	bool animDone = false;
	bool started = false;
	std::string suffix,path;
	suffix.clear();
	path.clear();
	std::string move = "up";
	std::string facing = "up";
	bool walkingAnim = false;
	double walkingAnimCounter = 0;
	std::string dir = " ";
	int walkingFrame = 0;
	Home.Init("Shit Game"); // initializes screen
	Home.backgroundTint = BLACK;
	CharacterTexts real;
	std::vector<std::vector<CharacterTexts>> playerTexts;
	std::vector<CharacterTexts> empty;
	for (int i = 0; i < 3; i++) {
		playerTexts.push_back(empty);
		if (i == 0) {
			suffix = ".png";
		}
		else {
			suffix.clear();
			suffix = "_" + std::to_string(i) + ".png";
		}
		path = "assets/down/bottom" + suffix;
		real.bottom = LoadTexture(path.c_str());
		path = "assets/down/top" + suffix;
		real.top = LoadTexture(path.c_str());
		playerTexts[i].push_back(real);
		path = "assets/up/bottom" + suffix;
		real.bottom = LoadTexture(path.c_str());
		path = "assets/up/top" + suffix;
		real.top = LoadTexture(path.c_str());
		playerTexts[i].push_back(real);
		path = "assets/left/bottom" + suffix;
		real.bottom = LoadTexture(path.c_str());
		path = "assets/left/top" + suffix;
		real.top = LoadTexture(path.c_str());
		playerTexts[i].push_back(real);
		path = "assets/right/bottom" + suffix;
		real.bottom = LoadTexture(path.c_str());
		path = "assets/right/top" + suffix;
		real.top = LoadTexture(path.c_str());
		playerTexts[i].push_back(real);
	}

	textBox.hitboxes.push_back(EntityHitbox(400, 600, 400, 200, 0));
	textBox.hitboxTexts.push_back(LoadTexture("assets/textbox.png"));
	Player.addBox(EntityHitbox((Point) { 0, 16 }, 32, 32));
	Player.addBox(EntityHitbox((Point) { 0, 0 }, 32, 32));
	Player.addTexture(playerTexts[0][DtoI(move)].bottom);
	Player.addTexture(playerTexts[0][DtoI(move)].top);
	Home.entities.push_back(*Player.ent);
	Home.ReadFromFile("assets/Bedroom.ce");
	Home.Load();
	while (!WindowShouldClose()) {
		BeginDrawing();
//			Player.tint(Home.CheckMove(DoublePointToPoint(Player.hitboxes(0)->pos)) ? WHITE : RED);
			DT = GetFrameTime();
			charAnimCounter += DT;
			textAnimCounter += DT;
			walkingAnimCounter += DT;
			if (started) {
				Home.Draw();
				MoveEntities();
				DrawEntities();
			}
			else{
				ClearBackground(BLACK);
			}
			
			Player.hitboxes[1].pos.x = Player.hitboxes[0].pos.x;
			Player.hitboxes[1].pos.y = Player.hitboxes[0].pos.y - 32;
			if (!started) {
				if (!animDone) {
					textBoxOpen = true;
					textBoxText = "Lorem Ipsum";
					animDone = false;
				}
				else if(textBoxOpen == false) {
					started = true;
				}
			}
			if (IsKeyPressed(KEY_E) && !textBoxOpen) {
				if (TriggerCollision(Player) == 1) {
					if (currentLocation == "Home2") {
						Home.entities.clear();
						Home.entities.push_back(*Player.ent);
						Home.ReadFromFile("assets/Bedroom.ce");
						currentLocation = "Bedroom";
						Home.Load();
					}
					if (currentLocation == "Home") {
						textBoxOpen = true;
						animDone = false;
						textBoxText = "Its a table.\nThere are books far past\nyour understanding on it.";
					}
					if (currentLocation == "Street") {
						Home.entities.clear();
						Home.entities.push_back(*Player.ent);
						Home.ReadFromFile("assets/Home.ce");
						currentLocation = "Home";
						Home.Load();
					}
				}
				if (TriggerCollision(Player) == 2) {
					if (currentLocation == "Home2") {
						textBoxOpen = true;
						animDone = false;
						textBoxText = "yes yes yes\ni am so good at making\ndialogue";
					}
					if (currentLocation == "Home") {
						textBoxOpen = true;
						animDone = false;
						textBoxText = "You should really get going.";
					}
				}
				if (TriggerCollision(Player) == 4) {
					if (currentLocation == "Bedroom") {
						textBoxOpen = true;
						animDone = false;
						textBoxText = "I wish...";
					}
					if (currentLocation == "Home") {
						textBoxOpen = true;
						animDone = false;
						textBoxText = "im not done with this bit bozo";
					}
				}
			}
			else if (IsKeyPressed(KEY_E) && animDone) {
				textBoxOpen = false;
			}
			if (TriggerCollision(Player) == 3) {
				if (currentLocation == "Home2") {
					Home.entities.clear();
					Home.entities.push_back(*Player.ent);
					Home.ReadFromFile("assets/Home.ce");
					currentLocation = "Home";
					Home.Load();
				}
				else if (currentLocation == "Home") {
					Home.entities.clear();
					Home.entities.push_back(*Player.ent);
					Home.ReadFromFile("assets/Home2.ce");
					currentLocation = "Home2";
					Home.Load();
				}
				else if (currentLocation == "Bedroom") {
					Home.entities.clear();
					Home.entities.push_back(*Player.ent);
					Home.ReadFromFile("assets/Home2.ce", { 1,9 });
					currentLocation = "Home2";
					Home.Load();
				}
			}
			if (textBoxOpen) {
				if (!animDone) {
					if (charAnimIndex == 0) {
						currentTextBoxText = textBoxText[0];
					}
					if (charAnimCounter >= 0.05) {
						currentTextBoxText += textBoxText[++charAnimIndex];
						charAnimCounter = 0;
					}
				}
				DrawTexture(textBox.hitboxTexts[0], textBox.hitboxes[0].pos.x, textBox.hitboxes[0].pos.y, WHITE);
				for (int i = 200; i > 0; i--) {
					if (MeasureText(textBoxText.c_str(), i) < textBox.hitboxes[0].width &&
						MeasureTextEx(GetFontDefault(),textBoxText.c_str(),i,100).y < textBox.hitboxes[0].height) {
						textSize = i;
						break;
					}
				} // badly optimized but im lazy
				if (!animDone) {
					DrawText(currentTextBoxText.c_str(), textBox.hitboxes[0].pos.x, textBox.hitboxes[0].pos.y, textSize, WHITE);
				}
				else {
					DrawText(textBoxText.c_str(), textBox.hitboxes[0].pos.x, textBox.hitboxes[0].pos.y, textSize, WHITE);
				}
				if (currentTextBoxText == textBoxText) {
					textAnimCounter = 0;
					charAnimIndex = 0;
					animDone = true;
					currentTextBoxText.clear();
				}
			}
			if (!textBoxOpen) {
				move = CheckMovement(Player, Home);
				if (move != " ") {
					facing = move;
					Player.hitboxTexts[0] = playerTexts[walkingFrame][DtoI(facing)].bottom;
					Player.hitboxTexts[1] = playerTexts[walkingFrame][DtoI(facing)].top;
					move = CheckMovement(Player, Home);
					if (!walkingAnim) {
						walkingFrame = 1;
					}
					walkingAnim = true;
				}
				else {
					if (walkingAnim) {
						Player.hitboxTexts[0] = playerTexts[0][DtoI(facing)].bottom;
						Player.hitboxTexts[1] = playerTexts[0][DtoI(facing)].top;
					}
					walkingAnim = false;
					walkingFrame = 0;
					walkingAnimCounter = 0;
				}
			}
			if (walkingAnim) {
				if (walkingAnimCounter > 0.2) {
					if (walkingFrame == 1) {
						walkingFrame = 2;
					}
					else if(walkingFrame == 2) {
						walkingFrame = 1;
					}
					walkingAnimCounter = 0;
				}
			}
//			std::cout << Player.hitboxes[0].pos.x / Home.tileSizeX << " , " << Player.hitboxes[0].pos.y / Home.tileSizeY << std::endl;
		EndDrawing();
	}
}
