// king-spi.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "king-spi.h"

//全局變量 保存 系統 原服務提供者的 實現
WSPPROC_TABLE ProcTable;

int WSPAPI WSPStartup(
	__in   WORD wVersionRequested,
	__out  LPWSPDATA lpWSPData,
	__in   LPWSAPROTOCOL_INFO lpProtocolInfo,
	__in   WSPUPCALLTABLE UpcallTable,
	__out  LPWSPPROC_TABLE lpProcTable
)
{
	KG_INFO("WSPStartup begin");

	try
	{
		packed_catalog_items_t items;
		//沒有 安裝信息
		if(!install_t::is_install(items))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//將 %XXX% 環境變量 解析到路徑
		char path[MAX_PATH] = {0};
		if(!ExpandEnvironmentStringsA(items[0]->item.Path, path, MAX_PATH))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		
		//加載 dll
		HMODULE moudle = LoadLibraryA(path);
		if(!moudle)
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//獲取 初始化 函數
		LPWSPSTARTUP WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(moudle,"WSPStartup");
		if(!WSPStartupFunc)
		{
			FreeLibrary(moudle);
			return WSAEPROVIDERFAILEDINIT;
		}

		int rs = WSPStartupFunc(wVersionRequested,lpWSPData,lpProtocolInfo,UpcallTable,lpProcTable);
		if(rs)
		{
			return rs;
		}


		//保存 原實現
		ProcTable = *lpProcTable;
		
		//要攔截的 實現
		lpProcTable->lpWSPAccept = WSPAccept;
		lpProcTable->lpWSPAddressToString = WSPAddressToString;
		lpProcTable->lpWSPAsyncSelect = WSPAsyncSelect;

		lpProcTable->lpWSPBind = WSPBind;
		lpProcTable->lpWSPCancelBlockingCall = WSPCancelBlockingCall;
		lpProcTable->lpWSPCleanup = WSPCleanup;

		lpProcTable->lpWSPCloseSocket = WSPCloseSocket;
		lpProcTable->lpWSPConnect = WSPConnect;
		lpProcTable->lpWSPDuplicateSocket = WSPDuplicateSocket;

		lpProcTable->lpWSPEnumNetworkEvents = WSPEnumNetworkEvents;
		lpProcTable->lpWSPEventSelect = WSPEventSelect;
		lpProcTable->lpWSPGetOverlappedResult = WSPGetOverlappedResult;

		lpProcTable->lpWSPGetPeerName = WSPGetPeerName;
		lpProcTable->lpWSPGetSockName = WSPGetSockName;
		lpProcTable->lpWSPGetSockOpt = WSPGetSockOpt;

		lpProcTable->lpWSPGetQOSByName = WSPGetQOSByName;
		lpProcTable->lpWSPIoctl = WSPIoctl;
		lpProcTable->lpWSPJoinLeaf = WSPJoinLeaf;

		lpProcTable->lpWSPListen = WSPListen;
		lpProcTable->lpWSPRecv = WSPRecv;
		lpProcTable->lpWSPRecvDisconnect = WSPRecvDisconnect;

		lpProcTable->lpWSPRecvFrom = WSPRecvFrom;
		lpProcTable->lpWSPSelect = WSPSelect;
		lpProcTable->lpWSPSend = WSPSend;

		lpProcTable->lpWSPSendDisconnect = WSPSendDisconnect;
		lpProcTable->lpWSPSendTo = WSPSendTo;
		lpProcTable->lpWSPSetSockOpt = WSPSetSockOpt;

		lpProcTable->lpWSPShutdown = WSPShutdown;
		lpProcTable->lpWSPSocket = WSPSocket;
		lpProcTable->lpWSPStringToAddress = WSPStringToAddress;

		KG_INFO("WSPStartup success");
	}
	catch(const boost::system::system_error&)
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	return 0;
}
SOCKET WSPAPI WSPAccept(
  __in     SOCKET s,
  __out    struct sockaddr* addr,
  __inout  LPINT addrlen,
  __in     LPCONDITIONPROC lpfnCondition,
  __in     DWORD dwCallbackData,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPAccept");
	return ProcTable.lpWSPAccept(s,addr,addrlen,lpfnCondition,dwCallbackData,lpErrno);
}

int WSPAPI WSPAddressToString(
  __in     LPSOCKADDR lpsaAddress,
  __in     DWORD dwAddressLength,
  __in     LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out    LPWSTR lpszAddressString,
  __inout  LPDWORD lpdwAddressStringLength,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPAddressToString");
	return ProcTable.lpWSPAddressToString(lpsaAddress
		,dwAddressLength
		,lpProtocolInfo
		,lpszAddressString
		,lpdwAddressStringLength
		,lpErrno
		);
}

int WSPAPI WSPAsyncSelect(
  __in   SOCKET s,
  __in   HWND hWnd,
  __in   unsigned int wMsg,
  __in   long lEvent,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPAsyncSelect");
	return ProcTable.lpWSPAsyncSelect(s
		,hWnd
		,wMsg
		,lEvent
		,lpErrno
		);
}

