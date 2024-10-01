#ifndef __PLAY_H__
#define __PLAY_H__

class Image;
class GameBase;

class Play
{
public:
	Play();
	~Play();

	void update(GameBase* parent, float dt);

private:
	Image* m_pImage;
};

#endif  // __PLAY_H__