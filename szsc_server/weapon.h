#ifndef INCLUDE_weapon_H
#define INCLUDE_weapon_H


#include "judge_situation.h"


//�������������������������������������������������������������������������������������������������������������

void refresh_weapon(client_member* c1,fight_room* this_room)//��������仯�Ż��õ��ú���������ˢ��������
{
	
	int count,count2;
	for(count=0;count<weaponlimit-1;count++)
	if(c1->p1->w[count]==false&&c1->p1->w[count+1]==true)
	{
		//���ĳ������Ϊ�գ�����һ��������װ�����򽫺�һ�����۵�װ�����ݷ���ǰһ�������ۣ���һ�������ÿգ���������������Ϊ�ǿ�
		c1->p1->weapon[count]=c1->p1->weapon[count+1];//�����Ÿ���
		c1->p1->w[count]=true;
		c1->p1->exattack_weapon[count]=c1->p1->exattack_weapon[count+1];//��������������
	    c1->p1->exattack_weapon_turn[count]=c1->p1->exattack_weapon_turn[count+1];//�����ûغ���ʱ����������
		c1->p1->exattack_weapon_time[count]=c1->p1->exattack_weapon_time[count+1];//�����ôβ�����ʱ����������
		


		c1->p1->weaponeffect001[count]=c1->p1->weaponeffect001[count+1];//�лʽ�ʣ��Ч��1��������
		c1->p1->weaponeffect002[count]=c1->p1->weaponeffect002[count+1];//�лʽ�ʣ��Ч��2��������
		c1->p1->weaponeffect051[count]=c1->p1->weaponeffect051[count+1];//5��ʳ������װ��1Ч��ʣ�����
		c1->p1->weaponeffect052[count]=c1->p1->weaponeffect052[count+1];//5��ʳ������װ��2Ч��ʣ�����
		c1->p1->weaponeffect06[count]=c1->p1->weaponeffect06[count+1];//6�Ŷ��շ���ħ��ָʾ�����


		for(count2=0;count2<weaponeffectlimit;count2++)//Ч������
		{
			c1->p1->w_e[count][count2]=c1->p1->w_e[count+1][count2];
			c1->p1->weapon_effect[count][count2]=c1->p1->weapon_effect[count+1][count2];
			c1->p1->w_e[count+1][count2]=false;
		}
		c1->p1->w[count+1]=false;//��һ���ÿ�
		c1->p1->weapon[count+1]=-1;

	}
	weapon_change(c1,this_room);
}


void delete_weapon(client_member* c1,int whichweapon,fight_room* this_room)//ɾ��ĳ�����ڵ�һ������,�òۺ�Ϊwhichweapon
{
	int count;
	if(whichweapon>weaponlimit||whichweapon<0){cout<<"�쳣�����볬��������\n";return;}
	if(c1->p1->w[whichweapon]==false){cout<<"�쳣���ÿ���û�п�";return;}

	c1->p1->w[whichweapon]=false;
	c1->p1->weapon[whichweapon]=-1;
	c1->p1->exattack_weapon[whichweapon]=0;//����װ���������ӵĹ�������0
	c1->p1->exattack_weapon_turn[whichweapon]=0;//����װ�������ûغ����ӵĹ�������0
	c1->p1->exattack_weapon_time[whichweapon]=0;//����װ�������ôβ������ӵĹ�������0

	c1->p1->weaponeffect001[whichweapon]=0;//�лʽ�Ч��1������0
	c1->p1->weaponeffect002[whichweapon]=0;//�лʽ�Ч��2������0
	c1->p1->weaponeffect051[whichweapon]=0;//5��ʳ������װ��1Ч��ʣ�����
	c1->p1->weaponeffect052[whichweapon]=0;//5��ʳ������װ��2Ч��ʣ�����
	c1->p1->weaponeffect06[whichweapon]=0;//6�Ŷ��շ���ħ��ָʾ�����


	for(count=0;count<weaponeffectlimit;count++)//Ч�����
	{
		c1->p1->w_e[whichweapon][count]=false;
		c1->p1->weapon_effect[whichweapon][count]=-1;
	}
	
	refresh_weapon(c1,this_room);//ˢ��������			
}


void delete_weapon_all(client_member* c1,fight_room* this_room)//ɾ�������ȫ������
{
	int count,count2;

	for(count=0;count<weaponlimit;count++)
	{
		c1->p1->w[count]=false;//����ȫ�����
		c1->p1->weapon[count]=false;
		c1->p1->w_valid[count]=true;//��Ч״̬
		c1->p1->exattack_weapon[count]=0;//����װ���������ӵĹ�������0
		c1->p1->exattack_weapon_turn[count]=0;//����װ�������ûغ����ӵĹ�������0
		c1->p1->exattack_weapon_time[count]=0;//����װ�������ôβ������ӵĹ�������0
		for(count2=0;count2<weaponeffectlimit;count2++)
		{
			c1->p1->w_e[count][count2]=false;//����װ��Ч�����
			c1->p1->weapon_effect[count][count2]=-1;
		}
		c1->p1->weaponeffect001[count]=0;//�лʽ�Ч��1������0
		c1->p1->weaponeffect002[count]=0;//�лʽ�Ч��2������0
		c1->p1->weaponeffect051[count]=0;//5��ʳ������װ��1Ч��ʣ�����
		c1->p1->weaponeffect052[count]=0;//5��ʳ������װ��2Ч��ʣ�����
		c1->p1->weaponeffect06[count]=0;//6�Ŷ��շ���ħ��ָʾ�����
	}
	weapon_change(c1,this_room);
}



