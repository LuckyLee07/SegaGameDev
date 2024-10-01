#ifndef __STRING_RENDER_H__
#define __STRING_RENDER_H__

class Image;

// 字符串绘制类
// 单例模式示例
class StringRender
{
public:
	static StringRender* instance();
	static void create(const char* fontFileName);
	static void destroy(); //释放对象
	// 绘制函数
	void draw(int x, int y, const char* str, unsigned color = 0xffffff) const;

private:
	StringRender(const char* fontFileName); // 隐藏构造函数
	~StringRender();	// 隐藏析构函数
	StringRender(const StringRender&); //屏蔽拷贝构造函数

private:
	Image* m_pFontImage;
	static StringRender* m_pInstance;
};

#endif  // __STRING_RENDER_H__