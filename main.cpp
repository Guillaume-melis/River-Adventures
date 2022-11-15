//Importation
#include "raylib.h"
#include "raymath.h"
#include <iostream>
using namespace std;

int main(void){
    
    //Initialisation
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    static int framesCounter = 0;
    int rotation = 0;
    float Temps;

    framesCounter = 0;

    InitWindow(screenWidth, screenHeight, "The Maze Runner");
    SetTargetFPS(60);
    ToggleFullscreen();

    //Génération de la carte
    Image imageMap = LoadImage("Map_inverse.png");       
    Mesh mesh = GenMeshCubicmap(imageMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);
    Color *mapPixels = LoadImageColors(imageMap);
    
    //Chargement des textures
    Texture2D Map = LoadTextureFromImage(imageMap);
    Texture2D main = LoadTexture("main_boussole.png");
    Texture2D End = LoadTexture("End.png");
    Texture2D texture = LoadTexture("Textures.png");  
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; 

    //Définition de la camera
    Camera camera = {{ 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };            
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    //Position de l'apparition du joueur dans le Labyrinthe
    Vector3 mapPosition = { -Map.width/2, 0.0f, -Map.height/2};

    //Déchargement de l'image de la carte
    UnloadImage(imageMap);             
        
    //Le jeu    
    while (!WindowShouldClose()){
       
        //Minuteur
        Temps = (float)framesCounter/60;

        //Circonférence du joeur
        float playerRadius = 0.1f;

        //Update
        Vector3 oldCamPos = camera.position;    
        UpdateCamera(&camera);
        framesCounter++;

        Vector2 playerPos = { camera.position.x, camera.position.z };

        //Affichage du "The End"
        Rectangle TheEnd ={0, 0, 1920, 1080};

        //Affichage de la main avec la boussole
        Rectangle BoussoleMain = {0, 0,main.width, main.height };   

        //Génération coordonnées X et Y
        float playerCellX = (float)(playerPos.x - mapPosition.x + 0.5f);
        float playerCellY = (float)(playerPos.y - mapPosition.z + 0.5f);

        //Mise à jour des coordonnées X
        if (playerCellX < 0) playerCellX = 0;
        else if (playerCellX >= Map.width) playerCellX = Map.width - 1;

        //Mise à jours des coordonnées Y
        if (playerCellY < 0) playerCellY = 0;
        else if (playerCellY >= Map.height) playerCellY = Map.height - 1;

        //Génération des colisions
        for (int y = 0; y < Map.height; y++){
            for (int x = 0; x < Map.width; x++){
                if ((mapPixels[y*Map.width + x].r == 255) && (CheckCollisionCircleRec(playerPos, playerRadius,(Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f }))){
                    camera.position = oldCamPos;
                }
            }
        }
            
        //Affichage
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawModel(model, mapPosition, 1.0f, WHITE);                     
            EndMode3D();

            //Afficher le temps
            DrawText(TextFormat("Temps: %.02f", Temps), 800, 50, 50, BLUE);

            //Afficher Coordonnées X et Y
            if (playerCellX>50){
                DrawText(TextFormat("Position X: %.02f", (float)playerCellX), 50, 30, 50, RED);
            }

            if (playerCellY>50){
                DrawText(TextFormat("Position Y: %.02f", (float)playerCellY), 50, 100, 50, RED);
            }

            if (50>playerCellX>25){
                DrawText(TextFormat("Position X: %.02f", (float)playerCellX), 50, 30, 50, ORANGE);
            }

            if (50>playerCellY>25){
                DrawText(TextFormat("Position Y: %.02f", (float)playerCellY), 50, 100, 50, ORANGE);
            }

            if (25>playerCellX){
                DrawText(TextFormat("Position X: %.02f", (float)playerCellX), 50, 30, 50, GREEN);
            }

            if (25>playerCellY){
                DrawText(TextFormat("Position Y: %.02f", (float)playerCellY), 50, 100, 50, GREEN);
            }

            //Afficher la main avec la boussole
            DrawTextureRec(main, BoussoleMain, Vector2{700,675},RAYWHITE); 

        EndDrawing();

        //Si le joueur arrive à la fin du labyrinthe
        if (15<playerCellX<20 && playerCellY==99){
            ClearBackground(BLACK);
            BeginDrawing();
                DrawTexturePro(End, TheEnd, TheEnd, Vector2Zero(),0, WHITE);
                DrawText(TextFormat("Vous êtes sortie du Labyrinthe"),500,500,50,RED);   
                DrawText(TextFormat("Votre temps :%.02f", Temps),500, 600,50,RED);
                
            EndDrawing();

            UnloadImageColors(mapPixels);
            UnloadTexture(Map);       
            UnloadTexture(texture);         
            UnloadTexture(main);
            UnloadModel(model);

            WaitTime(15);

            if (IsKeyPressed(KEY_ENTER)){
                //Déchargement de toutes les textures
                CloseWindow();
            }

        }

    }

    //Déchargement de toutes les textures
    UnloadImageColors(mapPixels);
    UnloadTexture(Map);       
    UnloadTexture(texture);         
    UnloadTexture(main);           
    UnloadModel(model);             

    //Fermeture de la fenêtre
    CloseWindow();                  
    
    return 0;
}

 