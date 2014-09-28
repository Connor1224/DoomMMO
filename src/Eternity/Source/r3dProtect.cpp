#include "r3dPCH.h"
#include "r3dProtect.h"

#define PublicsymbolsFile()   namespace { char archivemember##__LINE__; }
PublicsymbolsFile()
#if DEBUG_OPTION
      // code to debug the warning LNK4221
#endif // DEBUG_OPTION