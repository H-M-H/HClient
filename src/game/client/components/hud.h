/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_HUD_H
#define GAME_CLIENT_COMPONENTS_HUD_H
#include <game/client/component.h>
#include <string> // H-Client

class CHud : public CComponent
{
    // H-Client
    struct CPlayerInfoLine
    {
        CPlayerInfoLine(std::string text, int type)
        {
            m_Text = text;
            m_Type = type;
        }

        std::string m_Text;
        unsigned int m_Type;
    };
    //

	float m_Width, m_Height;
	float m_AverageFPS;

	void RenderCursor();

	void RenderFps();
	void RenderConnectionWarning();
	void RenderTeambalanceWarning();
	void RenderVoting();
	void RenderHealthAndAmmo(const CNetObj_Character *pCharacter, int localID);
	void RenderGameTimer();
	void RenderSuddenDeath();
	void RenderScoreHud();
	void RenderSpectatorHud();
	void RenderWarmupTimer();

    //H-Client
	void RenderSelectorSpectatorHud();
	void RenderRecord(); //DDRace
	void RenderPlayerInfo();

	virtual void OnMessage(int MsgType, void *pRawMsg);
	//

	void MapscreenToGroup(float CenterX, float CenterY, struct CMapItemGroup *PGroup);
public:
	CHud();

	virtual void OnReset();
	virtual void OnRender();

	float m_CheckpointDiff;
	int m_DDRaceTime;
	int m_LastReceivedTimeTick;
	int m_CheckpointTick;
	int m_DDRaceTick;
	bool m_FinishTime;
	float m_ServerRecord;
	float m_PlayerRecord;
	bool m_DDRaceTimeReceived;
};

#endif
