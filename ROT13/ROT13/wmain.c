#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

wchar_t g_repTblA[27] = L"NOPQRSTUVWXYZABCDEFGHIJKLM";
wchar_t g_repTblN[11] = L"5678901234";

wchar_t __cdecl rot13(wchar_t c)
{
	c = towupper(c);

	if (c >= L'A' && c <= L'Z')
	{
		c -= L'A';
		c = g_repTblA[(int)c];
	}

	return c;
}

wchar_t __cdecl  rot5(wchar_t c)
{
	if (c >= L'0' && c <= L'9')
	{
		c -= L'0';
		c = g_repTblN[(int)c];
	}
	return c;
}

int __cdecl wmain(int argc, wchar_t *argv[])
{
	FILE *fp = NULL;
	unsigned char *buffer = NULL;
	unsigned int i, len;
	errno_t result;
	size_t read;
	wchar_t wc;

	if (argc < 2)
	{
		_putws(L"Usage: ROT13 inputfile\n");
		return 0;
	}

	result = _wfopen_s(&fp, argv[1], L"r+");
	if (result != 0)
	{
		wprintf_s(L"Error occured when opening \'%s\'. Ensure that your user account has access to open this file.\n", argv[1], errno);
		return -1;
	}


	// print to stderr in case user is piping rotation output to a file
	fputws(L"WARNING: This program is meant for educational purposes only; it is not meant to be used as a serious cryptographic tool. ROT13 is trivially breakable.\n", stderr);

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);

	buffer = (unsigned char*)malloc(len + 1);
	read = fread(buffer, 1, len, fp);


	fclose(fp);
	fp = NULL;

	for (i = 0; i < read; i++)
	{
		wc = *(buffer + i) & 0xFF;
		if (iswalpha(wc))
			putwchar(rot13(wc));
		else if (iswdigit(wc))
			putwchar(rot5(wc));
		else
			putwchar(wc);
	}

	free(buffer);
	buffer = NULL;
	putwchar(L'\n');
	return 0;
}