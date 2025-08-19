#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEDoorTriggerBox.generated.h"

class UBoxComponent;
class APEDoorActor;

/**
 * Ʈ���ſ� Pawn�� ������ ���� ����.
 * - ���� �� ���� ����(Details���� �迭�� ����)
 * - ���� �� �ݱ� / ���� �ݱ� �ɼ�
 * - �ټ��� Pawn ���� ó��(���� ī��Ʈ ���)
 */
UCLASS()
class PROJECTESCAPE_API APEDoorTriggerBox : public AActor
{
    GENERATED_BODY()

public:
    APEDoorTriggerBox();

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
    void ScheduleClose();
    void CancelScheduledClose();

public:
    /** Ʈ���� �ڽ�(�浹: Pawn=Overlap) */
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* Trigger;

    /** ������ ����(PEDoorActor) */
    UPROPERTY(EditInstanceOnly, Category = "Door")
    TArray<TObjectPtr<APEDoorActor>> LinkedDoors;

    /** Ʈ���ſ� ������ ��ٷ� ���� ���� */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bOpenOnEnter = true;

    /** Ʈ���ſ��� ��� ������ �ٷ� �ݱ� */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bCloseOnExit = true;

    /** ���� �ݱ�(��). 0�̸� ���� ����. bCloseOnExit=false �� ��쿡�� ����� �� ����(���� ���� �Ŀ��� ī��Ʈ 0 �������� ���� �ݱ�) */
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0.0"))
    float CloseDelay = 0.0f;

    /** �÷��̾ ����(�ٸ� Pawn ����) */
    UPROPERTY(EditAnywhere, Category = "Filter")
    bool bOnlyAffectPlayer0 = true;

private:
    /** Ʈ���� ���� Pawn �� */
    int32 OverlapCount = 0;

    /** ���� �ݱ� Ÿ�̸� */
    FTimerHandle CloseTimerHandle;
};
