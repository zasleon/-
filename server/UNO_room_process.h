#ifndef INCLUDE_UNO_room_process_H
#define INCLUDE_UNO_room_process_H

#include"server_general_function.h"

void UNO_ini_room()//所有房间初始化
{
	UNO_empty_room_pointer=0;//指向第一个
	int i,j;
	for(i=0;i<UNO_room_limit;i++)
	{
		uno_room[i].room_member=0;//房间内人数为0
		uno_room[i].who_is_master=-1;//房间为空
		for(j=0;j<UNO_member_limit;j++)
		{
			uno_room[i].player[j].state=UNO_empty;//位子状态设为空
			
		}
	}
}

void UNO_create_room(client_member* c1)//用户创建房间
{
	if(UNO_room_in_use==UNO_room_limit)//如果房间全满
	{special_signal(c1,UNO_room_full);return;}
	if(c1->state!=in_online)//当前状态无法创建uno房间!
	{special_signal(c1,UNO_cannot_create_room);return;}

	//到这一步必然能创建房间
	int this_room_number=UNO_empty_room_pointer;
	
	while(uno_room[this_room_number].who_is_master!=-1)
	{
		cout<<"UNO 错误！！！\n";
		Sleep(100);
		this_room_number=UNO_empty_room_pointer;
		if(UNO_room_in_use==UNO_room_limit)//创建失败!
		{special_signal(c1,UNO_room_full);return;}
	}

	uno_room[this_room_number].who_is_master=0;//当前房间房主设置为位置0者
	UNO_room_in_use++;

	int temp=UNO_empty_room_pointer;
	if(UNO_room_in_use!=UNO_room_limit)//如果房间没满
		while(true)//搜寻下一个空房间，赋予UNO_empty_room_pointer
		{
			temp++;//看下一个房间
			if(temp==UNO_room_limit)temp=0;//若到达最后一个房间，从头开始找

			if(uno_room[temp].who_is_master==-1)//如果该房间为空
			{
				UNO_empty_room_pointer=temp;//确认房间号，赋予UNO_empty_room_pointer
				break;//跳出循环
			}
		
		}

	uno_room[this_room_number].player[0].c1=c1;
	uno_room[this_room_number].player[0].state=UNO_human;//该位子状态设置为人类玩家
	c1->uno_player=&uno_room[this_room_number].player[0];
	c1->room_No=this_room_number;//确认玩家所在房间号
	special_signal(c1,UNO_create_room_success);//加入成功!
	c1->state=UNO_in_room;//更新玩家状态为“在uno房间内”
	uno_room[this_room_number].room_member++;//房间内人数为1
	
}


void UNO_show_room_in_use(client_member* c1)//向他展示所有能加入的房间
{
	special_signal(c1,UNO_show_roomlist);
	char StrBuf[MAX_PATH] = {0};
	char Buf[MAX_PATH] = {0};

	int count;
	for(count=0;count<UNO_room_limit;count++)
		if(uno_room[count].who_is_master!=-1)
		{
			memset(StrBuf,0,sizeof(StrBuf));//房间号+房主姓名+人数
			addtext(StrBuf,count+1);//号房间
			addtext(StrBuf,"\t");
			addtext(StrBuf,uno_room[count].player[uno_room[count].who_is_master].c1->client_name);
			addtext(StrBuf,"\t");
			addtext(StrBuf,uno_room[count].room_member);
			//strcat(StrBuf,"(该房间已满!)");
			
			addtext(StrBuf,"\n");
			confirm_send_success(c1,StrBuf);
		}
		
		special_signal(c1,UNO_show_roomlist_end);
}

void UNO_enter_room(client_member* c1)//用户进入别人的房间
{

	if(UNO_room_in_use==0)//如果没有使用着的房间
	{special_signal(c1,UNO_no_room_in_use);return;}
	if(c1->state!=in_online)//当前状态无法进入别人的uno房间!
	{special_signal(c1,UNO_cannot_enter_room);return;}

	//执行到这里必然能选择别人的房间并加入
	c1->state=UNO_in_roomlist;
	UNO_show_room_in_use(c1);//向他展示所有能加入的房间

	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	int choice;//存储玩家作出的选择
	while(true)//选择房间死循环
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//玩家做出选择，如果此时断开通讯

		cout<<"用户输入："<<StrBuf<<endl;
		if(atoi(StrBuf)==UNO_apply_exit_roomlist)//选择取消加入房间!即将返回原页面……
		{
			special_signal(c1,UNO_cancel_enter_room);
			c1->state=in_online;
			return;
		}
		
						
		choice=atoi(StrBuf);//转换数据为数字
						
		if(choice==0)//confirm_send_success(c1,"输入了无效值!\n");
		{continue;}
		if(choice>UNO_room_limit||choice<0)//confirm_send_success(c1,"输入超出房间数量上限!\n");
		{continue;}

		choice--;

		if(uno_room[choice].who_is_master==-1)//confirm_send_success(c1,"该房间内没人!\n");
		{continue;}
		if(uno_room[choice].room_member==UNO_member_limit)//confirm_send_success(c1,"该房间内人满!\n");
		{continue;}
							
		c1->state=UNO_in_room;//执行到这一步必然加入成功
		uno_room[choice].room_member++;//房间内人数+1
		c1->guest=true;//加入房间的人是房间客人
		c1->room_No=choice;
		c1->state=UNO_in_room;//更新玩家状态为“在uno房间内”

		for(int i=0;i<UNO_member_limit;i++)
			if(uno_room[choice].player[i].state==UNO_empty)//如果当前位子为空
			{
				uno_room[choice].player[i].state=UNO_human;//该位子状态设置为人类玩家
				c1->uno_player=&uno_room[choice].player[i];//互相标识
				uno_room[choice].player[i].c1=c1;//互相标识
				GetLocalTime(&c1->uno_player->enter_time);//截取当前时间，用于房主变动
				break;
			}

		c1->room_No=choice;//保存房间号信息

		special_signal(c1,UNO_enter_room_success);//加入成功!
		

		for(int i=0;i<UNO_member_limit;i++)//进行广播有人加入房间
		{
			client_member* c2;
			if(uno_room[choice].player[i].state!=UNO_human)//如果当前位子上不是活人，跳下一个
				continue;
			if(uno_room[choice].player[i].c1->member_No!=c1->member_No)//如果当前位子上不是本人
				c2=uno_room[choice].player[i].c1;
			else
				continue;

			SuspendThread(hthread[c2->member_No-1]);//锁住他的监听线程
			special_signal(c2,UNO_someone_get_in);//告诉房间有人进来了
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//告诉房间主人进来者姓名
			confirm_send_success(c2,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			ResumeThread(hthread[c2->member_No-1]);//解放他的监听线程

		}

		break;//执行到这里说明加入成功且结算结束
	}//选择房间死循环

}

