

#include "UI/HUD/PEUIDamage.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"


APEUIDamage::APEUIDamage()
{
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	SetRootComponent(WidgetComp);

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}




void APEUIDamage::BeginPlay()
{
	Super::BeginPlay();

	if (DamageWidgetclass)
	{
		WidgetComp->SetWidgetClass(DamageWidgetclass);
	}

	UpdateDamageUI();
	
}

void APEUIDamage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void APEUIDamage::UpdateDamageUI()
{
	if (UUserWidget* DamageIns = WidgetComp->GetUserWidgetObject())
	{
		if (UTextBlock* DamageText = Cast<UTextBlock>(DamageIns->GetWidgetFromName(TEXT("DamageText"))))
		{
			DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DamageAmount)));
		}
	}

	FVector ActorLocation = GetActorLocation();
	float RandXY = FMath::FRandRange(-25.0f, 25.0f);
	float RandZ = FMath::FRandRange(25.0f, 85.0f);
	FVector EndLocation = ActorLocation + FVector(RandXY, RandXY, RandZ);
	SetActorLocation(EndLocation);

	
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&APEUIDamage::DestroySelf,
		1.5f,
		false
	);
}

void APEUIDamage::DestroySelf()
{
	Destroy();
}