int WSPAPI WSPBind(
  __in   SOCKET s,
  __in   const struct sockaddr* name,
  __in   int namelen,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPBind");
	return ProcTable.lpWSPBind(s
		,name
		,namelen
		,lpErrno);
}

int WSPAPI WSPCancelBlockingCall(
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPCancelBlockingCall");
	return ProcTable.lpWSPCancelBlockingCall(lpErrno);
}
int WSPAPI WSPCleanup(
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPCleanup");
	return ProcTable.lpWSPCleanup(lpErrno);
}
int WSPAPI WSPCloseSocket(
  __in   SOCKET s,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPCloseSocket");
	return ProcTable.lpWSPCloseSocket(s,lpErrno);
}
int WSPAPI WSPConnect(
  __in   SOCKET s,
  __in   const struct sockaddr* name,
  __in   int namelen,
  __in   LPWSABUF lpCallerData,
  __out  LPWSABUF lpCalleeData,
  __in   LPQOS lpSQOS,
  __in   LPQOS lpGQOS,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPConnect");
	return ProcTable.lpWSPConnect(s
		,name
		,namelen
		,lpCallerData
		,lpCalleeData
		,lpSQOS
		,lpGQOS
		,lpErrno
		);
}
int WSPAPI WSPDuplicateSocket(
  __in   SOCKET s,
  __in   DWORD dwProcessId,
  __out  LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPDuplicateSocket");
	return ProcTable.lpWSPDuplicateSocket(s
		,dwProcessId
		,lpProtocolInfo
		,lpErrno
		);
}
int WSPAPI WSPEnumNetworkEvents(
  __in   SOCKET s,
  __in   WSAEVENT hEventObject,
  __out  LPWSANETWORKEVENTS lpNetworkEvents,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPEnumNetworkEvents");
	return ProcTable.lpWSPEnumNetworkEvents(s
		,hEventObject
		,lpNetworkEvents
		,lpErrno
		);
}
int WSPAPI WSPEventSelect(
  __in   SOCKET s,
  __in   WSAEVENT hEventObject,
  __in   long lNetworkEvents,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPEventSelect");
	return ProcTable.lpWSPEventSelect(s
		,hEventObject
		,lNetworkEvents
		,lpErrno
		);
}
BOOL WSPAPI WSPGetOverlappedResult(
  __in   SOCKET s,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __out  LPDWORD lpcbTransfer,
  __in   BOOL fWait,
  __out  LPDWORD lpdwFlags,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPGetOverlappedResult");
	return ProcTable.lpWSPGetOverlappedResult(s
		,lpOverlapped
		,lpcbTransfer
		,fWait
		,lpdwFlags
		,lpErrno
		);
}
int WSPAPI WSPGetPeerName(
  __in     SOCKET s,
  __out    struct sockaddr* name,
  __inout  LPINT namelen,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPGetPeerName");
	return ProcTable.lpWSPGetPeerName(s
		,name
		,namelen
		,lpErrno
		);
}
int WSPAPI WSPGetSockName(
  __in     SOCKET s,
  __out    struct sockaddr* name,
  __inout  LPINT namelen,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPGetSockName");
	return ProcTable.lpWSPGetSockName(s
		,name
		,namelen
		,lpErrno
		);
}
int WSPAPI WSPGetSockOpt(
  __in     SOCKET s,
  __in     int level,
  __in     int optname,
  __out    char* optval,
  __inout  LPINT optlen,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPGetSockOpt");
	return ProcTable.lpWSPGetSockOpt(s
		,level
		,optname
		,optval
		,optlen
		,lpErrno
		);
}
BOOL WSPAPI WSPGetQOSByName(
  __in     SOCKET s,
  __inout  LPWSABUF lpQOSName,
  __out    LPQOS lpQOS,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPGetQOSByName");
	return ProcTable.lpWSPGetQOSByName(s
		,lpQOSName
		,lpQOS
		,lpErrno
		);
}

