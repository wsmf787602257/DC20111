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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sqlite3.h>

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
	ADDMember = 1,
	ADDAdmin
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


int updatefds(int maxfd, fd_set readfds);

void cli_info(int fd, struct sockaddr_in cin);

int clientInteraction(int fd);

int sendDataAccordingToChoose(int fd);

int login(int fd);

int addMember(int fd);

int deleteMember(int fd);

int viewSomeoneInformation(int fd);

int viewSelfInfromation(int fd);

int modifySelfInformation(int fd);

int attendanceSelfRecord(int fd);

int attendance(int fd);

int modifyThisMember(int fd);

int attendanceThisMemberRecord(int fd);

int deleteChoose(int fd);

int attendanceMonth(int fd);

int addChoose(int fd);

int serverInitSqlite3(void);

struct sockaddr_in serin;
int sfd, maxfd;
Emif member;
fd_set readfds, temp;
struct sockaddr_in cin;
sqlite3 *employeedb;

#endif
