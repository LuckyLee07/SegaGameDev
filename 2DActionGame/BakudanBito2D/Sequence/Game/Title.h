#ifndef __TITLE_H__
#define __TITLE_H__

class Image;
class Parent;

class Title
{
public:
	Title();
	~Title();

	void update(Parent* parent);

private:
	Image* m_pImage;
};

#endif  // __TITLE_H__