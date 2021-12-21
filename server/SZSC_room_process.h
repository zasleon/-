#ifndef INCLUDE_SZSC_room_process_H
#define INCLUDE_SZSC_room_process_H

#include "server_general_function.h"

void room_broadcast(fight_room* this_room,char* message1)//房间内广播信息
{
	int count;
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL&&this_room->c[count]!=NULL)
		{
			confirm_send_success(this_room->c[count],message1);//统一广播发送这句话
		}
		else
			return;
}


bool check_sb_offline(client_member* p,fight_room* this_room)//检查战斗时是否有人退出,返回true表明有人退出
{
	int count;
	for(count=0;count<playernumber;count++)
	{
		if(this_room->c[count]==NULL||this_room->c[count]->state==in_room)
		{
			this_room->ready_number=0;
			special_signal(p,someone_offline);//有人断线!
			return true;
		}
	}
	return false;
}


void exit_the_room(client_member* member)//用户退出房间
{
	char StrBuf[MAX_PATH] = {0};

	player_room[member->room_No].ready_number=0;
	bool host=false;//是否是房间主人
	client_member* c2;
	fight_room* this_room=&player_room[member->room_No];//确认退出者在哪个房间

	if(!member->guest)//确定为房间主人
		host=true;

	member->state=in_online;//状态全变为初始态
	member->guest=false;
	if(host)//如果是以主人身份退出
	{
		if(this_room->c[1]==NULL)//宾客位如果没人
		{
			this_room->room_in_use=false;//房间解除占用状态
			return;//完成用户退出房间手续
		}
		if(this_room->c[1]->member_No==robot_symbol)//如果说房间里有机器人
		{
			this_room->c[1]=NULL;
			this_room->room_in_use=false;
			return;//完成用户退出房间手续
		}
		else
			this_room->c[0]=this_room->c[1];//房间中有别人，将那个人提为房间主人
	}

	this_room->c[1]=NULL;//房间宾客设为空
	c2=this_room->c[0];//确定房间中另外一个人
	c2->guest=false;//必定成为房间主人
	if(c2==NULL){this_room->room_in_use=false;return;}//如果另外一个人也突然断线，房间事件结束

	SuspendThread(hthread[c2->member_No-1]);//封锁另一个人原本进行的动作
	Sleep(200);
	special_signal(c2,someone_leave);//告诉他：有人从房间离开了
	if(host)special_signal(c2,you_are_host);//如果离开的是房间主人,告诉宾客他成为了房间主人
		
	addtext(StrBuf,member->client_name);addtext(StrBuf,"\n");
	confirm_send_success(c2,StrBuf);//发送离开者名字

	if(c2->device==win_console&&c2->state==in_room)
		special_signal(c2,show_choice);//显示选择菜单
	//c2->state=in_room;//无论在不在游戏状态，都回到房间状态
	ResumeThread(hthread[c2->member_No-1]);//解除另一个人进行的动作
	
}

bool check_empty_room()//检查有没有空房间，有的话返回true，没有则返回false
{
	int count;
	for(count=0;count<roomlimit;count++)
	{
		if(!player_room[count].room_in_use)
		{
			empty_room_pointer=count;
			return true;
		}
	}
	return false;
}
bool check_in_use_room()//检查有没有人在用房间
{
	int count;
	for(count=0;count<roomlimit;count++)
	{
		if(player_room[count].room_in_use)
		{
			return true;//存在有被使用的房间
		}
	}
	return false;//全为空
}
void show_all_room(client_member* member)//显示被使用的房间
{
	char StrBuf[MAX_PATH] = {0};
	char Buf[MAX_PATH] = {0};

	int count;

	for(count=0;count<roomlimit;count++)
		if(player_room[count].room_in_use)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,count+1);
			addtext(StrBuf,"号房间:\t");
			addtext(StrBuf,player_room[count].c[0]->client_name);
			if(player_room[count].c[1]!=NULL)
			{
				addtext(StrBuf,"\t");
				addtext(StrBuf,player_room[count].c[1]->client_name);
				//strcat(StrBuf,"(该房间已满!)");
			}
			addtext(StrBuf,"\n");
			confirm_send_success(member,StrBuf);
		}
		
		special_signal(member,stop_show_room);
}
#endif