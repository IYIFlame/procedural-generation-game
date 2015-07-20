// Fill out your copyright notice in the Description page of Project Settings.

#include "pNoiseTest.h"
#include "PDemo.h"
#include "PerlinNoise.h"
#include "ppm.h"
#include <iostream>
#include "Brick.h"
#include <unordered_map>

using namespace std;
// Sets default values
APDemo::APDemo()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

// Define the size of the image
unsigned int width = 600, height = 450;
PerlinNoise pn(5);
float noise_threshold = 0.68;
// Called when the game starts or when spawned
void APDemo::BeginPlay()
{
	Super::BeginPlay();

	FString GameDir = FPaths::GameDir();
	FString CompleteFilePath = GameDir + "map_settings.txt";

	FString FileData = "";
	FFileHelper::LoadFileToString(FileData, *CompleteFilePath);
	int map_hash = FCString::Atoi(*FileData);
	//UE_LOG(LogTemp, Warning, TEXT("map_hash is %i and the path is %s"), map_hash, *CompleteFilePath);
	pn = PerlinNoise(map_hash);

	// Create an empty PPM image
	ppm image(width, height);

	unsigned int kk = 0;
	// Visit every pixel of the image and assign a color generated with Perlin noise
	for (unsigned int i = 0; i < height; ++i) {     // y
		for (unsigned int j = 0; j < width; ++j) {  // x
			double x = (double)j / ((double)width);
			double y = (double)i / ((double)height);
			// Typical Perlin noise
			double n = pn.noise(10 * x, 10 * y, 0.8);
			// Wood like structure
			//n = 20 * pn.noise(x, y, 0.8);
			//n = n - floor(n);

			n = n < noise_threshold ? 0 : n;//used to create a clear distinction between space and land
			// Map the values to the [0, 255] interval, for simplicity we use 
			// tones of grey
			image.r[kk] = floor(255 * n);
			image.g[kk] = floor(255 * n);
			image.b[kk] = floor(255 * n);
			kk++;
		}
	}

	// Save the image in a binary PPM file
	int result = image.write("figure_8_R.ppm");
}

float despawn_dist = 70;
unordered_map<string, ABrick*> bricks;
string bricks_to_be_removed[1000];//figure a better way to set a size
unsigned int br = 0;
int spawn_height = 0;

// Thoughout the function we delete all in-world coordinates by 200.
// This is to make a cube in the real world be equal to a pixel from the map image.
void APDemo::Tick( float DeltaTime )
{	
	Super::Tick( DeltaTime );
	
	UWorld* World = GetWorld();
	APlayerCameraManager* PlayerCameraManager = World->GetFirstPlayerController()->PlayerCameraManager;
	FVector cam_position = PlayerCameraManager->GetCameraLocation();
	
	if (br != 0){ //go through brick_to... and remove the marked ones from bricks
		for (unsigned int i = 0; i < br; ++i){
			ABrick* Brick = bricks[bricks_to_be_removed[i]];
			bricks.erase(bricks_to_be_removed[i]);
			Brick->Destroy();
			bricks_to_be_removed[i] = "";
		}
		br = 0;
	}
	
	//now go through bricks and mark the new bricks to be removed next frame
	for (auto it = bricks.begin(); it != bricks.end(); ++it){
		FVector brick_loc = it->second->GetActorLocation();
		float distance = ( (brick_loc - cam_position).Size() )/200;
		if (distance >= despawn_dist){
			bricks_to_be_removed[br] = it->first;
			++br;
		}
	}

	//int spawn_height_sector = (int)floor(cam_position.Z / 200) / 80;
	//spawn_height = 160 * spawn_height_sector;

	//this magic tho
	int spawn_height_sector = (int)floor(cam_position.Z / 200);
	int sector_offset = spawn_height_sector == 0 ? 0 : (spawn_height_sector / abs(spawn_height_sector)) * 80;
	spawn_height_sector += sector_offset;
	spawn_height_sector = spawn_height_sector / 160;	
	spawn_height = 160 * spawn_height_sector;
	

	int distance_fwd = 60;
	int distance_left = 60;
	int start_i = (int)floor(cam_position.X / 200);
	int start_j = (int)floor(cam_position.Y / 200);
	for (int i = start_i - distance_fwd; i < start_i + distance_fwd; ++i) {     // y
		for (int j = start_j - distance_left; j < start_j + distance_left; ++j) {  // x
			double x = (double)(abs(j) % width) / ((double)width);// we use % to make sure we never go out of the defined map
			double y = (double)(abs(i) % height) / ((double)height);

			double n = pn.noise(10 * x, 10 * y, 0.8);
			n = n < noise_threshold ? 0 : n;//used to create a clear distinction between space and land
			FVector brick_position = FVector(i * 200, j * 200, spawn_height * 200);
			float distance = ((brick_position - cam_position).Size()) / 200;

			if (distance < despawn_dist){
				string key = to_string(i) + to_string(j);

				if (n > 0 && !bricks[key]){
					float brick_height = n;
					ABrick* Brick = World->SpawnActor<ABrick>(ABrick::StaticClass());
					Brick->SetPositionAndHeight(brick_position, brick_height, noise_threshold);
					bricks[key] = Brick;
				}
			}
		}
	}	
}


