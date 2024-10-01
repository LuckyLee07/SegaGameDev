#ifndef __GAME_BASE_H__
#define __GAME_BASE_H__

class State;
class Clear;
class Loading;
class Menu;
class Play;
class Parent;

class GameBase
{
public:
	enum SeqID
	{
		SEQ_STATE_SELECT, // 上层
		SEQ_TITLE,	// 上层
		
		SEQ_CLEAR,
		SEQ_MENU,
		SEQ_PLAY,

		SEQ_NONE,
	};
	GameBase(int stateId);
	~GameBase();
	
	State* state();

	void moveTo(SeqID seqId);
	void update(Parent* parent, float dt = 0.0f);
	void startLoding();
private:
	State* m_pState;
	SeqID m_next;
	int m_stateId;

	Clear* m_pClear;
	Loading* m_pLoading;
	Menu* m_pMenu;
	Play* m_pPlay;
};

#endif  // __GAME_BASE_H__