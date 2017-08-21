// king-spi.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "king-spi.h"

//ȫ��׃�� ���� ϵ�y ԭ�����ṩ�ߵ� ���F
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
		//�]�� ���b��Ϣ
		if(!install_t::is_install(wcs,item))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//�� %XXX% �h��׃�� ������·��
		char path[MAX_PATH] = {0};
		if(!ExpandEnvironmentStringsA(item.Path, path, MAX_PATH))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		
		//���d dll
		HMODULE moudle = LoadLibraryA(path);
		if(!moudle)
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//�@ȡ ��ʼ�� ����
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


		//���� ԭ���F
		ProcTable = *lpProcTable;

		//Ҫ�r�ص� ���F
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