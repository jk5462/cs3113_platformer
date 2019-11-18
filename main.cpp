#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"

#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "YouWin.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music* music;

GLuint fontTextureID;

Scene* currentScene;
Scene* sceneList[6];


void SwitchToScene(Scene* scene) {
    scene->Initialize();
	if (scene == sceneList[0]) {
		scene->state.player.life = 3;
	}
	else {
		scene->state.player.life = currentScene->state.player.life;
	}
	currentScene = scene;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	fontTextureID = Util::LoadTexture("font.png");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	music = Mix_LoadMUS("bgm.mp3");
	Mix_PlayMusic(music, -1);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	sceneList[0] = new MainMenu();
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();
	sceneList[4] = new GameOver();
	sceneList[5] = new YouWin();

	SwitchToScene(sceneList[0]);
}


void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        currentScene->state.player.Jump();
                        break;
					case SDLK_RETURN:
						if (currentScene == sceneList[0]) {
							//Game Start
							SwitchToScene(sceneList[1]);
						}
						else if (currentScene == sceneList[4] || currentScene == sceneList[5]) {
							//Game Over
							//You Win
							SwitchToScene(sceneList[0]);
						}
                }
                break;
        }
    }
    
	currentScene->state.player.velocity.x = 0;
    
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
	if (currentScene->state.player.life <= 0) return;
    if (keys[SDL_SCANCODE_A])
    {
		currentScene->state.player.velocity.x = -3.0f;
    }
    else if  (keys[SDL_SCANCODE_D])
    {
		currentScene->state.player.velocity.x = 3.0f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
		currentScene->Update(FIXED_TIMESTEP);     
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;

	viewMatrix = glm::mat4(1.0f);

	if (currentScene->state.player.position.y > -3) {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -currentScene->state.player.position.y, 0));
	}
	else {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 3, 0));
	}

	if (currentScene->state.player.position.x > 5) {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player.position.x, 0, 0));
	}
	else {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
	}

	//if (currentScene->state.player.life <= 0) {
	//	SwitchToScene(sceneList[4]);
	//}
}


void Render() {
	glUseProgram(program.programID);
    glClear(GL_COLOR_BUFFER_BIT);

	program.SetViewMatrix(viewMatrix);
    
	currentScene->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
		if (currentScene->state.nextLevel >= 0) SwitchToScene(sceneList[currentScene->state.nextLevel]);
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