bool compare_enter_time(SYSTEMTIME challenger,SYSTEMTIME former_master)//返回true表示前者比后者大
{
	bool result=false;//不比房主候补早
	if(challenger.wYear<former_master.wYear)
		result=true;
	else
	{
		if(challenger.wMonth<former_master.wMonth)
			result=true;
		else
		{
			if(challenger.wDay<former_master.wDay)
				result=true;
			else
			{
				if(challenger.wHour<former_master.wHour)
					result=true;
				else
				{
					if(challenger.wMinute<former_master.wMinute)
						result=true;
					else
						if(challenger.wMilliseconds<former_master.wMilliseconds)
							result=true;
				}
			}
		}
	}
	return result;
}

void UNO_exit_room(client_member* c1)//用户退出房间
{
	char StrBuf[MAX_PATH] = { 0 };

	if(c1->state!=UNO_in_room)//当前状态无法退出uno房间!
	{special_signal(c1,UNO_cannot_exit_room);return;}

	for(int i=0;i<UNO_member_limit;i++)
		if(uno_room[c1->room_No].player[i].c1!=NULL&&uno_room[c1->room_No].player[i].c1->member_No==c1->member_No)//找到要退出者
		{
			
			if(uno_room[c1->room_No].who_is_master==i)//如果退出者为房间主人,更换房主措施
			{
				int who_will_be_the_master=-1;

				for(int j=UNO_member_limit-1;j>=0;j--)
					if(i!=j&&uno_room[c1->room_No].player[j].state==UNO_human)//对所有位子检查，若当前位置上是活人
					{
						if(who_will_be_the_master==-1)//如果当前还没确认过房主候补，将当前位置上的人作为房主候补
							who_will_be_the_master=j;
						else//如果有房主候补了，比较两者的进入房间时间
						{
							if(compare_enter_time(uno_room[c1->room_No].player[j].enter_time,uno_room[c1->room_No].player[who_will_be_the_master].enter_time))
								who_will_be_the_master=j;
						}

					}
				uno_room[c1->room_No].who_is_master=who_will_be_the_master;//确认新房主
				if(who_will_be_the_master!=-1)//若当前房间不为空
					uno_room[c1->room_No].player[who_will_be_the_master].c1->guest=false;
				else//没人了,该房间没人使用
				{
					for(int k=0;k<UNO_member_limit;k++)
					{
						uno_room[i].player[k].state=UNO_empty;//位子状态设为空
					}
					UNO_room_in_use--;//总房间使用数量-1
				}
			}//房主变更手续结束
			for(int i=0;i<UNO_member_limit;i++)//进行广播有人加入房间
			{
				client_member* c2;
				if(uno_room[c1->room_No].player[i].state!=UNO_human)//如果当前位子上不是活人，跳下一个
					continue;
				if(uno_room[c1->room_No].player[i].c1->member_No!=c1->member_No)//如果当前位子上不是本人
					c2=uno_room[c1->room_No].player[i].c1;
				else
					continue;

				SuspendThread(hthread[c2->member_No-1]);//锁住他的监听线程
				special_signal(c2,UNO_someone_get_in);//告诉房间有人进来了
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//告诉房间主人进来者姓名
				confirm_send_success(c2,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				ResumeThread(hthread[c2->member_No-1]);//解放他的监听线程

			}
			uno_room[c1->room_No].player[i].c1=NULL;//两者取消关联
			c1->uno_player=NULL;
			uno_room[c1->room_No].room_member--;//房间内人数-1
			c1->state=in_online;//更改用户状态为“在线”
			c1->guest=false;
			uno_room[c1->room_No].player[i].state=UNO_empty;//当前位子状态设置为空
			special_signal(c1,UNO_leave_room_success);//离开成功!
			
			
			
			
			break;
		}


}



#endif