#include <stdio.h>

unsigned short kor_to_unicode(int gbk_code, FILE *in)
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
			unicode_val = 0x80;
		}
		else if ((gbk_code&0xff) == 0xff)
		{
			unicode_val = 0xf8f7;
		}
		else
		{
			if (gbk_code < 0x100)
			{
				unicode_val = 0;
			}
			else if (gbk_code < 0x4180)
			{
				unicode_val = 0x3f;
			}
			else
			{
				diff = gbk_code - 0x4180;
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
	char tbl_name[] = "V949KOR.TBL";
	char tbl_tidy_name[] = "V949KOR_TIDY.TBL";
	char tbl_large_name[] = "V949KOR_LARGE.TBL";
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

	fseek(in, 0x8300, 0);
	gbk_code = 0x4180;
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
		unicode_val = kor_to_unicode(gbk_code, in);
		fwrite(&unicode_val, 2, 1, out);
	}
	
	fclose(in);
	fclose(out);
	
	return 0;
}
