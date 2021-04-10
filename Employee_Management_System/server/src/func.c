#include "../include/func.h"

int updatefds(int maxfd, fd_set readfds)
{
	int i = 0;
	for(i=maxfd; i>2; i--)
	{
		if(FD_ISSET(i, &readfds))
		{
			return i;	
		}
	}

	return -1;
}


void cli_info(int fd, struct sockaddr_in cin)
{
	printf("[%s:%d]fd:%d  link successful\n", inet_ntoa(cin.sin_addr),\
			ntohs(cin.sin_port), fd);
}

int clientInteraction(int fd)
{
	int ret;
	memset(&member, 0, sizeof(member));
	do
	{
		ret = recv(fd, &member, sizeof(member), 0);
	}while(ret<0 && EINTR==errno);

	if(ret < 0)
	{
		perror("recv");
		FD_CLR(fd, &readfds);
		close(fd);

		//更新maxfd
		maxfd = updatefds(maxfd, readfds);
	}
	else if(0 == ret)
	{
		FD_CLR(fd, &readfds);
		close(fd);
		printf("[%s]fd:%d client shut down...\n", inet_ntoa(cin.sin_addr), fd);

		//更新maxfd
		maxfd = updatefds(maxfd, readfds);
	}
	else
	{
		printmember();
		ret = sendDataAccordingToChoose(fd);
		if(ret < 0)
		{
			fprintf(stderr, "sendDataAccordingToChoose error\n");
		}
	}

	return 0;	
}

int sendDataAccordingToChoose(int fd)
{
	int ret;
	switch(member.choose)
	{
	case LOGIN:
		ret = login(fd);
		printf("login successful\n");
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "login error\n");
		}
		break;
	case AddMember:
		ret = addMember(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "addmember error\n");
		}
		break;
	case DeleteMember:
		ret = deleteMember(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "deletemember error\n");
		}
		break;
	case ViewSomeoneInfromation:
		ret = viewSomeoneInformation(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "viewSomeoneInformation error\n");
		}
		break;
	case ViewSelfInfromation:
		ret = viewSelfInfromation(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "viewSelfInfromation error\n");
		}
		break;
	case ModifySelfInformation:
		ret = modifySelfInformation(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "modifySelfInformation error\n");
		}
		break;
	case AttendanceSelfRecord:
		ret = attendanceSelfRecord(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case Attendance:
		ret = attendance(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendance error\n");
		}
		break;
	case ModifyThisMember:
		ret = modifyThisMember(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "modifyThisMember error\n");
		}
		break;
	case AttendanceThisMemberRecord:
		ret = attendanceThisMemberRecord(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceThisMemberRecord error\n");
		}
		break;
	case ReturnToPreviousMenu:
		break;
	case DELETE_CHOOSE:
		ret = deleteChoose(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "deleteChoose error\n");
		}
		break;
	case ATTENDANCE_30:
		ret = attendanceMonth(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceMonth error\n");
		}
		break;
	case ADD_CHOOSE:
		ret = addMember(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "addmember error\n");
		}
		break;
	case NOWMEMBER:
		ret = nowMember(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "nowMember error\n");
		}
		break;
	default:
		memset(&member, 0, sizeof(member));
		member.choose = -1;
		strcpy(member.attendanceRecord, "No this choose\n");
		do
		{
			ret = send(fd, &member, sizeof(member), 0);
		}while(ret<0 && EINTR==errno);
		if(ret < 0)
		{
			perror("send");
			return -1;
		}
		break;
	}

	return 0;
} 

int nowMember(int fd)
{
	int row;
	int column;
	int i,j;
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select name, idnumber from user");

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<column;j++)
		{
			strcat(member.attendanceRecord, psresult[i+j+2]);
			strcat(member.attendanceRecord, " ");
		}
		strcat(member.attendanceRecord, "\n");
	}
	sqlite3_free_table(psresult);
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}
	
	return 0;
}

int sendtoclient(int fd)
{
	int ret = -1;
	do
	{
		ret = send(fd, &member, sizeof(member), 0);
	}while(ret<0 && EINTR==errno);
	if(ret < 0)
	{
		perror("send");
		return -1;
	}
	return 0;
}

int serverInitSqlite3(void)
{
	int ret = -1;
	employeedb = NULL;	

	ret = sqlite3_open("./employee.db", &employeedb);
	if(ret < 0)
	{
		fprintf(stderr, "%s\n", sqlite3_errmsg(employeedb));
		return -1;
	}
	return 0;
}

int adminCallback(void* arg,int f_num, char** f_value,char** f_name)
{
	int ret;
	
	ret = strcmp(member.code, f_value[1]);
	if(ret == 0)
	{
		member.choose = 1;
		member.identifier = f_value[2][0];
		strcpy(member.name, f_value[0]);
		((char*)arg)[0] = 'y';
	}
	else{
		((char*)arg)[0] = 'n';
	}

	return 0;
}

