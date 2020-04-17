// Copyright Cameron Wade 2020


#include "MoveWall.h"

// Sets default values for this component's properties
UMoveWall::UMoveWall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveWall::BeginPlay()
{
	Super::BeginPlay();

	InitialPos = GetOwner()->GetActorLocation();
	CurrentPos = InitialPos;
	DistanceToMoveX = DistanceToMoveX+ InitialPos.X;
	DistanceToMoveY = DistanceToMoveY+ InitialPos.Y;
	DistanceToMoveZ = DistanceToMoveZ+ InitialPos.Z;

	FindItemToEscape();
	FindTriggerVolume();
	// ...
}


// Called every frame
void UMoveWall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ContainsItemToEscape())
	{
		MoveWall(DeltaTime);
	}
	// ...
}

void UMoveWall::FindItemToEscape()
{
	if (!ItemToEscape)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an ItemToEscape assigned!"), *GetOwner()->GetName());
	}
}

void UMoveWall::FindTriggerVolume()
{
	if (!TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a TriggerVolume assigned!"), *GetOwner()->GetName());
	}
}

bool UMoveWall::ContainsItemToEscape()
{
	bool Test = false;
	TArray<AActor*> OverlappingActors;
	if (!TriggerVolume) {return false;}
	TriggerVolume->GetOverlappingActors(OUT OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor == ItemToEscape)
		{
			Test = true;
		}
	}
	return Test;
}

void UMoveWall::MoveWall(float DeltaTime)
{
	CurrentPos.X = FMath::Lerp(CurrentPos.X, DistanceToMoveX, DeltaTime*1.5f);
	CurrentPos.Y = FMath::Lerp(CurrentPos.Y, DistanceToMoveY, DeltaTime*1.5f);
	CurrentPos.Z = FMath::Lerp(CurrentPos.Z, DistanceToMoveZ, DeltaTime*1.5f);
	GetOwner()->SetActorLocation(CurrentPos);
}