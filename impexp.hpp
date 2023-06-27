#pragma once

#define LANCELOT_EXPORTS

#ifdef LANCELOT_EXPORTS
    #define LL_CALL __declspec(dllexport)
#else
    #define LL_CALL __declspec(dllimport)
#endif