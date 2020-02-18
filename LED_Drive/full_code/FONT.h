#ifndef _HEAD_FONT_
#define _HEAD_FONT_

#include<lang.h>
code uchar NUM[][16] = {
// 8 * 16
//0
0xFF,0x1F,0xEF,0xF7,0xF7,0xEF,0x1F,0xFF,
0xFF,0xF0,0xEF,0xDF,0xDF,0xEF,0xF0,0xFF,

//1
0xFF,0xFF,0xEF,0xEF,0x07,0xFF,0xFF,0xFF,
0xFF,0xFF,0xDF,0xDF,0xC0,0xDF,0xDF,0xFF,

//2
0xFF,0x8F,0xF7,0xF7,0xF7,0xF7,0x0F,0xFF,
0xFF,0xCF,0xD7,0xDB,0xDD,0xDE,0xCF,0xFF,

//3
0xFF,0xCF,0xF7,0xF7,0xF7,0x77,0x8F,0xFF,
0xFF,0xE7,0xDF,0xDE,0xDE,0xDD,0xE3,0xFF,

//4
0xFF,0xFF,0x7F,0xBF,0xCF,0x07,0xFF,0xFF,
0xFF,0xF9,0xFA,0xDB,0xDB,0xC0,0xDB,0xDB,

//5
0xFF,0x07,0x77,0x77,0x77,0xF7,0xF7,0xFF,
0xFF,0xE6,0xDF,0xDF,0xDF,0xEE,0xF1,0xFF,

//6
0xFF,0x1F,0xEF,0x77,0x77,0x6F,0xFF,0xFF,
0xFF,0xF0,0xEE,0xDF,0xDF,0xDF,0xE0,0xFF,

//7
0xFF,0xE7,0xF7,0xF7,0x77,0x97,0xE7,0xFF,
0xFF,0xFF,0xFF,0xC1,0xFE,0xFF,0xFF,0xFF,

//8
0xFF,0x8F,0x77,0xF7,0xF7,0x77,0x8F,0xFF,
0xFF,0xE3,0xDD,0xDE,0xDE,0xDD,0xE3,0xFF,

//9
0xFF,0x0F,0xF7,0xF7,0xF7,0xEF,0x1F,0xFF,
0xFF,0xFE,0xED,0xDD,0xDD,0xEE,0xF0,0xFF,
};

code uchar HanZi_16[][32] = {
// 16 * 16
// 智
0xFF,0xEF,0x6B,0xAC,0xCD,0xE1,0xCD,0xAD,0xEF,0xFF,0x81,0xBD,0xBD,0xBD,0x81,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0x00,0xFF,0xFF,0xFF,
// 能
0xFF,0xF7,0x33,0xB5,0xB6,0xB7,0xB5,0x33,0xE7,0xFF,0x80,0x77,0x77,0x7B,0x7D,0x1F,
0xFF,0xFF,0x00,0xED,0xED,0xAD,0x6D,0x80,0xFF,0xFF,0x81,0x77,0x77,0x7B,0x7D,0x1F,
//手
0xFF,0xFF,0xFF,0xDB,0xDB,0xDB,0xDB,0xDB,0x03,0xDD,0xDD,0xDD,0xDC,0xDD,0xFF,0xFF,
0xFF,0xFD,0xFD,0xFD,0xFD,0xFD,0xBD,0x7D,0x80,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,
//表
0xFF,0xFF,0xFB,0xDB,0xDB,0xDB,0xDB,0xDB,0x00,0xDB,0xDB,0xDB,0xDB,0xDB,0xFB,0xFF,
0xFF,0xDE,0xDE,0xEE,0xF6,0x02,0x7C,0xBE,0xDC,0xFA,0xF6,0xEE,0xD6,0xDA,0xBE,0xBE,
//星
0xFF,0xFF,0xFF,0xFF,0x41,0xD5,0xD5,0xD5,0x15,0xD5,0xD5,0xD5,0xC1,0xFF,0xFF,0xFF,
0xFF,0xFF,0xBB,0xBD,0xB6,0xB6,0xB6,0xB6,0x80,0xB6,0xB6,0xB6,0xB6,0xBE,0xBF,0xFF,
//期
0xFF,0xFF,0xFB,0x00,0xDB,0xDB,0xDB,0x00,0xFB,0xFF,0x01,0xDD,0xDD,0xDD,0x01,0xFF,
0xFF,0x77,0xB7,0xD0,0xF6,0xF6,0xE6,0x50,0xB7,0xCF,0xF0,0xFD,0xBD,0x7D,0x80,0xFF,

//温
0xFF,0xEF,0x9F,0xFD,0x73,0xFF,0xFF,0x01,0x6D,0x6D,0x6D,0x6D,0x6D,0x01,0xFF,0xFF,
0xFF,0xFB,0xFB,0x81,0xFE,0xBF,0x81,0xBD,0xBD,0x81,0xBD,0x81,0xBD,0xBD,0x81,0xBF,
//度
0xFF,0xFF,0xFF,0x03,0xDB,0xDB,0xDB,0x03,0xDA,0xD9,0xDB,0x03,0xDB,0xDB,0xDB,0xFB,
0xFF,0xBF,0xCF,0x70,0x7F,0x7B,0xB3,0xAA,0xDA,0xDA,0xDA,0xAA,0xB3,0x7F,0x7F,0x7F,

//：
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xC9,0xC9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//度 符号
0xFF,0xF9,0xF6,0xF6,0x19,0x07,0xF3,0xFB,0xFD,0xFD,0xFD,0xFD,0xFD,0xFB,0xE1,0xFF,
0xFF,0xFF,0xFF,0xFF,0xF8,0xE0,0xCF,0xDF,0xBF,0xBF,0xBF,0xBF,0xBF,0xDF,0xEF,0xFF,

//↓
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xF3,0xC7,0x00,0xC7,0xF3,0xFD,0xFF,0xFF,0xFF,0xFF

};

