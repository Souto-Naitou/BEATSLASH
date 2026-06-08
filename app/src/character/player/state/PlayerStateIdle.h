#pragma once

#include <component/IState.h>

struct PlayerStateContext;

class PlayerStateIdle : public IState<PlayerStateContext>
{
public:


    void Enter(PlayerStateContext& ctx) override;


    void Update(PlayerStateContext& ctx) override;


    void Exit(PlayerStateContext& ctx) override;

};