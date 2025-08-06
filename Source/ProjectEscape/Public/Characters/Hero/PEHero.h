#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PEHero.generated.h"

class UPEHeroInputComponent;

UCLASS()
class PROJECTESCAPE_API APEHero : public ACharacter
{
	GENERATED_BODY()

public:
	APEHero();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPEHeroInputComponent> HeroInputComponent;

protected:
	virtual void BeginPlay() override;
};
