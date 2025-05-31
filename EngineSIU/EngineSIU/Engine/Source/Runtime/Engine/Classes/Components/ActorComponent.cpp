#include "ActorComponent.h"

#include "GameFramework/Actor.h"


void UActorComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    
    TMap<FString, FString>& Properties = OutProperties;
    
    Properties.Add(TEXT("ComponentName"), *GetName());
    Properties.Add(TEXT("ComponentClass"), *GetClass()->GetName());
    
    Properties.Add(TEXT("ComponentOwner"), *GetOwner()->GetName());
    Properties.Add(TEXT("ComponentOwnerClass"), *GetOwner()->GetClass()->GetName());

    
    //Properties.Add(TEXT("bRegistered"), bRegistered ? TEXT("true") : TEXT("false"));
    //Properties.Add(TEXT("bWantsInitializeComponent"), bWantsInitializeComponent ? TEXT("true") : TEXT("false"));
    Properties.Add(TEXT("bIsActive"), bIsActive ? TEXT("true") : TEXT("false"));
    Properties.Add(TEXT("bAutoActive"), bAutoActive ? TEXT("true") : TEXT("false"));
    
}

void UActorComponent::SetProperties(const TMap<FString, FString>& Properties)
{
    // Super::ApplyProperties(Properties); // 만약 UObject 레벨에 있다면

    const FString* TempStr = nullptr;

    // --- 설정 값 복원 ---

    TempStr = Properties.Find(TEXT("bAutoActive")); // bAutoActive 변수가 있다고 가정
    if (TempStr)
    {
        // 이 플래그는 보통 BeginPlay나 InitializeComponent 등에서
        // SetActive를 호출할지 여부를 결정하는 데 사용될 수 있습니다.
        // 여기서는 플래그 값 자체만 복원합니다.
        this->bAutoActive = TempStr->ToBool();
    }


    // --- 초기 상태 값 복원 (Setter 함수 사용 권장) ---

    // bIsActive: 컴포넌트의 활성화 상태 설정
    TempStr = Properties.Find(TEXT("bIsActive"));
    if (TempStr)
    {
        // bool bShouldBeActive = TempStr->ToBool();
        // SetActive(bShouldBeActive); // SetActive 함수 호출 (구현 필요)

        // 임시로 직접 설정 (만약 SetActive 함수가 없다면)
        this->bIsActive = TempStr->ToBool();
        // 주의: 이 경우 SetActive에 포함될 수 있는 부가 로직이 누락될 수 있습니다.
    }
}

/* ActorComponent가 Actor와 World에 등록이 되었다는 전제하에 호출됩니다 */
void UActorComponent::InitializeComponent()
{
    assert(!bHasBeenInitialized);

    bHasBeenInitialized = true;
}

void UActorComponent::UninitializeComponent()
{
    assert(bHasBeenInitialized);

    bHasBeenInitialized = false;
}

void UActorComponent::BeginPlay()
{
    bHasBegunPlay = true;
}

void UActorComponent::TickComponent(float DeltaTime)
{
}

void UActorComponent::EndPhysicsTickComponent(float DeltaTime)
{
}

void UActorComponent::OnComponentDestroyed()
{
}

void UActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    assert(bHasBegunPlay);

    bHasBegunPlay = false;
}

void UActorComponent::DestroyComponent(bool bPromoteChildren)
{
    if (bIsBeingDestroyed)
    {
        return;
    }

    bIsBeingDestroyed = true;

    // Owner에서 Component 제거하기
    if (AActor* MyOwner = GetOwner())
    {
        MyOwner->RemoveOwnedComponent(this);
        if (MyOwner->GetRootComponent() == this)
        {
            MyOwner->SetRootComponent(nullptr);
        }
    }

    if (bHasBegunPlay)
    {
        EndPlay(EEndPlayReason::Destroyed);
    }

    if (bHasBeenInitialized)
    {
        UninitializeComponent();
    }

    OnComponentDestroyed();

    // 나중에 ProcessPendingDestroyObjects에서 실제로 제거됨
    GUObjectArray.MarkRemoveObject(this);
}

void UActorComponent::Activate()
{
    // TODO: Tick 다시 재생
    bIsActive = true;
}

void UActorComponent::Deactivate()
{
    // TODO: Tick 멈추기
    bIsActive = false;
}
