#include "FObjectDuplicator.h"

#include "Object.h"
#include "ObjectFactory.h"

UObject* FObjectDuplicator::DuplicateObject(UObject* Src)
{
    // 1) 이미 복제됐으면 재사용
    if (DuplicatedMap.Contains(Src) == true)
    {
        return DuplicatedMap[Src];
    }
    
    // ——— DestOuter 결정 ———
    UObject* SrcOuter = Src->GetOuter();
    UObject* DestOuter = nullptr;
    if (SrcOuter && DuplicatedMap.Contains(SrcOuter))
    {
        // SrcOuter가 이미 복제됐다면 그 복제본을 Outer로 사용
        DestOuter = DuplicatedMap[SrcOuter];
    }
    else
    {
        // 그렇지 않으면 루트 DestOuter
        DestOuter = Params.DestOuter;
    }
    
    // ——— DestName 결정 ———
    FName DestName;
    if (Src == Params.Source)
    {
        // 루트 객체
        DestName = Params.DestName;
    }
    else
    {
        // 중첩된 객체: 원본 이름 그대로
        DestName = TEXT("CopyOf_") + Src->GetName();
    }
    

    // 3) 새 인스턴스 생성 및 맵에 등록
    UObject* NewObj = FObjectFactory::ConstructObject(Src->GetClass(), DestOuter, DestName);
    DuplicatedMap[Src] = NewObj;

    // 4) 프로퍼티별 복제
    {
        // 1) Child → Parent → GrandParent 순서로 TempStructs에 쌓기
        TArray<UStruct*> TempStructs;
        for (UStruct* Iter = Src->GetClass(); Iter; Iter = Iter->GetSuperStruct())
        {
            TempStructs.Add(Iter);
        }
        // 이제 TempStructs = [Child, Parent, GrandParent, ...]

        // 2) StructStack에 순서대로 Push (TempStructs[0] 부터 TempStructs.Last() 순서)
        TArray<UStruct*> StructQueue;
        for (int32 i = TempStructs.Num() - 1; i >= 0; --i)
        {
            StructQueue.Add(TempStructs[i]);
        }
        // StructStack = [GrandParent, Parent, Child, ...]

        for (UStruct* Iter : StructQueue)
        {
            for (FProperty* Prop : Iter->GetProperties())
            {
                // ShallowCopy 검사
                if (HasAnyFlags(Prop->Flags, DuplicateTransient))
                {
                    continue;
                }
                
                if (HasAnyFlags(Prop->Flags, EPropertyFlags::ShallowCopy))
                {
                    void* SrcPtr = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(Src) + Prop->Offset);
                    void* DstPtr = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(NewObj) + Prop->Offset);
                    FPlatformMemory::Memcpy(DstPtr, SrcPtr, static_cast<size_t>(Prop->Size));
                    continue;
                }

                // 원본/대상 주소 계산
                void* SrcPtr = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(Src) + Prop->Offset);
                void* DstPtr = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(NewObj) + Prop->Offset);

                // 타입별 복제 로직 위임
                Prop->CopyData(SrcPtr, DstPtr, *this);
            }
        }
    }

    // 5) 서브오브젝트 복제 및 후처리 훅
    NewObj->DuplicateSubObjects(Src, SrcOuter, *this);
    NewObj->PostDuplicate();

    return NewObj;
}
