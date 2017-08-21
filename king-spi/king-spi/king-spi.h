#pragma once

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
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
SOCKET WSPAPI WSPAccept(
  __in     SOCKET s,
  __out    struct sockaddr* addr,
  __inout  LPINT addrlen,
  __in     LPCONDITIONPROC lpfnCondition,
  __in     DWORD dwCallbackData,
  __out    LPINT lpErrno
);