int userCallback(void* arg,int f_num, char** f_value,char** f_name)
{
	int ret;
	
	ret = strcmp(member.code, f_value[2]);
	if(ret == 0)
	{
		member.choose = 1;
		member.identifier = 'u';
		strcpy(member.name, f_value[0]);
		strcpy(member.username, f_value[1]);
		strcpy(member.code, f_value[2]);
		member.sex = f_value[3][0];
		strcpy(member.phone, f_value[4]);
		member.age = atoi(f_value[5]);
		strcpy(member.position, f_value[6]);
		member.salary = atof(f_value[7]);
		member.idnumber = atoi(f_value[8]);
		((char*)arg)[0] = 'y';
	}
	else{
		((char*)arg)[0] = 'n';
	}

	return 0;
}

int login(int fd)
{
	int ret = -1;
	char identifier = 'n'; 

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from admin where username='%s'", member.username);

	ret = sqlite3_exec(employeedb, sql, adminCallback, &identifier, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from user where username='%s'", member.username);

	ret = sqlite3_exec(employeedb, sql, userCallback, &identifier, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	if(identifier == 'n')
	{
		member.choose = -1;
		strcpy(member.attendanceRecord, "username or code error");
	}
	printmember();
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

void printmember(void)
{
	printf("username : %s\n", member.username);
	printf("code : %s\n", member.code);
	printf("sex : %c\n", member.sex);
	printf("phone : %s\n", member.phone);
	printf("name : %s\n", member.name);
	printf("age : %d\n", member.age);
	printf("position : %s\n", member.position);
	printf("attendanceRecord : %s\n", member.attendanceRecord);
	printf("salary : %lf\n", member.salary);
	printf("choose : %d\n", member.choose);
	printf("identifier : %c\n", member.identifier);
	printf("idnumber : %d\n\n\n\n\n\n\n\n\n\n", member.idnumber);
}

int userMaxIdCallback(void* arg,int f_num, char** f_value,char** f_name)
{
	member.idnumber = atoi(f_value[0])+1;

	return 0;
}

int addMember(int fd)
{
	int ret;
	int flags = 1;
	if(member.identifier == 'm')
	{
		memset(sql, 0, sizeof(256));
		sprintf(sql, "select * from user where name='%s'", member.name);

		ret = sqlite3_exec(employeedb, sql, addUserCallback, &flags, &errmsg);
		if(ret < 0)
		{
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			return -1;
		}
		if(flags == 1)
		{
			memset(sql, 0, sizeof(256));
			sprintf(sql, "select max(idnumber) as maxvalue from user");

			ret = sqlite3_exec(employeedb, sql, userMaxIdCallback, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}

			memset(sql, 0, sizeof(256));
			sprintf(sql, "insert into user values(%s, %s, %s, %c, %s, %d, %s, %lf, %d)", member.name, \
					member.username, member.code, member.sex, member.phone, member.age, member.position,\
					member.salary, member.idnumber);

			printf("%s\n",sql);
			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "create table if not exists %shistory(date char, time char)", member.name);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "create table if not exists %stoday(date char, time char)", member.name);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			}
		}
	}else{
		memset(sql, 0, sizeof(256));
		sprintf(sql, "select * from admin where username='%s'", member.username);

		ret = sqlite3_exec(employeedb, sql, addAdminCallback, &flags, &errmsg);
		if(ret < 0)
		{
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			return -1;
		}
		if(flags == 1)
		{
			memset(sql, 0, sizeof(256));
			sprintf(sql, "select max(idnumber) as maxvalue from admin");

			ret = sqlite3_exec(employeedb, sql, userMaxIdCallback, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}

			memset(sql, 0, sizeof(256));
			sprintf(sql, "insert into admin values(%s, %s, %c, %d)", member.username, member.code, member.identifier, member.idnumber);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
		}
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int addAdminCallback(void* arg,int f_num, char** f_value,char** f_name)
{
	member.choose = -1;
	strcpy(member.attendanceRecord, "Admin already exist!");
	*(int*)arg = 0;
	return 0;
}

int addUserCallback(void* arg,int f_num, char** f_value,char** f_name)
{
	if(strcmp(member.username, f_value[1]) == 0 && strcmp(member.code, f_value[2]) == 0)
	{
		member.choose = -1;
		strcpy(member.attendanceRecord, "User already exist!");
		*(int*)arg = 0;
	}else{
		*(int*)arg = 1;
	}
	return 0;
}

int deleteMember(int fd)
{

	return 0;
}

int viewSomeoneInformation(int fd)
{

	return 0;
}

int viewSelfInfromation(int fd)
{

	return 0;
}

int modifySelfInformation(int fd)
{

	return 0;
}

int attendanceSelfRecord(int fd)
{

	return 0;
}

int attendance(int fd)
{

	return 0;
}

int modifyThisMember(int fd)
{

	return 0;
}

int attendanceThisMemberRecord(int fd)
{

	return 0;
}

int deleteChoose(int fd)
{

	return 0;
}

int attendanceMonth(int fd)
{

	return 0;
}

