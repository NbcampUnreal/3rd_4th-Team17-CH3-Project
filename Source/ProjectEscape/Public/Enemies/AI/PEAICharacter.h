// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PEAICharacter.generated.h"

UCLASS()
class PROJECTESCAPE_API APEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APEAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 300.0f;

	void SetMovementSpeed(float NewSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


};
