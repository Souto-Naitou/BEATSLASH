#pragma once
#include "IState.h"


template <typename Context>
class StateMachine
{
public:
    using State = IState<Context>;

    void ChangeState(State* pNewState, Context& ctx)
    {
        if (pCurrentState_)
        {
            pCurrentState_->Exit(ctx);
        }

        if (pNewState)
        {
            pNewState->Enter(ctx);
        }

        pCurrentState_ = pNewState;
    }

    void Update(Context& ctx)
    {
        if (pCurrentState_)
        {
            pCurrentState_->Update(ctx);
        }
    }

private:
    State* pCurrentState_ = nullptr;
};