#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PELiftArrivalTrigger.generated.h"

class UBoxComponent;
class APEDoorActor;

/** 엘리베이터(PEDoorActor)가 겹치면 입구 문(들)을 연다 */
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
    /** 감지용 트리거 박스 (엘리베이터 캐빈이 겹쳐야 함) */
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* Trigger;

    /** 이 트리거가 “엘리베이터 캐빈”으로 인정할 PEDoorActor (필수 지정) */
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TObjectPtr<APEDoorActor> ElevatorCab = nullptr;

    /** 엘리베이터 도착 시 열릴 입구 문(들) */
    UPROPERTY(EditInstanceOnly, Category = "Link")
    TArray<TObjectPtr<APEDoorActor>> EntranceDoors;

    /** 떠날 때 닫을지 여부 (간단 옵션) */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bCloseOnLeave = true;

    /** 원샷(한 번만 열기) */
    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bOneShot = false;

private:
    bool bFiredOnce = false;
};
