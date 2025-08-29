// ChenGangQiang


#include "Subsystems/FrontendUISubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"

#include "FrontendDebugHelper.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFrontendUISubsystem::RegisterCreatePrimaryLayoutWidget(UWidget_PrimaryLayout* InCreateLayout)
{
	check(InCreateLayout);
	CreatePrimaryLayout = InCreateLayout;

	Debug::Print(TEXT("Primary layout widget stored."));
}

void UFrontendUISubsystem::PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void (EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
		[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
		{
			UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
			
			check(LoadedWidgetClass && CreatePrimaryLayout);

			UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatePrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);
			
			UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
				LoadedWidgetClass,
				[AsyncPushStateCallback](UWidget_ActivatableBase& CreateWidgetInstance)
				{
					AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreateWidgetInstance);
				}
			);
			AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			
		}
		)
	);
}
