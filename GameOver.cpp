#include "GameOver.h"
#include "Util.h"


void GameOver::Initialize() {
	fontTextureID = Util::LoadTexture("font.png");
	state.nextLevel = -1;
}

void GameOver::Update(float deltaTime) {

}

void GameOver::Render(ShaderProgram* program) {
	glm::mat4 viewMatrix = glm::mat4(1);
	program->SetViewMatrix(viewMatrix);
	Util::DrawText(program, fontTextureID, "GAME OVER", 1.0f, -0.5f, glm::vec3(-2, 0.5, 0));
	Util::DrawText(program, fontTextureID, "Press Enter to restart", 0.5f, -0.25f, glm::vec3(-2.7, -0.5, 0));
	
}