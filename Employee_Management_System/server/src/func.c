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
	case RETURNINFROMATION:
		ret = viewSomeoneInformation(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "viewSomeoneInformation error\n");
		}
		break;
	case ModifyName:
		ret = modifyName(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifyAge:
		ret = modifyAge(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifySex:
		ret = modifySex(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifySalary:
		ret = modifySalary(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifyUsername:
		ret = modifyUsername(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifyCode:
		ret = modifyCode(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifyPhone:
		ret = modifyPhone(fd);
		memset(&member, 0, sizeof(member));
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case ModifyPosition:
		ret = modifyPosition(fd);
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
	case ReturnToPreviousMenu:
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
	case AttendanceThisMemberRecord:
		ret = todayAttendance(fd);
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
			strcat(member.attendanceRecord, psresult[i*column+j+column]);
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
	printmember();
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
	printf("adminCallback y\n");
	}
	else{
		((char*)arg)[0] = 'n';
	printf("adminCallback n\n");
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
	printf("userCallback y\n");
	}
	else{
		((char*)arg)[0] = 'n';
	printf("userCallback n\n");
	}

	return 0;
}

int login(int fd)
{
	int ret = -1;
	char identifier = 'n'; 

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from admin where username=\"%s\"", member.username);

	ret = sqlite3_exec(employeedb, sql, adminCallback, &identifier, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from user where username=\"%s\"", member.username);

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
	printf("idnumber : %d\n\n", member.idnumber);
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
			sprintf(sql, "insert into user values('%s', '%s', '%s', '%c', '%s', %d, '%s', %lf, %d, '%c')", member.name, \
					member.username, member.code, member.sex, member.phone, member.age, member.position,\
					member.salary, member.idnumber, 'u');

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "create table if not exists %s_%d_history(date char, time char)", member.name, member.idnumber);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "create table if not exists %s_%d_today(date char, time char)", member.name, member.idnumber);

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
			sprintf(sql, "insert into admin values('%s', '%s', '%c', %d)", member.username, member.code, member.identifier, member.idnumber);

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
	int ret;
	char name[40];
	int idnumber;
	int row;
	int column;

	if(member.name[0]!='\0')
	{
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select name, idnumber from user where name='%s'", member.name);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
		strcpy(name, psresult[2]);
		idnumber = atoi(psresult[3]);
	printf("%s %d\n",name,idnumber);


		sqlite3_free_table(psresult);
	}else{
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select name, idnumber from user where idnumber=%d", member.idnumber);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(member.idnumber >= 100){
		strcpy(name, psresult[2]);
		idnumber = atoi(psresult[3]);
	}

		sqlite3_free_table(psresult);
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "drop table %s_%d_history", name, idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	memset(sql, 0, sizeof(256));
	sprintf(sql, "drop table %s_%d_today", name, idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "delete from user where name='%s'", member.name);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "delete from user where idnumber=%d", member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "delete from admin where username='%s'", member.name);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	memset(sql, 0, sizeof(256));
	sprintf(sql, "delete from admin where idnumber=%d", member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}

	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}


	return 0;
}

int viewSomeoneInformation(int fd)
{
	int ret;
	int row;
	int column;
	if(member.name[0]!='\0')
	{
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from user where name='%s'", member.name);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
		member.choose = -1;
		strcpy(member.attendanceRecord, "no this member\n");
		goto send;
	}
		strcpy(member.name, psresult[column]);
		strcpy(member.username, psresult[column+1]);
		strcpy(member.code, psresult[column+2]);
		member.sex =  psresult[column+3][0];
		strcpy(member.phone, psresult[column+4]);
		member.age = atoi(psresult[column+5]);
		strcpy(member.position, psresult[column+6]);
		member.salary = atof(psresult[column+7]);
		member.idnumber = atoi(psresult[column+8]);
		member.identifier =  psresult[column+9][0];
	}else{
	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from user where idnumber=%d", member.idnumber);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
		member.choose = -1;
		strcpy(member.attendanceRecord, "no this member\n");
		goto send;
	}
		strcpy(member.name, psresult[column]);
		strcpy(member.username, psresult[column+1]);
		strcpy(member.code, psresult[column+2]);
		member.sex =  psresult[column+3][0];
		strcpy(member.phone, psresult[column+4]);
		member.age = atoi(psresult[column+5]);
		strcpy(member.position, psresult[column+6]);
		member.salary = atof(psresult[column+7]);
		member.idnumber = atoi(psresult[column+8]);
		member.identifier =  psresult[column+9][0];

	}
send:
	sqlite3_free_table(psresult);

	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}
	return 0;
}

