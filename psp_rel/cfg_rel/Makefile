all:

	make -C ./../../psp_rel/HAL -j16
	make -C ./../../psp_rel/mmm_drv/hal_drv -j16

	make -C ./../../psp_rel/mmm_drv/adc_channel -j16
	make -C ./../../psp_rel/mmm_drv/record_module -j16

	make -C ./../../psp_rel/mmm_drv/music_player -j16
	make -C ./../../psp_rel/mmm_drv/music_pcm_player -j16
	make -C ./../../psp_rel/mmm_drv/music_bt_player -j16
	make -C ./../../psp_rel/mmm_drv/speech_player -j16

	make -C ./../../psp_rel/usermodule/enhanced

	make -C ./../../case/cfg




clean:
	make -C ./../../psp_rel/HAL                         clean
	make -C ./../../psp_rel/mmm_drv/hal_drv             clean

	make -C ./../../psp_rel/mmm_drv/adc_channel			clean
	make -C ./../../psp_rel/mmm_drv/record_module       clean

	make -C ./../../psp_rel/mmm_drv/music_player		clean
	make -C ./../../psp_rel/mmm_drv/music_pcm_player	clean
	make -C ./../../psp_rel/mmm_drv/music_bt_player     clean
	make -C ./../../psp_rel/mmm_drv/speech_player       clean

	make -C ./../../psp_rel/usermodule/enhanced     clean

	make -C ./../../case/cfg                        clean

