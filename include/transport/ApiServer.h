#ifndef HOLISTIC_TRACKING_API_H
#define HOLISTIC_TRACKING_API_H

#define EXPORT

#include <malloc.h>
#ifdef _WIN32
#ifdef EXPORT
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
#else
#include <stdlib.h>

#ifdef EXPORT
#define EXPORT_API __attribute__((visibility ("default")))
#else
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif 

#ifndef EXPORT_API
#define EXPORT_API
#endif

	EXPORT_API int FancyJingMsgInit(int uid_, int server_, int port_);
	EXPORT_API int FancyJingMsgRun();
	EXPORT_API int FancyJingMsgSendToPeer(uint32_t dest_id, const char* data, int len);
	EXPORT_API int FancyJingMsgRegisterRecvCallback(void(*callback)(const char*, int, int));
	EXPORT_API int FancyJingMsgStatus();
	EXPORT_API int FancyJingMsgRelease();
        EXPORT_API const char* FancyJingMsgVersion();

#ifdef __cplusplus
}
#endif 

#endif // !HOLISTIC_TRACKING_API_H
