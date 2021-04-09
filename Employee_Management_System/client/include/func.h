#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define ADD_CHOOSE 103
#define LOGIN 100
#define DELETE_CHOOSE 101
#define SERVER_ADDR "192.168.31.54"
#define SERVER_PORT 2021
#define ATTENDANCE_30 102
#define SHOWLAST30RECORD 1

#define TCPON 1
#define ADMIN 'a'

typedef struct employeeInformation
{
	char username[40];
	char code[40];
	char sex;
	char phone[20];
	char name[40];
	int age;
	char position[20];
	char attendanceRecord[1024];
	double salary;	
	int choose;
	char identifier;
	int idnumber;
}Emif;

typedef struct
{
	char str[40];
	int idnumber;
	int flag;
}Numorname;

//管理员界面
enum{
	ReturnToPreviousMenu = 0,
	AddMember,//1
	DeleteMember,//2
	ViewSomeoneInfromation,//3
//员工界面
	ViewSelfInfromation,//4
	ModifySelfInformation,//5
	AttendanceSelfRecord,//6
	Attendance,//7
//管理员查看个人界面
	ModifyThisMember,//8
	AttendanceThisMemberRecord//9
};
//工号姓名
enum{
	ISIDNUMBER = 0,
	ISNAME
};
//信息
enum{
	name = 1,
	age,
	sex,
	salary,
	username,
	code,
	phone,
	position
};

enum{
	ModifyName = 200,
	ModifyAge,
	ModifySex,
	ModifySalary,
	ModifyUsername,
	ModifyCode,
	ModifyPhone,
	ModifyPosition,
};
//连接服务器
int connectServer(void);

//登录界面
int mainInterface(void);

//总交互系统
int interactiveSystem(void);

//时间显示
void nowtime(void);

//管理员修改信息
int modifySomeoneInfromation(void);

//管理员界面
int adminInterface(void);

//普通人员界面
int userInterface(void);

//管理员添加成员
int addMember(void);

//管理员删除成员
int deleteMember(void);

//管理员查看考勤记录
int attendanceRecord(void);

//用户查看自己的信息
int viewSelfInfromation(void);

//用户修改自己的信息
int modifySelfInfromation(void);

//查看某人信息
int viewSomeoneInfromation(void);

//输入姓名还是idnumber
Numorname numberOrName(const char* str);

//返回成员信息
int returnMemberInfromation(const char *str);

//打卡
int attendance(void);

//发送并接受服务器的数据
int sendAndRecv(void);

struct sockaddr_in serin;
int sfd;
Emif member;

#endif
