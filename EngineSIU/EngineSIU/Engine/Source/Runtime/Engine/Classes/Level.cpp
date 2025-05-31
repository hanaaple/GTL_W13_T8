#include "Level.h"
#include "GameFramework/Actor.h"
#include "UObject/Casts.h"


void ULevel::InitLevel(UWorld* InOwningWorld)
{
    OwningWorld = InOwningWorld;

}

void ULevel::Release()
{
    for (AActor* Actor : Actors)
    {
        Actor->EndPlay(EEndPlayReason::WorldTransition);
        TSet<UActorComponent*> Components = Actor->GetComponents();
        for (UActorComponent* Component : Components)
        {
            GUObjectArray.MarkRemoveObject(Component);
        }
        GUObjectArray.MarkRemoveObject(Actor);
    }
    Actors.Empty();
}

void ULevel::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);
    const ULevel* SrcLevel = static_cast<const ULevel*>(Source);

    Actors.Empty();

    for (AActor* SrcActor : SrcLevel->Actors)
    {
        // 같은 Duplicator 인스턴스를 재사용
        UObject* Copied = Duplicator.DuplicateObject(SrcActor);
        AActor* NewActor = static_cast<AActor*>(Copied);
        Actors.Add(NewActor);
    }
}

// UObject* ULevel::Duplicate(UObject* InOuter)
// {
//     ThisClass* NewLevel = Cast<ThisClass>(Super::Duplicate(InOuter));
//
//     NewLevel->OwningWorld = OwningWorld;
//
//     for (AActor* Actor : Actors)
//     {
//         NewLevel->Actors.Emplace(static_cast<AActor*>(Actor->Duplicate(InOuter)));
//     }
//
//     return NewLevel;
// }
