#ifndef MEDIAPIPE_HOLISTIC_TRACKING_DLL_H
#define MEDIAPIPE_HOLISTIC_TRACKING_DLL_H

#include <functional>

#include "DynamicModuleLoader.h"
using namespace DynamicModuleLoaderSpace;
#if 0
typedef int (*FuncFancyJingMsgInit)(const char* model_path);
typedef int (*FuncFancyJingMsgRun)(int uid_, int server_, int port_);
typedef int (*FuncFancyJingMsgSendToPeer(uint32_t dest_id, char* data, int len);
typedef int (*FuncFancyJingMsgRegisterRecvCallback(void(*callback)(const char*, int));
  EXPORT_API int FancyJingMsgSetIPAddr(const char*);
	typedef int (*FuncFuncFancyJingMsgRelease)();
#endif

class FancyMsgChanLibInterfaceDll
{
public:
        typedef int (*FuncFancyJingMsgInit)(int uid_, int server_, int port_);
        typedef int (*FuncFancyJingMsgRun)();
        typedef int (*FuncFancyJingMsgRelease)();
        typedef int (*FuncFancyJingMsgSetIPAddr)(const char*);
	typedef const char* (*FuncFancyJingMsgVersion)();
        
	typedef int (*FuncFancyJingMsgSendToPeer)(uint32_t dest_id, const char* data, int len);
	typedef int (*FuncFancyJingMsgAddConnAddr)(const char*, const char*, int, int, int, char);
	typedef void (*FuncFancyjingCallback)(const char*, int, int);
	typedef void (*FuncFancyjingConnCallback)(unsigned int, bool);
        typedef int (*FuncFancyJingMsgRegisterRecvCallback)(FuncFancyjingCallback);
        typedef int (*FuncFancyJingMsgRegisterConnCallback)(FuncFancyjingConnCallback);

public:
	FancyMsgChanLibInterfaceDll();
	virtual~ FancyMsgChanLibInterfaceDll();

public:
	bool LoadLibFromPath(const std::string& dll_path);
	bool UnLoadDll();
	bool GetAllFunctions();

public:
	FuncFancyJingMsgInit         m_FancyJingMsgInit;
	FuncFancyJingMsgRun          m_FancyJingMsgRun;
        FuncFancyJingMsgRelease      m_FancyJingMsgRelease;
	FuncFancyJingMsgSendToPeer   m_FancyJingMsgSendToPeer;
	FuncFancyJingMsgVersion      m_FancyJingMsgVersion;
	FuncFancyJingMsgSetIPAddr    m_FancyJingMsgSetIPAddr;
	FuncFancyJingMsgAddConnAddr  m_FancyJingMsgAddConnAddr;
        FuncFancyJingMsgRegisterRecvCallback m_FancyJingMsgRegisterRecvCallback;
        FuncFancyJingMsgRegisterConnCallback m_FancyJingMsgRegisterConnCallback;
private:
	DynamicModuleLoader m_DynamicModuleLoader;
};
#endif // !MEDIAPIPE_HOLISTIC_TRACKING_DLL_H
