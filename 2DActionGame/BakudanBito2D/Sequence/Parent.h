#ifndef __PARENT_H__
#define __PARENT_H__

class Title;
class GameBase;
class StateSelect;

class Parent
{
public:
	enum SeqID
	{
		SEQ_STATE_SELECT,
		SEQ_TITLE,
		SEQ_GAME,

		SEQ_NONE,
	};
	Parent();
	~Parent();
	void update(float dt);

	void moveTo(SeqID seqId);
	void setStateId(int stateId);

private:
	Title* m_pTitle;
	StateSelect* m_pStateSelect;
	GameBase* m_pMainGame;

	SeqID m_next;
	int m_stateId;
};

#endif  // __PARENT_H__