#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interface/PEInteractable.h"
#include "PEDoorSwitch.generated.h"

class APEDoorActor;

UCLASS()
class PROJECTESCAPE_API APEDoorSwitch : public AActor, public IPEInteractable
{
    GENERATED_BODY()

public:
    APEDoorSwitch();

    UPROPERTY(VisibleAnywhere) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;

    // Details �гο��� ���� ���� ���� ����
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TArray<TObjectPtr<APEDoorActor>> LinkedDoors;

    // IPEInteractable ���� (UFUNCTION �ƴ�, �׳� C++ �����Լ�)
    virtual void Interact(AActor* Interactor) override;
    virtual bool IsInteractable() const override { return true; }
};