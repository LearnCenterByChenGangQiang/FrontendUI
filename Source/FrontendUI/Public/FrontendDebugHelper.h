#pragma once

/**
 * Debug 命名空间：提供简单的调试工具方法。
 * 
 * 功能：
 * - 在屏幕上显示调试信息，同时在日志中输出相同信息。
 * - 支持自定义消息颜色和唯一标识符（用于区分不同的调试信息）。
 */
namespace Debug
{
	/**
	 * 显示调试消息。
	 * 
	 * 功能：
	 * - 在屏幕上显示调试信息，持续 7 秒。
	 * - 将调试信息输出到 Unreal 引擎日志（LogTemp）中。
	 * 
	 * @param Msg 要显示的调试消息内容。
	 * @param Color 调试消息的颜色（默认使用随机颜色）。
	 * @param InKey 唯一标识符，用于更新或覆盖已有的屏幕调试消息（默认为 -1 表示新消息）。
	 */
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			// 在屏幕上显示调试消息，持续时间为 7 秒
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg);

			// 在日志中输出调试消息（日志级别为 Warning）
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}