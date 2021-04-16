#ifndef INCLUDE_deck_card_H
#define INCLUDE_deck_card_H

#include "special_transmit.h"

using namespace std;
//�ֿ��䶯���鿨��������ʹ�ÿ�
//������ʹ�ÿ���refresh
//�鿨��refresh��card_change��Ϣ����
//card_change����ʱ������Ч��
//��������������������������������������������������ר�ŶԿ��顢�鿨�������������������������������������������������������

int random_card(int i)//�����������������迨Ƭ��ţ����ֿ�Ƭ�鵽��Ȩ�ز�ͬ�������ʵ�����
{
switch(i)
	{
	case 0:;
    case 1:return 60;;
	case 2:
	case 3:;
	case 4:return 30;
	case 5:;
	case 6:;
	case 7:return 61;;
	case 8:
	case 9:;
	case 10:return 0;
	case 11:;
	case 12:;
	case 13:return 62;
	case 14:;
	case 15:;
	case 16:return 31;
	case 17:;
	case 18:
	case 19:return 64;;
	case 20:
	case 21:;
	case 22:return 1; ;
	case 23:
	case 24:;
	case 25:return 63;
	case 26:;
	case 27:
	case 28:return 32;
	case 29:;
	case 30:return 65;;
	case 31:
	case 32:;
	case 33:return 2;;
	case 34:
	case 35:
	case 36:return 66;;;
	case 37:
	case 38:
	case 39:return 33;;;
	case 40:
	case 41:return 3;;
	case 42:;
	case 43:
	case 44:return 77;;;
	case 45:;
	case 46:
	case 47:return 34;;
	case 48:;
	case 49:;
	case 50:return 4;
	case 51:;
	case 52:;
	case 53:return 70;
	case 54:;
	case 55:;
	case 56:return 35;
	case 57:;
	case 58:;
	case 59:return 5;;
	case 60:
	case 61:;
	case 62:return 71;
	case 63:;
	case 64:return 72;
	case 65:;
	case 66:
	case 67:return 36;;
	case 68:
	case 69:;
	case 70:return 6;
	case 71:;
	case 72:;
	case 73:return 73;
	case 74:;
	case 75:return 37;
	case 76:;
	case 77:;
	case 78:return 75;
	case 79:;
	case 80:
    case 81:return 7;
	case 82:;
	case 83:;
	case 84:return 76;
	case 85:;
	case 86:;
	case 87:return 38;
	case 88:;
	case 89:;
	case 90:return 8;
    case 91:;
	case 92:;
	case 93:return 74;;
	case 94:;
	case 95:return 39;
	case 96:;
	case 97:;
	case 98:return 40;
	case 99:;
	case 100:return 9;
	default:return 0;


	}
}



void refresh_hideeffect(client_member* c1)//���¿�Ƭ��Ч��
{
	int count;
	for(count=0;count<cardlimit;count++)
		if(c1->p1->c[count])
		{
			c1->p1->h[count]=true;
			c1->p1->hideeffect[count]=get_hideeffect(c1->p1->card[count]);
		}
		else
		{
			c1->p1->h[count]=false;
			c1->p1->hideeffect[count]=-1;
			count++;
			break;
		}
	for(;count<cardlimit;count++)
	{
		c1->p1->h[count]=false;
		c1->p1->hideeffect[count]=-1;
	}
}


int discard_all(client_member* c1,fight_room* this_room)//�����ֿ�
{
	int count,count2;
	count2=0;
	if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)//�����߶���Ч��
	{
		for(count=0;count<cardlimit;count++)
			if(c1->p1->c[count]==true)
			{
				c1->p1->c[count]=false;
				count2++;
			}
			else
				break;
		refresh_card(c1,this_room);
	}
	else//��������˶����ֿ�Ч��
	{
		for(count=0;count<cardlimit;count++)
			if(c1->p1->c[count]==true)
			{
				
				count2++;//�鿴�ֿ������������˺���ֵ����
			}
			else
				break;
	}
	return count2;//��ʱ��Ҫͳ���Լ����˼����ֿ�

}

