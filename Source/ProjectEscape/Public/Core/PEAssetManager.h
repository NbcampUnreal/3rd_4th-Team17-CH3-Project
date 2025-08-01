#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PEAssetManager.generated.h"

UCLASS()
class PROJECTESCAPE_API UPEAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;
};
