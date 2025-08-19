#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PELiftArrivalTrigger.generated.h"

class UBoxComponent;
class APEDoorActor;

/** ����������(PEDoorActor)�� ��ġ�� �Ա� ��(��)�� ���� */
UCLASS()
class PROJECTESCAPE_API APELiftArrivalTrigger : public AActor
{
    GENERATED_BODY()
public:
    APELiftArrivalTrigger();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep,
        const FHitResult& Sweep);

    UFUNCTION()
    void OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 BodyIndex);

    void OpenAll();
    void CloseAll();

public:
    /** ������ Ʈ���� �ڽ� (���������� ĳ���� ���ľ� ��) */
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* Trigger;

    /** �� Ʈ���Ű� ������������ ĳ������ ������ PEDoorActor (�ʼ� ����) */
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TObjectPtr<APEDoorActor> ElevatorCab = nullptr;

    /** ���������� ���� �� ���� �Ա� ��(��) */
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TArray<TObjectPtr<APEDoorActor>> EntranceDoors;

    /** ���� �� ������ ���� (���� �ɼ�) */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bCloseOnLeave = true;

    /** ����(�� ���� ����) */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bOneShot = false;

private:
    bool bFiredOnce = false;
};
