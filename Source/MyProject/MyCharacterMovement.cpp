// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovement.h"

// Sets default values
AMyCharacterMovement::AMyCharacterMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMyCharacterMovement::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("is it gonna be added?"));
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!IMC_Normal_Controls.IsNull())
			{
				InputSystem->ClearAllMappings();
				InputSystem->AddMappingContext(IMC_Normal_Controls.LoadSynchronous(), 0);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("added"));
			}
		}
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(IMC_Normal_Controls.LoadSynchronous(), 0);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("added"));
		}
	}
	TeleportDistance = 100.0;
	IsTeleporting = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("is it added"));
}

// Called every frame
void AMyCharacterMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacterMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_KB_WInOut, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_WInOut); //perhaps ETriggerEvent::Ongoing ?
	Input->BindAction(IA_KB_WLeftRight, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_WLeftRight);
	Input->BindAction(IA_KB_LUpDown, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_LUpDown);
	Input->BindAction(IA_KB_LLeftRight, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_LLeftRight);
	Input->BindAction(IA_KB_Jump, ETriggerEvent::Triggered, this, &AMyCharacterMovement::Jump);
	Input->BindAction(IA_KB_Jump, ETriggerEvent::Completed, this, &AMyCharacterMovement::StopJumping);
	Input->BindAction(IA_KB_TP_Up, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_TP_Up);
	Input->BindAction(IA_KB_TP_Right, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_TP_Right);
	Input->BindAction(IA_KB_TP_Forward, ETriggerEvent::Triggered, this, &AMyCharacterMovement::KB_TP_Forward);
	Input->BindAction(IA_KB_TP_Up, ETriggerEvent::Triggered, this, &AMyCharacterMovement::StopTeleporting);
	Input->BindAction(IA_KB_TP_Right, ETriggerEvent::Triggered, this, &AMyCharacterMovement::StopTeleporting);
	Input->BindAction(IA_KB_TP_Forward, ETriggerEvent::Triggered, this, &AMyCharacterMovement::StopTeleporting);
	
}

void AMyCharacterMovement::KB_WInOut(const FInputActionInstance& Instance)
{
	/*FVector ForwardVector = GetActorForwardVector();
	SetActorLocation(GetActorLocation() + (ForwardVector * Speed * FApp::GetDeltaTime()));*/

	/*if (Instance.GetValue().Get<float>() != 0.0f) {
		AddMovementInput(GetActorForwardVector(), Instance.GetValue().Get<float>());
	}*/

	// find out which direction is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Instance.GetValue().Get<float>());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
}
void AMyCharacterMovement::KB_WLeftRight(const FInputActionInstance& Instance)
{
	/*FVector RightVector = GetActorRightVector();
	SetActorLocation(GetActorLocation() + (RightVector * Speed * FApp::GetDeltaTime()));*/

	/*if (Instance.GetValue().Get<float>() != 0.0f) {
		AddMovementInput(GetActorRightVector(), Instance.GetValue().Get<float>());
	}*/

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Instance.GetValue().Get<float>());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
}
void AMyCharacterMovement::KB_LUpDown(const FInputActionInstance& Instance)
{
	if (Instance.GetValue().Get<float>() != 0.0f) {
		AddControllerPitchInput(Instance.GetValue().Get<float>());
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
}
void AMyCharacterMovement::KB_LLeftRight(const FInputActionInstance& Instance)
{
	if (Instance.GetValue().Get<float>() != 0.0f) {
		AddControllerYawInput(Instance.GetValue().Get<float>());
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
}
void AMyCharacterMovement::KB_TP_Forward(const FInputActionInstance& Instance)
{
	FVector ForwardVector = GetActorForwardVector();
	SetActorLocation(GetActorLocation() + (ForwardVector * TeleportDistance));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
	IsTeleporting = true;
}

void AMyCharacterMovement::KB_TP_Right(const FInputActionInstance& Instance)
{
	FVector RightVector = GetActorRightVector();
	SetActorLocation(GetActorLocation() + (RightVector * TeleportDistance));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
	IsTeleporting = true;
}

void AMyCharacterMovement::KB_TP_Up(const FInputActionInstance& Instance)
{
	FVector UpVector = GetActorUpVector();
	SetActorLocation(GetActorLocation() + (UpVector * TeleportDistance));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Message goes here"));
	IsTeleporting = true;
}

void AMyCharacterMovement::StopTeleporting(const FInputActionInstance& Instance) 
{
	//wait for some time
	IsTeleporting = false;
}

