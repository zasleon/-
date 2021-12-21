#ifndef INCLUDE_UNO_service_H
#define INCLUDE_UNO_service_H

#include "UNO_deck_process.h"

void UNO_provide_service(client_member* c1,int choice)
{
	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	char Buf[MAX_PATH] = { 0 };//专门用于转换非字符形式数值为字符
	
	switch(choice)
	{
		case UNO_apply_create_room:
			UNO_create_room(c1);
			break;
		case UNO_apply_enter_room:
			UNO_enter_room(c1);
			break;
		case UNO_apply_exit_room:
			UNO_exit_room(c1);
			break;
	}
}

#endif