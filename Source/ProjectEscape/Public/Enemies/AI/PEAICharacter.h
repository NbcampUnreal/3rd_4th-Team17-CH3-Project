#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PEAICharacter.generated.h"

UCLASS()
class PROJECTESCAPE_API APEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APEAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FRotator RotateSpeed = FRotator(0.0f, 540.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AirControl = 0.2f;

	void SetMovementSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;

public:	


};
