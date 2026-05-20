#pragma once
#include <debug/DebugUIWrapper.h>
#include <debug/DebugEntry.h>
#include <source_location>
#include <functional>
#include <string>

#ifdef _DEBUG

    #define GameParameter(type, name, value)\
    GameParameterData<type> name{std::source_location::current().file_name(), #name, value}

    #define GameParameterView(type, name, value) \
    type name = value; \
    GameParameterViewData<type> view_##name{std::source_location::current().file_name(), #name, &name}


    #define EnableDebug(category) DebugEntry debugEntry{ std::source_location::current().file_name(), category }

#else

    #define GameParameter(type, name, value) static constexpr type name = value
    #define EnableDebug(category)

#endif


template <typename ValueType>
class GameParameterData
{
public:
    using OnChangeCallback = std::function<void(const ValueType&)>;

    GameParameterData(
        const std::string& id,
        const std::string& name,
        ValueType&& value = {})
        : v_(std::move(value))
    {
        DebugUIWrapper::GetInstance()->HandleParameter(id, name, &v_, [this]() {
            if (onChange_)
            {
                onChange_(v_);
            }
        });
    }

    GameParameterData(const GameParameterData&) = delete;
    GameParameterData& operator=(const GameParameterData&) = delete;

    operator ValueType() const { return v_; }
    ValueType& operator=(const ValueType& newValue)
    {
        v_ = newValue;
        if (onChange_)
        {
            onChange_(v_);
        }
        return v_;
    }
    operator ValueType& () { return v_; }
    operator const ValueType& () const { return v_; }
    ValueType* operator-> () { return &v_; }
    const ValueType* operator-> () const { return v_; }

    ValueType* GetPtr() { return &v_; }
    void SetOnChange(OnChangeCallback cb) { onChange_ = std::move(cb); }

private:
    ValueType v_ = {};
    OnChangeCallback onChange_;
};

template <typename ValueType>
class GameParameterViewData
{
public:
    GameParameterViewData(
        const std::string& id,
        const std::string& name,
        const ValueType* pValue = nullptr)
    {
        DebugUIWrapper::GetInstance()->HandleParameter(id, name, pValue);
    }
};
