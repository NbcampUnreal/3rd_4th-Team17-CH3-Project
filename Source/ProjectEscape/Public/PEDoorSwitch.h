#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interface/PEInteractable.h"
#include <Items/Components/PEInteractableComponent.h>
#include "PEDoorSwitch.generated.h"

class APEDoorActor;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ESwitchCommand : uint8
{
    Toggle,     // ���
    ForceOpen,  // ���������͸� 2�� ��ġ��
    ForceClose  // ���������͸� 1�� ��ġ��
};

UCLASS()
class PROJECTESCAPE_API APEDoorSwitch : public AActor, public IPEInteractable
{
    GENERATED_BODY()

public:
    APEDoorSwitch();

    UPROPERTY(VisibleAnywhere) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Link|Interactor")
    TObjectPtr<UPEInteractableComponent> InteractableComponent;

    UPROPERTY(EditAnywhere, Category = "Switch")
    ESwitchCommand Command = ESwitchCommand::Toggle;

    // Details �гο��� ���� ���� ���� ����
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TArray<TObjectPtr<APEDoorActor>> LinkedDoors;

    // IPEInteractable ���� (UFUNCTION �ƴ�, �׳� C++ �����Լ�)
    virtual void Interact(AActor* Interactor) override;
    virtual bool IsInteractable() const override { return true; }


    virtual void ShowInteractionUI();
    virtual void HideInteractionUI();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> InteractWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetComponent> InteractWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    float InteractionUIOffsetZ = 10.0f;
};