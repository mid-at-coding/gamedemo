#ifndef entity_h
#define entity_h
#include <vector>
#include <cmath>
class EntityHitbox{ // hitboxes, no drawing
public :
	double speedX = 0;
	double speedY = 0; 
	DoublePoint pos; // topleft corner
	int width = 1;
	int height = 1;
	double speed = 1; // unused
	double debugMoveCounter = 0;
	EntityHitbox(double posX, double posY, int widthin, int len ,  double speedin = 0){
		pos.x = posX;
		pos.y = posY;
		width = widthin;
		height = len;
		speed = speedin;
	}
	EntityHitbox(DoublePoint posIn, int widthin, int len ,  double speedin = 0){
		pos = posIn;
		width = widthin;
		height = len;
		speed = speedin;
	}
	EntityHitbox(Point posIn, int widthin, int len, double speedin = 0) {
		pos = PointToDoublePoint(posIn);
		width = widthin;
		height = len;
		speed = speedin;
	}
	EntityHitbox() {
		pos = { 0,0 };
	}
	void Move(){ 																																													
		pos.x += DT * speedX;
		pos.y += DT * speedY;
	}
};
class EntityContainer{ // container class for entities
public :
	bool isTrigger = false;
	int triggerID = 0;
	std::vector<EntityHitbox> hitboxes;
	std::vector<Texture2D> hitboxTexts;
	Color tint = WHITE;
	double debugDrawCounter = 0;
	bool Colliding(DoublePoint p){ // point

		for(int i = 0; i < hitboxes.size(); i++){
//			if (DEBUG) { std::cout << "Checking Point " << p.x << " , " << p.y << " for hitbox " << i; }
			if(p.x >= hitboxes[i].pos.x && p.x <= (hitboxes[i].pos.x + hitboxes[i].width)){ // check if x coord is inside
				if(p.y >= hitboxes[i].pos.y && p.y <= (hitboxes[i].pos.y + hitboxes[i].height)){ // check if y coord is inside
//					std::cout << " True" << std::endl;
					return true;
				}
			}
		}
//		std::cout << " False" << std::endl;
		return false;
	}
	bool Colliding(Point p){
		return(Colliding(PointToDoublePoint(p)));
	}
	bool Colliding(EntityHitbox ent){
		for(int i = 0; i < hitboxes.size(); i++){
			if (Colliding((DoublePoint){ ent.pos.x , ent.pos.y})) { // topleft
				return true;
			} 
			if(Colliding((DoublePoint){ (ent.pos.x + ent.width) , ent.pos.y })){ // topright
				return true;
			}
			if(Colliding((DoublePoint){ (ent.pos.x + ent.width) , (ent.pos.y + ent.height)})){ // bottomright
				return true;
			}
			if(Colliding((DoublePoint){ ent.pos.x  , (ent.pos.y + ent.height) })){ // bottomleft
				return true;
			}
		}
		return false;
	}
	bool Colliding(EntityContainer ent){
		for (int i = 0; i < ent.hitboxes.size(); i++) {
			if (Colliding(ent.hitboxes[i])) {
				return true;
			}
		}
		return false;
	}
};

EntityContainer LOADED_ENTITIES[ENTITY_MAX];
int LOADED_ENTITIES_HEAD = 0;
void initEntityArr() {
	EntityContainer empty;
	for (int i = 0; i < ENTITY_MAX; i++) {
		LOADED_ENTITIES[i] = empty;
	}
	LOADED_ENTITIES_HEAD = 0;
}
void clearEntities() {
	initEntityArr();
}
void clearEntitiesExceptFirst() {
	EntityContainer empty;
	for (int i = 1; i < ENTITY_MAX; i++) {
		LOADED_ENTITIES[i] = empty;
	}
	LOADED_ENTITIES_HEAD = 1;
}

