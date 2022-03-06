#include <stdlib.h>

const char *CORE_GetModuleName(const char *file_name, const char *module_name)
{
	const char *temp_ptr;


	if (module_name == NULL)
	{
		temp_ptr = file_name;
		while (*temp_ptr != 0)
		{
			if ((*temp_ptr == '/') ||
				(*temp_ptr == '\\'))
			{
				module_name = temp_ptr;
			}

			temp_ptr++;
		}

		if (module_name == NULL)
		{
			module_name = file_name;
		}
		else
		{
			module_name++;
		}
	}

	return module_name;
}