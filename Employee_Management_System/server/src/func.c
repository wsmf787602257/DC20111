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
		return -1;
	}
	else if(0 == ret)
	{
		FD_CLR(fd, &readfds);
		close(fd);
		printf("[%s]fd:%d client shut down...\n", inet_ntoa(cin.sin_addr), fd);

		//更新maxfd
		maxfd = updatefds(maxfd, readfds);
		return -1;
	}
	else
	{
		ret = sendDataAccordingToChoose(fd);
		if(ret < 0)
		{
			return -1;
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

		break;
	case AddMember:

		break;
	case DeleteMember:

		break;
	case ViewSomeoneInfromation:

		break;
	case ViewSelfInfromation:

		break;
	case ModifySelfInformation:

		break;
	case AttendanceSelfRecord:

		break;
	case Attendance:

		break;
	case ModifyThisMember:

		break;
	case AttendanceThisMemberRecord:

		break;
	case ReturnToPreviousMenu:
		break;
	case DELETE_CHOOSE:

		break;
	case ATTENDANCE_30:

		break;
	case ADD_CHOOSE:

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
