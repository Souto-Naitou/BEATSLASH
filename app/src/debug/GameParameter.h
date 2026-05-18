#pragma once
#include <debug/DebugUIWrapper.h>
#include <debug/DebugEntry.h>
#include <source_location>

#ifdef _DEBUG

    #define GameParameter(type, name, value)\
    GameParameterData<type> name{std::source_location::current().file_name(), #name, value}

    #define EnableDebug(category) DebugEntry debugEntry{ std::source_location::current().file_name(), category }

#else

    #define GameParameter(type, name, value) static constexpr type name = value
    #define EnableDebug(category)

#endif


template <typename ValueType>
class GameParameterData
{
public:
    GameParameterData(const std::string& id, const std::string& name, ValueType&& value = {}): v(std::move(value))
    {
        DebugUIWrapper::GetInstance()->HandleParameter(id, name, &v);
    }

    GameParameterData(const GameParameterData&) = delete;
    GameParameterData& operator=(const GameParameterData&) = delete;

    operator ValueType() const { return v; }

    ValueType* GetPtr() { return &v; }

private:
    ValueType v = {};
};
