@echo off
echo ***************Gen tts_rc.lib ********************************
..\..\psp_rel\tools\tts_maker.exe .\tts_res\
echo.

@echo off
echo ***************Gen config.bin*********************************
..\tools\Gen_config\Genbin.exe     cfg_boot.txt config.txt cfg_dbg.txt cfg_aec.txt cfg_dae.txt cfg_ae2.txt cfg_drcv.txt cfg_okm.txt cfg_geq.txt        config.bin
..\tools\Gen_config\Genheader.exe  cfg_boot.txt config.txt cfg_dbg.txt cfg_aec.txt cfg_dae.txt cfg_ae2.txt cfg_drcv.txt cfg_okm.txt cfg_geq.txt        ..\inc\config_id.h
echo.

@echo off

echo ***************Gen res_lib.lib********************************
..\..\psp_rel\tools\res_packer.exe  .\res_lib\
..\..\psp_rel\tools\lib_packer.exe  .\font\
echo.

echo ***************get afi.bin version ***************************
..\..\psp_rel\tools\getver  ..\..\psp_rel\bin\AFI.bin
echo.

@echo off
echo ***************Gen Firmware***********************************
..\..\psp_rel\tools\Maker.exe -c .\fwimage.cfg -o .\US281B_BTBOX.fw -mf
..\..\psp_rel\tools\FW2X.exe -fs .\US281B_BTBOX.fw FW_Size_Check
pause