int supply_card()																			//���ˢ�¿�������deck����
{
	int i;
	srand((unsigned)time(NULL)*rand()); 
	i=random_card(rand()%(100-0));//���ȡֵ0-100,��ѯ�����ֱ�ʾ�Ŀ�Ƭ���к�
	return i;
}

int deck1[decklimit];//���飬Q��ΪʲôҪԤ�迨�飿����ֱ��������ţ�A����Щ��ɫ�С�Ԥ��δ��(��Ԥ�⼴���鵽�Ŀ�)���������������ҪԤ�迨��

int garbage[garbagelimit];//Ĺ�أ��Ͽ������д����

void refresh_deck(int* deck)//���ÿ��1�ſ�����õ��ù���,��get_card�����Ż��õ��ú���������ˢ�¿������һ��
{
	int i;
	for(i=0;i<decklimit;i++)
		deck[i]=deck[i+1];//֮ǰ���ſ�ȫ����ǰ��һ��
	deck[decklimit-1]=supply_card();//�����ſ��������
}

void ini_deck(int* deck)																					//ȫ������ϴ��
{
    //Sleep(1000);
	int i;
	for(i=0;i<decklimit;i++)
	deck[i]=supply_card();
	
}

void get_card(int number,client_member* c1,fight_room* this_room)															//�����deck����鿨
{	
	if(number<1){cout<<"�鿨�쳣����Ƭ��Ϊ��"<<number<<endl;return;}

	char StrBuf[MAX_PATH] = { 0 };

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��");addtext(StrBuf,number);addtext(StrBuf,"��!\n");
	room_broadcast(this_room,StrBuf);//�㲥c1��ȡx��
	
	int i=0;
	while(number>0)
	{
		if(c1->p1->c[cardlimit-1])//����ֿ�����
		{		
			if(c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false)//��������ж�������������������ܣ�ֱ�������鿨����
			{
					
				confirm_send_success(c1,"���ֿ��Ѵ�����!���Զ����ֿ��������鿨!\n");
				i=cardlimit-1-discard_free_choose(c1,true,this_room);
			}
			if(c1->p1->c[cardlimit-1])
			{
				confirm_send_success(c1,"�㲻����!�������γ鿨����!\n");
				break;
			}//����ֿ��������ģ�˵�����붪���ܶ�����ֹͣ�����鿨��������ѭ��
			
		}
		if(!c1->p1->c[i])//�����ҵ�i�񿨲�Ϊ�գ����г鿨����������һ������
		{
			c1->p1->card[i]=this_room->deck[0];
			c1->p1->c[i]=true;
			refresh_deck(this_room->deck);//���ÿ��1�ſ���ˢ�¿������һ��
			i++;
			number--;
			//auto_judge(c1,i_get_card);//��Ҫ�����г鿨����ʹ�ú���ʹ�ô˺������б���Ч������
		}
		else
		{
			i++;//����һ�������޿�
		}
			
	}//������ˣ�������ѡ��ֹͣ�����鿨
	card_change(c1,this_room);//ˢ���ֿ�����
}

void rule_get_card(client_member* c1,fight_room* this_room)//�����Գ鿨
{
	int count2,count;
	count2=0;
	count=0;
			if(!c1->p1->c[2])//���غϵ���Ҳ���3���ֿ�������²�����5�ţ����������ȡ2���ֿ�
			{
				for(;count2<=4;count2++)
					if(!c1->p1->c[count2])count++;
				get_card(count,c1,this_room);
			}
			else
				get_card(2,c1,this_room);
}

void get_specific_card(int number,player* p1)													//��ȡָ����Ƭ
{
	int i=0;
	while(i<cardlimit)
	{
		
		if(p1->c[i]==false)//�����ҵ�i�񿨲�Ϊ�գ����г鿨����������һ������
		{
			p1->card[i]=number;//��ָ����Ƭ��Ϊnumber�Ŀ��������p
			p1->c[i]=true;
			return ;
		}
		else
		{
			i++;
		}
		if(i==10){cout<<"������ֿ������Ѵ����ޣ��޷������鿨"<<endl;return;}
	}
}

