#include "Core/PEAssetManager.h"
#include "Core/PEGameplayTags.h"
#include "Core/PELogChannels.h"

void UPEAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FPEGameplayTags::InitializeNativeTags();

	UE_LOG(LogPE, Display, TEXT("UPEAssetManager::StartInitialLoading()"));
}