code uchar Sing[][16] = {
// 8 * 16

// :
0xFF,0xFF,0xFF,0x7F,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x9E,0x9E,0xFF,0xFF,0xFF,
// -
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
// c
0xFF,0x7F,0x9F,0xEF,0xEF,0xEF,0xEF,0x8F,
0xFF,0xE0,0xDF,0xBF,0xBF,0xBF,0xDF,0xEF,

// o
0xFF,0xC7,0xBB,0x7D,0x7D,0x7D,0x7D,0xBB,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

};
code uchar RiZi[][32] = {
// 年
0xFF,0xFF,0xDF,0xE7,0x38,0xBB,0xBB,0xBB,0xBB,0x03,0xBB,0xBB,0xBB,0xBB,0xFB,0xFF,
0xFF,0xFB,0xFB,0xFB,0xF8,0xFB,0xFB,0xFB,0xFB,0x00,0xFB,0xFB,0xFB,0xFB,0xFB,0xFB,

// 月
0xFF,0xFF,0xFF,0xFF,0x01,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x01,0xFF,0xFF,
0xFF,0x7F,0xBF,0xCF,0xF0,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xBD,0x7D,0x80,0xFF,0xFF,

// 日
0xFF,0xFF,0xFF,0xFF,0x01,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x01,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0x00,0xFF,0xFF,0xFF,

// 时
0xFF,0xFF,0x03,0x7B,0x7B,0x7B,0x03,0xFF,0xEF,0xEF,0xEF,0xEF,0xEF,0x00,0xEF,0xEF,
0xFF,0xFF,0xC0,0xEF,0xEF,0xEF,0xC0,0xFF,0xFF,0xFE,0xF9,0xBF,0x7F,0x80,0xFF,0xFF,
// 分
0xFF,0x7F,0xBF,0xDF,0x6F,0x77,0x79,0x7F,0x7F,0x7F,0x7C,0x73,0xEF,0xDF,0xBF,0x7F,
0xFF,0xFF,0x7F,0xBF,0xDF,0xE7,0xF8,0xFF,0xBF,0x7F,0xBF,0xC0,0xFF,0xFF,0xFF,0xFF,
// 秒
0xFF,0xDB,0xDB,0x5B,0x01,0xDC,0xDD,0xFF,0x3F,0xC7,0xFF,0x00,0xFF,0xF7,0xEF,0x9F,
0xFF,0xF7,0xF9,0xFE,0x00,0xFE,0xF9,0x7E,0x7F,0xBF,0xBF,0xD8,0xEF,0xF3,0xFC,0xFF,
};

code uchar HZ_Num[][32] = {
//一
0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//二
0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xFF,0xFF,
0xFF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,
//三
0xFF,0xFF,0xFB,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0xFB,0xFF,
0xFF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
//四
0xFF,0xFF,0x03,0xFB,0xFB,0xFB,0x03,0xFB,0xFB,0xFB,0x03,0xFB,0xFB,0xFB,0x03,0xFF,
0xFF,0xFF,0x80,0xD7,0xDB,0xDC,0xDF,0xDF,0xDF,0xDF,0xDE,0xDD,0xDD,0xDD,0x80,0xFF,
//五
0xFF,0xFF,0xFD,0xBD,0xBD,0xBD,0x3D,0x81,0xBD,0xBD,0xBD,0xBD,0x3D,0xFD,0xFD,0xFF,
0xFF,0xBF,0xBF,0xBF,0xBF,0x87,0xB8,0xBF,0xBF,0xBF,0xBF,0xBF,0x80,0xBF,0xBF,0xBF,
//六
0xFF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDE,0xDD,0xD3,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0xFF,0xFF,0xBF,0xDF,0xEF,0xF3,0xFC,0xFF,0xFF,0xFF,0xFE,0xFD,0xFB,0xE7,0x9F,0xFF,
//日
0xFF,0xFF,0xFF,0xFF,0x01,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x01,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0x00,0xFF,0xFF,0xFF,
};
#endif