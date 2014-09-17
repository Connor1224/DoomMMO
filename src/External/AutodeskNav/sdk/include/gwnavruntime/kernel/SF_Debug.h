/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

/**************************************************************************

PublicHeader:   Kernel
Filename    :   KY_Debug.h
Content     :   General purpose debugging support
Created     :   July 18, 2001
Authors     :   Brendan Iribe, Michael Antonov

Notes       :   Debug warning functionality is enabled
                if and only if KY_BUILD_DEBUG is defined.

**************************************************************************/

#ifndef INC_KY_Kernel_Debug_H
#define INC_KY_Kernel_Debug_H

#include "gwnavruntime/kernel/SF_Config.h"

// If not in debug build, macros do nothing
#if !defined(KY_BUILD_DEBUG)
# define KY_NO_DEBUG_OUTPUT
#endif

#ifndef KY_NO_DEBUG_OUTPUT

    #include "gwnavruntime/kernel/SF_Types.h"
    #include "gwnavruntime/kernel/SF_Log.h"

    namespace Kaim {

    void LogDebugMessage(const LogMessageHelper& helper);

    }


    // Simple output with prefix tag
    #define KY_DEBUG_OUTPUTN(msgtype, msg)                                 LogDebugMessage( Kaim::LogMessageHelper(msgtype, __FILE__, __LINE__).MakeMessage msg );
    #define KY_DEBUG_OUTPUTN_IF(cond, msgtype, msg)                        do { if (cond) { KY_DEBUG_OUTPUTN(msgtype, msg); } } while (0)

    #define KY_DEBUG_OUTPUT(cond, msgtype, str)                            KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str))
    #define KY_DEBUG_OUTPUT1(cond, msgtype, str, p1)                       KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1))
    #define KY_DEBUG_OUTPUT2(cond, msgtype, str, p1, p2)                   KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1, p2))
    #define KY_DEBUG_OUTPUT3(cond, msgtype, str, p1, p2, p3)               KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1, p2, p3))
    #define KY_DEBUG_OUTPUT4(cond, msgtype, str, p1, p2, p3, p4)           KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1, p2, p3, p4))
    #define KY_DEBUG_OUTPUT5(cond, msgtype, str, p1, p2, p3, p4, p5)       KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1, p2, p3, p4, p5))
    #define KY_DEBUG_OUTPUT6(cond, msgtype, str, p1, p2, p3, p4, p5, p6)   KY_DEBUG_OUTPUTN_IF(cond, msgtype, (str, p1, p2, p3, p4, p5, p6))

    // Assertion with messages
    // Unlike KY_ASSERT, these macros display a message before breaking
    #define KY_DEBUG_ASSERTN(cond, msg)                                    do { if (!(cond)) { KY_DEBUG_OUTPUTN(Kaim::Log_DebugAssert, msg); KY_DEBUG_BREAK; } } while (0)                           

    #define KY_DEBUG_ASSERT(cond, str)                                     KY_DEBUG_ASSERTN(cond, (str))
    #define KY_DEBUG_ASSERT1(cond, str, p1)                                KY_DEBUG_ASSERTN(cond, (str, p1))
    #define KY_DEBUG_ASSERT2(cond, str, p1, p2)                            KY_DEBUG_ASSERTN(cond, (str, p1, p2))
    #define KY_DEBUG_ASSERT3(cond, str, p1, p2, p3)                        KY_DEBUG_ASSERTN(cond, (str, p1, p2, p3))
    #define KY_DEBUG_ASSERT4(cond, str, p1, p2, p3, p4)                    KY_DEBUG_ASSERTN(cond, (str, p1, p2, p3, p4))
    #define KY_DEBUG_ASSERT5(cond, str, p1, p2, p3, p4, p5)                KY_DEBUG_ASSERTN(cond, (str, p1, p2, p3, p4, p5))
    #define KY_DEBUG_ASSERT6(cond, str, p1, p2, p3, p4, p5, p6)            KY_DEBUG_ASSERTN(cond, (str, p1, p2, p3, p4, p5, p6))


