#include <iostream>
#include "FancyMsgChanLibInterfaceDll.h"

FancyMsgChanLibInterfaceDll::FancyMsgChanLibInterfaceDll()
{
	m_FancyJingMsgInit = nullptr;
	m_FancyJingMsgRun = nullptr;
	m_FancyJingMsgRelease = nullptr;
	m_FancyJingMsgSendToPeer = nullptr;
        m_FancyJingMsgRegisterRecvCallback = nullptr;
}

FancyMsgChanLibInterfaceDll::~FancyMsgChanLibInterfaceDll()
{
	if (m_DynamicModuleLoader.GetDynamicModuleState())
	{
		m_DynamicModuleLoader.UnloadDynamicModule();
	}
}

bool FancyMsgChanLibInterfaceDll::LoadLibFromPath(const std::string& dll_path)
{
	std::string dllPath = dll_path;
        std::cout << "FancyMsgChanLibInterfaceDll::LoadFancyMsgChanLibInterfaceDll path:" << dll_path << std::endl;
	if (m_DynamicModuleLoader.IsFileExist(dllPath))
	{
		std::cout << "FancyMsgChanLibInterfaceDll::LoadFancyMsgChanLibInterfaceDll file exist and load dynamic module..." << std::endl;
		if (m_DynamicModuleLoader.LoadDynamicModule(dllPath))
		{
			return true;
		}
	}
        std::cout << "FancyMsgChanLibInterfaceDll::LoadFancyMsgChanLibInterfaceDll load dynamic module error!" << std::endl;
	return false;
}

bool FancyMsgChanLibInterfaceDll::UnLoadDll()
{
	if (m_DynamicModuleLoader.UnloadDynamicModule())
	{
		return true;
	}

	return false;
}

bool FancyMsgChanLibInterfaceDll::GetAllFunctions()
{
	std::cout << "FancyMsgChanLibInterfaceDll::GetAllFunctions begin......." << std::endl;
	if (m_DynamicModuleLoader.GetDynamicModuleState())
	{
		void* pFancyMsgInit = m_DynamicModuleLoader.GetFunction("FancyJingMsgInit");
		if (pFancyMsgInit != nullptr)
		{
			m_FancyJingMsgInit = (FuncFancyJingMsgInit)(pFancyMsgInit);
			if (m_FancyJingMsgInit != nullptr)
			{
				std::cout << "GetAllFunctions for func FancyMsgInit success......" << std::endl;
			}
			else
			{
				std::cout << "GetAllFunctions for func FancyMsgInit error 1......" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "GetAllFunctions for func FancyMsgInit error 2......" << std::endl;
			return false;
		}

		void* pFancyMsgRun = m_DynamicModuleLoader.GetFunction("FancyJingMsgRun");
		if (pFancyMsgRun != nullptr)
		{
			m_FancyJingMsgRun = (FuncFancyJingMsgRun)(pFancyMsgRun);
			if (m_FancyJingMsgRun != nullptr)
			{
                               std::cout << "GetAllFunctions for func FancyMsgRun success......" << std::endl;
			}
			else
			{
				std::cout << "GetAllFunctions for func FancyMsgRun error 1......" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "GetAllFunctions for func FancyMsgRun error 2......" << std::endl;
			return false;
		}


		void* pFancyMsgRelease = m_DynamicModuleLoader.GetFunction("FancyJingMsgRelease");
		if (pFancyMsgRelease != nullptr)
		{
			m_FancyJingMsgRelease = (FuncFancyJingMsgRelease)(pFancyMsgRelease);
			if (m_FancyJingMsgRelease != nullptr)
			{
                                std::cout << "GetAllFunctions for func FancyMsgRelease success......" << std::endl;
			}
			else
			{
				std::cout << "GetAllFunctions for func FancyMsgRelease error 1......" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "GetAllFunctions for func FancyMsgRelease error 2......" << std::endl;
			return false;
		}

                void* pFancyMsgSetIPAddr = m_DynamicModuleLoader.GetFunction("FancyJingMsgSetIPAddr");
                if (pFancyMsgSetIPAddr != nullptr)
                {
                        m_FancyJingMsgSetIPAddr = (FuncFancyJingMsgSetIPAddr)(pFancyMsgSetIPAddr);
                        if (m_FancyJingMsgSetIPAddr != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgSetIPAddr success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgSetIPAddr error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgSetIPAddr error 2......" << std::endl;
                        return false;
                }

                void* pFancyMsgAddConnAddr = m_DynamicModuleLoader.GetFunction("FancyJingMsgAddConnAddr");
                if (pFancyMsgAddConnAddr != nullptr)
                {
                        m_FancyJingMsgAddConnAddr = (FuncFancyJingMsgAddConnAddr)(pFancyMsgAddConnAddr);
                        if (m_FancyJingMsgAddConnAddr != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgAddConnAddr success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgAddConnAddr error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgAddConnAddr error 2......" << std::endl;
                        return false;
                }

                void* pFancyMsgVersion = m_DynamicModuleLoader.GetFunction("FancyJingMsgVersion");
                if (pFancyMsgVersion != nullptr)
                {
                        m_FancyJingMsgVersion = (FuncFancyJingMsgVersion)(pFancyMsgVersion);
                        if (m_FancyJingMsgVersion != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgVersion success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgVersion error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgVersion error 2......" << std::endl;
                        return false;
                }		

                void* pFancyMsgSendToPeer = m_DynamicModuleLoader.GetFunction("FancyJingMsgSendToPeer");
                if (pFancyMsgSendToPeer != nullptr)
                {
                        m_FancyJingMsgSendToPeer  = (FuncFancyJingMsgSendToPeer)(pFancyMsgSendToPeer);
                        if (m_FancyJingMsgSendToPeer != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgSendToPeer success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgSendToPeer error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgSendToPeer error 2......" << std::endl;
                        return false;
                }

                void* pFancyMsgRegisterRecvCallback = m_DynamicModuleLoader.GetFunction("FancyJingMsgRegisterRecvCallback");
                if (pFancyMsgRegisterRecvCallback != nullptr)
                {
                        m_FancyJingMsgRegisterRecvCallback = (FuncFancyJingMsgRegisterRecvCallback)(pFancyMsgRegisterRecvCallback);
                        if (m_FancyJingMsgRegisterRecvCallback != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgRegisterRecvCallback success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgRegisterRecvCallback error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgRegisterRecvCallback error 2......" << std::endl;
                        return false;
                }

                void* pFancyMsgRegisterConnCallback = m_DynamicModuleLoader.GetFunction("FancyJingMsgRegisterConnCallback");
                if (pFancyMsgRegisterConnCallback != nullptr)
                {
                        m_FancyJingMsgRegisterConnCallback = (FuncFancyJingMsgRegisterConnCallback)(pFancyMsgRegisterConnCallback);
                        if (m_FancyJingMsgRegisterConnCallback != nullptr)
                        {
                                std::cout << "GetAllFunctions for func FancyMsgRegisterConnCallback success......" << std::endl;
                        }
                        else
                        {
                                std::cout << "GetAllFunctions for func FancyMsgRegisterConnCallback error 1......" << std::endl;
                                return false;
                        }
                }
                else
                {
                        std::cout << "GetAllFunctions for func FancyMsgRegisterConnCallback error 2......" << std::endl;
                        return false;
                }		
	}

	return true;
}
