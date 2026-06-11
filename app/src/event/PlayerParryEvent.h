#pragma once

namespace Event::PlayerParry
{
    struct Rejected { const Tako::Vector3& position; };
    struct True { Tako::Vector3 position; };
}