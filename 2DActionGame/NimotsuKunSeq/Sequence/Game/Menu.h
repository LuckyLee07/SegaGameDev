#ifndef __MENU_H__
#define __MENU_H__

class Image;
class GameBase;

class Menu
{
public:
	Menu();
	~Menu();

	void update(GameBase* parent);

private:
	Image* m_pImage;
};

#endif  // __MENU_H__