void show_card_name(player* p1)															//��ʾ�����ֿ����ƣ�����ʾЧ��
{
	
	int i;
	for(i=0;i<cardlimit;i++)
	if(p1->c[i]==true)cout<<"��"<<i+1<<"��:"<<search_card(p_name,p1->card[i])<<endl;
	
}
void refresh_card(client_member* c1,fight_room* this_room)                    //����п�ʹ�ú���Ҫ����ʹ���������,�����ֿ������������ֿ�
{
	int i,k;

	k=1;
	for(i=0;i<cardlimit-1;i++)
		if(c1->p1->c[i]==false)
		{	//���i���ۿգ������k�������п�����k���۵Ŀ�����i���ۣ�k�����ÿգ��ÿ���������Ϊ�ǿ�
			while(c1->p1->c[k]==false)
			{
				k++;
				if(k==cardlimit)break;//��ͷ�ˣ�ǿ���˳�
			}
			if(k==cardlimit)break;//��ͷ�ˣ�ǿ���˳�

			c1->p1->card[i]=c1->p1->card[k];//kû���ף���kλ�ϵĿ��Ƶ���i����
			c1->p1->c[i]=true;
			c1->p1->c[k]=false;
			k=i+2;//��Ϊÿ��ѭ����i++��������ǰk=i+2
		}
		else 
		{
			k=i+2;//��Ϊÿ��ѭ����i++��������ǰk=i+2
			if(k>=cardlimit)break;
		}
	card_change(c1,this_room);
}
void discard_card(client_member*c1,int whichcard,fight_room* this_room)//������һ���۵Ŀ�
{
	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//���߶���
		return;
	c1->p1->c[whichcard]=false;
	refresh_card(c1,this_room);
}
int use_card(client_member*c1,int whichcard,bool hide_effect,int* locktime,fight_room* this_room)//��������һ���۵Ŀ�
{
	char StrBuf[MAX_PATH]={0};

	int result=still_fight;
	int effect_cardnumber;
	int count;

	if(hide_effect)//����Ƿ�������Ч��
		effect_cardnumber=c1->p1->hideeffect[whichcard];//��ȡ����
	else
		effect_cardnumber=c1->p1->card[whichcard];//��ȡ����

	if(judge_weapon(effect_cardnumber))//�������������ʾ��ƬЧ��
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," װ��������: ");addtext(StrBuf,search_card(p_name,effect_cardnumber));addtext(StrBuf," !\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
	}
	else
	{
		for(count=0;count<playernumber;count++)
		{
			special_signal(this_room->c[count],card_launch);//�㲥��ʹ�ÿ�Ƭ���¼�����
		}
		addtext(StrBuf,c1->p1->name);if(c1->device!=win_console)addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//memset(StrBuf,0,sizeof(StrBuf));//�㲥���ͷ�����Ƭ������
		addtext(StrBuf,effect_cardnumber);addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//�㲥���������ĸ����ţ��ͻ����Զ�������ʾ��������ϸЧ��
		*locktime=*locktime+1;//��������+1
		addtext(StrBuf,"locktime=");addtext(StrBuf,*locktime);addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//�㲥��������
	}

	c1->p1->c[whichcard]=false;//�����ÿ�
	refresh_card(c1,this_room);//ˢ���ֿ�
	result=auto_judge(c1,i_card_used,this_room);//ʹ�����ֿ�

	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	result=fight_back(c2,c1,sb_card_used,this_room,locktime);
	if(result!=i_failed_effect)//�ж��Ƿ񷢶������������������
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��Ƭ�����ɹ�!\n");
		room_broadcast(this_room,StrBuf);
		result=auto_judge(c1,NULL,i_card_used_S,this_room,locktime);//�ֿ�ʹ�óɹ�
	}
	else
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��Ƭ����ʧ��!\n");
		room_broadcast(this_room,StrBuf);
	}

	//if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;//����������ֿ�ʹ�óɹ��������
	return result;
}

