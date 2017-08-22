#include "stdafx.h"
#include "log.h"
#define LOG_FILE	"d:/logs/log.txt"
void write_log(const char* msg)
{
	std::ofstream f(LOG_FILE,std::ios::app | std::ios::out);
	if(!f.is_open())
	{
		return;
	}
	f<<GetCurrentProcessId()<<"	"<<msg<<"\r\n";
}