#include <base/math.h>
#include <base/system.h>
#include <game/version.h>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <engine/shared/config.h>
#include <engine/external/json-parser/json.h>
#include "geoip.h"

static NETSOCKET invalid_socket = {NETTYPE_INVALID, -1, -1};

static LOCK m_GeoIPLock = 0;
NETADDR CGeoIP::m_HostAddress;
NETSOCKET CGeoIP::m_Socket = invalid_socket;

CGeoIP::CGeoIP()
{
    m_GeoIPLock = lock_create();
    m_pGeoIPThread = 0x0;
    m_Active = true;
}

void CGeoIP::Init()
{
    mem_zero(&m_HostAddress, sizeof(m_HostAddress));

    //Lookup
    if(net_host_lookup("www.telize.com", &m_HostAddress, NETTYPE_IPV4) != 0)
    {
        dbg_msg("GeoIP","ERROR: Can't run host lookup.");
        m_Active = false;
        return;
    }
    m_HostAddress.port = 80;
}

void CGeoIP::Search(InfoGeoIPThread *pGeoInfo)
{
    if (m_pGeoIPThread)
    {
    	net_tcp_close(m_Socket);
        thread_destroy(m_pGeoIPThread);
        m_pGeoIPThread = 0x0;
    }

    m_pGeoIPThread = thread_create(ThreadGeoIP, pGeoInfo);
}

void CGeoIP::GetInfo(std::string ip, IGeoIP::GeoInfo *geoInfo)
{
    dbg_msg("GeoIP", "Searching geolocation of '%s'...", ip.c_str());

    NETADDR bindAddr;
    mem_zero(&bindAddr, sizeof(bindAddr));
    char aNetBuff[1024];
    std::string jsonData;

    //Connect
    bindAddr.type = NETTYPE_IPV4;
    m_Socket = net_tcp_create(bindAddr);
    if(net_tcp_connect(m_Socket, &m_HostAddress) != 0)
    {
        dbg_msg("GeoIP","ERROR: Can't connect.");
        net_tcp_close(m_Socket);
        geoInfo->m_CountryCode = "NULL";
        return;
    }

    //Send request
    str_format(aNetBuff, sizeof(aNetBuff), "GET /geoip/%s HTTP/1.0\r\nHost: www.telize.com\r\n\r\n", ip.c_str());
    net_tcp_send(m_Socket, aNetBuff, strlen(aNetBuff));

    //read data
    std::string NetData;
    int TotalRecv = 0;
    int TotalBytes = 0;
    int CurrentRecv = 0;
    bool isHead = true;
    int enterCtrl = 0;
    while ((CurrentRecv = net_tcp_recv(m_Socket, aNetBuff, sizeof(aNetBuff))) > 0)
    {
        for (int i=0; i<CurrentRecv ; i++)
        {
            if (isHead)
            {
                if (aNetBuff[i]=='\n')
                {
                    enterCtrl++;
                    if (enterCtrl == 2)
                    {
                        isHead = false;
                        NetData.clear();
                        continue;
                    }

                    std::transform(NetData.begin(), NetData.end(), NetData.begin(), ::tolower);
					if (NetData.find("content-length:") != std::string::npos)
                    {
                        sscanf(NetData.c_str(), "content-length:%d", &TotalBytes);
                        if (TotalBytes == 0)
                            sscanf(NetData.c_str(), "content-length: %d", &TotalBytes);
                    }

                    NetData.clear();
                    continue;
                }
                else if (aNetBuff[i]!='\r')
                    enterCtrl=0;

                NetData+=aNetBuff[i];
            }
            else
            {
                if (TotalBytes == 0)
                {
                    net_tcp_close(m_Socket);
                    dbg_msg("GeoIP","ERROR: Error with size received data.");
                    geoInfo->m_CountryCode = "NULL";
                    return;
                }

                jsonData += aNetBuff[i];

                TotalRecv++;
                if (TotalRecv == TotalBytes)
                    break;
            }
        }
    }

    //Finish
    net_tcp_close(m_Socket);

    // parse json data
	json_settings JsonSettings;
	mem_zero(&JsonSettings, sizeof(JsonSettings));
	char aError[256];
	json_value *pJsonData = json_parse_ex(&JsonSettings, jsonData.c_str(), jsonData.length(), aError);
	if (pJsonData == 0)
	{
		dbg_msg("GeoIP", "Error: %s", aError);
		return;
	}

	// generate configurations
	const json_value &countryCode = (*pJsonData)["country_code"];
	if (countryCode.type == json_string) geoInfo->m_CountryCode = (const char *)countryCode;
	const json_value &countryName = (*pJsonData)["country"];
	if (countryName.type == json_string) geoInfo->m_CountryName = (const char *)countryName;
	const json_value &isp = (*pJsonData)["isp"];
	if (isp.type == json_string) geoInfo->m_Isp = (const char *)isp;
}

void CGeoIP::ThreadGeoIP(void *params)
{
    InfoGeoIPThread *pInfoThread = static_cast<InfoGeoIPThread*>(params);

    lock_wait(m_GeoIPLock);
    GetInfo(pInfoThread->ip, pInfoThread->m_pGeoInfo);
    lock_release(m_GeoIPLock);
}