int discard_choose_weapon(client_member* c1,fight_room* this_room)//���־޽�ר�ã�ѡ��һ������������,���ض�����һ������
{
	char StrBuf[MAX_PATH] = { 0 };
	int choice,card_number;

	if(c1->device==win_console)
		confirm_send_success(c1,"�붪��1��װ����!\n��ѡ��:");
	else
	{
		confirm_send_success(c1,"�붪��1��װ����!\n");
		special_signal(c1,state_discard_one_card);//�л�����һ�ſ���ͼ�ν���
	}
		while(true)
		{
			
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
			
			if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ƬЧ���ѷ���!�޷�ȡ��!������ѡ��");continue;}//���ȡ��ѡ�����ֱ�ӷ���
			choice=atoi(StrBuf);
			if(choice==0){confirm_send_success(c1,"������Ч�ַ�!������ѡ��");continue;}
			choice--;
			if(choice>=cardlimit||choice<0){confirm_send_success(c1,"���볬���ֿ���//���ޣ�������ѡ���ֿ�:");continue;}	
			if(c1->p1->c[choice]==false){confirm_send_success(c1,"��Ǹ�����ֿ���Ϊ�գ�������ѡ���ֿ�:");continue;}
			if(judge_weapon(c1->p1->card[choice]))//����ÿ��ţ��жϸ��ֿ��Ƿ�Ϊװ����
				{
					card_number=c1->p1->card[choice];					
					discard_card(c1,choice,this_room);//�������ֿ�			
					break;
				}
			else
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"�ÿ�����װ����!������ѡ��:");
				else
					confirm_send_success(c1,"�ÿ�����װ����!������ѡ��!\n");
			}
		}
		if(c1->device!=win_console)special_signal(c1,state_return_normal);//ͼ�ν��水ť��ԭ
		return card_number;
}




