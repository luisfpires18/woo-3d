#include "WooGameMode.h"

#include "WooWarriorCharacter.h"

AWooGameMode::AWooGameMode()
{
	DefaultPawnClass = AWooWarriorCharacter::StaticClass();
}
