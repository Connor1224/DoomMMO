/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_Logger_H
#define Navigation_Logger_H

#include "gwnavruntime/kernel/SF_Debug.h"
#include "gwnavruntime/kernel/SF_Log.h"
#include "gwnavruntime/base/memory.h"


// KY_NO_LOG_OUTPUT is different from KY_NO_DEBUG_OUTPUT
// => KY_NO_DEBUG_OUTPUT disables KY_DEBUG_XXX macros
// => KY_NO_LOG_OUTPUT disables all KY_LOG_XXX macros and Kaim::LogStream() << 
#if defined(KY_BUILD_SHIPPING)
# define KY_NO_LOG_OUTPUT
#endif

#ifndef KY_NO_LOG_OUTPUT
#include "gwnavruntime/kernel/SF_Std.h"
#endif


namespace Kaim
{

class VisualDebugServer;


/// BaseLog must used instead of Kaim::Log due to parti pris for Navigation's VisualDebug
class BaseLog : public Log
{
public:
	virtual void SetVisualDebugServer(VisualDebugServer* server) = 0;
	virtual VisualDebugServer* GetVisualDebugServer() = 0;

	/// BaseLog static functions must be used instead of Log::
	static BaseLog* GetGlobalBaseLog() { return (BaseLog*) GetGlobalLog(); }
	static void     SetGlobalBaseLog(BaseLog *log) { SetGlobalLog(log); }
};


static const char Endl = '\n';


// LogStream useful to print to log with << sematics, usage:
// LogStream stream;
// stream << "x=" << x << "\n";
class LogStream
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public:
#ifndef KY_NO_LOG_OUTPUT
	template <typename T>
	LogStream& Log(const char* format, const T& value)
	{
		if (Kaim::BaseLog::GetGlobalBaseLog())
			Kaim::BaseLog::GetGlobalBaseLog()->LogMessage(format, value);
		return *this;
	}
#endif
};


#ifndef KY_NO_LOG_OUTPUT
	inline LogStream& operator<<(LogStream& s, KyInt32 value)     { return s.Log("%i", value); }
	inline LogStream& operator<<(LogStream& s, KyUInt32 value)    { return s.Log("%u", value); }
	inline LogStream& operator<<(LogStream& s, KyInt64 value)     { return s.Log("%i", (KyInt32)value); }
	inline LogStream& operator<<(LogStream& s, KyUInt64 value)    { return s.Log("%u", (KyUInt32)value); }
	inline LogStream& operator<<(LogStream& s, float value)       { return s.Log("%f", value); }
	inline LogStream& operator<<(LogStream& s, double value)      { return s.Log("%f", value); }
	inline LogStream& operator<<(LogStream& s, bool value)        { return value ? s.Log("%s", "true") : s.Log("%s", "false"); }
	inline LogStream& operator<<(LogStream& s, char value)        { return s.Log("%c", value); }
	inline LogStream& operator<<(LogStream& s, const char* str)   { return s.Log("%s", str); }
#else
	inline LogStream& operator<<(LogStream& s, KyInt32 value)     { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, KyUInt32 value)    { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, KyInt64 value)     { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, KyUInt64 value)    { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, float value)       { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, double value)      { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, bool value)        { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, char value)        { KY_UNUSED(value); return s; }
	inline LogStream& operator<<(LogStream& s, const char* str)   { KY_UNUSED(str);   return s; }
#endif

// usage
// FloatFormatter fmt4f("%.4f");
// stream << fmt4f(x) << fmt4f(y) << fmt4f(z);
class FloatFormatter
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public:
	FloatFormatter(const char* format = "%.2f") : m_format(format) {}

	char* operator()(KyFloat32 value)
	{
	#ifndef KY_NO_LOG_OUTPUT
		SFsprintf(m_buffer, ArraySize(m_buffer), m_format, value);
		return m_buffer;
	#else
		KY_UNUSED(value);
		return KY_NULL;
	#endif
	}