//����card_discard�����ɹ�������few_card����ʧ�ܣ�����i_cancel_effect����ȡ��
int discard_choose(int number,client_member* c1,fight_room* this_room)//���������ֿ�������ѡ��number�����Ŀ�
{
	if(c1->member_No==robot_symbol)return i_cancel_effect;//����ǻ����ˣ�Ĭ�ϲ��ᶪ��
	int choice;
    int cardnumber=0;//ȷ���ֿ�����
	int count,count2,count3;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int cachecard[cardlimit];
	int locktime=0;

	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//�Ƿ����߶���Ч��
		return card_discard;//���߶���Ч����ֱ�������궪������

	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//ȷ���ֿ�����
		cardnumber++;

	if(cardnumber<number)
	{
		confirm_send_success(c1,"�ֿ���������!\n");
		return few_card;//�ֿ����㣬�޷�����
	}
	
	for(count=0;count<cardlimit;count++)//��ʼ��ѡ����
		cachecard[count]=-1;

	if(c1->device==win_console)
		confirm_send_success(c1,"��ѡ����Ҫ�����ֿ�(ѡ��0��ʾ�����֮ǰ���е�ѡ��,��ѡһ��0���ʾ��ȡ��������):");
	else
	{
		addtext(StrBuf,"�붪");addtext(StrBuf,number);addtext(StrBuf,"���ֿ�!\n");
		confirm_send_success(c1,StrBuf);

		if(number==1)
			special_signal(c1,state_discard_one_card);//ͼ�ν���ͻ���ר��
		else
			special_signal(c1,state_discard_muti_card);
	}
	count=0;
	while(number>0)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			if(count==0)
			{
				confirm_send_success(c1,"��ѡ��ȡ������!\n");
				special_signal(c1,state_return_normal);
				return i_cancel_effect;//ȡ����������
			}
			for(;count>=0;count--)//�����ѡ��
			{cachecard[count]=-1;}
			confirm_send_success(c1,"ԭ��ѡ����ȫ����������������룺");
			count=0;//ͳ����ѡ�ֿ��������¹���
			continue;
		}

		choice=atoi(StrBuf);
		if(choice==0){confirm_send_success(c1,"������Ч�ַ������������룺");continue;}

		choice--;
		if(choice<0||choice>=cardlimit){confirm_send_success(c1,"����Խ���ֿ���Χ!����������:");continue;}//����ѭ������ѡ��
		if(!c1->p1->c[choice]){confirm_send_success(c1,"�ÿ���Ϊ��!����������:");continue;}
		
		count3=0;
		for(count2=0;count2<cardlimit;count2++)
		{
			if(cachecard[count2]==-1)break;//�ѽ���ûѡ�������ѭ�����
			if(cachecard[count2]==choice)
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"�ÿ��ѱ�ѡ��!��ѡ��������:");
				else
					confirm_send_success(c1,"�ÿ��ѱ�ѡ��!��ѡ��������!\n");
				count3=1;break;
			}
		}
		if(count3==1)continue;//count3=1�����ÿ��ѱ�ѡ��!����ѭ������ѡ��

		cachecard[count]=choice;//�Ǽ���ѡ����
		count++;
		if(number>count)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"����Ҫ��");addtext(StrBuf,number-count);
			if(c1->device==win_console)
				addtext(StrBuf,"�ſ��������ѡ��:");
			else
				addtext(StrBuf,"�ſ��������ѡ��!\n");
			confirm_send_success(c1,StrBuf);
		}
		else 
		{
			confirm_send_success(c1,"ѡ����ɣ�\n");
			break;//ѡ�����,�˳�ѡ����ѭ��
		}
	}
	count=0;
    while(cachecard[count]!=-1&&count<11)
	{
		c1->p1->c[cachecard[count]]=false;//�ÿ����ÿգ������Ѿ�����
		count++;
	}

	special_signal(c1,state_return_normal);
	refresh_card(c1,this_room);
	return card_discard;//��ʾ�������̳ɹ�����
}


