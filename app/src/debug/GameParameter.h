#pragma once

#ifdef _DEBUG
#define GameParameter
#else
#define GameParameter static constexpr;
#endif