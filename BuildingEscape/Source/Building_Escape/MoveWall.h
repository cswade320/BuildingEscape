// Copyright Cameron Wade 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "MoveWall.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UMoveWall : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveWall();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FindItemToEscape();
	void FindTriggerVolume();
	bool ContainsItemToEscape();
	void MoveWall(float DeltaTime);

private:

	FVector InitialPos;
	FVector CurrentPos;

	UPROPERTY(EditAnywhere)
	float DistanceToMoveX = 0.0f;

	UPROPERTY(EditAnywhere)
	float DistanceToMoveY = 0.0f;

	UPROPERTY(EditAnywhere)
	float DistanceToMoveZ = 0.0f;

	UPROPERTY(EditAnywhere)
	AActor* ItemToEscape = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* TriggerVolume = nullptr;

	
};
