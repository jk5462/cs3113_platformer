#include "Level3.h"
#include "Util.h"
#include <string>

#define Level3_WIDTH 36
#define Level3_HEIGHT 8

#define ENEMY_COUNT 4

unsigned int Level3_data[] =
{
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3,
	3, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 3,
	3, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 1, 0, 0, 2, 2, 3,
	3, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 3
};

void Level3::Reset() {
	state.player.position = glm::vec3(1, -3, 0);
	state.player.dead = false;

	state.enemies[0].position = glm::vec3(11, 0, 0);
	state.enemies[0].aiState = PATROL;
	state.enemies[0].velocity.x = -2.0f;
	state.enemies[0].isActive = true;
	state.enemies[0].life = 1;

	state.enemies[1].position = glm::vec3(16, -3, 0);
	state.enemies[1].aiState = JUMP;
	state.enemies[1].isActive = true;
	state.enemies[1].life = 1;

	state.enemies[2].position = glm::vec3(20, -3, 0);
	state.enemies[2].aiState = IDLE;
	state.enemies[2].isActive = true;
	state.enemies[2].life = 1;

	state.enemies[3].position = glm::vec3(30, -2, 0);
	state.enemies[3].aiState = JUMP;
	state.enemies[3].isActive = true;
	state.enemies[3].life = 1;
}

void Level3::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(Level3_WIDTH, Level3_HEIGHT, Level3_data, mapTextureID, 1.0f, 4, 1);
	
	state.player.entityType = PLAYER;
	state.player.width = 0.7f;
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("me.png");
	state.player.bounce = Mix_LoadWAV("bounce.wav");

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].width = 0.7;
		state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
		state.enemies[i].textureID = Util::LoadTexture("evil.png");
	}

	Reset();

	state.nextLevel = -1;
}

void Level3::Update(float deltaTime) {
	state.player.Update(deltaTime, state.enemies, ENEMY_COUNT, state.map);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, NULL, 0, state.map);
	}
	if (state.player.dead && state.player.life > 0) {
		Reset();
	}
	if (state.player.life <= 0) {
		state.nextLevel = 4;
	}
	if (state.player.position.x > 34) {
		state.nextLevel = 5;
	}

}

void Level3::Render(ShaderProgram* program) {
	GLuint fontTextureID = Util::LoadTexture("font.png");
	state.map->Render(program);
	Util::DrawText(program, fontTextureID, "Life: " + std::to_string(state.player.life), 0.5f, -0.25f, glm::vec3(1, -3, 0));
	state.player.Render(program);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Render(program);
	}

}