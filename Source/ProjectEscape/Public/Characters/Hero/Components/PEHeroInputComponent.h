#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEHeroInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UPEHeroInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPEHeroInputComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
