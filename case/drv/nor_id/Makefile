#**************************************************************************
#                                                                         *
#   PROJECT     : MIPS port for UCOS-II                                   *
#                                                                         *
#   MODULE      : MakeFile                                                *
#                                                                         *
#   AUTHOR      : mengzh                                                  *
#                                                                         *
#   PROCESSOR   : MIPS 4KEc (32 bit RISC) - Actions board                 *
#                                                                         *
#   Tool-chain  :  Cygnus                                                 *
#                                                                         *
#   DESCRIPTION :                                                         *
#                                                                         *
#   targets are :                                                         *
#                                                                         *
#*************************************************************************/
# **********************************************
# Type of Processor	& Board
# **********************************************
Processor =	mips
# **********************************************
# Directories
# **********************************************
#��makefile�ļ��Ĵ��Ŀ¼Ϊ��ʼ�㣬ָ����������ĸ�Ŀ¼�����·�� uvob
ROOT	  =	./../../..
#ͬʱΪ�˷��㣬����ָ������Ŀ¼������һ�������ĸ�ʽΪ $(��������)
PSPRELROOT = $(ROOT)/psp_rel
PSPLIBDIR  = $(PSPRELROOT)/lib

# **********************************************
# Name of application ������Ա���޸Ĵ�(1)
IMAGENAME = nor_id
#��Ҫ�����Դ�ļ��Ĵ��λ��,������Ա���޸Ĵ�(2)
SRC = ..
SRCDIR_16 = .
SRCDIR_32 = 
#Ŀ���ļ��������ļ���·��,������Ա���޸Ĵ�(3)
OBJECT_BIN_PATH = ./../../fwpkg/drv
# **********************************************

#��������
DRV_BUILDER = $(PSPRELROOT)/tools/drv_builder.exe
AP_BUILDER = $(PSPRELROOT)/tools/ap_builder.exe
OBJCOPY   = sde-objcopy 
#��Ž���ļ���Ŀ¼�����������Ϊ��ǰĿ¼�������ɵĽ���ļ����ڵ�ǰĿ¼
LINKDIR	  =	.
#ָ���������̵��ļ�������·������Դ�ļ���·��д�ϼ���
VPATH	=	$(SRCDIR_16) $(SRCDIR_32)
# **********************************************
# Image	file names and map file
# **********************************************
#ָ������ļ�������
IMAGE_ELF =	$(IMAGENAME).exe
IMAGE_LST =	$(IMAGENAME).lst
IMAGE_REC =	$(IMAGENAME).rec
IMAGE_BIN =	$(IMAGENAME).sys
MAP		  =	$(IMAGENAME).map
TARGET_DRV = $(IMAGENAME).drv
TARGET_BIN = $(IMAGENAME).bin
TARGET_EXT   = exe lst map bin info
TARGET_FILES = $(foreach n,$(TARGET_EXT),$(IMAGENAME).$(n))
# **********************************************
# Compiler and linker options
# **********************************************
#ָ�����������õ�ͷ�ļ�������·����һ��Դ��includeʱֻд��ͷ�ļ������ƣ�û��·����Ϣ����ʱ�͸������ѡ��ȥ������ͷ�ļ�
INCLUDE	  =	-I$(PSPRELROOT)/include -I$(PSPRELROOT)/include/ucos -I$(PSPRELROOT)/include/drv



CC_OPTS    	    = -G0 -g -Wall -fsigned-char -mtune=4kec -mips32r2 -fno-builtin $(INCLUDE) -EL -c
CC        		= sde-gcc
LD              = sde-ld
OBJCOPY         = sde-objcopy
OBJDUMP         = sde-objdump
.PHONY : all
all:$(IMAGENAME)

$(IMAGENAME):
	$(CC) $(INCLUDE) -O2 $(CC_OPTS) spi_nor_id_tbl.c -o spi_nor_id_tbl.o
	$(OBJCOPY) -O binary spi_nor_id_tbl.o $@.bin
	cp $@.bin $(OBJECT_BIN_PATH)/$@.bin 

	rm *.o
	@echo ----------------------------------------
	@echo ��ϲ��,����ɹ�����С�ˣ�

	
clean:
	rm -f *.o *.bin *.exe *.elf *.lst *.map 
