// Fill out your copyright notice in the Description page of Project Settings.

#include "pNoiseTest.h"
#include "Brick.h"

// Sets default values
ABrick::ABrick()
{	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Box1(TEXT("StaticMesh'/Game/Meshes/Box_StaticMesh.Box_StaticMesh'"));

	RootComponent = StaticMeshComponent;
	StaticMeshComponent->Mobility = EComponentMobility::Movable;//so that we can change its position to the correct one after spawning
	StaticMeshComponent->SetStaticMesh(Box1.Object); 
}

void ABrick::SetPositionAndHeight(FVector position, float height, float min_height){
	SetActorLocation(position);

	FTransform old_transform = GetTransform();
	FVector old_size = old_transform.GetScale3D();
	float new_height = old_size.Z + (height-min_height)*200;
	old_size.Z = new_height;
	old_transform.SetScale3D(old_size);
	SetActorTransform(old_transform);
	/*
	FTransform old_transform = GetTransform();
	FVector old_size = old_transform.GetScale3D();
	float new_height = old_size.Z + (height - min_height) * 200;
	this->StaticMeshComponent->SetRelativeScale3D(old_size);
	*/
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrick::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

