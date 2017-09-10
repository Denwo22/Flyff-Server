#ifndef __APPDEFINE_H__
#define __APPDEFINE_H__

#include "ResData.h"           // 윈도 고유 ID 모음 

const int APP_SHOP_   = APP_SHOPEX;
const int  APP_CONFIRM_BUY_ = APP_CONFIRM_BUYEX;

#if __VER >= 11 // __CSC_VER11_4
const int  APP_MESSENGER_   = APP_MESSENGER_EX;
#else //__CSC_VER11_4
const int  APP_MESSENGER_   = APP_MESSENGER;
#endif //__CSC_VER11_4

#endif