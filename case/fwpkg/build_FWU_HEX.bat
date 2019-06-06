..\..\psp_rel\tools\FW2X.exe -fssf US281B_BTBOX.fw MakeBin AFI UPGRADE.IMG
copy ..\..\psp_rel\bin\upgrade.bin/b + UPGRADE.IMG/b UPGRADE.FWU
copy UPGRADE.FWU UPGRADE.HEX
del UPGRADE.IMG
pause