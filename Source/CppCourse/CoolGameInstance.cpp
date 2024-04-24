#include "CoolGameInstance.h"
#include "CoolPauseMenu.h"

void UCoolGameInstance::OpenPauseMenu(APlayerController* Owner)
{
	if (ensureMsgf(PauseMenuClass, TEXT("You need to specify a pause menu class")))
	{
		auto* menu = NewObject<UCoolPauseMenu>(this, PauseMenuClass);
		menu->SetOwningPlayer(Owner);
		menu->AddToViewport();
		menu->SetFocus();
	}
}
