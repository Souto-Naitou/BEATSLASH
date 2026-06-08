#pragma once

template <typename Context>
class IState
{
public:
    virtual ~IState() = default;
    virtual void Enter(Context&) = 0;
    virtual void Update(Context&) = 0;
    virtual void Exit(Context&) = 0;
};