// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;

USTRUCT()
struct FInputActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MovementInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CameraZoomInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MouseLookInputAction;

	
};

/**
 * Player Character is the base class to use for any Client controlled Characters.
 * This class gets it's Ability System Component and Attribute Sets from the Player State Base class.
 */
UCLASS(Blueprintable)
class MYPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	



public:
	APlayerCharacter();

	// Camera Component FreeLook enabled -BW
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	bool IsFreeLooking = false;
	
	// Must override this pawn function to implement enhanced input
	void PawnClientRestart() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

	/** Base camera zoom rate for behind-the-back camera, in cm/sec. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseCameraZoomRate;

	/** Maximum camera zoom distance for behind-the-back camera, in cm. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float MaxCameraZoom;
	

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);
	
	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);


	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);
	
#pragma region INPUT

	UPROPERTY(EditDefaultsOnly)
	FInputActions InputActions;
	
	// INPUT ACTIONS


	
	

	// INPUT MAPPINGS
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Controls|InputMapping")
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Controls|InputMapping")
	UInputMappingContext* MouseLookInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Controls|InputMapping")
	int32 BaseMappingPriority = 0;
	
	// INPUT FUNCTIONS

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	void CameraZoom_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CameraZoom(const FInputActionValue& Value);
	
	//void CameraZoom_Implementation(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void EnhancedJump(const FInputActionValue& Value);

	UFUNCTION()
	void MouseLook(const FInputActionValue& Value);
	void MouseLookOff(const FInputActionValue& Value);


#pragma endregion 

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Camera Root Position relative to Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector CameraRelativeLocation;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCameraComponent; }

	
protected:

	// Called on the server to acknowledge possession of this Character.
	virtual void PossessedBy(AController* NewController) override;

	// Called on the client when the Character is assigned it's Player State.
	virtual void OnRep_PlayerState() override;

	// Called to bind input to this Character.
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called to set-up Ability input bindings for this Character.
	void InitializeAbilityBindings();

	bool AbilityBindingsInitialized = false;	
};
