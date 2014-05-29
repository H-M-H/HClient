#include "texturepack.h"

#include <engine/storage.h>
#include <engine/shared/config.h>
#include <game/generated/client_data.h>
#include <string>


void CTexturePack::Init()
{
    m_pStorage = Kernel()->RequestInterface<IStorage>();
    m_pGraphics = Kernel()->RequestInterface<IGraphics>();
    m_pTextRender = Kernel()->RequestInterface<ITextRender>();
    m_pSound = Kernel()->RequestInterface<ISound>();
    SearchThemes();
}

bool CTexturePack::Load(const char *rawpackname)
{
    if (!m_pStorage)
        return false;

    char path[255];
    str_format(path, sizeof(path), "themes/%s/style.tws", rawpackname);
    IOHANDLE styleFile = m_pStorage->OpenFile(path, IOFLAG_READ, IStorage::TYPE_ALL);

    if (styleFile)
    {
        char buff[255];

        while (io_read_line(styleFile, buff, sizeof(buff)) > 0)
        {
            std::string line(buff);
            size_t dpos = line.find_first_of('=');
            std::string paramName(line.substr(0, dpos));

            if (line.empty() || dpos == line.length() || line.at(0) == '#')
                continue;

            if (paramName.compare("list_header_background_color") == 0)
                str_copy(g_Config.m_hcListHeaderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListHeaderBackgroundColor));
            else if (paramName.compare("list_header_text_color") == 0)
                str_copy(g_Config.m_hcListHeaderTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListHeaderTextColor));
            else if (paramName.compare("list_footer_background_color") == 0)
                str_copy(g_Config.m_hcListFooterBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListFooterBackgroundColor));
            else if (paramName.compare("list_footer_text_color") == 0)
                str_copy(g_Config.m_hcListFooterTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListFooterTextColor));
            else if (paramName.compare("list_background_color") == 0)
                str_copy(g_Config.m_hcListBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListBackgroundColor));
            else if (paramName.compare("list_text_color") == 0)
                str_copy(g_Config.m_hcListTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListTextColor));
            else if (paramName.compare("list_item_selected_color") == 0)
                str_copy(g_Config.m_hcListItemSelectedColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListItemSelectedColor));
            else if (paramName.compare("list_item_odd_color") == 0)
                str_copy(g_Config.m_hcListItemOddColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListItemOddColor));
            else if (paramName.compare("list_column_selected_color") == 0)
                str_copy(g_Config.m_hcListColumnSelectedColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcListColumnSelectedColor));
            else if (paramName.compare("serverbrowser_list_group_header_background_color") == 0)
                str_copy(g_Config.m_hcServerbrowserListGroupHeaderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcServerbrowserListGroupHeaderBackgroundColor));
            else if (paramName.compare("serverbrowser_list_group_header_text_color") == 0)
                str_copy(g_Config.m_hcServerbrowserListGroupHeaderTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcServerbrowserListGroupHeaderTextColor));
            else if (paramName.compare("serverbrowser_list_extra_info_background_color") == 0)
                str_copy(g_Config.m_hcServerbrowserListExtraInfoBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcServerbrowserListExtraInfoBackgroundColor));
            else if (paramName.compare("serverbrowser_list_extra_info_text_color") == 0)
                str_copy(g_Config.m_hcServerbrowserListExtraInfoTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcServerbrowserListExtraInfoTextColor));
            else if (paramName.compare("container_header_background_color") == 0)
                str_copy(g_Config.m_hcContainerHeaderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcContainerHeaderBackgroundColor));
            else if (paramName.compare("container_header_text_color") == 0)
                str_copy(g_Config.m_hcContainerHeaderTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcContainerHeaderTextColor));
            else if (paramName.compare("container_background_color") == 0)
                str_copy(g_Config.m_hcContainerBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcContainerBackgroundColor));
            else if (paramName.compare("container_text_color") == 0)
                str_copy(g_Config.m_hcContainerTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcContainerTextColor));
            else if (paramName.compare("subcontainer_header_background_color") == 0)
                str_copy(g_Config.m_hcSubcontainerHeaderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSubcontainerHeaderBackgroundColor));
            else if (paramName.compare("subcontainer_header_text_color") == 0)
                str_copy(g_Config.m_hcSubcontainerHeaderTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSubcontainerHeaderTextColor));
            else if (paramName.compare("subcontainer_background_color") == 0)
                str_copy(g_Config.m_hcSubcontainerBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSubcontainerBackgroundColor));
            else if (paramName.compare("subcontainer_text_color") == 0)
                str_copy(g_Config.m_hcSubcontainerTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSubcontainerTextColor));
            else if (paramName.compare("popup_header_background_color") == 0)
                str_copy(g_Config.m_hcPopupHeaderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPopupHeaderBackgroundColor));
            else if (paramName.compare("popup_header_text_color") == 0)
                str_copy(g_Config.m_hcPopupHeaderTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPopupHeaderTextColor));
            else if (paramName.compare("popup_background_color") == 0)
                str_copy(g_Config.m_hcPopupBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPopupBackgroundColor));
            else if (paramName.compare("popup_text_color") == 0)
                str_copy(g_Config.m_hcPopupTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPopupTextColor));
            else if (paramName.compare("editbox_background_color") == 0)
                str_copy(g_Config.m_hcEditboxBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcEditboxBackgroundColor));
            else if (paramName.compare("editbox_text_color") == 0)
                str_copy(g_Config.m_hcEditboxTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcEditboxTextColor));
            else if (paramName.compare("button_background_color") == 0)
                str_copy(g_Config.m_hcButtonBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcButtonBackgroundColor));
            else if (paramName.compare("button_text_color") == 0)
                str_copy(g_Config.m_hcButtonTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcButtonTextColor));
            else if (paramName.compare("trackbar_background_color") == 0)
                str_copy(g_Config.m_hcTrackbarBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcTrackbarBackgroundColor));
            else if (paramName.compare("trackbar_slider_background_color") == 0)
                str_copy(g_Config.m_hcTrackbarSliderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcTrackbarSliderBackgroundColor));
            else if (paramName.compare("progressbar_background_color") == 0)
                str_copy(g_Config.m_hcProgressbarBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcProgressbarBackgroundColor));
            else if (paramName.compare("progressbar_slider_background_color") == 0)
                str_copy(g_Config.m_hcProgressbarSliderBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcProgressbarSliderBackgroundColor));
            else if (paramName.compare("mainmenu_text_color") == 0)
                str_copy(g_Config.m_hcMainmenuTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcMainmenuTextColor));
            else if (paramName.compare("mainmenu_background_top_color") == 0)
                str_copy(g_Config.m_hcMainmenuBackgroundTopColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcMainmenuBackgroundTopColor));
            else if (paramName.compare("mainmenu_background_bottom_color") == 0)
                str_copy(g_Config.m_hcMainmenuBackgroundBottomColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcMainmenuBackgroundBottomColor));
            else if (paramName.compare("paneltab_selected_background_color") == 0)
                str_copy(g_Config.m_hcPaneltabSelectedBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPaneltabSelectedBackgroundColor));
            else if (paramName.compare("paneltab_selected_text_color") == 0)
                str_copy(g_Config.m_hcPaneltabSelectedTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPaneltabSelectedTextColor));
            else if (paramName.compare("paneltab_background_color") == 0)
                str_copy(g_Config.m_hcPaneltabBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPaneltabBackgroundColor));
            else if (paramName.compare("paneltab_text_color") == 0)
                str_copy(g_Config.m_hcPaneltabTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcPaneltabTextColor));
            else if (paramName.compare("settings_paneltab_selected_background_color") == 0)
                str_copy(g_Config.m_hcSettingsPaneltabSelectedBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSettingsPaneltabSelectedBackgroundColor));
            else if (paramName.compare("settings_paneltab_selected_text_color") == 0)
                str_copy(g_Config.m_hcSettingsPaneltabSelectedTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSettingsPaneltabSelectedTextColor));
            else if (paramName.compare("settings_paneltab_background_color") == 0)
                str_copy(g_Config.m_hcSettingsPaneltabBackgroundColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSettingsPaneltabBackgroundColor));
            else if (paramName.compare("settings_paneltab_text_color") == 0)
                str_copy(g_Config.m_hcSettingsPaneltabTextColor, line.substr(dpos+1).c_str(), sizeof(g_Config.m_hcSettingsPaneltabTextColor));
        }

        io_close(styleFile);
    }

    str_copy(m_RawPackName, rawpackname, sizeof(m_RawPackName));

    // Load Font
	char aFilename[512];
	str_format(path, sizeof(path), "themes/%s/font.ttf", rawpackname); // FIXME: Not use generic name and add option for select font size
	IOHANDLE File = m_pStorage->OpenFile(path, IOFLAG_READ, IStorage::TYPE_ALL, aFilename, sizeof(aFilename));
	if(File)
	{
		io_close(File);
        m_pTextRender->SetFont(m_pTextRender->LoadFont(aFilename));
	} else
	{
	    File = m_pStorage->OpenFile("fonts/DejaVuSans.ttf", IOFLAG_READ, IStorage::TYPE_ALL, aFilename, sizeof(aFilename));
	    if (File)
        {
            io_close(File);
            m_pTextRender->SetFont(m_pTextRender->LoadFont(aFilename));
        }
	}

    // Load Images
    LoadImages();

	// Load Sounds
	//LoadSounds();

    return true;
}