#else 

    // Simple output with prefix tag                                        
    #define KY_DEBUG_OUTPUTN(msgtype, msg)                                 ((void)0)
    #define KY_DEBUG_OUTPUTN_IF(cond, msgtype, msg)                        ((void)0)
    #define KY_DEBUG_OUTPUT(cond, msgtype, str )                           ((void)0)
    #define KY_DEBUG_OUTPUT1(cond, msgtype, str, p1)                       ((void)0)
    #define KY_DEBUG_OUTPUT2(cond, msgtype, str, p1, p2)                   ((void)0)
    #define KY_DEBUG_OUTPUT3(cond, msgtype, str, p1, p2, p3)               ((void)0)
    #define KY_DEBUG_OUTPUT4(cond, msgtype, str, p1, p2, p3, p4)           ((void)0)
    #define KY_DEBUG_OUTPUT5(cond, msgtype, str, p1, p2, p3, p4, p5)       ((void)0)
    #define KY_DEBUG_OUTPUT6(cond, msgtype, str, p1, p2, p3, p4, p5, p6)   ((void)0)

    // Assertion with messages
    // Unlike KY_ASSERT, these macros display a message before breaking
    #define KY_DEBUG_ASSERTN(cond, msg)                                    ((void)0)
    #define KY_DEBUG_ASSERT(cond, str)                                     ((void)0)
    #define KY_DEBUG_ASSERT1(cond, str, p1)                                ((void)0)
    #define KY_DEBUG_ASSERT2(cond, str, p1, p2)                            ((void)0)
    #define KY_DEBUG_ASSERT3(cond, str, p1, p2, p3)                        ((void)0)
    #define KY_DEBUG_ASSERT4(cond, str, p1, p2, p3, p4)                    ((void)0)
    #define KY_DEBUG_ASSERT5(cond, str, p1, p2, p3, p4, p5)                ((void)0)
    #define KY_DEBUG_ASSERT6(cond, str, p1, p2, p3, p4, p5, p6)            ((void)0)

#endif // KY_BUILD_DEBUG

