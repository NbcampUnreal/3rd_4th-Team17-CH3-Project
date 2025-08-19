#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEDoorTriggerBox.generated.h"

class UBoxComponent;
class APEDoorActor;

/**
 * 트리거에 Pawn이 들어오면 문을 연다.
 * - 여러 문 연결 가능(Details에서 배열로 지정)
 * - 나갈 때 닫기 / 지연 닫기 옵션
 * - 다수의 Pawn 동시 처리(점유 카운트 방식)
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
    /** 트리거 박스(충돌: Pawn=Overlap) */
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* Trigger;

    /** 연결할 문들(PEDoorActor) */
    UPROPERTY(EditInstanceOnly, Category = "Door")
    TArray<TObjectPtr<APEDoorActor>> LinkedDoors;

    /** 트리거에 들어오면 곧바로 문을 연다 */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bOpenOnEnter = true;

    /** 트리거에서 모두 나가면 바로 닫기 */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bCloseOnExit = true;

    /** 지연 닫기(초). 0이면 지연 없음. bCloseOnExit=false 인 경우에도 사용할 수 있음(전부 나간 후에만 카운트 0 기준으로 지연 닫기) */
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0.0"))
    float CloseDelay = 0.0f;

    /** 플레이어만 반응(다른 Pawn 무시) */
    UPROPERTY(EditAnywhere, Category = "Filter")
    bool bOnlyAffectPlayer0 = true;

private:
    /** 트리거 내부 Pawn 수 */
    int32 OverlapCount = 0;

    /** 지연 닫기 타이머 */
    FTimerHandle CloseTimerHandle;
};
