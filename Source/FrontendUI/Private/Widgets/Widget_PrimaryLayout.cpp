// Copyright ChenGangQiang. All rights reserved.


#include "Widgets/Widget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can't find widget stack for tag '%s'"), *InTag.ToString());
	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories="Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
