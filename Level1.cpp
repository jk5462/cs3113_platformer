#include "Level1.h"
#include "Util.h"
#include <string>

#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 8

#define ENEMY_COUNT 2

unsigned int level1_data[] =
{
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 1, 3,
	3, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 3,
	3, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
	3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
};

void Level1::Reset() {
	state.player.position = glm::vec3(1, -3, 0);
	state.player.dead = false;

	state.enemies[0].position = glm::vec3(6, -5, 0);
	state.enemies[0].aiState = PATROL;
	state.enemies[0].velocity.x = -2.0f;
	state.enemies[0].isActive = true;
	state.enemies[0].life = 1;

	state.enemies[1].position = glm::vec3(16, -4, 0);
	state.enemies[1].aiState = PATROL;
	state.enemies[1].velocity.x = -2.0f;
	state.enemies[1].isActive = true;
	state.enemies[1].life = 1;
}

void Level1::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	
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

void Level1::Update(float deltaTime) {
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
	if (state.player.position.x > 18) {
		state.nextLevel = 2;
		Mix_PlayChannel(-1, clear, 0);
	}

}

void Level1::Render(ShaderProgram* program) {
	GLuint fontTextureID = Util::LoadTexture("font.png");
	state.map->Render(program);
	Util::DrawText(program, fontTextureID, "A: Left", 0.5f, -0.25f, glm::vec3(1, 0, 0));
	Util::DrawText(program, fontTextureID, "D: Right", 0.5f, -0.25f, glm::vec3(1, -0.75, 0));
	Util::DrawText(program, fontTextureID, "Space: Jump", 0.5f, -0.25f, glm::vec3(1, -1.5, 0));
	Util::DrawText(program, fontTextureID, "Life: " + std::to_string(state.player.life), 0.5f, -0.25f, glm::vec3(1, -3, 0));
	state.player.Render(program);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Render(program);
	}

}