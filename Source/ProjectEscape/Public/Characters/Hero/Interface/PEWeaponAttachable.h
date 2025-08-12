#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEWeaponAttachable.generated.h"

UINTERFACE()
class UPEWeaponAttachable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTESCAPE_API IPEWeaponAttachable
{
	GENERATED_BODY()

public:
	virtual void AttachWeapon(AActor* WeaponActor) = 0;
	virtual void DetachWeapon(AActor* WeaponActor) = 0;
};