// Conditional warnings - "SF Warning: " prefix
#define KY_DEBUG_WARNINGN(msg)                                 KY_DEBUG_OUTPUTN(Kaim::Log_DebugWarning, msg)
#define KY_DEBUG_WARNINGN_IF(cond, msg)                        KY_DEBUG_OUTPUTN_IF(cond, Kaim::Log_DebugWarning, msg)
#define KY_DEBUG_WARNING(cond, str)                            KY_DEBUG_OUTPUT(cond,  Kaim::Log_DebugWarning, str)
#define KY_DEBUG_WARNING1(cond, str, p1)                       KY_DEBUG_OUTPUT1(cond, Kaim::Log_DebugWarning, str,  p1)
#define KY_DEBUG_WARNING2(cond, str, p1, p2)                   KY_DEBUG_OUTPUT2(cond, Kaim::Log_DebugWarning, str,  p1,p2)
#define KY_DEBUG_WARNING3(cond, str, p1, p2, p3)               KY_DEBUG_OUTPUT3(cond, Kaim::Log_DebugWarning, str,  p1,p2,p3)
#define KY_DEBUG_WARNING4(cond, str, p1, p2, p3, p4)           KY_DEBUG_OUTPUT4(cond, Kaim::Log_DebugWarning, str,  p1,p2,p3,p4)
#define KY_DEBUG_WARNING5(cond, str, p1, p2, p3, p4, p5)       KY_DEBUG_OUTPUT5(cond, Kaim::Log_DebugWarning, str,  p1,p2,p3,p4,p5)
#define KY_DEBUG_WARNING6(cond, str, p1, p2, p3, p4, p5, p6)   KY_DEBUG_OUTPUT6(cond, Kaim::Log_DebugWarning, str,  p1,p2,p3,p4,p5,p6)
// Conditional errors - "SF Error: " prefix
#define KY_DEBUG_ERRORN(msg)                                   KY_DEBUG_OUTPUTN(Kaim::Log_DebugError, msg)
#define KY_DEBUG_ERRORN_IF(cond, msg)                          KY_DEBUG_OUTPUTN_IF(cond, Kaim::Log_DebugError, msg)
#define KY_DEBUG_ERROR(cond, str)                              KY_DEBUG_OUTPUT(cond,  Kaim::Log_DebugError, str)
#define KY_DEBUG_ERROR1(cond, str, p1)                         KY_DEBUG_OUTPUT1(cond, Kaim::Log_DebugError, str,  p1)
#define KY_DEBUG_ERROR2(cond, str, p1, p2)                     KY_DEBUG_OUTPUT2(cond, Kaim::Log_DebugError, str,  p1,p2)
#define KY_DEBUG_ERROR3(cond, str, p1, p2, p3)                 KY_DEBUG_OUTPUT3(cond, Kaim::Log_DebugError, str,  p1,p2,p3)
#define KY_DEBUG_ERROR4(cond, str, p1, p2, p3, p4)             KY_DEBUG_OUTPUT4(cond, Kaim::Log_DebugError, str,  p1,p2,p3,p4)
#define KY_DEBUG_ERROR5(cond, str, p1, p2, p3, p4, p5)         KY_DEBUG_OUTPUT5(cond, Kaim::Log_DebugError, str,  p1,p2,p3,p4,p5)
#define KY_DEBUG_ERROR6(cond, str, p1, p2, p3, p4, p5, p6)     KY_DEBUG_OUTPUT6(cond, Kaim::Log_DebugError, str,  p1,p2,p3,p4,p5,p6)
// Conditional messages - no prefix
#define KY_DEBUG_MESSAGEN(msg)                                 KY_DEBUG_OUTPUTN(Kaim::Log_DebugMessage, msg)
#define KY_DEBUG_MESSAGEN_IF(cond, msg)                        KY_DEBUG_OUTPUTN_IF(cond, Kaim::Log_DebugMessage, msg)
#define KY_DEBUG_MESSAGE(cond, str)                            KY_DEBUG_OUTPUT(cond,  Kaim::Log_DebugMessage, str)
#define KY_DEBUG_MESSAGE1(cond, str, p1)                       KY_DEBUG_OUTPUT1(cond, Kaim::Log_DebugMessage, str,  p1)
#define KY_DEBUG_MESSAGE2(cond, str, p1, p2)                   KY_DEBUG_OUTPUT2(cond, Kaim::Log_DebugMessage, str,  p1,p2)
#define KY_DEBUG_MESSAGE3(cond, str, p1, p2, p3)               KY_DEBUG_OUTPUT3(cond, Kaim::Log_DebugMessage, str,  p1,p2,p3)
#define KY_DEBUG_MESSAGE4(cond, str, p1, p2, p3, p4)           KY_DEBUG_OUTPUT4(cond, Kaim::Log_DebugMessage, str,  p1,p2,p3,p4)
#define KY_DEBUG_MESSAGE5(cond, str, p1, p2, p3, p4, p5)       KY_DEBUG_OUTPUT5(cond, Kaim::Log_DebugMessage, str,  p1,p2,p3,p4,p5)
#define KY_DEBUG_MESSAGE6(cond, str, p1, p2, p3, p4, p5, p6)   KY_DEBUG_OUTPUT6(cond, Kaim::Log_DebugMessage, str,  p1,p2,p3,p4,p5,p6)
// Conditional messages with '\n' - no prefix
#define KY_DEBUG_MESSAGE_LINEN(msg)                                 KY_DEBUG_OUTPUTN(Kaim::Log_DebugMessageLine, msg)
#define KY_DEBUG_MESSAGE_LINEN_IF(cond, msg)                        KY_DEBUG_OUTPUTN_IF(cond, Kaim::Log_DebugMessageLine, msg)
#define KY_DEBUG_MESSAGE_LINE(cond, str)                            KY_DEBUG_OUTPUT(cond,  Kaim::Log_DebugMessageLine, str)
#define KY_DEBUG_MESSAGE_LINE1(cond, str, p1)                       KY_DEBUG_OUTPUT1(cond, Kaim::Log_DebugMessageLine, str,  p1)
#define KY_DEBUG_MESSAGE_LINE2(cond, str, p1, p2)                   KY_DEBUG_OUTPUT2(cond, Kaim::Log_DebugMessageLine, str,  p1,p2)
#define KY_DEBUG_MESSAGE_LINE3(cond, str, p1, p2, p3)               KY_DEBUG_OUTPUT3(cond, Kaim::Log_DebugMessageLine, str,  p1,p2,p3)
#define KY_DEBUG_MESSAGE_LINE4(cond, str, p1, p2, p3, p4)           KY_DEBUG_OUTPUT4(cond, Kaim::Log_DebugMessageLine, str,  p1,p2,p3,p4)
#define KY_DEBUG_MESSAGE_LINE5(cond, str, p1, p2, p3, p4, p5)       KY_DEBUG_OUTPUT5(cond, Kaim::Log_DebugMessageLine, str,  p1,p2,p3,p4,p5)
#define KY_DEBUG_MESSAGE_LINE6(cond, str, p1, p2, p3, p4, p5, p6)   KY_DEBUG_OUTPUT6(cond, Kaim::Log_DebugMessageLine, str,  p1,p2,p3,p4,p5,p6)

// Convenient simple type value object output
// - v must be a stack object
#define KY_DEBUG_SINT(v)                                       KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%d)",   int (v))
#define KY_DEBUG_UINT(v)                                       KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%u)",   unsigned(v))
#define KY_DEBUG_HEX(v)                                        KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(0x%X)", unsigned(v))
#define KY_DEBUG_FLOAT(v)                                      KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%f)",   Kaim::Double(v))
#define KY_DEBUG_DOUBLE(v)                                     KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%f)",   Kaim::Double(v))
#define KY_DEBUG_CHAR(v)                                       KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%c)",   char(v))
#define KY_DEBUG_WCHAR(v)                                      KY_DEBUG_OUTPUT1(1, Kaim::Log_DebugMessage, #v "(%lc)",   wchar_t(v))


#endif
