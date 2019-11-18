#include "YouWin.h"
#include "Util.h"


void YouWin::Initialize() {
	fontTextureID = Util::LoadTexture("font.png");
	state.nextLevel = -1;
}

void YouWin::Update(float deltaTime) {
	return;
}

void YouWin::Render(ShaderProgram* program) {
	glm::mat4 viewMatrix = glm::mat4(1);
	program->SetViewMatrix(viewMatrix);
	Util::DrawText(program, fontTextureID, "YOU WIN!", 1.0f, -0.5f, glm::vec3(-1.67, 0, 0));
}