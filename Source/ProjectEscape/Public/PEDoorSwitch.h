//#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PEInteractable.h"
//#include "PEDoorSwitch.generated.h"

class APEDoorActor;

UCLASS()
class APEDoorSwitch : public AActor, public IPEInteractable
{
    GENERATED_BODY()

public:
    APEDoorSwitch();

    UPROPERTY(VisibleAnywhere) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;

    // ������ �гο��� ���� ���� ����
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TArray<TObjectPtr<APEDoorActor>> LinkedDoors;

    // IPEInteractable
    virtual void Interact_Implementation(APawn* InstigatorPawn) override;
    virtual FText GetInteractText_Implementation() override
    {
        return FText::FromString(TEXT("��ȣ�ۿ�(E)"));
    }
};