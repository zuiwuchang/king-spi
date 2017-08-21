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
	std::ofstream f("c:/log.txt",std::ios::binary | std::ios::app | std::ios::out);
	if(f.is_open())
	{
		std::string str = "WSPStartup\r\n";
		f.write(str.data(),str.size());
	}

	try
	{
		std::wstring wcs;
		PackedCatalogItem item;
		//沒有 安裝信息
		if(!install_t::is_install(wcs,item))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//將 %XXX% 環境變量 解析到路徑
		char path[MAX_PATH] = {0};
		if(!ExpandEnvironmentStringsA(item.Path, path, MAX_PATH))
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
		lpProcTable->lpWSPSocket = WSPSocket;
		lpProcTable->lpWSPAccept = WSPAccept;
	}
	catch(const boost::system::system_error&)
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	return 0;
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
	std::ofstream f("c:/log.txt",std::ios::binary | std::ios::app | std::ios::out);
	if(f.is_open())
	{
		std::string str = "WSPSocket\r\n";
		f.write(str.data(),str.size());
	}
	return ProcTable.lpWSPSocket(af
		,type
		,protocol
		,lpProtocolInfo
		,g
		,dwFlags
		,lpErrno);
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
	std::ofstream f("c:/log.txt",std::ios::binary | std::ios::app | std::ios::out);
	if(f.is_open())
	{
		std::string str = "WSPAccept\r\n";
		f.write(str.data(),str.size());
	}
	return ProcTable.lpWSPAccept(s,addr,addrlen,lpfnCondition,dwCallbackData,lpErrno);
}