class Entity { // this is so scuffed and i hate it dont hmu i will break down if you ask me about it
public : 
	EntityContainer* ent;
	Entity() {
		if (DEBUG) {
			std::cout << "Entity Constructer Called" << std::endl;
		}
		if (LOADED_ENTITIES_HEAD > ENTITY_MAX) {
			// fuck 
			// easy way out = crash
			if (EMAX_CRASH) {
				std::cerr << "EMAX_CRASH";
				exit(1);
			}
			else { // uh um hm help
				ent = &LOADED_ENTITIES[ENTITY_MAX]; // bad solution
				if (DEBUG) {
					std::cout << "Entity Overflow! Entity replaced end of array" << std::endl;
				}
			}
		}
		else {
			ent = &LOADED_ENTITIES[LOADED_ENTITIES_HEAD++];
			if (DEBUG) {
				std::cout << "Entity Loaded Succesfully" << std::endl;
			}
		}
		return;
	}
	void AddToGArry() {
		if (DEBUG) {
			std::cout << "Entity Constructer Called" << std::endl;
		}
		if (LOADED_ENTITIES_HEAD > ENTITY_MAX) {
			// fuck 
			// easy way out = crash
			if (EMAX_CRASH) {
				std::cerr << "EMAX_CRASH";
				exit(1);
			}
			else { // uh um hm help
				ent = &LOADED_ENTITIES[ENTITY_MAX]; // bad solution
				if (DEBUG) {
					std::cout << "Entity Overflow! Entity replaced end of array" << std::endl;
				}
			}
		}
		else {
			ent = &LOADED_ENTITIES[LOADED_ENTITIES_HEAD++];
			if (DEBUG) {
				std::cout << "Entity Loaded Succesfully" << std::endl;
			}
		}
		return;
	}
	Entity(EntityContainer in) {
		ent = &in;
	}
	Entity operator = (Entity const& in) { // without this it would simply replace the pointers and that is usually not what is intended
		ent->debugDrawCounter = in.ent->debugDrawCounter;
		ent->hitboxes = in.ent->hitboxes;
		ent->hitboxTexts = in.ent->hitboxTexts;
		ent->isTrigger = in.ent->isTrigger;
		ent->tint = in.ent->tint;
	}
	bool OutOfBounds() {
		for (int i = 0; i < ent->hitboxes.size(); i++) {
			if (std::min(hitboxes[i].pos.x, hitboxes[i].pos.y) < 0) {
				return true;
			}
			if (hitboxes[i].pos.x + hitboxes[i].width > SCREENX) {
				return true;
			}
			if (hitboxes[i].pos.y + hitboxes[i].height > SCREENY) {
				return true;
			}
		}
		return false;
	}
	// inheritance wont work here so
	bool Colliding(DoublePoint p) { return (*ent).Colliding(p); };
	bool Colliding(Point p) { return (*ent).Colliding(p); };
	bool Colliding(EntityHitbox entin) { return (*ent).Colliding(entin); };
	bool Colliding(Entity entin) { return (*ent).Colliding(*entin.ent); };
	bool Colliding(EntityContainer entin) { return (*ent).Colliding(entin); };
	void isTrigger(bool trigger) { (*ent).isTrigger = trigger; }
	bool isTrigger() { return (*ent).isTrigger; }
	void addBox(EntityHitbox in) {
		ent->hitboxes.push_back(in);
		hitboxes = ent->hitboxes.data();
	}
	void addTexture(Texture2D in) {
		ent->hitboxTexts.push_back(in);
		hitboxTexts = ent->hitboxTexts.data();
	}
//	EntityHitbox* hitboxes(int x) { return &(ent->hitboxes[x]); }
//	Texture2D* hitboxTexts(int x) { return &(ent->hitboxTexts[x]); }
	Color tint() { return ent->tint; }
	void tint(Color in) { ent->tint = in;}
	EntityHitbox* hitboxes;
	Texture2D* hitboxTexts;
};

void DrawEntities() { // dont question it im begging you
	for (int k = 1; k < LOADED_ENTITIES_HEAD; k++) {
		if (!LOADED_ENTITIES[k].isTrigger) {
			for (int i = 0; i < LOADED_ENTITIES[k].hitboxes.size(); i++) {
				DrawTextureRec(LOADED_ENTITIES[k].hitboxTexts[i],
					{ 0 , 0 , (LOADED_ENTITIES[k].hitboxes[i].width), (LOADED_ENTITIES[k].hitboxes[i].height) },
					{ (LOADED_ENTITIES[k].hitboxes[i].pos.x) , (LOADED_ENTITIES[k].hitboxes[i].pos.y) },
					LOADED_ENTITIES[k].tint);
				if (DEBUG) {
					LOADED_ENTITIES[k].debugDrawCounter += DT;
					if (LOADED_ENTITIES[k].debugDrawCounter >= 1) { // so console doesnt get spammed
						LOADED_ENTITIES[k].debugDrawCounter = 0;
						std::cout << "At Draw : Entity : " << k << " Hitbox : " << i << " " << LOADED_ENTITIES[k].hitboxes[i].pos.x << " " << LOADED_ENTITIES[k].hitboxes[i].pos.y << std::endl;
					}
				}
			}
		}
	}
	// draw entity 0 last, because it's typically the player
	if (!LOADED_ENTITIES[0].isTrigger) {
		for (int i = 0; i < LOADED_ENTITIES[0].hitboxes.size(); i++) {
			DrawTextureRec(LOADED_ENTITIES[0].hitboxTexts[i],
				{ 0 , 0 , (LOADED_ENTITIES[0].hitboxes[i].width), (LOADED_ENTITIES[0].hitboxes[i].height) },
				{ (LOADED_ENTITIES[0].hitboxes[i].pos.x) , (LOADED_ENTITIES[0].hitboxes[i].pos.y) },
				LOADED_ENTITIES[0].tint);
			if (DEBUG) {
				LOADED_ENTITIES[0].debugDrawCounter += DT;
				if (LOADED_ENTITIES[0].debugDrawCounter >= 1) { // so console doesnt get spammed
					LOADED_ENTITIES[0].debugDrawCounter = 0;
					std::cout << "At Draw : Entity : " << 0 << " Hitbox : " << i << " " << LOADED_ENTITIES[0].hitboxes[i].pos.x << " " << LOADED_ENTITIES[0].hitboxes[i].pos.y << std::endl;
				}
			}
		}
	}
}
void MoveEntities() {
	for (int i = 0; i < LOADED_ENTITIES_HEAD; i++) {
		if (!LOADED_ENTITIES[i].isTrigger) {
			for (int k = 0; k < LOADED_ENTITIES[k].hitboxes.size(); k++) {
				LOADED_ENTITIES[i].hitboxes[k].Move();
			}
		}
	}
}
#endif
