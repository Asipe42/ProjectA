// Copyright Modin


#include "UI/PAStatusBarUI.h"

#include "Components/ProgressBar.h"

void UPAStatusBarUI::SetFillColorAndOpacity(const FLinearColor& Color)
{
	ProgressBar->SetFillColorAndOpacity(Color);
}

void UPAStatusBarUI::SetRatio(const float Ratio)
{
	ProgressBar->SetPercent(Ratio);
}

void UPAStatusBarUI::NativePreConstruct()
{
	/*
	 * 초기화
	 *	- Proc 1. ProgressBar 색상 및 투명도 설정
	 *	- Proc 2. ProgressBar 비율 설정
	 */
	
	Super::NativePreConstruct();

	// Proc 1
	SetFillColorAndOpacity(DefaultFillColor);

	// Proc 2
	SetRatio(1.f);
}
