# Microsoft Developer Studio Project File - Name="Minia_Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Minia_Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Minia_Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Minia_Server.mak" CFG="Minia_Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Minia_Server - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Minia_Server - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DirectGroup/전략제품개발팀/EagleHorn/Src/HsUtil/AntiCpSvr/Minia_Server", BLOCBAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Minia_Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\External\Inc" /I "..\NetworkLib" /I "..\..\Include" /I "..\..\OptionPack\AntiCpXSvr" /I "..\NetSvMngr\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Advapi32.lib Ws2_32.lib NetSvMngr.lib AntiCpXSvr.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"..\..\..\Build\Release" /libpath:"..\..\External\Lib" /libpath:"..\NetworkLib" /libpath:"..\..\Lib\Win\x86\Multithreaded\AntiCrack" /libpath:"..\..\Lib\Win\x86\Multithreaded DLL\AntiCrack"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=IF NOT EXIST ..\..\..\Build MD ..\..\..\Build	IF NOT EXIST ..\..\..\Build\Release MD ..\..\..\Build\Release	copy .\Release\Minia_Server.exe ..\..\..\Build\Release	copy .\Release\Minia_Server.map ..\..\..\Build\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Minia_Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\External\Inc" /I "..\NetworkLib" /I "..\..\Include" /I "..\..\OptionPack\AntiCpXSvr" /I "..\NetSvMngr\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Advapi32.lib Ws2_32.lib NetSvMngr.lib AntiCpXSvr.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\Build\Debug" /libpath:"..\..\External\Lib" /libpath:"..\NetworkLib" /libpath:"..\..\Lib\Win\x86\Multithreaded\AntiCrack" /libpath:"..\..\Lib\Win\x86\Multithreaded DLL\AntiCrack"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=IF NOT EXIST ..\..\..\Build MD ..\..\..\Build	IF NOT EXIST ..\..\..\Build\Debug MD ..\..\..\Build\Debug	copy .\Debug\Minia_Server.exe ..\..\..\Build\Debug	copy .\Debug\Minia_Server.pdb ..\..\..\Build\Debug	copy .\Debug\Minia_Server.map ..\..\..\Build\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Minia_Server - Win32 Release"
# Name "Minia_Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Minia_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Minia_Server.rc
# End Source File
# Begin Source File

SOURCE=.\Minia_Server_Op.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Minia_Server_Op.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\MiniA_Server.exe.manifest
# End Source File
# End Target
# End Project
