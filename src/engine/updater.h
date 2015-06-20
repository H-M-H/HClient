/*
    unsigned char*
*/
#ifndef ENGINE_UPDATER_H
#define ENGINE_UPDATER_H

#include "kernel.h"
#include <game/client/components/menus.h>
#include <string>

class IUpdater : public IInterface
{
	MACRO_INTERFACE("autoupdate", 0)
public:
	virtual void CheckUpdates(CMenus *pMenus) = 0;
	virtual void DoUpdates(CMenus *pMenus) = 0;
	virtual bool Updated() = 0;
	virtual bool NeedResetClient() const = 0;
    virtual bool NeedUpdateClient() const = 0;
	virtual bool NeedUpdateServer() const = 0;
	virtual void ExecuteExit() = 0;
	virtual const char* GetNewVersion() const = 0;
    virtual std::vector<std::string>& GetFilesToRemove() = 0;
	virtual std::vector<std::string>& GetFilesToDownload() = 0;

    virtual const char* GetCurrentDownloadFileName() const = 0;
	virtual float GetCurrentDownloadProgress() = 0;
};

struct InfoUpdatesThread
{
    IUpdater *m_pAutoUpdate;
    CMenus *m_pMenus;
};

void ThreadUpdates(void *);

#endif
