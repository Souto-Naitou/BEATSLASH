#pragma once

// Forward declaration 
class Player;

class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual void Enter(Player&) {};
    virtual void Update(Player&) = 0;
    virtual void Exit(Player&) {};
};