void CTexturePack::SearchThemes()
{
    m_pStorage->ListDirectory(1, "themes", ThemeScan, this);
}

void CTexturePack::LoadTheme(const char *themeName)
{
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "themes/%s/info.tws", themeName);
	IOHANDLE InfoFile = m_pStorage->OpenFile(aBuf, IOFLAG_READ, IStorage::TYPE_ALL);
	if (!InfoFile)
        return;

    CTheme theme;
	char buffLine[255];
	while (io_read_line(InfoFile, buffLine, sizeof(buffLine)) > 0)
    {
        std::string line(buffLine);
        size_t dpos = line.find_first_of('=');
        std::string paramName(line.substr(0, dpos));

        if (dpos == line.length())
            continue;

        if (paramName.compare("packname") == 0)
            str_copy(theme.m_Name, line.substr(dpos+1).c_str(), sizeof(theme.m_Name));
        else if (paramName.compare("version") == 0)
            str_copy(theme.m_Version, line.substr(dpos+1).c_str(), sizeof(theme.m_Version));
        else if (paramName.compare("author") == 0)
            str_copy(theme.m_Author, line.substr(dpos+1).c_str(), sizeof(theme.m_Author));
        else if (paramName.compare("mail") == 0)
            str_copy(theme.m_Mail, line.substr(dpos+1).c_str(), sizeof(theme.m_Mail));
        else if (paramName.compare("website") == 0)
            str_copy(theme.m_Web, line.substr(dpos+1).c_str(), sizeof(theme.m_Web));
    }
	io_close(InfoFile);

    str_copy(theme.m_FolderName, themeName, sizeof(theme.m_FolderName));
    AddTheme(theme);
}

