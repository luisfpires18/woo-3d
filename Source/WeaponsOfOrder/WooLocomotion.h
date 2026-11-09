#pragma once

#include "CoreMinimal.h"

// Which looping clip the Warrior plays. Pure logic so it can be unit tested (WOO-014, #14).
enum class EWooLocomotionState : uint8 { Idle, Walk, Run, Air };

namespace WooLocomotion
{
	// Below this ground speed (cm/s) the Warrior counts as standing.
	constexpr float IdleSpeed = 10.f;

	inline EWooLocomotionState Pick(float GroundSpeed, bool bFalling, float WalkMaxSpeed)
	{
		if (bFalling) return EWooLocomotionState::Air;
		if (GroundSpeed < IdleSpeed) return EWooLocomotionState::Idle;
		// Halfway between walk and run speed, so acceleration does not flicker between clips.
		return GroundSpeed <= WalkMaxSpeed * 1.25f ? EWooLocomotionState::Walk : EWooLocomotionState::Run;
	}
}
