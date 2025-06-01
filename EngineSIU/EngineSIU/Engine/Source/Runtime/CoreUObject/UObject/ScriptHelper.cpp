#include "ScriptHelper.h"
#include <sstream>

namespace
{
    std::string ExtractTypeNameString(const std::string& str)
    {
        std::string_view RawTypeNameString = str;

        constexpr auto TrimWhitespace = [](std::string_view Sv) -> std::string_view
        {
            if (Sv.empty())
            {
                return Sv;
            }
            const char* WhitespaceChars = " \t\n\r\f\v";
            auto FirstNonWhitespace = Sv.find_first_not_of(WhitespaceChars);
            if (std::string_view::npos == FirstNonWhitespace)
            {
                return {};
            }
            auto LastNonWhitespace = Sv.find_last_not_of(WhitespaceChars);
            return Sv.substr(FirstNonWhitespace, LastNonWhitespace - FirstNonWhitespace + 1);
        };

        // constexpr로 알파벳/숫자인지 확인하는 헬퍼 람다
        constexpr auto IsAlnumChar = [](char C) -> bool
        {
            return (C >= 'a' && C <= 'z') ||
                (C >= 'A' && C <= 'Z') ||
                (C >= '0' && C <= '9');
        };

        std::string_view ProcessedTypeNameString = TrimWhitespace(RawTypeNameString);
        bool WasModified;

        // 후행 한정자 (포인터, 참조, cv-한정자) 제거
        do
        {
            WasModified = false;
            std::string_view TempTrimmedString = TrimWhitespace(ProcessedTypeNameString);

            if (TempTrimmedString.ends_with("&&"))
            {
                ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 2);
                WasModified = true;
            }
            else if (TempTrimmedString.ends_with("&"))
            {
                ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 1);
                WasModified = true;
            }
            else if (TempTrimmedString.ends_with("*"))
            {
                ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 1);
                WasModified = true;
            }
            else if (TempTrimmedString.length() >= 6 && TempTrimmedString.substr(TempTrimmedString.length() - 6) ==
                " const")
            {
                ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 6);
                WasModified = true;
            }
            else if (TempTrimmedString.length() >= 5 && TempTrimmedString.substr(TempTrimmedString.length() - 5) == "const")
            {
                if (TempTrimmedString.length() == 5 ||
                    (TempTrimmedString.length() > 5 && !IsAlnumChar(TempTrimmedString[TempTrimmedString.length() - 6])))
                {
                    ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 5);
                    WasModified = true;
                }
            }
            else if (TempTrimmedString.length() >= 9 && TempTrimmedString.substr(TempTrimmedString.length() - 9) ==
                " volatile")
            {
                ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 9);
                WasModified = true;
            }
            else if (TempTrimmedString.length() >= 8 && TempTrimmedString.substr(TempTrimmedString.length() - 8) ==
                "volatile")
            {
                if (TempTrimmedString.length() == 8 ||
                    (TempTrimmedString.length() > 8 && !IsAlnumChar(TempTrimmedString[TempTrimmedString.length() - 9])))
                {
                    ProcessedTypeNameString = TempTrimmedString.substr(0, TempTrimmedString.length() - 8);
                    WasModified = true;
                }
            }

            if (WasModified)
            {
                ProcessedTypeNameString = TrimWhitespace(ProcessedTypeNameString);
            }
        }
        while (WasModified);

        // 선행 cv-한정자 제거
        do
        {
            WasModified = false;
            std::string_view TempTrimmedString = TrimWhitespace(ProcessedTypeNameString);

            if (TempTrimmedString.length() >= 6 && TempTrimmedString.substr(0, 6) == "const ")
            {
                ProcessedTypeNameString = TempTrimmedString.substr(6);
                WasModified = true;
            }
            else if (TempTrimmedString.length() >= 5 && TempTrimmedString.substr(0, 5) == "const")
            {
                if (TempTrimmedString.length() == 5 ||
                    (TempTrimmedString.length() > 5 && !IsAlnumChar(TempTrimmedString[5])))
                {
                    ProcessedTypeNameString = TempTrimmedString.substr(5);
                    WasModified = true;
                }
            }
            else if (TempTrimmedString.length() >= 9 && TempTrimmedString.substr(0, 9) == "volatile ")
            {
                ProcessedTypeNameString = TempTrimmedString.substr(9);
                WasModified = true;
            }
            else if (TempTrimmedString.length() >= 8 && TempTrimmedString.substr(0, 8) == "volatile")
            {
                if (TempTrimmedString.length() == 8 ||
                    (TempTrimmedString.length() > 8 && !IsAlnumChar(TempTrimmedString[8])))
                {
                    ProcessedTypeNameString = TempTrimmedString.substr(8);
                    WasModified = true;
                }
            }

            if (WasModified)
            {
                ProcessedTypeNameString = TrimWhitespace(ProcessedTypeNameString);
            }
        }
        while (WasModified);

        // 선행 타입 키워드 ("class ", "struct ", "enum ", "union ") 제거
        constexpr std::string_view TypePrefixKeywords[] = {"class ", "struct ", "enum ", "union "};
        do
        {
            WasModified = false;
            std::string_view TempTrimmedString = TrimWhitespace(ProcessedTypeNameString);
            for (const std::string_view KeywordPrefix : TypePrefixKeywords)
            {
                if (TempTrimmedString.starts_with(KeywordPrefix))
                {
                    ProcessedTypeNameString = TempTrimmedString.substr(KeywordPrefix.size());
                    WasModified = true;
                    break;
                }
            }
            if (WasModified)
            {
                ProcessedTypeNameString = TrimWhitespace(ProcessedTypeNameString);
            }
        }
        while (WasModified);

        // 네임스페이스 제거 (bIsNameOnly == true 인 경우에만 적용)
        // 이 로직은 다른 모든 정제 작업 (cv, ptr/ref, class/struct 키워드) 이후에 적용됩니다.
        std::string_view FinalTypeNameString = TrimWhitespace(ProcessedTypeNameString);
        // auto LastColonColonPos = FinalTypeNameString.rfind("::");
        // if (LastColonColonPos != std::string_view::npos)
        // {
        //     // "::" 뒤에 실제 이름이 있는지 확인 (예: "MyNamespace::"와 같은 경우 방지)
        //     if (LastColonColonPos + 2 < FinalTypeNameString.length())
        //     {
        //         FinalTypeNameString = FinalTypeNameString.substr(LastColonColonPos + 2);
        //     }
        // }

        return std::string(FinalTypeNameString); 
    }
}

