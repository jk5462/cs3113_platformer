#include "MainMenu.h"
#include "Util.h"
#include <SDL_mixer.h>


#define ENEMY_COUNT 2

unsigned int mainmenu_data[] =
{
	0
};

void MainMenu::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(1, 1, mainmenu_data, mapTextureID, 1.0f, 4, 1);
	fontTextureID = Util::LoadTexture("font.png");
	state.nextLevel = -1;
}

void MainMenu::Update(float deltaTime) {
	return;
}

void MainMenu::Render(ShaderProgram* program) {
	glm::mat4 viewMatrix = glm::mat4(1);
	program->SetViewMatrix(viewMatrix);
	Util::DrawText(program, fontTextureID, "Platformer", 1.0f, -0.5f, glm::vec3(-2.3, 0.5, 0));
	Util::DrawText(program, fontTextureID, "Press Enter to Start", 0.5f, -0.25f, glm::vec3(-2.5, -0.5, 0));
}