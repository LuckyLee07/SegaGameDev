#ifndef __STATE_SELECT_H__
#define __STATE_SELECT_H__

class Image;
class Parent;

class StateSelect
{
public:
	StateSelect();
	~StateSelect();

	void update(Parent* parent);

private:
	Image* m_pImage;
};

#endif  // __STATE_SELECT_H__