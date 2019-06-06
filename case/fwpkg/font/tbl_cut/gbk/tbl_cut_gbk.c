#include <stdio.h>

unsigned short gbk_to_unicode(int gbk_code, FILE *in)
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
		if ((gbk_code&0xff) == 0x80)
		{
			unicode_val = 0x20ac;
		}
		else if ((gbk_code&0xff) == 0xff)
		{
			unicode_val = 0xf8f5;
		}
		else
		{
			if (gbk_code < 0x100)
			{
				unicode_val = 0;
			}
			else if (gbk_code < 0x4080)
			{
				unicode_val = 0x3f;
			}
			else
			{
				diff = gbk_code - 0x4080;
				index = diff/0x100*0x80;
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
	char tbl_name[] = "V936GBK.TBL";
	char tbl_tidy_name[] = "V936GBK_TIDY.TBL";
	char tbl_large_name[] = "V936GBK_LARGE.TBL";
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

	fseek(in, 0x8100, 0);
	gbk_code = 0x4080;
	for (;gbk_code <= 0xffff;)
	{
		fread(&unicode_val, 2, 1, in);
		fwrite(&unicode_val, 2, 1, out);
		gbk_code++;
		if ((gbk_code & 0xff) == 0x00)
		{
			gbk_code += 0x80;
			fseek(in, 0x100, 1);
		}
	}
	
	fclose(in);
	fclose(out);
	
	in = fopen(tbl_tidy_name, "rb");
	out = fopen(tbl_large_name, "wb");
	
	gbk_code = 0;
	for (; gbk_code <= 0xffff; gbk_code++)
	{
		unicode_val = gbk_to_unicode(gbk_code, in);
		fwrite(&unicode_val, 2, 1, out);
	}
	
	fclose(in);
	fclose(out);
	
	return 0;
}
