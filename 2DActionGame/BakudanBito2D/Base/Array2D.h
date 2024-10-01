#ifndef __ARRAY_2D_H__
#define __ARRAY_2D_H__

template<class T>
class Array2D
{
public:
	Array2D() :m_data(nullptr),
		m_rows(0), m_cols(0)
	{
	}
	Array2D(int rows, int cols) :
		m_rows(rows), m_cols(cols)
	{
		m_data = new T[rows * cols];
	}
	~Array2D()
	{
		delete[] m_data;
		m_data = nullptr;
	}
	void init(int rows, int cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_data = new T[rows * cols];
	}
	T& operator()(int row, int col)
	{
		return m_data[row * m_cols + col];
	}
	const T& operator()(int row, int col) const
	{
		return m_data[row * m_cols + col];
	}
private:
	T* m_data;
	int m_rows; //行
	int m_cols; //列
};

#endif  // __ARRAY_2D_H__