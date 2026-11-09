#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WooLocomotion.h"
#include "WooWarriorCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UAnimSequence;
struct FInputActionValue;

// Predefined playable Warrior: third-person orbit camera, walk/run/jump (WOO-014, #14).
// Input actions are created in code, so no input assets are needed.
UCLASS()
class WEAPONSOFORDER_API AWooWarriorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWooWarriorCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "Camera") TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Camera") TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Movement") float RunSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = "Movement") float WalkSpeed = 180.f;
	UPROPERTY(EditAnywhere, Category = "Camera") float LookRate = 1.f;

	// Ground speed (cm/s) at which each clip looks right at play rate 1. Calibration knobs: tune by eye.
	UPROPERTY(EditAnywhere, Category = "Animation") float WalkClipSpeed = 150.f;
	UPROPERTY(EditAnywhere, Category = "Animation") float RunClipSpeed = 450.f;

	UPROPERTY(EditAnywhere, Category = "Animation") TObjectPtr<UAnimSequence> IdleAnim;
	UPROPERTY(EditAnywhere, Category = "Animation") TObjectPtr<UAnimSequence> WalkAnim;
	UPROPERTY(EditAnywhere, Category = "Animation") TObjectPtr<UAnimSequence> RunAnim;

	EWooLocomotionState GetLocomotionState() const { return State; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SetWalking(bool bWalk);
	void UpdateAnimation();
	void DrawDebugHud() const;

	UPROPERTY() TObjectPtr<UInputMappingContext> Mapping;
	UPROPERTY() TObjectPtr<UInputAction> MoveAction;
	UPROPERTY() TObjectPtr<UInputAction> LookAction;
	UPROPERTY() TObjectPtr<UInputAction> JumpAction;
	UPROPERTY() TObjectPtr<UInputAction> WalkAction;

	EWooLocomotionState State = EWooLocomotionState::Idle;
	bool bStateSet = false;
	float JumpStartZ = 0.f;
	float JumpPeakZ = 0.f;
	double NextTraceTime = 0.0;
};