FPropertyString::FPropertyString(const std::string& Type, const std::string& Signature)
{
    this->Signature = Signature;
    this->Type = ExtractTypeNameString(Type);
}

std::string FPropertyString::TypeHint() const
{
    std::stringstream buffer;
    buffer << "---@field " << Signature << " " << Type;
    return buffer.str();
}

FFunctionString::FFunctionString(const std::string& Type, const std::string& Signature, const std::string& Arguments)
{
    this->Signature = Signature;
    this->Type = ExtractTypeNameString(Type);
    std::string str = Arguments;
    std::string argument;
    while (!str.empty())
    {
        uint64 pos = str.find_first_of(',');
        argument = str.substr(0, pos);

        const char* WhitespaceChars = " \t\n\r\f\v";
        uint64 WhitespacePos = argument.find_last_of(WhitespaceChars);
        std::string type = ExtractTypeNameString(argument.substr(0, WhitespacePos));
        std::string name = argument.substr(WhitespacePos + 1);
        Argument.Emplace(type, name);

        if (pos == std::string::npos)
        {
            str = "";
        }
        else
        {
            str = str.substr(pos + 1);
        }
    }
}

std::string FFunctionString::TypeHint() const
{
    std::stringstream buffer;
    for (const auto& [type, name]: Argument)
    {
        buffer << "---@param " << name << " " << type << '\n'; 
    }
    buffer << "---@return " << Type;
    return buffer.str();
}

std::string FFunctionString::Annotation(const std::string& className) const
{
    std::stringstream buffer;
    if (!className.empty())
    {
        buffer << "function " << className << ':' << Signature;
    }
    else
    {
        buffer << "function " << Signature;
    }
    buffer << '(';
    for (auto iter = Argument.begin(); iter != Argument.end();)
    {
        std::string name = iter->Value;
        buffer << name;

        auto next = ++iter;
        if (next != Argument.end())
        {
            buffer << ", ";
        }
    }
    buffer << ") end";
    return buffer.str();
}

FClassString::FClassString(const std::string& Signature, const std::string& ParentClass)
{
    this->Signature = Signature;
    this->ParentClass = ParentClass;
}

std::string FClassString::Annotation() const
{
    std::stringstream buffer;
    if (ParentClass.empty())
    {
        buffer << "---@class " << Signature << '\n';
    }
    else
    {
        buffer << "---@class " << Signature << " : " << ParentClass << '\n';
    }
    
    for (const auto& property: Properties)
    {
        buffer << property.TypeHint() << '\n';
    }
    buffer << "local " << Signature << " = {}" << '\n';
    
    buffer << '\n';

    for (const FFunctionString& funcString: Functions)
    {
        buffer << funcString.TypeHint() << '\n';
        buffer << funcString.Annotation(Signature) << '\n';
        buffer << '\n';
    }
    return buffer.str();
}

void FClassString::AddProperty(const std::string& Type, const std::string& Signature)
{
    Properties.Add(FPropertyString(Type, Signature));
}

void FClassString::AddFunction(const std::string& Type, const std::string& Signature, const std::string& Arguments)
{
    Functions.Add(FFunctionString(Type, Signature, Arguments));
}
