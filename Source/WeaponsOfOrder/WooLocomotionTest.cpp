#include "Misc/AutomationTest.h"
#include "WooLocomotion.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWooLocomotionPickTest, "WeaponsOfOrder.Locomotion.Pick",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FWooLocomotionPickTest::RunTest(const FString&)
{
	const float Walk = 180.f;
	TestTrue(TEXT("standing is Idle"), WooLocomotion::Pick(0.f, false, Walk) == EWooLocomotionState::Idle);
	TestTrue(TEXT("creep is Idle"), WooLocomotion::Pick(5.f, false, Walk) == EWooLocomotionState::Idle);
	TestTrue(TEXT("walk speed is Walk"), WooLocomotion::Pick(180.f, false, Walk) == EWooLocomotionState::Walk);
	TestTrue(TEXT("slight overshoot stays Walk"), WooLocomotion::Pick(220.f, false, Walk) == EWooLocomotionState::Walk);
	TestTrue(TEXT("run speed is Run"), WooLocomotion::Pick(500.f, false, Walk) == EWooLocomotionState::Run);
	TestTrue(TEXT("falling wins over speed"), WooLocomotion::Pick(500.f, true, Walk) == EWooLocomotionState::Air);
	TestTrue(TEXT("falling while still"), WooLocomotion::Pick(0.f, true, Walk) == EWooLocomotionState::Air);
	return true;
}

#endif
