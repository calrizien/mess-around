// Copyright Brandon Winston 2021


#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerState.h"
#include "BaseAbilitySystemComponent.h"
#include "GameplayAbilityBindings.h"
#include "BaseAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerCharacterLog, Warning, All);



void APlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
			//Subsystem->AddMappingContext(MouseLookInputMappingContext, 1);
		}
	}
}

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	BaseCameraZoomRate = 50.0f;
	MaxCameraZoom = 500.0;
	CameraRelativeLocation = FVector(0.0f,0.0f, 64.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), "Head");
	CameraBoom->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach Camera Boom to Camera
	FirstPersonCameraComponent->SetRelativeLocation(CameraRelativeLocation); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}


void APlayerCharacter::InitializeAbilityBindings()
{
	if (AbilityBindingsInitialized || !GetPlayerState() || !AbilitySystemComponent.Get() || !InputComponent)
	{
		return;
	}

	// Binds GA input to the Input component.
	// These input bindings can be edited in "GameplayAbilityBindings.h" (Do not remove "None", "Confirm", or "Cancel")
	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("EGameplayAbilityBindings"), static_cast<int32>(EGameplayAbilityBindings::Confirm), static_cast<int32>(EGameplayAbilityBindings::Cancel)));

	AbilityBindingsInitialized = true;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!GetPlayerState() || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	ABasePlayerState* PlayerStateBase = Cast<ABasePlayerState>(GetPlayerState());

	if (PlayerStateBase)
	{
		AbilitySystemComponent = PlayerStateBase->AbilitySystemComponent;

		// Set the Owner and Avatar actor for the Ability System Component. (On the Server)
		AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);

		AttributeSet = PlayerStateBase->Attributes;



		GiveDefaultAbilities();

		ApplyDefaultEffects();
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABasePlayerState* PlayerStateBase = Cast<ABasePlayerState>(GetPlayerState());

	if (PlayerStateBase)
	{
		AbilitySystemComponent = PlayerStateBase->AbilitySystemComponent;

		// Set the Owner and Avatar actor for the Ability System Component. (On the Client)
		AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);

		AttributeSet = PlayerStateBase->Attributes;

		InitializeAbilityBindings();
	}
}

// MARK: ENHANCED INPUT START

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializeAbilityBindings();
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly.
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// There are ways to bind a UInputAction* to a handler function and multiple types of ETriggerEvent that may be of interest.

		// This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
		if (InputActions.MovementInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.MovementInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (InputActions.CameraZoomInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.CameraZoomInputAction, ETriggerEvent::Started, this, &APlayerCharacter::CameraZoom);
		}
		
		if (InputActions.JumpInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.JumpInputAction, ETriggerEvent::Started, this, &APlayerCharacter::EnhancedJump);
		}

		if (InputActions.LookInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.LookInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		if (InputActions.MouseLookInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.MouseLookInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MouseLook);
			PlayerEnhancedInputComponent->BindAction(InputActions.MouseLookInputAction, ETriggerEvent::Completed, this, &APlayerCharacter::MouseLookOff);
		}



		
		// This calls the handler function (a UFUNCTION) by name on every tick while the input conditions are met, such as when holding a movement key down.
		/*if (MyOtherInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(MyOtherInputAction, ETriggerEvent::Triggered, this, TEXT("MyOtherInputHandlerFunction"));
		}*/
	}
	
	/* UN-Enhanced Input Bindings
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);*/
	
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// Debug log output to confirm that the handler function is running.
	// UE_LOG(LogTemp, Warning, TEXT("%s called with Input Action Value %s (magnitude %f)"), TEXT("__FUNCTION__"), *Value.ToString(), Value.GetMagnitude());
	// Use the GetType() function to determine Value's type, and the [] operator with an index between 0 and 2 to access its data.
switch (Value.GetValueType())
{
case EInputActionValueType::Boolean:
	if (Value.IsNonZero())
	{
		// Add movement in Forward/Backward direction
		MoveForward(1.0);
	}
	break;
case EInputActionValueType::Axis1D:
	if (Value.IsNonZero())
	{
		// Add movement in Forward/Backward direction
		MoveForward(Value.GetMagnitude());
	}
	break;
case EInputActionValueType::Axis2D:
	if (Value.GetMagnitude() != 0.0f)
	{
		if (Value[1] != 0.0f)
		{
			// Add movement in Forward/Backward direction
			MoveForward(Value[1]);
		}

		if (Value[0] != 0.0f)
		{
			// Add movement in Left/Right direction
			MoveRight(Value[0]);
		}
	}
	break;
case EInputActionValueType::Axis3D:
	if (Value.GetMagnitude() != 0.0f)
	{
		if (Value[1] != 0.0f)
		{
			// Add movement in Forward/Backward direction
			MoveForward(Value[1]);
		}

		if (Value[0] != 0.0f)
		{
			// Add movement in Left/Right direction
			MoveRight(Value[0]);
		}
	}
	break;
default: break;
}

}

/*void APlayerCharacter::CameraZoom(const FInputActionValue& Value)
{
}*/

/** Zoom Function for Behind-the-back camera intended for mousewheel */

void APlayerCharacter::CameraZoom_Implementation(const FInputActionValue& Value)
{

	// UE_LOG(LogTemp, Warning, TEXT("Camera Zoom Attempted! %s called with Input Action Value %s (magnitude %f)"), TEXT("__FUNCTION__"), *Value.ToString(), Value.GetMagnitude());
	
	// Zoom in if value is positive (mouse wheel up)
	if (Value.GetMagnitude() > 0) {
		UE_LOG(PlayerCharacterLog, Warning, TEXT("Camera Zoom In Attempted! %s called with Input Action Value %s (magnitude %f)"), TEXT("__FUNCTION__"), *Value.ToString(), Value.GetMagnitude());
		
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - BaseCameraZoomRate, 0.0f, MaxCameraZoom);
	}

    // Zoom out if value is negative (mouse wheel down) 
	if (Value.GetMagnitude() < 0)
	{
		UE_LOG(PlayerCharacterLog, Warning, TEXT("Camera Zoom OUT Attempted! %s called with Input Action Value %s (magnitude %f)"), TEXT("__FUNCTION__"), *Value.ToString(), Value.GetMagnitude());
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + BaseCameraZoomRate, 0.0f, MaxCameraZoom);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	TurnAtRate(Value[0]);
	
	LookUpAtRate(Value[1]*(-1.0f)); // TODO: Eventually replace the -1 with a bool option for inverted mouselook
}

void APlayerCharacter::EnhancedJump(const FInputActionValue& Value)
{
	Jump();
}

void APlayerCharacter::MouseLook(const FInputActionValue& Value)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(Value.GetMagnitude() == true)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		
			
		}
		if(Value.GetMagnitude() == false)
		{
			
		}
		
	}
	
}

void APlayerCharacter::MouseLookOff(const FInputActionValue& Value)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
	}

}

// ENHANCED INPUT END

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}





