#pragma once


class IPlayerSkill
{
public:
    virtual ~IPlayerSkill() = default;

    virtual void Activate() = 0;
    virtual void Update() = 0;
    virtual void End() = 0;

    virtual bool IsActive() const = 0;
};