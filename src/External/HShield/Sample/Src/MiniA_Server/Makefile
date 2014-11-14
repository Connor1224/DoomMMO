CPPFLAGS= -DUNIX -DNOANTICPEX
CFLAGS= -fPIC -Wall -W -O2 -g
CXXFLAGS= -g -Wall -W
LDFLAGS= -L../../OptionPack/AntiCpSvr
LDDADD= -lpthread $(LIBANTICPSVR) -Wl,-rpath,/usr/local/lib -Wl,-rpath,../../OptionPack/AntiCpSvr -Wl,-rpath,.

.c.o .C.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $*.o
.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

OBJS= \
      UnixMain.o \
      ACPSvcOp.o \
      NetProcessingUnix.o

all: 
	@case `gcc -dumpmachine` in \
	x86_64*) make acpsvc LIBANTICPSVR=-lanticpsvr64 ;;\
	*) make acpsvc LIBANTICPSVR=-lanticpsvr ;; \
	esac

acpsvc: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDDADD)
	
clean:
	rm -f $(OBJS)

ACPSvc.o: ACPSvc.cpp ACPSvc.h
ACPSvcOp.o: ACPSvc.cpp ACPSvcOp.h

