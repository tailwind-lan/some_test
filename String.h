#include <stdio.h>

class String
{
public:
	String(const char *str = nullptr);				// 普通构造函数
	String(int i);
	String(const String &other);					// 拷贝构造函数
	~String(void);									// 析构函数
	String &operator = (const String &other);		// 赋值函数
	void WriteToFile();
private:
	char *m_data; // 用于保存字符串
};
