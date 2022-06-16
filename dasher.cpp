#include "raylib.h"

struct AnimData {
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

bool isOnGround(AnimData data, int windowHeight) {
  return data.pos.y >= windowHeight - data.rec.height;
}

int main() {
  //Array with window dimensions
  int windowDimensions[2];
  windowDimensions[0] = 512; // Window width
  windowDimensions[1] = 380; // Window height

  // Initialize the window
  InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

  // Acceleration due to gravity (pixels/frame)/s
  const int gravity{1'000};

  // Nebula variables
  Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

  const int sizeOfNebulae{10};
  AnimData nebulae[sizeOfNebulae]{};

  for(int i = 0; i < sizeOfNebulae; i++) {
    nebulae[i].rec.x = 0.0;
    nebulae[i].rec.y = 0.0;
    nebulae[i].rec.width =  nebula.width/8;
    nebulae[i].rec.height = nebula.height/8;
    nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
    nebulae[i].frame = 0;
    nebulae[i].runningTime = 0.0;
    nebulae[i].updateTime = 0.0;
    nebulae[i].pos.x = windowDimensions[0] + i * 300; // Increase pos.x by 300 each nebula
  }

  // Nebula X velocity (pixels/second)
  int nebVel{-200};

  // Scarfy variables
  Texture2D scarfy = LoadTexture("textures/scarfy.png");
  AnimData scarfyData;
  scarfyData.rec.width = scarfy.width/6;
  scarfyData.rec.height = scarfy.height;
  scarfyData.rec.x = 0;
  scarfyData.rec.y = 0;
  scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
  scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0/12.0;
  scarfyData.runningTime = 0.0;

  // Is the rectangle in the air?
  bool isInAir = false;
  // Jump velocity (pixels/second)
  const int jumpVel{-600};

  int velocity{0};

  SetTargetFPS(60);
  while(!WindowShouldClose()) {

    // Delta time (time since last frame)
    const float dT{GetFrameTime()};

    // Start drawing
    BeginDrawing();
    ClearBackground(WHITE);

    // Perform ground check
    if(isOnGround(scarfyData, windowDimensions[1])) {
      // Rectangle is on the ground
      velocity = 0;
      isInAir = false;
    } else {
      // Rectangle is in the air
      // Apply gravity
      velocity += gravity * dT;
      isInAir = true;
    }

    // Jump check
    if(IsKeyPressed(KEY_SPACE) && !isInAir) {
      velocity += jumpVel; // Move right
    }
 
    for(int i = 0; i < sizeOfNebulae; i++) {
      // Update the position of each nebula
      nebulae[i].pos.x += nebVel * dT;
    }

    // Update Scarfy position
    scarfyData.pos.y += velocity * dT;

    // Update running time
    scarfyData.runningTime += dT;
    if(scarfyData.runningTime >= scarfyData.updateTime && !isInAir) {
      scarfyData.runningTime = 0.0;
      // Update animation frame
      scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
      scarfyData.frame++;
      
      if(scarfyData.frame >= 5) {
        scarfyData.frame = 0;
      }
    }

    for(int i = 0; i < sizeOfNebulae; i++) {
      // Update nebula animation frame
      nebulae[0].runningTime += dT;
      if(nebulae[i].runningTime >= nebulae[i].updateTime) {
        nebulae[i].runningTime = 0.0;
        // Update animation frame
        nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
        nebulae[i].frame++;
        
        if(nebulae[i].frame >= 7) {
          nebulae[i].frame = 0;
        }
      }
    }

    for(int i = 0; i < sizeOfNebulae; i++) {
      // Draw nebula
      DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
    }

    // Draw Scarfy
    DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

    // Stop drawing
    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}