private:
	const char* m_format;
	char m_buffer[64];
};

}

#define KY_LOG_BIG_TITLE_BEGIN(prefix, title) \
	   prefix << "==============================================================\n" \
	<< prefix << "======== " << title << "\n\n"

#define KY_LOG_BIG_TITLE_END(prefix, title) \
	   prefix << "======== " << title << "\n" \
	<< prefix << "==============================================================\n\n"
	
#define KY_LOG_SMALL_TITLE_BEGIN(prefix, title) \
	   prefix << "------------------------------------------------------\n" \
	<< prefix << "---- " << title << "\n"

#define KY_LOG_SMALL_TITLE_END(prefix, title) \
	   prefix << "---- " << title << "\n" \
	<< prefix << "------------------------------------------------------\n\n"




#ifndef KY_NO_LOG_OUTPUT

	namespace Kaim
	{
		void LogMessage(const LogMessageHelper& helper);
	}

	// To log in DEBUG/RELEASE but not in SHIPPING
	// usage KY_LOG_XXX(("x = %d; y = %d", x, y));
	// KY_LOG_TEXT:    prefix=""         , suffix=""
	// KY_LOG_MESSAGE: prefix=""         , suffix="\n"
	// KY_LOG_WARNING: prefix="Warning: ", suffix="\n"
	// KY_LOG_ERROR:   prefix="Error: "  , suffix="\n"
	#define KY_LOG_TEXT(args)      KY_MACRO_START LogMessage( Kaim::LogMessageHelper(Kaim::Log_Message, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_MESSAGE(args)   KY_MACRO_START LogMessage( Kaim::LogMessageHelper(Kaim::Log_MessageLine, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_WARNING(args)   KY_MACRO_START LogMessage( Kaim::LogMessageHelper(Kaim::Log_Warning, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_ERROR(args)     KY_MACRO_START LogMessage( Kaim::LogMessageHelper(Kaim::Log_Error, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END

	// usage KY_LOG_OUTPUT((logMessageId ,"x = %d; y = %d", x, y))
	// where logMessageId is of LogMessageId, this will command prefix and suffix used, and it allows to specify your own channel
	#define KY_LOG_OUTPUT(args)    KY_MACRO_START LogMessage( Kaim::LogMessageHelper(__FILE__, __LINE__).MakeMessage args );  KY_MACRO_END


	#define KY_LOG_TEXT_IF(condition, args)      KY_MACRO_START if (condition) LogMessage( Kaim::LogMessageHelper(Kaim::Log_Message, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_MESSAGE_IF(condition, args)   KY_MACRO_START if (condition) LogMessage( Kaim::LogMessageHelper(Kaim::Log_MessageLine, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_WARNING_IF(condition, args)   KY_MACRO_START if (condition) LogMessage( Kaim::LogMessageHelper(Kaim::Log_Warning, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_ERROR_IF(condition, args)     KY_MACRO_START if (condition) LogMessage( Kaim::LogMessageHelper(Kaim::Log_Error, __FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
	#define KY_LOG_OUTPUT_IF(condition, args)    KY_MACRO_START if (condition) LogMessage( Kaim::LogMessageHelper(__FILE__, __LINE__).MakeMessage args );  KY_MACRO_END
#else
	#define KY_LOG_TEXT(args)     ((void)0)
	#define KY_LOG_MESSAGE(args)  ((void)0)
	#define KY_LOG_WARNING(args)  ((void)0)
	#define KY_LOG_ERROR(args)    ((void)0)
	#define KY_LOG_OUTPUT(args)   ((void)0)

	#define KY_LOG_TEXT_IF(condition, args)     ((void)0)
	#define KY_LOG_MESSAGE_IF(condition, args)  ((void)0)
	#define KY_LOG_WARNING_IF(condition, args)  ((void)0)
	#define KY_LOG_ERROR_IF(condition, args)    ((void)0)
	#define KY_LOG_OUTPUT_IF(condition, args)   ((void)0)

#endif


#endif //Navigation_Logger_H