void CTexturePack::LoadImages()
{
    char aBuf[255];
    for (int i=0; i<g_pData->m_NumImages; i++)
    {
        mem_zero(aBuf, sizeof(aBuf));
        str_format(aBuf, sizeof(aBuf), "themes/%s/%s", g_Config.m_hcTheme, g_pData->m_aImages[i].m_pFilename);
        IOHANDLE File = m_pStorage->OpenFile(aBuf, IOFLAG_READ, IStorage::TYPE_ALL);
        if (!File)
            str_copy(aBuf, g_pData->m_aImages[i].m_pFilename, sizeof(aBuf));
        else
            io_close(File);

        if (g_pData->m_aImages[i].m_Id != -1)
            Graphics()->UnloadTexture(g_pData->m_aImages[i].m_Id);
        g_pData->m_aImages[i].m_Id = Graphics()->LoadTexture(aBuf, IStorage::TYPE_ALL, CImageInfo::FORMAT_AUTO, 0);
    }
}

void CTexturePack::LoadSounds()
{
    char aBuf[255];
    m_pSound->StopAll();
    for (int s=0; s<g_pData->m_NumSounds; s++)
    {
		for(int i = 0; i < g_pData->m_aSounds[s].m_NumSounds; i++)
		{
            str_format(aBuf, sizeof(aBuf), "themes/%s/%s", g_Config.m_hcTheme, g_pData->m_aSounds[s].m_aSounds[i].m_pFilename);
            IOHANDLE file = m_pStorage->OpenFile(aBuf, IOFLAG_READ, IStorage::TYPE_ALL);
            if (!file)
                str_copy(aBuf, g_pData->m_aSounds[s].m_aSounds[i].m_pFilename, sizeof(aBuf));
            else
                io_close(file);

			g_pData->m_aSounds[s].m_aSounds[i].m_Id = m_pSound->LoadWV(aBuf);
		}
    }
}

int CTexturePack::ThemeScan(const char *pName, int IsDir, int DirType, void *pUser)
{
    CTexturePack *pSelf = (CTexturePack*)pUser;
	if(!IsDir)
		return 0;

    pSelf->LoadTheme(pName);
	return 0;
}