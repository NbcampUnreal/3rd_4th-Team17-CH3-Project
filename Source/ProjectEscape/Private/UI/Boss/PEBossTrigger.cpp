// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Boss/PEBossTrigger.h"
#include "Player/PEPlayerController.h"
#include "Characters/Hero/PEHero.h"
#include "Characters/Enemies/AI/PEAIBossCharacter.h"
#include "Kismet/GameplayStatics.h"


APEBossTrigger::APEBossTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetBoxExtent(FVector(100.0f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APEBossTrigger::OnOverlapBegin);

	bIsBind = false;
}


void APEBossTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEBossTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap"));

	APEHero* Hero = Cast<APEHero>(OtherActor);
	if (Hero)
	{
		APEAIBossCharacter* Boss = Cast<APEAIBossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APEAIBossCharacter::StaticClass()));

		if (Boss)
		{
			APEPlayerController* PC = Cast<APEPlayerController>(Hero->GetController());
			if (PC)
			{
				if (!bIsBind)
				{
					OnBoxTrigger.AddUObject(PC, &APEPlayerController::ShowBossUI);
					bIsBind = true;
					OnBoxTrigger.Broadcast();
					UE_LOG(LogTemp, Warning, TEXT("pc on and broadcaston"));
				}

			}
		}
	}

}
