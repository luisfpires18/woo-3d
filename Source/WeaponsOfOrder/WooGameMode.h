#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WooGameMode.generated.h"

// Spawns the playable Warrior (WOO-014, #14). Set as the GameMode override of test maps.
UCLASS()
class WEAPONSOFORDER_API AWooGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWooGameMode();
};
