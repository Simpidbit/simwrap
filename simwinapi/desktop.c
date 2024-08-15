#include "simwinapi.h"
#include <stdio.h>

HDESK
create_desktop(wchar_t* dtitle)
{
	HDESK dsk = CreateDesktopW(dtitle, NULL, NULL, 0, 0, NULL);
	if (dsk == NULL) {
		printf("失败\n");
		printf("%d\n", GetLastError());
	}
	return dsk;
}
