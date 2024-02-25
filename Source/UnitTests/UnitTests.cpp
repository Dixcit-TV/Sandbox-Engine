// UnitTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <unordered_map>

#include "Core\Log\Logger.h"

#define UT_ENUM_TO_TYPE_W_String    std::wstring
#define UT_ENUM_TO_TYPE_Float       float
#define UT_ENUM_TO_TYPE_Int8        int8_t
#define UT_ENUM_TO_TYPE_Int16       int16_t
#define UT_ENUM_TO_TYPE_Int32       int32_t
#define UT_ENUM_TO_TYPE_UInt8       uint8_t
#define UT_ENUM_TO_TYPE_UInt16      uint16_t
#define UT_ENUM_TO_TYPE_UInt32      uint32_t

#define UT_ENUM_TO_TYPE(TypeEnum) UT_ENUM_TO_TYPE_##TypeEnum

template<typename TypeName>
void LoadParamValue(TypeName& outValue, const wchar_t* value)
{
    if constexpr (std::is_same_v<TypeName, UT_ENUM_TO_TYPE(W_String)>)
        outValue = std::wstring(value);
    else if constexpr (std::is_same_v<TypeName, UT_ENUM_TO_TYPE(Float)>)
        outValue = wcstof(value, nullptr);
    else if constexpr (std::is_same_v<TypeName, UT_ENUM_TO_TYPE(Int8)>
        || std::is_same_v<TypeName, UT_ENUM_TO_TYPE(Int16)>
        || std::is_same_v<TypeName, UT_ENUM_TO_TYPE(Int32)>)
        outValue = wcstol(value, nullptr, 10);
    else if constexpr (std::is_same_v<TypeName, UT_ENUM_TO_TYPE(UInt8)>
        || std::is_same_v<TypeName, UT_ENUM_TO_TYPE(UInt16)>
        || std::is_same_v<TypeName, UT_ENUM_TO_TYPE(UInt32)>)
        outValue = wcstoul(value, nullptr, 10);
    else if constexpr (std::is_same_v<TypeName, void>)
        SDBX_W_LOG(ERROR_LOG, L"Cannot extract void type parameter.");
}

struct ParamInfo
{
    enum class ParamType
    {
        W_String
        , Float
        , Int8
        , Int16
        , Int32
        , UInt8
        , UInt16
        , UInt32
        , Void
    };

    const std::wstring ParamKey;
    const std::wstring ParamDefinition;
    const ParamType Type;

    explicit ParamInfo(const std::wstring& paramKey, const std::wstring& paramDefinition, ParamType type)
        : ParamKey{ paramKey }, ParamDefinition{ paramDefinition }, Type{ type } {}
};

struct InputParams
{
    const std::wstring Folder;
    const std::wstring Section;
    const std::wstring Test;
    bool Break;

    explicit InputParams() = default;
    //void Parse(int argc, wchar_t* argv[]);
};

static const std::unordered_map<std::wstring, ParamInfo*> ConsoleParams
{
    { L"--Help", new ParamInfo(L"--Help", L"Display list of command arguments available", ParamInfo::ParamType::Void) }
    , { L"--Folder", new ParamInfo(L"--Folder", L"", ParamInfo::ParamType::W_String) }
    , { L"--Section", new ParamInfo(L"--Section", L"Name of the test section to run.", ParamInfo::ParamType::W_String) }
    , { L"--Test", new ParamInfo(L"--Test", L"Name of the test to run.", ParamInfo::ParamType::W_String) }
    , { L"--Break", new ParamInfo(L"--Break", L"Stop running subsequent test when a failing test is discovered.", ParamInfo::ParamType::Void) }
};

void PrintHelpText()
{
    std::wcout << L"<============ UnitTest Framework Settings ============>\n\n";
    for (const auto& paramPair : ConsoleParams)
    {
        std::wcout << L"+ " << paramPair.first << L": " << paramPair.second->ParamDefinition << L"\n";
    }
    std::wcout << L"\n";
}

int wmain(int argc, wchar_t* argv[])
{
    SDBX_W_ASSERT_MSG(!argc, L"No application parameters provided!");
    
    static const std::unordered_map<std::wstring, ParamInfo*> InParams{};

    const size_t paramCount{ ConsoleParams.size() };
    for (size_t idx{}; idx < paramCount; ++idx)
    {

    }

    for (int param{}; param < argc; ++param)
    {
        
    }
}
