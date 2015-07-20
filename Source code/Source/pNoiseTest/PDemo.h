// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PDemo.generated.h"

UCLASS()
class PNOISETEST_API APDemo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDemo();	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//UTextRenderComponent* CountdownText;
	
};
