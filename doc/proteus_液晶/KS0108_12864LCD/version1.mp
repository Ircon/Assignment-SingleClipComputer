NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   0054   0AE1 =   2785. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  _icon1_char_table
       0054  __lit_start
       0055  _icon1_bmp
       0059  _frame_char_table
       005A  _frame_bmp
       005E  _fonts
       0072  _six_dot_glyph_table
       0662  _six_dot_width_table
       06C1  _nine_dot_glyph_table
       0B35  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0B35   046D =   1133. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0B35  __idata_start
       0FA2  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0FA2   0F74 =   3956. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       07D1  __text_start
       07D1  __start
       07F1  _exit
       07F2  _port_init
       07FD  _init_devices
       0805  _project_init
       080B  _main
       08E1  __delay_us
       08E7  __delay_ms
       08FA  _pgm_read_byte
       08FD  _pgm_read_word
       0901  _lcd_bitmap
       094B  _lcd_glyph
       09CD  _lcd_text
       0A98  _lcd_text_p
       0B64  _lcd_fill
       0B82  _lcd_erase
       0B86  _lcd_update
       0BBD  _lcd_update_all
       0BC1  _lcd_dot
       0C05  _lcd_line
       0D4A  _lcd_circle
       0DE7  _lcd_rect
       0E19  _lcd_box
       0E3A  _lcd_out_dat_l
       0E4C  _lcd_out_dat_r
       0E5E  _lcd_out_ctl_l
       0E70  _lcd_out_ctl_r
       0E82  _lcd_out_ctl
       0E97  _lcd_out_dat
       0EAC  _choose_left_screen
       0EAF  _choose_right_screen
       0EB2  _choose_full_screen
       0EB5  _choose_no_screen
       0EB8  _lcd_read_byte
       0EBA  _lcd_is_busy
       0EC3  _lcd_init
       0ECD  _write_rect
       0ECE  _read_rect
       0ECF  _write_to_xy
       0EEE  _set_xy
       0F09  _write_to_lcd
       0F11  push_arg4
       0F13  push_arg2
       0F16  mod16s
       0F1A  div16s
       0F34  mod16u
       0F36  div16u
       0F37  xdiv16u
       0F50  empy16s
       0F60  pop_gset2
       0F63  pop_gset3
       0F66  pop_gset4
       0F69  pop_gset5
       0F6C  push_gset5
       0F6E  push_gset4
       0F70  push_gset3
       0F72  push_gset2
       0F74  push_gset1
       0F77  pop_gset1
       0F78  pop
       0F8B  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   046D =   1133. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       00C5  _l_mask_array
       00CD  _l_display_array
       04CD  __data_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0004 =      4. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\icc\lib\crtatmega.o	[ crtatmega.s ]
main.o	[ main.c ]
m32macro.o	[ m32macro.c ]
bitmap.o	[ bitmap.c ]
font.o	[ font.c ]
graphic.o	[ graphic.c ]
LCD_DRIVER.o	[ LCD_DRIVER.c ]
<library>	[ asave.s, div16s.s, div16u.s, emul16s.s, gpop2.s, gpop3.s, gpop4.s, gpop5.s, gpush.s ]

User Global Definitions

ram_end = 0x85f
hwstk_size = 0x10

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:1.1024

