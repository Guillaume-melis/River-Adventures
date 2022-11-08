// #include <rcamera.h>
#include "raylib.h"

#include <stdlib.h>           // Required for: free()
#include <unistd.h> // Required for: free()
#include <iostream> 
#include <cstdio>
#include <string>
#include <conio.h>
using namespace std;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------



int main(void)
{
    
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    static int framesCounter = 0;
    // menu();

    framesCounter = 0;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - first person maze");

    
    Camera camera = { { 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Image imMap = LoadImage("Map_inverse.png");      
    Texture2D cubicmap = LoadTextureFromImage(imMap);       
    Mesh mesh = GenMeshCubicmap(imMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);

    
    Texture2D texture = LoadTexture("Texture.png");    
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;             

    Texture2D perso = LoadTexture("main_boussole.png");

    int frameWidth = perso.width;
    int frameHeight = perso.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0, 0, frameWidth, (float)perso.height };

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { screenWidth/2.0f, screenHeight/2.0f, frameWidth*2.0f, frameHeight*2.0f };

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    // Vector2 origin = { (float)frameWidth, (float)frameHeight };

    int rotation = 0;


        

    
    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);             

    


    Vector3 mapPosition = { -cubicmap.width/2 -3 , 0.0f, -cubicmap.height/2  };  

    SetCameraMode(camera, CAMERA_FIRST_PERSON);    
    SetTargetFPS(60);               

    // Main game loop
    
    while (!WindowShouldClose())    
    {
       
        Vector3 oldCamPos = camera.position;    

        UpdateCamera(&camera);

        
        framesCounter++;

        
        Vector2 playerPos = { camera.position.x, camera.position.z };
        float playerRadius = 0.1f;  

        int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);

        
        if (playerCellX < 0) playerCellX = 0;
        else if (playerCellX >= cubicmap.width) playerCellX = cubicmap.width - 1;

        if (playerCellY < 0) playerCellY = 0;
        else if (playerCellY >= cubicmap.height) playerCellY = cubicmap.height - 1;

        
        
        for (int y = 0; y < cubicmap.height; y++)
        {
            for (int x = 0; x < cubicmap.width; x++)
            {
                if ((mapPixels[y*cubicmap.width + x].r == 255) &&       
                    (CheckCollisionCircleRec(playerPos, playerRadius,
                    (Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f })))
                {
                    
                    camera.position = oldCamPos;
                }
            }
        }

        if (playerCellX > 50.0f  && playerCellY <= 50.0f ){
            ClearBackground(RAYWHITE);
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, RED); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, RED); 
        }
        if (40.0f > playerCellX > 20.0f && 80.0f > playerCellY > 60.0f ){
            ClearBackground(RAYWHITE);
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, ORANGE); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, ORANGE); 
            EndDrawing();
        }
        if (playerCellX < 20.0f && 100.0f >playerCellY > 85.0f ){
            ClearBackground(RAYWHITE);
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, GREEN); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, GREEN); //Les deux autres du haut non + ça marche pas 
            EndDrawing();
        }



        float Temps;

    
        

        if (playerCellX == 17 && playerCellY == 98 ) {
            Temps = (float)framesCounter / 60;
            // string Temps_string = to_string(Temps);
            // string filename3("Temps_Jeu.txt");
            // FILE *o_file = fopen(filename3.c_str(), "w+");

            // if (o_file){
                // fwrite(filename3.c_str(), 1, filename3.size(), o_file);
                // cout << "Done Writing!" << endl;
            // }


            BeginDrawing();
             DrawText(TextFormat("WELL DONE, YOU FINISHED THE GAME !!"),500,500,50,RED);   
             DrawText(TextFormat("YOUR TIME :%.02f", (float)framesCounter/60),500, 500,50,RED);
             WaitTime(5)
            
            // sleep(15);


            EndDrawing();
            UnloadImageColors(mapPixels);   
            UnloadTexture(cubicmap);        
            UnloadTexture(texture);         
            // UnloadTexture(Personnage);
            UnloadModel(model);             

            CloseWindow();       

        }
        
        
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawModel(model, mapPosition, 1.0f, WHITE);                     
            EndMode3D();


            DrawText(TextFormat("TIME: %.02f", (float)framesCounter/60), 800, 50, 50, BLUE);
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, RED); //3; 46
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, GREEN); //13; 88

            
            // DrawTexturePro(Personnage, sourceRect, targetRect, float rotation ,origin, WHITE);
            // DrawBillboard((Camera)camera,(Texture2D)perso, (Vector3)oldCamPos, 0.8, WHITE); //ça marche tjrs pas, jsp pk
            DrawTextureRec(perso, sourceRec, Vector2{780,660},RAYWHITE); 



            

            // DrawFPS(10, 10);
            
            

        EndDrawing();
        
    }

    
    UnloadImageColors(mapPixels);   // Unload color array

    UnloadTexture(cubicmap);        // Unload cubicmap texture
    UnloadTexture(texture);         // Unload map texture
    UnloadTexture(perso); // Unload personnage texture)
    UnloadModel(model);             // Unload map model

    CloseWindow();                  // Close window and OpenGL context
    

    return 0;
}

