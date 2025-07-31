#include "Core/PEAssetManager.h"
#include "Core/PEGameplayTags.h"

void UPEAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FPEGameplayTags::InitializeNativeTags();
}
