#include <iostream>
#include "DynamicModuleLoader.h"

namespace DynamicModuleLoaderSpace
{
	DynamicModuleLoader::DynamicModuleLoader() :m_DynamicModulePtr(NULL), m_ErrorMessage(""), m_DynamicModuleState(DynamicModuleState::DMS_UnLoaded)
	{
	}

	DynamicModuleLoader::~DynamicModuleLoader()
	{
	}

	bool DynamicModuleLoader::IsFileExist(const std::string filePath)
	{
		std::fstream file;
		file.open(filePath, std::ios::in);
		if (file)
		{
			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DynamicModuleLoader::LoadDynamicModule(const std::string dynamicModulePath)
	{
		if (IsFileExist(dynamicModulePath))
		{
#ifdef WINDOWS
			m_DynamicModulePtr = LoadLibrary(dynamicModulePath.c_str());
#else
			std::cout << "DynamicModuleLoader::LoadDynamicModule file path:" << dynamicModulePath << std::endl;
			m_DynamicModulePtr = dlopen(dynamicModulePath.c_str(), RTLD_NOW);
#endif // WINDOWS
			std::cout << " load dll libary finish" << std::endl;
			std::cout << "DynamicModuleLoader::LoadDynamicModule finish and ptr is not null?" << m_DynamicModulePtr << std::endl;
			if (m_DynamicModulePtr != NULL)
			{
				std::cout << "load dll libary module success" << std::endl;
				m_DynamicModuleState = DynamicModuleState::DMS_Loaded;
				return true;
			}
			else
			{
#ifdef WINDOWS
				int errorCode = GetLastError();
#else  
				int errorCode = errno;
				std::cout << "open dll libary error for:" << dlerror() << std::endl;
#endif
				GetInternalErrorMessge(errorCode);
				return false;
			}

		}

		return false;
	}

	void* DynamicModuleLoader::GetFunction(const std::string functionName)
	{
		if (m_DynamicModulePtr)
		{
			void* tempFunctionPtr = NULL;
#ifdef WINDOWS
			tempFunctionPtr = GetProcAddress(m_DynamicModulePtr, functionName.c_str());
#else
			std::cout << "DynamicModuleLoader::GetFunction for func:" << functionName << std::endl;
			tempFunctionPtr = dlsym(m_DynamicModulePtr, functionName.c_str());
#endif // WINDOWS

			if (tempFunctionPtr != NULL)
			{
				return tempFunctionPtr;
			}
			else
			{
#ifdef WINDOWS
				int errorCode = GetLastError();
#else
				int errorCode = errno;
				std::cout << "open dll libary error for:" << dlerror() << std::endl;
#endif
				GetInternalErrorMessge(errorCode);
			}
		}

		return NULL;
	}

	bool DynamicModuleLoader::UnloadDynamicModule()
	{
		if (m_DynamicModulePtr)
		{
#ifdef WINDOWS
			if (FreeLibrary(m_DynamicModulePtr) == 0)
			{
				int errorCode = GetLastError(); // 得到错误代码
				GetInternalErrorMessge(errorCode);
				return false;
			}
#else
			dlclose(m_DynamicModulePtr);
#endif
			m_DynamicModuleState = DynamicModuleState::DMS_UnLoaded;
			return true;
		}
		return false;
	}

#ifdef WINDOWS

#endif


	void DynamicModuleLoader::GetInternalErrorMessge(int errorCode)
	{
		std::string errorMessge = "";
#ifdef WINDOWS
		LPVOID lpMsgBuf;
		if (FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		))
		{
			errorMessge = (LPCTSTR)(lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
#else
		errorMessge = dlerror();
#endif // WINDOWS

		m_ErrorMessage = errorMessge;
	}

	std::string DynamicModuleLoader::GetErrorMessage()
	{
		return m_ErrorMessage;
	}

	bool DynamicModuleLoader::GetDynamicModuleState()
	{
		return (bool)m_DynamicModuleState;
	}
}
