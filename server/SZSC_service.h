#ifndef INCLUDE_SZSC_service_H
#define INCLUDE_SZSC_service_H

#include "start_one_fight.h"

void SZSC_provide_service(client_member* c1,int choice)
{
	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	char Buf[MAX_PATH] = { 0 };//专门用于转换非字符形式数值为字符
	int count;
	switch(choice)
	{
		case SZSC_apply_create_room://创建房间
				if(c1->device!=android_phone)confirm_send_success(c1,"您选择了创建房间!\n");
				if(c1->state==in_room)
				{confirm_send_success(c1,"你已在房间内!无法继续创建!\n");break;}
				if(!check_empty_room())//检查是否有空房间
				{confirm_send_success(c1,"当前房间已满，无法继续创建!\n");break;}
				
				player_room[empty_room_pointer].room_in_use=true;
				player_room[empty_room_pointer].c[0]=c1;
				c1->state=in_room;
				c1->room_No=empty_room_pointer;

				special_signal(c1,get_in_room);//创建成功!
					
				show_room_state();
				break;
			case SZSC_apply_enter_room:
				if(c1->device!=android_phone)confirm_send_success(c1,"您选择了加入房间!\n");
				if(c1->state!=in_online)
				{confirm_send_success(c1,"当前状态无法继续加入房间!\n");break;}
				if(!check_in_use_room())
				{special_signal(c1,no_room_in_use);break;}
				//{confirm_send_success(c1,"当前没有使用正在使用的房间!请重新选择!\n");continue;}

				special_signal(c1,please_choose_room);//表明可以选择房间了

				show_all_room(c1);
				while(true)//选择房间死循环开始
				{				

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//玩家做出选择，如果此时断开通讯

					cout<<"用户输入："<<StrBuf<<endl;
					if(strlen(StrBuf)==1&&StrBuf[0]=='0')
					{	
						special_signal(c1,cancel_enter_room);//你选择取消加入房间!即将返回原页面……
						break;
					}
						
					choice=atoi(StrBuf);//转换数据为数字
						
					if(choice==0){confirm_send_success(c1,"输入了无效值!\n");continue;}
					if(choice>roomlimit||choice<0){confirm_send_success(c1,"输入超出房间数量上限!\n");continue;}

					choice--;

					if(player_room[choice].room_in_use==false){confirm_send_success(c1,"该房间内没人!\n");continue;}
					if(player_room[choice].c[playernumber-1]!=NULL){confirm_send_success(c1,"该房间内人满!\n");continue;}
							
					c1->state=in_room;//执行到这一步必然加入成功
					c1->guest=true;//加入房间的人是房间客人

					player_room[choice].c[1]=c1;
					c1->room_No=player_room[choice].c[0]->room_No;

					special_signal(c1,enter_room_success);//加入成功!

					SuspendThread(hthread[player_room[choice].c[0]->member_No-1]);//锁住房间主人监听线程
					special_signal(player_room[choice].c[0],someone_get_in);//告诉房间主人有人进来了
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//告诉房间主人进来者姓名
					confirm_send_success(player_room[choice].c[0],StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					if(player_room[choice].c[0]->device==win_console)special_signal(player_room[choice].c[0],show_choice);//如果是win_console客户端，显示选项
					ResumeThread(hthread[player_room[choice].c[0]->member_No-1]);//解放房间主人监听线程
					show_room_state();//服务器自己显示现在房间状态
					break;
			
				}//选择房间死循环结束
				break;
			case SZSC_apply_exit_room:
				if(c1->device==win_console)confirm_send_success(c1,"您选择了退出房间!\n");
				if(c1->state==in_game){confirm_send_success(c1,"正在游戏中!无法退出房间!\n");break;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//你当前没在房间内!请重新选择

				exit_the_room(c1);//执行退出
				special_signal(c1,leave_room);//告诉他退出成功!
				show_room_state();//服务器自己显示现在房间状态
				break;
			case SZSC_apply_start_game:
				if(c1->device==win_console)confirm_send_success(c1,"您选择了开始战斗!\n");

				if(c1->state==in_game){confirm_send_success(c1,"你已在游戏中!无法开始新的!\n");break;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//你不在房间内!无法开始战斗!
				if(c1->guest){confirm_send_success(c1,"你不是房间主人!无法开始战斗!\n");break;}//如果不是房间主人
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"房间内人数不足!无法开始战斗!\n");break;}//如果房间没有宾客
				
				//执行到这里，已经确定在房间中当主人，而且有宾客
				for(count=0;count<playernumber;count++)
					if(player_room[c1->room_No].c[count]!=NULL)
					{
						player_room[c1->room_No].c[count]->state=in_game;
						if(count!=0)special_signal(player_room[c1->room_No].c[count],event_happen_N);//由于他人还卡在server服务端，用event_happen_N激活一下进入战斗函数
					}
					else
					{
						for(count=0;count<playernumber;count++)
							if(player_room[c1->room_No].c[count]!=NULL)
								player_room[c1->room_No].c[count]->state=in_room;
						confirm_send_success(c1,"房间内人数不足!无法开始战斗!\n");break;
					}
				if(count!=playernumber)break;

				start_fight(c1,&player_room[c1->room_No]);//执行完表示游戏结束
				c1->state=in_room;//游戏结束，回到房间状态
				break;
			case SZSC_apply_add_robot://添加机器人
				if(c1->state!=in_room)//确定不是在游戏中，如果不在房间内
				{special_signal(c1,you_not_in_room);break;}//你不在房间内!
				if(player_room[c1->room_No].c[0]->member_No!=c1->member_No){confirm_send_success(c1,"只有房间主人才能添加机器人!\n");break;}	
				if(player_room[c1->room_No].c[1]!=NULL){confirm_send_success(c1,"房间内已有别人!无法添加!\n");break;}
				player_room[c1->room_No].c[1]=&Robot[c1->room_No];
				confirm_send_success(c1,"添加成功!!!\n");
				break;
			case SZSC_apply_remove_robot://移除机器人
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//如果不在房间内
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"房间内没有机器人!无法移除!\n");break;}
				if(player_room[c1->room_No].c[1]->member_No==c1->member_No){confirm_send_success(c1,"你是房间客人，只有房间主人有权限修改机器人!\n");break;}
				if(player_room[c1->room_No].c[1]->member_No!=robot_symbol){confirm_send_success(c1,"房间内有活人!无法移除机器人!\n");break;}
				player_room[c1->room_No].c[1]=NULL;//进行移除
				confirm_send_success(c1,"移除成功!\n");
				break;
	}
}



#endif