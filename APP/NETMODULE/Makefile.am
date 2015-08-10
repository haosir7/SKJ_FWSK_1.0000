#AWE-4000R Project

noinst_LIBRARIES = libnetmodule.a

libnetmodule_a_SOURCES = \
./gui_net/NetManageMenu.cpp  ./gui_net/ADSLInfoSetWin.cpp ./gui_net/ApnAccPointSetWin.cpp ./gui_net/LocalNetworkMenu.cpp ./gui_net/M2MManageMenu.cpp \
./gui_net/Net3GWlenMenu.cpp ./gui_net/Net3GWlenSetWin.cpp ./gui_net/NetIPModelWin.cpp ./gui_net/NetModelEditWin.cpp ./gui_net/NetModeSetMenu.cpp \
./gui_net/NetServerSetWin.cpp ./gui_net/NetServMenu.cpp ./gui_net/NetWlenCardWin.cpp ./gui_net/NetworkingSetWin.cpp ./gui_net/NetworkingShowWin.cpp \
./gui_net/NetworkSetMenu.cpp ./gui_net/VPNInfoSetWin.cpp ./gui_net/WirelessInfoWin.cpp ./gui_net/WirelessViewWin.cpp ./gui_net/NetAutoDialSetWin.cpp \
netManager.cpp transfers_base.cpp trans_CURL.cpp trans_http.cpp trans_direct.cpp trans_abstrac.cpp SendBySocket.cpp SendByWmmp.cpp Sending.cpp \
LANConnection.cpp ADSLConnection.cpp 3GConnection.cpp M2M_APN_Connection.cpp M2M_WMMP_Connection.cpp M2M_Common.cpp VPNConnection.cpp DoNothing.cpp CommonCheck.cpp CommonGetIPinfo.cpp netConnection.cpp netdbWork.cpp \
./proc/MycUrlHttp.cpp ./proc/socketClient.c ./proc/adslProc.cpp ./proc/vpnProc.c ./proc/ipInfoProc.c ./proc/setToFileProc.c ./proc/command_at.c ./proc/wmmp.c ./proc/drv_modem.c ./proc/decode.c


#libnetmodule_a_LIBADD = ./proc/libnetproc.a

#SUBDIRS = proc

INCLUDES = -I../../SYS -I../../TPL -I../../GUI -I../../DB -I../../LIB -I../../DRV   -I./proc -I./gui_net -I../ -I./ 
