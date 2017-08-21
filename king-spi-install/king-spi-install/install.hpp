#ifndef KING_INSTALL_HEADER_HPP
#define KING_INSTALL_HEADER_HPP

#define KING_SPI_DLL		"king-spi.dll"
#define KING_INSTALL_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\King_Winsock_Spi"
#define KING_INSTALL_NAME	L"Name"
#define KING_INSTALL_ITEM	L"Item"
#define	KING_CATALOG_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries"
#define KING_CATALOG_ITEM	L"PackedCatalogItem"


CREATE_CATEGORY(install,"install");

//安裝信息
struct install_info
{
	//原 基礎服務提供者 在註冊表中的 key 名稱
	std::wstring name;
	//原 基礎服務提供者 dll 全路徑
	std::wstring path;
};
//註冊表 安裝 卸載 封裝
class install_t
{
private:
	install_t();
	~install_t();
	class scoped_hkey
		:boost::noncopyable
	{
	private:
		HKEY _key;
	public:
		scoped_hkey(HKEY key):_key(key)
		{
		}
		~scoped_hkey()
		{
			if(_key)
			{
				RegCloseKey(_key);
			}
		}
	};
	struct PackedCatalogItem
	{
		char Path[MAX_PATH];
		WSAPROTOCOL_INFO Info;
	};
	//返回 原 基礎服務 提供者 key 和 PackedCatalogItem
	static void basic_provider(std::wstring& name,PackedCatalogItem& item)
	{
		HKEY key;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,KING_CATALOG_KEY,0,KEY_READ,&key))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open catalog key")));
		}
		scoped_hkey lKey(key);

		//enum
		DWORD pos = 0;
		wchar_t keyName[MAX_PATH];
		while(RegEnumKey(key,pos,keyName, MAX_PATH) == ERROR_SUCCESS)
		{
			if(basic_provider(key,keyName,item))
			{
				name = keyName;
				return;
			}
			pos++;
		}

		BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad find basic_provider")));
	}
	static bool basic_provider(HKEY key,wchar_t* keyName,PackedCatalogItem& item)
	{
		HKEY subKey; 
		if(ERROR_SUCCESS != RegOpenKeyEx(key,keyName,0,KEY_READ,&subKey))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open basic_provider")));
		}
		scoped_hkey lKey(subKey);

		DWORD len = sizeof(item);
		if(ERROR_SUCCESS != RegQueryValueEx(subKey,KING_CATALOG_ITEM,0,NULL,(LPBYTE)&item,&len) || len != sizeof(item))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad query basic_provider")));
		}

		WSAPROTOCOL_INFO& info = item.Info;
		if(info.ProtocolChain.ChainLen != 1 //基礎服務提供者
			|| info.iAddressFamily != AF_INET	//驗證 ip 
			|| info.iSocketType != SOCK_STREAM	//tcp
			)
		{
			return false;
		}
		return true;
	}
	static void replace_spi(const std::wstring& name,PackedCatalogItem& item)
	{
		char fileName[MAX_PATH];
		GetModuleFileNameA(NULL,fileName,MAX_PATH);
		boost::filesystem::path path(fileName);
		path = path.parent_path();
		path /= KING_SPI_DLL;

		if(!boost::filesystem::exists(path))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get(std::string(KING_SPI_DLL) + " not found")));
		}

		std::string str = path.string();

		std::wstring keyName(KING_CATALOG_KEY);
		keyName += L"\\" + name;
		HKEY key;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,keyName.c_str(),0,KEY_WRITE,&key))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open provider item")));
		}
		scoped_hkey lKey(key);

		strcpy(item.Path,str.c_str());
		if(ERROR_SUCCESS != RegSetValueEx(key,KING_CATALOG_ITEM,0,REG_BINARY,(const BYTE*)&item,sizeof(item)))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad replace provider item")));
		}
	}
public:
	//安裝
	static void install()
	{
		//如果 已經 安裝 卸載
		uninstall();

		//返回 原 服務 提供者
		std::wstring name;
		PackedCatalogItem item;
		basic_provider(name,item);

		//創建 安裝 記錄信息
		HKEY key;
		if(ERROR_SUCCESS !=RegCreateKeyEx(HKEY_LOCAL_MACHINE
			,KING_INSTALL_KEY
			,0
			,NULL
			,REG_OPTION_VOLATILE
			,KEY_ALL_ACCESS
			,NULL
			,&key
			,NULL
			)
			)
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad create install key")));
		}
		scoped_hkey lKey(key);

		//save name
		if(ERROR_SUCCESS != RegSetValueEx(key,KING_INSTALL_NAME,0,REG_SZ,(const BYTE*)name.c_str(),name.size() * sizeof(wchar_t)))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad save install name")));
		}

		//save item
		if(ERROR_SUCCESS != RegSetValueEx(key,KING_INSTALL_ITEM,0,REG_BINARY,(const BYTE*)&item,sizeof(item)))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad save install path")));
		}

		//替換 原始 spi dll
		replace_spi(name,item);
	}
	//卸載
	static void uninstall()
	{
		//沒有安裝 直接 返回
		std::wstring name;
		PackedCatalogItem item;
		if(!is_install(name,item))
		{
			return;
		}
		std::string path = item.Path;
		if(name.empty() || path.empty())
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad install record")));
		}

		//打開 原key
		std::wstring keyName = KING_CATALOG_KEY;
		keyName += L"\\";
		keyName += name;
		HKEY key;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,keyName.c_str(),0,KEY_ALL_ACCESS,&key))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open provider key")));
		}
		scoped_hkey lKey(key);

		//恢復 key值
		DWORD len = sizeof(item);
		if(ERROR_SUCCESS != RegSetValueEx(key,KING_CATALOG_ITEM,0,REG_BINARY,(const BYTE*)&item,sizeof(item)))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad save provider item")));
		}

		//刪除 安裝 記錄
		RegDeleteKey(HKEY_LOCAL_MACHINE,KING_INSTALL_KEY);
	}
	//返回 是否已經 安裝
	static bool is_install(std::wstring& name,PackedCatalogItem& item)
	{
		HKEY key;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,KING_INSTALL_KEY,0,KEY_READ,&key))
		{
			return false;
		}
		scoped_hkey lKey(key);

		wchar_t cName[MAX_PATH];
		DWORD len = sizeof(wchar_t) * MAX_PATH;
		if(ERROR_SUCCESS != RegQueryValueEx(key,KING_INSTALL_NAME,0,NULL,(LPBYTE)cName,&len))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad query install name")));
		}

		len = sizeof(item);
		if(ERROR_SUCCESS != RegQueryValueEx(key,KING_INSTALL_ITEM,0,NULL,(LPBYTE)&item,&len))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad query install item")));
		}

		name = cName;
		return true;
	}

};

#endif	//KING_INSTALL_HEADER_HPP