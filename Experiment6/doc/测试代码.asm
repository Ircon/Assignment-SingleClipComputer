;*************************************
;CONTROLLER:ST7920(8BIT INTERFACE) *
;EDITOR:张炎 *
;MCU=89C52
;CRYSTAL:12MHz *
;RS=P2.0 RW=P2.1 E=/(WR*RD) *
;DB0-DB7=P0 *
;*************************************
 DATWP EQU 0100H ;写数据通道
 DATRP EQU 0300H ;读数据通道
 COMWP EQU 0000H ;写数据通道
 COMRP EQU 0200H ;读忙通道

 COM EQU 30H ;控制字暂存单元
 DAT EQU 31H ;显示数据暂存单元
 CODER EQU 32H ;字符代码暂存单元
 ADDR EQU 33H ;地址暂存单元
 ORG 0000H
 AJMP MAIN
 ORG 0030H
MAIN:
 MOV SP,#60H
 LCALL DEL_20MS
 LCALL DEL_20MS
 LCALL INI ;进入初始化子程序
 MOV ADDR,#80H
 MOV DPTR,#WEL_1
 LCALL W_LINE
 MOV ADDR,#90H
 MOV DPTR,#WEL_2
 LCALL W_LINE
 MOV ADDR,#88H
 MOV DPTR,#WEL_3
 LCALL W_LINE
 MOV ADDR,#98H
 MOV DPTR,#WEL_4
 LCALL W_LINE
 LCALL DEL_1500MS
 MOV ADDR,#80H
 MOV DPTR,#WEL_5
 LCALL W_LINE
 MOV ADDR,#90H
 MOV DPTR,#WEL_6
 LCALL W_LINE
 MOV ADDR,#88H
 MOV DPTR,#WEL_7
 LCALL W_LINE
 MOV ADDR,#98H
 MOV DPTR,#WEL_8
 LCALL W_LINE
 LCALL DEL_1500MS
 MOV ADDR,#80H ;
 MOV DPTR,#HZ_1 ;
 LCALL W_LINE
 MOV ADDR,#90H ; 

 MOV DPTR,#HZ_2 ;
 LCALL W_LINE
 MOV ADDR,#88H ;
 MOV DPTR,#HZ_3 ;
 LCALL W_LINE
 MOV ADDR,#98H ;
 MOV DPTR,#HZ_4 ;
 LCALL W_LINE
 LCALL DEL_1500MS ;延时后显示下一页
 MOV ADDR,#80H ;
 MOV DPTR,#HZ_5 ;
 LCALL W_LINE
 MOV ADDR,#90H ;
 MOV DPTR,#HZ_6 ;
 LCALL W_LINE
 MOV ADDR,#88H ;
 MOV DPTR,#HZ_7 ;
 LCALL W_LINE
 MOV ADDR,#98H ;
 MOV DPTR,#HZ_8 ;
 LCALL W_LINE
 LCALL DEL_1500MS
 LJMP MAIN
INI:
 MOV COM,#30H ;功能设定,基本指令
 LCALL WCOM
 MOV COM,#30H ;基本指令,8-bit 模式,基本指令
 LCALL WCOM
 MOV COM,#0CH ;显示开,游标关,反白关
 LCALL WCOM
 MOV COM,#01H ;清除显示
 LCALL WCOM
 MOV COM,#06H ;进入设定点,游标 7 右移,画面不移动
 LCALL WCOM
 RET
 
W_LINE:
 MOV COM,ADDR ;
 LCALL WCOM ;
 MOV R2,#16 ;16 个循环,连续写入 8 个汉字或是 16 个西文字符
W_L1: 
 MOV A,#00H
 MOVC A,@A+DPTR
 MOV CODER,A
 LCALL WCODE
 INC DPTR
 DJNZ R2,W_L1
 RET
 
 
WCOM:
 PUSH DPH
 PUSH DPL
 MOV DPTR,#COMRP
WCOM1:
 MOVX A,@DPTR
 JB ACC.7,WCOM1
 MOV DPTR,#COMWP
 MOV A,COM
 MOVX @DPTR,A
 POP DPL
 POP DPH
 RET
 
WCODE:
 PUSH DPH
 PUSH DPL
 MOV DPTR,#COMRP
WCODE1:
 MOVX A,@DPTR
 JB ACC.7,WCODE1
 MOV DPTR,#DATWP
 MOV A,CODER
 MOVX @DPTR,A
 POP DPL
 POP DPH
 RET
DEL_20MS:
 MOV R0,#20
D2:
 MOV R1,#200
D1:
 NOP
 NOP
 NOP 
 DJNZ R1,D1
 DJNZ R0,D2
 RET
DEL_100MS:
 LCALL DEL_20MS
 LCALL DEL_20MS
 LCALL DEL_20MS
 LCALL DEL_20MS
 LCALL DEL_20MS
 RET
DEL_500MS:
 LCALL DEL_100MS
 LCALL DEL_100MS
 LCALL DEL_100MS
 LCALL DEL_100MS
 LCALL DEL_100MS
 RET
DEL_1500MS:
 LCALL DEL_500MS
 LCALL DEL_500MS
 LCALL DEL_500MS
 RET
WEL_1:
 DB "欢迎使用人和液晶"
WEL_2:
 DB "WWW.GZLIYEDZ.COM"
WEL_3:
 db "020--87571530"
WEL_4:
 DB "128X64 全点阵显示"
WEL_5:
 DB "图形以及字符显示"
WEL_6:
 DB "可显示 8X4 个中文"
WEL_7:
 DB "内带标准中文字库"
WEL_8:
 DB "含 8192 个中文字型"
HZ_1: 
 DB "锦瑟无端五十弦，"
HZ_2:
 DB "一弦一柱思华年。"
HZ_3:
 DB "庄生晓梦迷蝴蝶，"
HZ_4:
 DB "望帝春心托杜鹃。"
HZ_5:
 DB "沧海月明珠有泪，"
HZ_6:
 DB "蓝田日暖玉生烟。"
HZ_7:
 DB "此情可待成追忆，"
HZ_8:
 DB "只是当时已惘然。"
END 