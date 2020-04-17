// Copyright Cameron Wade 2020


#include "OpenGate.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenGate::UOpenGate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenGate::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = OpenAngle + InitialYaw;

	FindPressurePlate();
	FindAudioComponent();

	// ...
	
}


// Called every frame
void UOpenGate::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenGate(DeltaTime);
	}
	else
	{
		CloseGate(DeltaTime);
	}
	// ...
}

float UOpenGate::TotalMassOfActors() const
{
	float TotalMass = 0.0f;
	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenGate::OpenGate(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime*2.0f);
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	CloseGateSound = false;
	if (!AudioComponent) {return;}
	if (!OpenGateSound)
	{
		AudioComponent->Play();
		OpenGateSound = true;
	}
}

void UOpenGate::CloseGate(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime*2.0f);
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	OpenGateSound = false;
	if (!AudioComponent) {return;}
	if (!CloseGateSound)
	{
		AudioComponent->Play();
		CloseGateSound = true;
	}
}

void UOpenGate::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a PressurePlate assigned!"), *GetOwner()->GetName());
	}
}

void UOpenGate::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The %s does not have a an audio component assigned"), *GetOwner()->GetName());
	}
}