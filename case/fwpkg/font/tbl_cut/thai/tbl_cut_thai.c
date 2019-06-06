#include <stdio.h>

unsigned short thai_to_unicode(int gbk_code, FILE *in)
{
	unsigned short unicode_val;
	int diff;
	int index;
	
	if ((gbk_code&0xff) <= 0x7f)
	{
		unicode_val = gbk_code&0xff;
	}
	else
	{
		if ((gbk_code&0xff) == 0xff)
		{
			unicode_val = 0xf8c8;
		}
		else if ((gbk_code&0xff) == 0xfe)
		{
			unicode_val = 0xf8c7;
		}
		else if ((gbk_code&0xff) == 0xfd)
		{
			unicode_val = 0xf8c6;
		}
		else if ((gbk_code&0xff) == 0xfc)
		{
			unicode_val = 0xf8c5;
		}
		else if ((gbk_code&0xff) == 0xde)
		{
			unicode_val = 0xf8c4;
		}
		else if ((gbk_code&0xff) == 0xdd)
		{
			unicode_val = 0xf8c3;
		}
		else if ((gbk_code&0xff) == 0xdc)
		{
			unicode_val = 0xf8c2;
		}
		else if ((gbk_code&0xff) == 0xdb)
		{
			unicode_val = 0xf8c1;
		}
		else if ((gbk_code&0xff) == 0xa0)
		{
			unicode_val = 0xa0;
		}
		else if (((gbk_code&0xff) >= 0xa1) && ((gbk_code&0xff) <= 0xda))
		{
			unicode_val = 0xe01 + ((gbk_code&0xff)-0xa1);
		}
		else if ((gbk_code&0xff) >= 0xdf)
		{
			unicode_val = 0xe3f + ((gbk_code&0xff)-0xdf);
		}
		else
		{
			{
				diff = gbk_code - 0x80;
				index = diff/0x100*0x20;
				index += (diff%0x80);
				fseek(in, index*2, 0);
				fread(&unicode_val, 2, 1, in);
			}
		}
	}
	
	return unicode_val;
}

int main(void)
{
	FILE *in, *out, *out_large;
	char tbl_name[] = "V874.TBL";
	char tbl_tidy_name[] = "V874_TIDY.TBL";
	char tbl_large_name[] = "V874_LARGE.TBL";
	int gbk_code;
	unsigned short unicode_val;
	int i;
	
	in = fopen(tbl_name, "rb");
	if (in == NULL)
	{
		printf("can't open %s\n", tbl_name);
		return 0;
	}
	out = fopen(tbl_tidy_name, "wb");

	fseek(in, 0x100, 0);
	gbk_code = 0x80;
	for (;gbk_code <= 0xffff;)
	{
		fread(&unicode_val, 2, 1, in);
		fwrite(&unicode_val, 2, 1, out);
		gbk_code++;
		if ((gbk_code & 0xff) == 0xa0)
		{
			gbk_code += 0xe0;
			fseek(in, 0x1c0, 1);
		}
	}
	
	fclose(in);
	fclose(out);
	
	in = fopen(tbl_tidy_name, "rb");
	out = fopen(tbl_large_name, "wb");
	
	gbk_code = 0;
	for (; gbk_code <= 0xffff; gbk_code++)
	{
		unicode_val = thai_to_unicode(gbk_code, in);
		fwrite(&unicode_val, 2, 1, out);
	}
	
	fclose(in);
	fclose(out);
	
	return 0;
}
