#pragma once

#ifdef POTATORCORE_EXPORTS
#define EX __declspec(dllexport)
#else
#define EX __declspec(dllimport)
#endif