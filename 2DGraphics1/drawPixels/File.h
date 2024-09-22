#ifndef __FILE_H__
#define __FILE_H__

class File
{
public:
	File(const char* fileName);
	~File();
	int size() const;
	const char* data() const;
	unsigned getUnsigned(int pos) const;
private:
	int m_size;
	char* m_data;
};

#endif  // __FILE_H__