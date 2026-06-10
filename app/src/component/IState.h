#pragma once

template <typename Context>
class IState
{
public:
    virtual ~IState() = default;
    virtual void Enter(Context& ctx) = 0;
    virtual void Update(Context& ctx) = 0;
    virtual void Exit(Context& ctx) = 0;
};