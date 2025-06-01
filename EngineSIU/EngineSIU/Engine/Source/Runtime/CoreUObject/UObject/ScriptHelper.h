#pragma once
#include "Container/Array.h"
#include "Container/Map.h"
#include "Container/String.h"

struct FPropertyString
{
public:
    FPropertyString(const std::string& Type, const std::string& Signature);
    std::string TypeHint() const;
private:
    std::string Signature;
    std::string Type;
};

struct FFunctionString
{
    FFunctionString(const std::string& Type, const std::string& Signature, const std::string& Arguments);
    std::string TypeHint() const;
    std::string Annotation(const std::string& className = "") const;
private:
    std::string Signature;
    std::string Type;
    // Type, Signature
    TMap<std::string, std::string> Argument;
};

struct FClassString
{
    FClassString(const std::string& Signature, const std::string& ParentClass);
    void AddProperty(const std::string& Type, const std::string& Signature);
    void AddFunction(const std::string& Type, const std::string& Signature, const std::string& Arguments);
    std::string Annotation() const;
private:
    TArray<FPropertyString> Properties;
    TArray<FFunctionString> Functions;
    std::string Signature;
    std::string ParentClass;
};