int discard_free_choose(client_member* c1,bool full,fight_room* this_room)//���������ֿ�������ѡ�����������Ŀ�,full�����Ƿ�����Ϊ�ֿ����˶����ɶ���
{
	if(c1->member_No==robot_symbol)return 0;//����ǻ����ˣ�Ĭ�ϲ��ᶪ��

	if(!c1->p1->c[0])return 0;//û���ֿ�ֱ���˳�

	int choice;
    int cardnumber=0;//ȷ���ֿ�����
	int count,count2,count3;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int cachecard[cardlimit];

	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//ȷ���ֿ�����
		cardnumber++;

	for(count=0;count<cardlimit;count++)//��ʼ��ѡ����
		cachecard[count]=-1;

	if(c1->device==win_console)
		confirm_send_success(c1,"(����y��ʾ��ȷ�����ѡ��;����0��ʾ�����֮ǰ���е�ѡ��;�����������ֱ�ʾ����)\n��ѡ��:");
	else
		special_signal(c1,state_discard_free_card);
	count=0;
	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

	
		if(StrBuf[0]=='y'&&strlen(StrBuf)==1)break;//ȷ�����ѡ��
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			for(;count>=0;count--)//�����ѡ��
			{cachecard[count]=-1;}
			if(c1->device==win_console)
				confirm_send_success(c1,"ԭ��ѡ����ȫ����������������룺");
			else
				confirm_send_success(c1,"ԭ��ѡ����ȫ�����!\n");
			count=0;//ͳ����ѡ�ֿ��������¹���
			continue;
		}

		choice=atoi(StrBuf);
		if(choice==0){confirm_send_success(c1,"������Ч�ַ������������룺\n");continue;}

		choice--;
		if(choice<0||choice>=cardlimit){confirm_send_success(c1,"����Խ���ֿ���Χ!����������:\n");continue;}//����ѭ������ѡ��
		if(!c1->p1->c[choice]){confirm_send_success(c1,"�ÿ���Ϊ��!����������:\n");continue;}
		
		count3=0;
		for(count2=0;count2<cardlimit;count2++)
		{
			if(cachecard[count2]==-1)break;//�ѽ���ûѡ�������ѭ�����
			if(cachecard[count2]==choice)
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"�ÿ��ѱ�ѡ��!��ѡ��������:");
				else
					confirm_send_success(c1,"�ÿ��ѱ�ѡ��!��ѡ��������!\n");
				count3=1;break;
			}

		}
		if(count3==1)continue;//count3=1�����ÿ��ѱ�ѡ��!����ѭ������ѡ��
		
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"��ѡ���� ");addtext(StrBuf,search_card(p_name,c1->p1->card[choice]));addtext(StrBuf,"!\n�����ѡ��:");
		confirm_send_success(c1,StrBuf);
		cachecard[count]=choice;//�Ǽ���ѡ����
		count++;//ѡ���ֿ�����+1
	}
	count=0;

	if(full)//�������Ϊ�ֿ�����û���鿨�����µ����ɶ��ֿ�,���߶���Ч��������
	{
		while(cachecard[count]!=-1&&count<cardnumber)
		{
			c1->p1->c[cachecard[count]]=false;//�ÿ����ÿգ������Ѿ�����
			count++;
		}

		refresh_card(c1,this_room);
	}

	if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)//�Ƿ����߶���Ч��
	{
		while(cachecard[count]!=-1&&count<cardnumber)
		{
			c1->p1->c[cachecard[count]]=false;//�ÿ����ÿգ������Ѿ�����
			count++;
		}

		refresh_card(c1,this_room);
	}
	else
	{
		special_signal(c1,state_return_normal);
		return cardnumber;
	}
	special_signal(c1,state_return_normal);
	return count;//�������̳ɹ�����,�����ܶ�������
}



int random_choose_card(int cardnumber)									//���ˢ�¿�������deck����
{
	int i;
	srand((unsigned)time(NULL)*7*cardnumber*rand()); 
	i=rand()%(cardnumber-0);//���ȡֵ0���ֿ�����-1,��ѯ�����ֱ�ʾ�Ŀ�Ƭ���к�
	return i;
}
void discard_random(int number,client_member* c1,fight_room* this_room)//�����������number�����Ŀ�
{
	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//�Ƿ������߶���Ч��
		return;//����ֱ�ӷ���
    int cardnumber=0;//ȷ���ֿ�����
	int choice;
	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//ȷ���ֿ�����
		cardnumber++;
	
	if(cardnumber<=number)//�ֿ����㣬ֱ������ֿ�����
	{
		discard_all(c1,this_room);
		return;
	}

	while(number!=0)
	{
		choice=random_choose_card(cardnumber-1);
		if(!c1->p1->c[choice])continue;//����ÿ����Ѿ�û�����������
		c1->p1->c[choice]=false;//�ÿ����ÿգ������Ѿ�����
		c1->p1->card[choice]=-1;
		number--;
	}

	refresh_card(c1,this_room);
	
	return;//��ʾ�������̳ɹ�����
}


void show_aims_card(client_member* c1,client_member* c2)//��c1չʾc2�����ֿ�
{
	int count,cardnumber;
	char StrBuf[MAX_PATH]={0};
	if(c1->device==win_console)
	{
		confirm_send_success(c1,"���չ����������ֿ���\n\n");
		
		for(count=0;count<cardlimit;count++)
			if(c2->p1->c[count]==true)
			{
				addtext(StrBuf,"��");addtext(StrBuf,count+1);addtext(StrBuf,"�ţ�");				
				addtext(StrBuf,search_card(p_name,c2->p1->card[count]));
				addtext(StrBuf,"\n");
				confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			}
	}
	else
	{
		cardnumber=0;//ͳ�ƿ�Ƭ����

		special_signal(c1,show_rivals_card_P);
		for(count=0;count<cardlimit;count++)
			if(c2->p1->c[count]==true)
				cardnumber++;
			else
				break;
			cout<<"�������ж������ֿ���-----------------------\n";
		sp_transmit(c1,cardnumber);//�����������ж����ֿ�
		for(count=0;count<cardnumber;count++)
		{
			cout<<"��"<<count<<"���ֿ���-----------------------\n";
			sp_transmit(c1,c2->p1->card[count]);//��������ֿ���Ϣ
		}
		cout<<"������ɣ�-----------------------\n";
	}

}

