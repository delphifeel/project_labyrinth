#ifndef __CORE_STRING_H__
#define __CORE_STRING_H__

class CoreString
{
public:
	char 	*buffer;
	int 	size;
	int		max_size;

	CoreString(const int customSize = 0);
	~CoreString();

	void 	Concat(const char *value);
	char* 	GetBuffer();
	int 	GetSize();
};

#endif
