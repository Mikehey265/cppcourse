#pragma once
#include "Blueprint/UserWidget.h"
#include "CoolPauseMenu.generated.h"

UCLASS()
class UCoolPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UCoolPauseMenu(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;
	void NativeDestruct() override;

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	// Input stuff
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	void MenuClose();

	UFUNCTION(BlueprintCallable)
	void MenuRestartLevel();

	UFUNCTION(BlueprintCallable)
	void MenuQuitGame();

	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetDefaultFocusWidget();
};
