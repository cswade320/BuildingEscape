// Copyright Cameron Wade 2020

#include "CloseDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UCloseDoor::UCloseDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCloseDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = OpenAngle + InitialYaw;

	FindItemToOpenDoor();
	FindPressurePlate();
	FindAudioComponent();
}


// Called every frame
void UCloseDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ContainsItemToOpenDoor())
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
	// ...
}

void UCloseDoor::FindItemToOpenDoor()
{
	if (!ItemToOpenDoor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an ItemToOpenDoor assigned!"), *GetOwner()->GetName());
	}
}

void UCloseDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a PressurePlate assigned!"), *GetOwner()->GetName());
	}
}

bool UCloseDoor::ContainsItemToOpenDoor()
{
	bool Test = false;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return false;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor == ItemToOpenDoor)
		{
			Test = true;
		}
	}
	return Test;
}

void UCloseDoor::OpenDoor(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime*1.0f);
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	CloseDoorSound = false;
	if (!AudioComponent) {return;}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UCloseDoor::CloseDoor(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime*2.0f);
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	OpenDoorSound = false;
	if (!AudioComponent) {return;}
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}

}

void UCloseDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The %s does not have a an audio component assigned"), *GetOwner()->GetName());
	}
}