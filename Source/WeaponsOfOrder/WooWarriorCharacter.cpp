#include "WooWarriorCharacter.h"

#include "Animation/AnimSequence.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogWoo014, Log, All);

namespace
{
	const TCHAR* StateName(EWooLocomotionState S)
	{
		switch (S)
		{
		case EWooLocomotionState::Walk: return TEXT("Walk");
		case EWooLocomotionState::Run: return TEXT("Run");
		case EWooLocomotionState::Air: return TEXT("Air");
		default: return TEXT("Idle");
		}
	}
}

AWooWarriorCharacter::AWooWarriorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(38.f, 92.f);

	// Character faces where it moves; the camera follows the mouse independently.
	bUseControllerRotationPitch = bUseControllerRotationYaw = bUseControllerRotationRoll = false;
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 540.f, 0.f);
	Move->MaxWalkSpeed = RunSpeed;
	Move->JumpZVelocity = 520.f;  // ~138 cm apex: clears the 90 cm test ledge
	Move->AirControl = 0.35f;
	Move->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	CameraBoom->TargetArmLength = 380.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;  // pull the camera in front of walls instead of clipping
	CameraBoom->ProbeSize = 14.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 12.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Quaternius humanoid: origin at the feet, faces +Y as imported (see Tools/woo006_map.py).
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/ThirdParty/WOO006/Warrior/Male_Ranger.Male_Ranger"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Outfit(TEXT("/Game/ThirdParty/WOO006/Materials/M_Ranger.M_Ranger"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Skin(TEXT("/Game/ThirdParty/WOO006/Materials/M_RangerSkin.M_RangerSkin"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Idle(TEXT("/Game/ThirdParty/WOO006/Warrior/Anim/UAL1_Standard_Armature_Idle_Loop.UAL1_Standard_Armature_Idle_Loop"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Walk(TEXT("/Game/ThirdParty/WOO006/Warrior/Anim/UAL1_Standard_Armature_Walk_Loop.UAL1_Standard_Armature_Walk_Loop"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Run(TEXT("/Game/ThirdParty/WOO006/Warrior/Anim/UAL1_Standard_Armature_Jog_Fwd_Loop.UAL1_Standard_Armature_Jog_Fwd_Loop"));

	USkeletalMeshComponent* Body = GetMesh();
	Body->SetSkeletalMeshAsset(MeshAsset.Object);
	Body->SetMaterial(0, Outfit.Object);
	Body->SetMaterial(1, Skin.Object);
	Body->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -92.f), FRotator(0.f, -90.f, 0.f));
	Body->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	IdleAnim = Idle.Object;
	WalkAnim = Walk.Object;
	RunAnim = Run.Object;
}

void AWooWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetInputMode(FInputModeGameOnly());  // capture the mouse for camera orbit
		PC->bShowMouseCursor = false;
	}
	UE_LOG(LogWoo014, Log, TEXT("WOO014 BEGIN run=%.0f walk=%.0f jumpZ=%.0f arm=%.0f"),
		RunSpeed, WalkSpeed, GetCharacterMovement()->JumpZVelocity, CameraBoom->TargetArmLength);
}

void AWooWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto MakeAction = [this](const TCHAR* Name, EInputActionValueType Type)
	{
		UInputAction* A = NewObject<UInputAction>(this, Name);
		A->ValueType = Type;
		return A;
	};
	MoveAction = MakeAction(TEXT("IA_Move"), EInputActionValueType::Axis2D);
	LookAction = MakeAction(TEXT("IA_Look"), EInputActionValueType::Axis2D);
	JumpAction = MakeAction(TEXT("IA_Jump"), EInputActionValueType::Boolean);
	WalkAction = MakeAction(TEXT("IA_Walk"), EInputActionValueType::Boolean);

	// WASD -> 2D move vector (X = right, Y = forward).
	Mapping = NewObject<UInputMappingContext>(this, TEXT("IMC_Warrior"));
	auto Swizzle = [this]() { return NewObject<UInputModifierSwizzleAxis>(this); };
	auto Negate = [this]() { return NewObject<UInputModifierNegate>(this); };
	Mapping->MapKey(MoveAction, EKeys::D);
	Mapping->MapKey(MoveAction, EKeys::A).Modifiers.Add(Negate());
	Mapping->MapKey(MoveAction, EKeys::W).Modifiers.Add(Swizzle());
	FEnhancedActionKeyMapping& Back = Mapping->MapKey(MoveAction, EKeys::S);
	Back.Modifiers.Add(Swizzle());
	Back.Modifiers.Add(Negate());
	Mapping->MapKey(LookAction, EKeys::Mouse2D);
	Mapping->MapKey(JumpAction, EKeys::SpaceBar);
	Mapping->MapKey(WalkAction, EKeys::LeftShift);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Mapping, 0);
		}
	}

	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWooWarriorCharacter::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWooWarriorCharacter::Look);
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindActionValueLambda(WalkAction, ETriggerEvent::Started, [this](const FInputActionValue&) { SetWalking(true); });
	Input->BindActionValueLambda(WalkAction, ETriggerEvent::Completed, [this](const FInputActionValue&) { SetWalking(false); });
}

void AWooWarriorCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller) return;
	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Axis.Y);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Axis.X);
}

void AWooWarriorCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X * LookRate);
	AddControllerPitchInput(-Axis.Y * LookRate);
}

void AWooWarriorCharacter::SetWalking(bool bWalk)
{
	GetCharacterMovement()->MaxWalkSpeed = bWalk ? WalkSpeed : RunSpeed;
	UE_LOG(LogWoo014, Log, TEXT("WOO014 GAIT %s"), bWalk ? TEXT("walk") : TEXT("run"));
}

void AWooWarriorCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UE_LOG(LogWoo014, Log, TEXT("WOO014 LANDED peak=%.1f start=%.1f land=%.1f"),
		JumpPeakZ - JumpStartZ, JumpStartZ, GetActorLocation().Z);
}

void AWooWarriorCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const bool bFalling = GetCharacterMovement()->IsFalling();
	const float Z = GetActorLocation().Z;
	if (bFalling && State != EWooLocomotionState::Air) { JumpStartZ = JumpPeakZ = Z; }
	if (bFalling) { JumpPeakZ = FMath::Max(JumpPeakZ, Z); }
	UpdateAnimation();

	const double Now = GetWorld()->GetTimeSeconds();
	if (Now >= NextTraceTime)
	{
		NextTraceTime = Now + 0.25;
		const float Arm = FVector::Dist(FollowCamera->GetComponentLocation(), CameraBoom->GetComponentLocation());
		UE_LOG(LogWoo014, Log, TEXT("WOO014 TRACE t=%.2f pos=%s speed=%.0f state=%s yaw=%.0f camYaw=%.0f camPitch=%.0f arm=%.0f"),
			Now, *GetActorLocation().ToCompactString(), GetVelocity().Size2D(), StateName(State),
			GetActorRotation().Yaw, GetControlRotation().Yaw, GetControlRotation().Pitch, Arm);
	}
	DrawDebugHud();
}

void AWooWarriorCharacter::UpdateAnimation()
{
	const float Speed = GetVelocity().Size2D();
	const EWooLocomotionState New = WooLocomotion::Pick(Speed, GetCharacterMovement()->IsFalling(), WalkSpeed);
	USkeletalMeshComponent* Body = GetMesh();
	if (!bStateSet || New != State)
	{
		// ponytail: single-node clip switch, no blending; an Anim Blueprint blend space replaces this in M1.2.
		UAnimSequence* Clip = New == EWooLocomotionState::Walk ? WalkAnim : New == EWooLocomotionState::Run ? RunAnim : IdleAnim;
		Body->PlayAnimation(Clip, true);
		State = New;
		bStateSet = true;
	}
	const float Rate = State == EWooLocomotionState::Walk ? Speed / WalkClipSpeed : State == EWooLocomotionState::Run ? Speed / RunClipSpeed : 1.f;
	Body->SetPlayRate(FMath::Clamp(Rate, 0.5f, 1.6f));
}

void AWooWarriorCharacter::DrawDebugHud() const
{
#if !UE_BUILD_SHIPPING
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!GEngine || !PC) return;
	FString Keys;
	for (const FKey& K : { EKeys::W, EKeys::A, EKeys::S, EKeys::D, EKeys::LeftShift, EKeys::SpaceBar })
	{
		if (PC->IsInputKeyDown(K)) Keys += K.GetDisplayName().ToString() + TEXT(" ");
	}
	float MouseX = 0.f, MouseY = 0.f;
	PC->GetInputMouseDelta(MouseX, MouseY);
	if (!FMath::IsNearlyZero(MouseX) || !FMath::IsNearlyZero(MouseY)) Keys += TEXT("Mouse ");
	GEngine->AddOnScreenDebugMessage(14001, 0.f, FColor::Yellow,
		FString::Printf(TEXT("INPUT: %s"), Keys.IsEmpty() ? TEXT("-") : *Keys), true, FVector2D(1.6f, 1.6f));
	GEngine->AddOnScreenDebugMessage(14002, 0.f, FColor::White,
		FString::Printf(TEXT("speed %.0f cm/s  state %s  arm %.0f"), GetVelocity().Size2D(), StateName(State),
			FVector::Dist(FollowCamera->GetComponentLocation(), CameraBoom->GetComponentLocation())), true, FVector2D(1.3f, 1.3f));
#endif
}
