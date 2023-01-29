// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class UInputAction;
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "Misc/App.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterMovement.generated.h"


UCLASS()
class MYPROJECT_API AMyCharacterMovement : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMyCharacterMovement();

	UPROPERTY(EditAnywhere, Category = "Components|Camera")
		UCameraComponent* camera;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_WInOut;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_WLeftRight;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_LUpDown;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_LLeftRight;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_Jump;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_TP_Up;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_TP_Right;
	UPROPERTY(EditAnywhere, Category = "Input | Actions")
		UInputAction* IA_KB_TP_Forward;
	UPROPERTY(EditAnywhere, Category = "Input")
		TSoftObjectPtr<UInputMappingContext> IMC_Normal_Controls;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PawnClientRestart() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void KB_WInOut(const FInputActionInstance& Instance);
	void KB_WLeftRight(const FInputActionInstance& Instance);
	void KB_LUpDown(const FInputActionInstance& Instance);
	void KB_LLeftRight(const FInputActionInstance& Instance);
	//void KB_Jump(const FInputActionInstance& Instance);
	void KB_TP_Forward(const FInputActionInstance& Instance);
	void KB_TP_Right(const FInputActionInstance& Instance);
	void KB_TP_Up(const FInputActionInstance& Instance);
	void StopTeleporting(const FInputActionInstance& Instance);

private:
	float TeleportDistance;
	bool IsTeleporting;

};
