#ifndef __LOADING_H__
#define __LOADING_H__

class Image;
class GameBase;

class Loading
{
public:
	Loading();
	~Loading();

	void update(GameBase* parent);

private:
	int m_count;
	bool m_started;
	Image* m_pImage;
};

#endif  // __LOADING_H__