bool weapon_arm(client_member* c1,int choice,fight_room* this_room)//��������������choice����װ��
{

	int count;
	int locktime=0;
	bool arm_success=false;


	for(count=0;count<weaponlimit;count++)
	{if(c1->p1->w[count]==false)break;}//�����⵽��װ����Ϊ�գ���ѡ��ÿղ�װ����
	
	c1->p1->w[count]=true;//�ȱ����Ѳ�Ϊ��
	c1->p1->weapon[count]=choice;//����������Ϊ�������Ŀ���

	switch(choice)//������������Ч�����Ӷ��⹥����
	{
	case 0:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1002;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1003;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1004;
		   c1->p1->weaponeffect001[count]=1;c1->p1->weaponeffect002[count]=4;
	   	   arm_success=true;break;//�лʽ�,+1,
	case 1:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1012;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1013;
		   arm_success=true;break;//������,+1
	case 2:c1->p1->exattack_weapon[count]=2;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1022;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1023;
		   arm_success=true;break;//����֮��+2
	case 3:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1032;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1033;
	   	   arm_success=true;break;//�ɽ�֮��+1
	case 4:c1->p1->exattack_weapon[count]=2;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1043;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1044;
		   arm_success=true;break;//���׽�,Ѫ-2����+2,
	case 5:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1051;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1052;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1053;
			c1->p1->weaponeffect051[count]=1;//5��ʳ������װ��1Ч��ʣ�����
			c1->p1->weaponeffect052[count]=1;//5��ʳ������װ��2Ч��ʣ�����
		   arm_success=true;break;//ʳ������װ��
	case 6:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1061;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1062;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1063;
		   c1->p1->weaponeffect06[count]=2;//ħ��ָʾ���ʼΪ2
		   arm_success=true;break;//���շ���
	case 7:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1071;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1072;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1073;
		   arm_success=true;break;//���ù���
	case 8:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1081;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1082;
		   arm_success=true;break;//�ư�����
	case 9:c1->p1->exattack_weapon[count]=2;
           c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1092;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1093;
		   arm_success=true;break;//�������¸�+2
	case 10:c1->p1->exattack_weapon[count]=2;arm_success=true;break;//��Ӱ��+2
		
	}

	if(arm_success)weapon_change(c1,this_room);
		
	if(choice==4)//���׽�Ѫ-3
		if(auto_judge(c1,i_will_be_effect_A,this_room)!=i_immune_effect)//���û����������Ѫ��������������ѪЧ������
		{
			hit_damage(3,c1,NULL,this_room,&locktime,"���׽���ȡ��װ���ߵ�������!\n",i_will_be_effect_A);
		}
	//���װ���ϸպ�����
	if(c1->p1->blood<=0&&c1->p1->overdeath==0)auto_judge(c1,i_will_die,this_room);

	if(arm_success)return true;

	
cout<<"����ı��!����װ��ʧ��!choice="<<choice<<endl;
return false;
}

void show_him_his_buff_effect(client_member* c1)
{
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;

	if(c1->device!=win_console){special_signal(c1,show_buff_list);return;}

			addtext(StrBuf,"�㵱ǰ�ļӸ�Ч����:\n");
			if(c1->p1->hideshield>0)
			{addtext(StrBuf,"1.����ܣ�ʣ��");addtext(StrBuf,c1->p1->hideshield);addtext(StrBuf,"�ֻأ�\n");}

			addtext(StrBuf,"�������뷢���ĸ�����ѡ0ȡ��������\n��ѡ��");
			confirm_send_success(c1,StrBuf);
}





void show_him_his_weapon_effect(client_member* c1)
{
	if(c1->device!=win_console){special_signal(c1,show_weapon_list);return;}//UI�ͻ�����ʾ��������
	int count,count2;
	char StrBuf[MAX_PATH] = { 0 };
	confirm_send_success(c1,"���������:\n");
		for(count=0;count<weaponlimit;count++)//���װ�������Ƿ�Ϊ��
			if(c1->p1->w[count]==true)//����Ϊ�գ������������
			{
				addtext(StrBuf,"����");
				addtext(StrBuf,count+1);
				addtext(StrBuf,":");
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count]));
				addtext(StrBuf,"\t");
				confirm_send_success(c1,StrBuf);
			}
		confirm_send_success(c1,"\n\n");
		for(count=0;count<weaponlimit;count++)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"����");
			addtext(StrBuf,count+1);
			addtext(StrBuf,":\t");
			for(count2=0;count2<weaponeffectlimit;count2++)
			{	
				if(c1->p1->w_e[count][count2]==true)//���н�ɫװ��������Ч����ʾ�������Ч������ʾ
				{
					addtext(StrBuf,"Ч��");
					addtext(StrBuf,count2+1);
					addtext(StrBuf,":");
					addtext(StrBuf,search_skill(c1->p1->weapon_effect[count][count2]));
					addtext(StrBuf,"\n");
					confirm_send_success(c1,StrBuf);
					
					memset(StrBuf,0,sizeof(StrBuf));
					
				}
				else 
					break;
				addtext(StrBuf,"\t");//�������Ч��
			}

		}
	confirm_send_success(c1,"\n\n�������뷢���ڼ�������(����0��ʾ��ȡ���ò�����):");

}

#endif