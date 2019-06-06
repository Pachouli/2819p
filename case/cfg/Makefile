#定义伪目标
.PHONY : all
.PHONY : clean

all:
	make -C ./../drv/general_drv -j16
	make -C ./../drv/welcome -j16

	make -C ./../ap/common -j16
	make -C ./../ap/ap_manager -j16
	make -C ./../ap/ap_config -j16

	make -C ./../ap/ap_btplay -j16
	make -C ./../ap/btplay_engine -j16
	make -C ./../ap/ap_btcall -j16
	make -C ./../ap/btcall_engine -j16
	make -C ./../ap/bluetooth_engine -j16
	make -C ./../ap/ap_bthid -j16
	
	make -C ./../ap/ap_music -j16
	make -C ./../ap/music_engine -j16
		
	make -C ./../ap/ap_linein -j16
	make -C ./../ap/linein_engine -j16
	
	make -C ./../ap/fm_engine -j16
	make -C ./../ap/ap_radio -j16
	
	make -C ./../ap/ap_usound -j16
	make -C ./../ap/usound_engine -j16
	
	make -C ./../ap/ap_cardreader -j16
	
	make -C ./../ap/ap_alarm -j16
	make -C ./../ap/ap_upgrade -j16
	
	make -C ./../ap_test/ATT -j16
		
clean:
	make -C ./../drv/general_drv clean
	make -C ./../drv/welcome clean
	
	make -C ./../ap/common clean
	make -C ./../ap/ap_manager clean
	make -C ./../ap/ap_config clean	
	
	make -C ./../ap/ap_btplay clean	
	make -C ./../ap/btplay_engine clean
	make -C ./../ap/ap_btcall clean
	make -C ./../ap/btcall_engine clean
	make -C ./../ap/bluetooth_engine clean	
	make -C ./../ap/ap_bthid clean
	
	make -C ./../ap/ap_music clean		
	make -C ./../ap/music_engine clean
	
	make -C ./../ap/ap_linein clean		
	make -C ./../ap/linein_engine clean
	
	make -C ./../ap/fm_engine clean
	make -C ./../ap/ap_radio  clean	
	
	make -C ./../ap/ap_usound  clean
	make -C ./../ap/usound_engine clean
	
	make -C ./../ap/ap_cardreader clean
	
	make -C ./../ap/ap_alarm clean
	make -C ./../ap/ap_upgrade clean
	
	make -C ./../ap_test/ATT clean
