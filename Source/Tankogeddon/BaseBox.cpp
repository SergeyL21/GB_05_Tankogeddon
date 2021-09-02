#include "BaseBox.h"
#include "TankPawn.h"

// --------------------------------------------------------------------------------------
// Sets default values
ABaseBox::ABaseBox()
{
	PrimaryActorTick.bCanEverTick = false;
	auto sceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
	RootComponent = sceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

// --------------------------------------------------------------------------------------
void ABaseBox::MeshOverlapBeginImpl(ATankPawn */*Pawn*/)
{
	return;
}

// --------------------------------------------------------------------------------------
void ABaseBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp,
								  AActor* OtherActor,
								  UPrimitiveComponent* OtherComp,
								  int32 OtherBodyIndex,
								  bool bFromSweep,
								  const FHitResult& SweepResult)
{
	auto playerPawn{ Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()) };
	if (OtherActor == playerPawn)
	{
		MeshOverlapBeginImpl(playerPawn);
		Destroy();
	}
}

