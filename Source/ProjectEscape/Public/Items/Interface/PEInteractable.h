#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEInteractable.generated.h"

UINTERFACE(BlueprintType)
class UPEInteractable : public UInterface
{
    GENERATED_BODY()
};

class IPEInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(APawn* InstigatorPawn);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FText GetInteractText();
};