int WSPAPI WSPIoctl(
  __in   SOCKET s,
  __in   DWORD dwIoControlCode,
  __in   LPVOID lpvInBuffer,
  __in   DWORD cbInBuffer,
  __out  LPVOID lpvOutBuffer,
  __in   DWORD cbOutBuffer,
  __out  LPDWORD lpcbBytesReturned,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPIoctl");
	return ProcTable.lpWSPIoctl(s
		,dwIoControlCode
		,lpvInBuffer
		,cbInBuffer
		,lpvOutBuffer
		,cbOutBuffer
		,lpcbBytesReturned
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
SOCKET WSPAPI WSPJoinLeaf(
  __in   SOCKET s,
  __in   const struct sockaddr* name,
  __in   int namelen,
  __in   LPWSABUF lpCallerData,
  __out  LPWSABUF lpCalleeData,
  __in   LPQOS lpSQOS,
  __in   LPQOS lpGQOS,
  __in   DWORD dwFlags,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPJoinLeaf");
	return ProcTable.lpWSPJoinLeaf(s
		,name
		,namelen
		,lpCallerData
		,lpCalleeData
		,lpSQOS
		,lpGQOS
		,dwFlags
		,lpErrno
		);
}
int WSPAPI WSPListen(
  __in   SOCKET s,
  __in   int backlog,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPListen");
	return ProcTable.lpWSPListen(s
		,backlog
		,lpErrno
		);
}
int WSPAPI WSPRecv(
  __in     SOCKET s,
  __inout  LPWSABUF lpBuffers,
  __in     DWORD dwBufferCount,
  __out    LPDWORD lpNumberOfBytesRecvd,
  __inout  LPDWORD lpFlags,
  __in     LPWSAOVERLAPPED lpOverlapped,
  __in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in     LPWSATHREADID lpThreadId,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPRecv");
	return ProcTable.lpWSPRecv(s
		,lpBuffers
		,dwBufferCount
		,lpNumberOfBytesRecvd
		,lpFlags
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
int WSPAPI WSPRecvDisconnect(
  __in   SOCKET s,
  __out  LPWSABUF lpInboundDisconnectData,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPRecvDisconnect");
	return ProcTable.lpWSPRecvDisconnect(s
		,lpInboundDisconnectData
		,lpErrno
		);
}
int WSPAPI WSPRecvFrom(
  __in     SOCKET s,
  __inout  LPWSABUF lpBuffers,
  __in     DWORD dwBufferCount,
  __out    LPDWORD lpNumberOfBytesRecvd,
  __inout  LPDWORD lpFlags,
  __out    struct sockaddr* lpFrom,
  __inout  LPINT lpFromlen,
  __in     LPWSAOVERLAPPED lpOverlapped,
  __in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in     LPWSATHREADID lpThreadId,
  __inout  LPINT lpErrno
)
{
	KG_INFO("WSPRecvFrom");
	return ProcTable.lpWSPRecvFrom(s
		,lpBuffers
		,dwBufferCount
		,lpNumberOfBytesRecvd
		,lpFlags
		,lpFrom
		,lpFromlen
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
int WSPAPI WSPSelect(
  __in     int nfds,
  __inout  fd_set* readfds,
  __inout  fd_set* writefds,
  __inout  fd_set* exceptfds,
  __in     const struct timeval* timeout,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPSelect");
	return ProcTable.lpWSPSelect(nfds
		,readfds
		,writefds
		,exceptfds
		,timeout
		,lpErrno
		);
}
int WSPAPI WSPSend(
  __in   SOCKET s,
  __in   LPWSABUF lpBuffers,
  __in   DWORD dwBufferCount,
  __out  LPDWORD lpNumberOfBytesSent,
  __in   DWORD dwFlags,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPSend");
	return ProcTable.lpWSPSend(s
		,lpBuffers
		,dwBufferCount
		,lpNumberOfBytesSent
		,dwFlags
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
int WSPAPI WSPSendDisconnect(
  __in   SOCKET s,
  __in   LPWSABUF lpOutboundDisconnectData,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPSendDisconnect");
	return ProcTable.lpWSPSendDisconnect(s
		,lpOutboundDisconnectData
		,lpErrno
		);
}
int WSPAPI WSPSendTo(
  __in   SOCKET s,
  __in   LPWSABUF lpBuffers,
  __in   DWORD dwBufferCount,
  __out  LPDWORD lpNumberOfBytesSent,
  __in   DWORD dwFlags,
  __in   const struct sockaddr* lpTo,
  __in   int iTolen,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPSendTo");
	return ProcTable.lpWSPSendTo(s
		,lpBuffers
		,dwBufferCount
		,lpNumberOfBytesSent
		,dwFlags
		,lpTo
		,iTolen
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
int WSPAPI WSPSetSockOpt(
  __in   SOCKET s,
  __in   int level,
  __in   int optname,
  __in   const char* optval,
  __in   int optlen,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPSetSockOpt");
	return ProcTable.lpWSPSetSockOpt(s
		,level
		,optname
		,optval
		,optlen
		,lpErrno
		);
}
int WSPAPI WSPShutdown(
  __in   SOCKET s,
  __in   int how,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPShutdown");
	return ProcTable.lpWSPShutdown(s
		,how
		,lpErrno
		);
}
SOCKET WSPAPI WSPSocket(
  __in   int af,
  __in   int type,
  __in   int protocol,
  __in   LPWSAPROTOCOL_INFO lpProtocolInfo,
  __in   GROUP g,
         DWORD dwFlags,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPSocket");
	return ProcTable.lpWSPSocket(af
		,type
		,protocol
		,lpProtocolInfo
		,g
		,dwFlags
		,lpErrno
		);
}
int WSPAPI WSPStringToAddress(
  __in     LPWSTR AddressString,
  __in     INT AddressFamily,
  __in     LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out    LPSOCKADDR lpAddress,
  __inout  LPINT lpAddressLength,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPStringToAddress");
	return ProcTable.lpWSPStringToAddress(AddressString
		,AddressFamily
		,lpProtocolInfo
		,lpAddress
		,lpAddressLength
		,lpErrno
		);
}