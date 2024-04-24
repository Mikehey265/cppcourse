#include "CoolPauseMenu.h"
#include "Widgets/SViewport.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UCoolPauseMenu::UCoolPauseMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UCoolPauseMenu::NativeConstruct()
{
	//GetOwningPlayer()->SetPause(true);
	Super::NativeConstruct();
}

void UCoolPauseMenu::NativeDestruct()
{
	APlayerController* Owner = GetOwningPlayer();
	if (Owner)
		Owner->SetPause(false);

	Super::NativeDestruct();
}

FReply UCoolPauseMenu::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UWidget* DefaultFocus = GetDefaultFocusWidget();
	if (DefaultFocus)
	{
		return FReply::Handled()
			.ReleaseMouseCapture()
			.ReleaseMouseLock()
			.SetUserFocus(DefaultFocus->GetCachedWidget().ToSharedRef());
	}
	else
	{
		return FReply::Handled()
			.ReleaseMouseCapture()
			.ReleaseMouseLock();
	}
}

void UCoolPauseMenu::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	RemoveFromParent();
}

FReply UCoolPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		RemoveFromParent();
		return FReply::Handled()
			.SetUserFocus(FSlateApplication::Get().GetGameViewport().ToSharedRef());
	}

	return FReply::Handled();
}

FReply UCoolPauseMenu::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

FReply UCoolPauseMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UCoolPauseMenu::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UCoolPauseMenu::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UCoolPauseMenu::MenuClose()
{
	RemoveFromParent();
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void UCoolPauseMenu::MenuRestartLevel()
{
	MenuClose();
	UGameplayStatics::OpenLevel(this, FName(UGameplayStatics::GetCurrentLevelName(this)));
}

void UCoolPauseMenu::MenuQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
