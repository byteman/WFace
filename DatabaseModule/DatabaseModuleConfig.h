#ifndef DatabaseModuleConfig_H
#define DatabaseModuleConfig_H

#ifdef DATABASEMODULE_EXPORTS
#define DATABASEMODULE_API __declspec(dllexport)
#else
#define DATABASEMODULE_API
#endif

#endif
