#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.h"
#include "Util.h"
#include <SDL_mixer.h>

enum  EntityType { PLAYER, PLATFORM, ENEMY };
enum  AIState { IDLE, PATROL, JUMP };

class Entity {
public:
	AIState aiState;
    EntityType entityType;
    bool isActive;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float width;
    float height;
    
    float speed;
    
	int life;
	bool dead;
	float timer;

    GLuint textureID;
    
    Entity();
    
    bool CheckCollision(Entity other);

	void CheckCollisionsX(Map* map);
	void CheckCollisionsY(Map* map);
    
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);

	void AI();

    void Update(float deltaTime, Entity *objects, int objectCount, Map *map);
    void Render(ShaderProgram *program);
    
    void Jump();
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    
};



