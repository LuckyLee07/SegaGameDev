#ifndef __CLEAR_H__
#define __CLEAR_H__

class Image;
class GameBase;

class Clear
{
public:
	Clear();
	~Clear();

	void update(GameBase* parent);

private:
	int m_count;
	Image* m_pImage;
};

#endif  // __CLEAR_H__