void plunder_ones_card(client_member* c1,client_member* c2,int number,fight_room* this_room)//numberΪҪ���������ſ���c1��c2
{
	if(auto_judge(c2,i_will_public_card,this_room)==i_immune_effect)
		{confirm_send_success(c1,"�����ֿ�ʧ��!\n");return ;}//�����չ����ܷ���������ֿ�Ч���������������ʧ��
	if(c2->p1->c[0]==false){confirm_send_success(c1,"���չ�������ֿ����޷�����!\n");return ;}

	int count,choice;
	char StrBuf[MAX_PATH]={0};
	
		if(c1->p1->c[cardlimit-1]==true)
		{
			confirm_send_success(c1,"���ֿ��������޷���������!\n");
			if(discard_free_choose(c1,true,this_room)==0)//����Լ������ֿ�
				return ;//ֱ�ӷ���
		}

		show_aims_card(c1,c2);//��c1չʾc2�ֿ�

		if(c1->device==win_console)confirm_send_success(c1,"\n��ѡ���ȡ����:");

		while(true)//�ͻ���ѡ���ȡ����
		{
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//�������ѡ�������ʱ�Ͽ�ͨѶ


			if(strlen(StrBuf)==1&&StrBuf[0]=='0'){confirm_send_success(c1,"��ѡ��ȡ�������ֿ�!\n");return;}

			choice=atoi(StrBuf);
			if(choice==0){confirm_send_success(c1,"������Ч�ַ�������������:");continue;}
			choice--;
			if(choice<0||choice>=cardlimit){confirm_send_success(c1,"���볬����ֵ������������:");continue;}
			if(c2->p1->c[choice]==false){confirm_send_success(c1,"����ҵĸÿ���Ϊ�գ��޷�����!����������:");continue;}
			if(c2->p1->c[choice]==true)break;
		}
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���ֿ���������!\n");
		room_broadcast(this_room,StrBuf);

		if(c1->p1->w[weaponlimit-1]==true)//�ж�װ�����Ƿ�����
		{	
			confirm_send_success(c1,"����װ������!�����װ��������ֱ��װ�������Ǽ����ֿ�!\n");
			for(count=0;count<cardlimit;count++)
				if(c1->p1->c[count]==false)
					break;
			c1->p1->c[count]=true;//�ֿ���������
			c1->p1->card[count]=c2->p1->card[choice];
		}
		else 
			if(!judge_weapon(c2->p1->card[choice]))
			{
				confirm_send_success(c1,"����Ĳ���װ����!\n");
				for(count=0;count<cardlimit;count++)
					if(c1->p1->c[count]==false)
						break;
				c1->p1->c[count]=true;//�ֿ���������
				c1->p1->card[count]=c2->p1->card[choice];
			}
			else
			{
				weapon_arm(c1,c2->p1->card[choice],this_room);

				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," �������ڵ�������");
				addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������: ");
				addtext(StrBuf,search_card(p_name,c2->p1->card[choice]));
				addtext(StrBuf," !������װ���Ͻ��н��������չ�!\n");
				room_broadcast(this_room,StrBuf);
			}
		c2->p1->c[choice]=false;
		refresh_card(c2,this_room);refresh_card(c1,this_room);
	return ;
}

//�����������������������������������������������������Կ��顢�鿨�Ĵ������������������������������������������������

#endif