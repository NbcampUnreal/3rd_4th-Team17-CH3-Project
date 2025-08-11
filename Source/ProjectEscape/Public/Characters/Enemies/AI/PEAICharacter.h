#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "PEAICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPawnDeath);

class APEAIController;

UCLASS()
class PROJECTESCAPE_API APEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APEAICharacter();
	virtual void PreInitializeComponents() override;

	FOnPawnDeath OnPawnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<APEAIController> AIControllerClassBP = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FRotator RotateSpeed = FRotator(0.0f, 540.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AirControl = 0.2f;

	void SetMovementSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	


};
