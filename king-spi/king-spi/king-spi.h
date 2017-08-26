#pragma once

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
);

SOCKET WSPAPI WSPAccept(
  __in     SOCKET s,
  __out    struct sockaddr* addr,
  __inout  LPINT addrlen,
  __in     LPCONDITIONPROC lpfnCondition,
  __in     DWORD dwCallbackData,
  __out    LPINT lpErrno
);

int WSPAPI WSPAddressToString(
  __in     LPSOCKADDR lpsaAddress,
  __in     DWORD dwAddressLength,
  __in     LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out    LPWSTR lpszAddressString,
  __inout  LPDWORD lpdwAddressStringLength,
  __out    LPINT lpErrno
);

int WSPAPI WSPAsyncSelect(
  __in   SOCKET s,
  __in   HWND hWnd,
  __in   unsigned int wMsg,
  __in   long lEvent,
  __out  LPINT lpErrno
);

int WSPAPI WSPBind(
  __in   SOCKET s,
  __in   const struct sockaddr* name,
  __in   int namelen,
  __out  LPINT lpErrno
);

int WSPAPI WSPCancelBlockingCall(
  __out  LPINT lpErrno
);

int WSPAPI WSPCleanup(
  __out  LPINT lpErrno
);

int WSPAPI WSPCloseSocket(
  __in   SOCKET s,
  __out  LPINT lpErrno
);

int WSPAPI WSPConnect(
  __in   SOCKET s,
  __in   const struct sockaddr* name,
  __in   int namelen,
  __in   LPWSABUF lpCallerData,
  __out  LPWSABUF lpCalleeData,
  __in   LPQOS lpSQOS,
  __in   LPQOS lpGQOS,
  __out  LPINT lpErrno
);

int WSPAPI WSPDuplicateSocket(
  __in   SOCKET s,
  __in   DWORD dwProcessId,
  __out  LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out  LPINT lpErrno
);

int WSPAPI WSPEnumNetworkEvents(
  __in   SOCKET s,
  __in   WSAEVENT hEventObject,
  __out  LPWSANETWORKEVENTS lpNetworkEvents,
  __out  LPINT lpErrno
);
int WSPAPI WSPEventSelect(
  __in   SOCKET s,
  __in   WSAEVENT hEventObject,
  __in   long lNetworkEvents,
  __out  LPINT lpErrno
);

BOOL WSPAPI WSPGetOverlappedResult(
  __in   SOCKET s,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __out  LPDWORD lpcbTransfer,
  __in   BOOL fWait,
  __out  LPDWORD lpdwFlags,
  __out  LPINT lpErrno
);

int WSPAPI WSPGetPeerName(
  __in     SOCKET s,
  __out    struct sockaddr* name,
  __inout  LPINT namelen,
  __out    LPINT lpErrno
);

int WSPAPI WSPGetSockName(
  __in     SOCKET s,
  __out    struct sockaddr* name,
  __inout  LPINT namelen,
  __out    LPINT lpErrno
);

int WSPAPI WSPGetSockOpt(
  __in     SOCKET s,
  __in     int level,
  __in     int optname,
  __out    char* optval,
  __inout  LPINT optlen,
  __out    LPINT lpErrno
);

BOOL WSPAPI WSPGetQOSByName(
  __in     SOCKET s,
  __inout  LPWSABUF lpQOSName,
  __out    LPQOS lpQOS,
  __out    LPINT lpErrno
);

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
);

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
);

int WSPAPI WSPListen(
  __in   SOCKET s,
  __in   int backlog,
  __out  LPINT lpErrno
);
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
);
int WSPAPI WSPRecvDisconnect(
  __in   SOCKET s,
  __out  LPWSABUF lpInboundDisconnectData,
  __out  LPINT lpErrno
);

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
);

int WSPAPI WSPSelect(
  __in     int nfds,
  __inout  fd_set* readfds,
  __inout  fd_set* writefds,
  __inout  fd_set* exceptfds,
  __in     const struct timeval* timeout,
  __out    LPINT lpErrno
);
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
);
int WSPAPI WSPSendDisconnect(
  __in   SOCKET s,
  __in   LPWSABUF lpOutboundDisconnectData,
  __out  LPINT lpErrno
);
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
);
int WSPAPI WSPSetSockOpt(
  __in   SOCKET s,
  __in   int level,
  __in   int optname,
  __in   const char* optval,
  __in   int optlen,
  __out  LPINT lpErrno
);
int WSPAPI WSPShutdown(
  __in   SOCKET s,
  __in   int how,
  __out  LPINT lpErrno
);
SOCKET WSPAPI WSPSocket(
  __in   int af,
  __in   int type,
  __in   int protocol,
  __in   LPWSAPROTOCOL_INFO lpProtocolInfo,
  __in   GROUP g,
         DWORD dwFlags,
  __out  LPINT lpErrno
);
int WSPAPI WSPStringToAddress(
  __in     LPWSTR AddressString,
  __in     INT AddressFamily,
  __in     LPWSAPROTOCOL_INFO lpProtocolInfo,
  __out    LPSOCKADDR lpAddress,
  __inout  LPINT lpAddressLength,
  __out    LPINT lpErrno
);