int attendance(int fd)
{
	int row, column, ret;
	char nowdate[40]; 
	char nowtime[40];
	strcpy(nowdate, timeDate());
	strcpy(nowtime, timeHour());

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from %s_%d_today", member.name, member.idnumber);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
	memset(sql, 0, sizeof(256));
	sprintf(sql, "insert into %s_%d_today values('%s', '%s')", member.name, member.idnumber, timeDate(), timeHour());
	printf("%s\n", sql);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	memset(sql, 0, sizeof(256));
	sprintf(sql, "insert into %s_%d_today values('%s', '0')", member.name, member.idnumber, timeDate());
	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	}else{
		if(strcmp(psresult[column], nowdate)<0)
		{
			memset(sql, 0, sizeof(256));
			sprintf(sql, "update %s_%d_today set date='%s' where date='%s'", member.name, member.idnumber,\
					nowdate, psresult[column]);
			printf("%s\n", sql);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "update %s_%d_today set time='%s' where time='%s'", member.name, member.idnumber,\
					nowtime, psresult[column+1]);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
			memset(sql, 0, sizeof(256));
			sprintf(sql, "update %s_%d_today set time='0' where time='%s'", member.name, member.idnumber,\
					 psresult[2*column+1]);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}
		}else
		{
			memset(sql, 0, sizeof(256));
			sprintf(sql, "update %s_%d_today set time='%s' where time='%s'", member.name, member.idnumber,\
					nowtime, psresult[2*column+1]);

			ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
			if(ret < 0)
			{
				fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
				return -1;
			}

		}
	}
	strcpy(member.attendanceRecord, nowdate);
	strcat(member.attendanceRecord, " ");
	strcat(member.attendanceRecord, nowtime);
	sqlite3_free_table(psresult);
	ret = update_history(nowdate, nowtime);
	if(ret < 0)
	{
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int update_history(char *date, char *time)
{
	int ret;
	int row, column;

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from %s_%d_history where date='%s'", member.name, member.idnumber, date);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
		memset(sql, 0, sizeof(256));
		sprintf(sql, "insert into %s_%d_history values('%s', '%s')", member.name, member.idnumber, date, time);
		ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
		if(ret < 0)
		{
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			return -1;
		}
		memset(sql, 0, sizeof(256));
		sprintf(sql, "insert into %s_%d_history values('%s', '0')", member.name, member.idnumber, date);
		ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
		if(ret < 0)
		{
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			return -1;
		}
	}else{
		memset(sql, 0, sizeof(256));
		sprintf(sql, "update %s_%d_history set time='%s' where time='%s'", member.name, member.idnumber,\
				time, psresult[2*column+1]);

		ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
		if(ret < 0)
		{
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			return -1;
		}

	}
	sqlite3_free_table(psresult);


	return 0;
}

int todayAttendance(int fd)
{
	int ret;
	int row, column, i, j;
	char date[40];
	strcpy(date, timeDate());

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from %s_%d_today", member.name, member.idnumber);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
		strcpy(member.attendanceRecord, "No attendance record!\n");
	}else{
		if(strcmp(psresult[column], date)<0)
		{
			strcpy(member.attendanceRecord, "No attendance record!\n");
		}else{
			if(strcmp(psresult[2*column+1], "0") == 0)
			{
				strcpy(member.attendanceRecord, psresult[column]);
				strcat(member.attendanceRecord, " ");
				strcat(member.attendanceRecord, psresult[column+1]);
				strcat(member.attendanceRecord, "\n");
			}else{
			memset(member.attendanceRecord, 0, sizeof(member.attendanceRecord));
			for(i=0;i<row;i++)
			{
				for(j=0;j<column;j++)
				{
					strcat(member.attendanceRecord, psresult[i*column+column+j]);
					strcat(member.attendanceRecord, " ");
				}
				strcat(member.attendanceRecord, "\n");

			}
			}
		}
	}
	sqlite3_free_table(psresult);
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int attendanceMonth(int fd)
{
	int ret;
	int row, column, i, j;

	memset(sql, 0, sizeof(256));
	sprintf(sql, "select * from %s_%d_history", member.name, member.idnumber);

	ret = sqlite3_get_table(employeedb, sql, &psresult, &row, &column, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free_table(psresult);
		return -1;
	}
	if(row == 0 && column == 0)
	{
		strcpy(member.attendanceRecord, "No attendance record!\n");
	}else{
		memset(member.attendanceRecord, 0, sizeof(member.attendanceRecord));
		for(i=0;i<row;i++)
		{
			for(j=0;j<column;j++)
			{
				strcat(member.attendanceRecord, psresult[i*column+column+j]);
				strcat(member.attendanceRecord, " ");
			}
			strcat(member.attendanceRecord, "\n");

		}
	}
	sqlite3_free_table(psresult);
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyName(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set name='%s' where idnumber=%d", member.name, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyAge(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set age=%d where idnumber=%d", member.age, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifySex(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set sex='%c' where idnumber=%d", member.sex, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifySalary(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set salary=%lf where idnumber=%d", member.salary, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyUsername(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set username='%s' where idnumber=%d", member.username, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyPhone(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set phone='%s' where idnumber=%d", member.phone, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyCode(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set code='%s' where idnumber=%d", member.code, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

int modifyPosition(int fd)
{
	int ret;
	memset(sql, 0, sizeof(256));
	sprintf(sql, "update user set position='%s' where idnumber=%d", member.position, member.idnumber);

	ret = sqlite3_exec(employeedb, sql, NULL, NULL, &errmsg);
	if(ret < 0)
	{
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		return -1;
	}
	ret = sendtoclient(fd);
	if(ret < 0)
	{
		return -1;
	}

	return 0;
}

char* timeDate(void)
{
	static char time_date[40];
	time(&t);
	nowtime = localtime(&t);
	sprintf(time_date, "%4d-%2d-%2d", nowtime->tm_year+1900, nowtime->tm_mon+1, nowtime->tm_mday);

	return time_date;
}

char* timeHour(void)
{
	static char time_hour[40];
	time(&t);
	nowtime = localtime(&t);
	sprintf(time_hour, "%2d:%2d:%2d", nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec);

	return time_hour;
}
