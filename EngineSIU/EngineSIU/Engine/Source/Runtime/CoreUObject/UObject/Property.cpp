#include "Property.h"
#include "Class.h"
#include "ScriptStruct.h"


void FProperty::Resolve()
{
}

void FUnresolvedPtrProperty::Resolve()
{
    FProperty::Resolve();

    if (Type == EPropertyType::Object)
    {
        return;
    }

    if (Type == EPropertyType::UnresolvedPointer)
    {
        if (std::holds_alternative<FName>(TypeSpecificData))
        {
            const FName& TypeName = std::get<FName>(TypeSpecificData);

            // ClassMap에서 먼저 검사
            if (UClass* FoundClass = UClass::FindClass(TypeName))
            {
                Type = EPropertyType::Object;
                TypeSpecificData = FoundClass;
                ResolvedProperty = new FObjectProperty{OwnerStruct, Name, Size, Offset, Flags, std::move(Metadata)};
                ResolvedProperty->TypeSpecificData = TypeSpecificData;
                return;
            }

            // StructMap에서 검사
            if (UScriptStruct* FoundStruct = UScriptStruct::FindScriptStruct(TypeName))
            {
                Type = EPropertyType::Struct;
                TypeSpecificData = FoundStruct;
                ResolvedProperty = new FStructProperty{OwnerStruct, Name, Size, Offset, Flags, std::move(Metadata)};
                ResolvedProperty->TypeSpecificData = TypeSpecificData;
                return;
            }
        }
    }

    Type = EPropertyType::Unknown;
    TypeSpecificData = std::monostate{};
    UE_LOGFMT(ELogLevel::Error, "Unknown Property Type: {}", Name);
}

void FUnresolvedPtrProperty::CopyData(const void* SrcPtr, void* DstPtr, FObjectDuplicator& Duplicator) const
{
    if (Type == EPropertyType::Unknown)
    {
        return;
    }
    
    ResolvedProperty->CopyData(SrcPtr, DstPtr, Duplicator);
}
