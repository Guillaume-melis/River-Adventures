// #include <rcamera.h>
#include "raylib.h"

#include <stdlib.h>           // Required for: free()
#include <unistd.h> // Required for: free()
#include <iostream> 
#include <cstdio>
using namespace std;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    static int framesCounter = 0;

    framesCounter = 0;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - first person maze");

    
    Camera camera = { { 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Image imMap = LoadImage("mini_laby.png");      
    Texture2D cubicmap = LoadTextureFromImage(imMap);       
    Mesh mesh = GenMeshCubicmap(imMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);

    
    Texture2D texture = LoadTexture("texture_murs.png");    
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;             

    
    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);             

    Texture2D Personnage = LoadTexture("main_boussole.png"); 

    // int *frameWidth = Personnage.width/6;
    // int *frameHeight = Personnage.height;

    
    // Rectangle sourceRect = Rectangle(0.0f, 0.0f, frameWidth, frameHeight);  

    
    // Rectangle targetRect = { screenWidth/2.0f, screenHeight/2.0f, *frameWidth * 2.0f, *frameHeight * 2.0f };

    // Vector2 origin = { (float)*frameWidth, (float)*frameHeight };



    Vector3 mapPosition = { -cubicmap.width/2 -3 , 0.0f, -cubicmap.height/2  };  

    SetCameraMode(camera, CAMERA_FIRST_PERSON);    
    SetTargetFPS(60);               

    // Main game loop
    // float rotation;
    
    while (!WindowShouldClose())    
    {
       
        Vector3 oldCamPos = camera.position;    

        UpdateCamera(&camera);

        // rotation = 0.0f;
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
                if ((mapPixels[y*cubicmap.width + x].r == 0) &&       
                    (CheckCollisionCircleRec(playerPos, playerRadius,
                    (Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f })))
                {
                    
                    camera.position = oldCamPos;
                }
            }
        }

        if (playerCellX >50 , playerCellY <=50 ){
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, RED); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, RED); 
        }
        else if (40>playerCellX >20 , 80> playerCellY >60 ){
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, ORANGE); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, ORANGE); 
            EndDrawing();
        }
        else if (playerCellX <20, 100>playerCellY >85 ){
            BeginDrawing();
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, GREEN); 
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, GREEN); //Les deux autres du haut + ça, ça marche pas 
            EndDrawing();
        }



        float Temps;

        if (playerCellX == 17 , playerCellY == 98 ) {
            Temps = (float)framesCounter / 60;
            f = fopen ("Temps_Jeu.txt", "r+"); //ça marche pas 
            fwrite (&Temps);
            fclose(f);

            BeginDrawing();
             DrawText(TextFormat("WELL DONE, YOU FINISHED THE GAME !!"),500,500,50,BLUE);   
             DrawText(TextFormat("YOUR TIME :%.02f", (float)framesCounter/60),500,50,BLUE);
            
            //  sleep(15);
            EndDrawing();
            UnloadImageColors(mapPixels);   // Unload color array

            UnloadTexture(cubicmap);        // Unload cubicmap texture
            UnloadTexture(texture);         // Unload map texture
            UnloadTexture(Personnage); // Unload personnage texture)
            UnloadModel(model);             // Unload map model

            CloseWindow();       

        }
        // if (playerCellX == 88 , playerCellY ==46 ) {
        //     BeginDrawing();
        //      DrawText(TextFormat("WELL DONE, YOU FINISHED THE GAME !!"),500,500,50,ORANGE);
        //      sleep(15);
        //     EndDrawing();
        //     UnloadImageColors(mapPixels);   // Unload color array

        //     UnloadTexture(cubicmap);        // Unload cubicmap texture
        //     UnloadTexture(texture);         // Unload map texture
        //     UnloadTexture(Personnage); // Unload personnage texture)
        //     UnloadModel(model);             // Unload map model

        //     CloseWindow();       

        // }
        
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawModel(model, mapPosition, 1.0f, WHITE);                     
            EndMode3D();

            // DrawTextureEx(cubicmap, (Vector2){ GetScreenWidth() - cubicmap.width*4.0f - 20, 20.0f }, 0.0f, 4.0f, WHITE);
            // DrawRectangleLines(GetScreenWidth() - cubicmap.width*4 - 20, 20, cubicmap.width*4, cubicmap.height*4, GREEN);

            // Draw player position radar
            // DrawRectangle(GetScreenWidth() - cubicmap.width*4 - 20 + playerCellX*4, 20 + playerCellY*4, 4, 4, RED);

            DrawText(TextFormat("TIME: %.02f", (float)framesCounter/60), 800, 50, 50, BLUE);
            DrawText(TextFormat("PLAYERPOS Y: %.02f", (float)playerCellY), 100, 30, 50, RED); //3; 46
            DrawText(TextFormat("PLAYERPOS X: %.02f", (float)playerCellX), 100, 100, 50, GREEN); //13; 88

            
            // DrawTexturePro(Personnage, sourceRect, targetRect, float rotation ,origin, WHITE);



            // DrawFPS(10, 10);
            
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadImageColors(mapPixels);   // Unload color array

    UnloadTexture(cubicmap);        // Unload cubicmap texture
    UnloadTexture(texture);         // Unload map texture
    UnloadTexture(Personnage); // Unload personnage texture)
    UnloadModel(model);             // Unload map model

    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
