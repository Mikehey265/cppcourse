#include "CoolHealthComponent.h"

void UCoolHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaximumHealth;
}