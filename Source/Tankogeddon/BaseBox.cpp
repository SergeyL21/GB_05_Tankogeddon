#include "BaseBox.h"
#include "TankPawn.h"

// --------------------------------------------------------------------------------------
// Sets default values
ABaseBox::ABaseBox()
{
	PrimaryActorTick.bCanEverTick = false;
	auto SceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
	RootComponent = SceneComponent;

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
	auto PlayerPawn{ Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()) };
	if (OtherActor == PlayerPawn)
	{
		MeshOverlapBeginImpl(PlayerPawn);
		Destroy();
	}
}

