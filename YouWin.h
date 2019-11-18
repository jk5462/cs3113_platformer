#include "Scene.h"

class YouWin : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;

	GLuint fontTextureID;
};