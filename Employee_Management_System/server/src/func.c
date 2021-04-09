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
		if(ret < 0)
		{
			fprintf(stderr, "login error\n");
		}
		break;
	case AddMember:
		ret = addMember(fd);
		if(ret < 0)
		{
			fprintf(stderr, "addmember error\n");
		}
		break;
	case DeleteMember:
		ret = deleteMember(fd);
		if(ret < 0)
		{
			fprintf(stderr, "deletemember error\n");
		}
		break;
	case ViewSomeoneInfromation:
		ret = viewSomeoneInformation(fd);
		if(ret < 0)
		{
			fprintf(stderr, "viewSomeoneInformation error\n");
		}
		break;
	case ViewSelfInfromation:
		ret = viewSelfInfromation(fd);
		if(ret < 0)
		{
			fprintf(stderr, "viewSelfInfromation error\n");
		}
		break;
	case ModifySelfInformation:
		ret = modifySelfInformation(fd);
		if(ret < 0)
		{
			fprintf(stderr, "modifySelfInformation error\n");
		}
		break;
	case AttendanceSelfRecord:
		ret = attendanceSelfRecord(fd);
		if(ret < 0)
		{
			fprintf(stderr, "attendanceSelfRecord error\n");
		}
		break;
	case Attendance:
		ret = attendance(fd);
		if(ret < 0)
		{
			fprintf(stderr, "attendance error\n");
		}
		break;
	case ModifyThisMember:
		ret = modifyThisMember(fd);
		if(ret < 0)
		{
			fprintf(stderr, "modifyThisMember error\n");
		}
		break;
	case AttendanceThisMemberRecord:
		ret = attendanceThisMemberRecord(fd);
		if(ret < 0)
		{
			fprintf(stderr, "attendanceThisMemberRecord error\n");
		}
		break;
	case ReturnToPreviousMenu:
		break;
	case DELETE_CHOOSE:
		ret = deleteChoose(fd);
		if(ret < 0)
		{
			fprintf(stderr, "deleteChoose error\n");
		}
		break;
	case ATTENDANCE_30:
		ret = attendanceMonth(fd);
		if(ret < 0)
		{
			fprintf(stderr, "attendanceMonth error\n");
		}
		break;
	case ADD_CHOOSE:
		ret = addChoose(fd);
		if(ret < 0)
		{
			fprintf(stderr, "addChoose error\n");
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

int login(int fd)
{
	

	return 0;
}

int addMember(int fd)
{
	

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

int addChoose(int fd)
{

	return 0;
}
