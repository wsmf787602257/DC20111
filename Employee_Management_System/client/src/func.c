#include "../include/func.h"

int connectServer(void)
{
#if TCPON
	int ret = -1;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0)
	{
		perror("socket");
		return -1;
	}
	int reuse = 1;
	int len = sizeof(reuse);

	ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, len);
	if(ret < 0)
	{
		perror("setsockopt");
		return -1;
	}

	serin.sin_family = AF_INET;
	serin.sin_port = htons(SERVER_PORT);
	serin.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	ret = connect(sfd, (struct sockaddr*)&serin, sizeof(serin));
	if(ret < 0)
	{
		perror("connect");
		return -1;
	}

#endif
	interactiveSystem(); 

#if TCPON
	ret = close(sfd);
	if(ret < 0)
	{
		perror("close");
		return -1;
	}
#endif
	return 0;
}

int interactiveSystem(void)
{
	int ret;
	memset(&member, 0, sizeof(Emif));
	while(1)
	{
		ret = mainInterface();
		if(ret < 0)
		{
			return -1;
		}
		if(member.identifier == ADMIN)
		{
			ret = adminInterface();
			if(ret < 0)
			{
				return -1;
			}
		}
		else
		{
			ret = userInterface();
			if(ret < 0)
			{
				return -1;
			}
		}
	} 
}

int mainInterface(void)
{
	int ret = -1;
	char username[256] = "";
	char code[40] = "";
	int number = 3;
	printf("welcome to HQYJ Employee Management System!\n\n");
	nowtime();
	printf("Hope you have a nice day!\n\n");
	while(1)
	{
		if(number != 3)
		{
			nowtime();
		}
		printf("please enter your username\n>>");
		scanf("%s", username);
		if(getchar()!='\n');
		bzero(member.username, 40);
		strcpy(member.username, username);
		printf("please enter your code\n>>");
		scanf("%s", code);
		if(getchar()!='\n');
		bzero(member.code, 40);
		strcpy(member.code, code);
		member.choose = LOGIN;

#if TCPON
		do{
			ret = send(sfd, (void*)&member, sizeof(Emif), 0);
		}while(ret < 0 && errno == EINTR);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
#endif

		member.choose = 0;
#if TCPON 
		do{
			ret = recv(sfd, (Emif *)&member, sizeof(Emif), 0);
		}while(ret < 0 && errno == EINTR);
		if(ret < 0)
		{
			perror("read");
			return -1; 
		}
		else if(ret == 0)
		{
			fprintf(stderr, "server:%s shutdown!\n", SERVER_ADDR);
			return -1;
		}
#endif
		if(member.choose >= 0)
		{
			printf("welcome back, %s.\n", member.name);
			return 0;
		}
		else
		{
			if(number <= 0)
			{
				printf("enter error too many!\nSystem exit!\n");
				return -1;
			}
			printf("Username or code error, please entry again\n");
			printf("You still have %d chances\n", number--);
			sleep(4);
			system("clear");
			continue;
		}
	}
}

void nowtime(void)
{
	time_t t;
	time(&t);
	struct tm *nowtime = localtime(&t);
	printf("本地时间为%d年%d月%d日 星期%d %d:%d:%d\n\n", nowtime->tm_year+1900, nowtime->tm_mon+1, nowtime->tm_mday, nowtime->tm_wday+1, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec);
}

int adminInterface(void)
{
	int ret;
	char loginChoose;
	while(1)
	{
		printf("-------------------------Admin Mode-------------------------\n");
		memset(&member, 0, sizeof(member));
		member.choose = LOGIN;
		ret = sendAndRecv();
		if(ret < 0)
		{
			return -1;
		}
		printf("%s", member.attendanceRecord);
		printf("Please enter your choose\n");
		printf("1.Add member\n");
		printf("2.Delete member\n");
		printf("3.View someone infromation\n");
		printf("0.Return to the previous menu\n");
		printf("Enter 'q' to exit system\n>>");
		if(scanf("%d", &member.choose) == 0)
		{
			printf("System exiting....");
			return -1;
		}
		if(getchar()!='\n');
		system("clear");

		if(member.choose != 0)
		{
			member.choose += 0;
		}

		switch(member.choose)
		{
		case AddMember:
			ret = addMember();

			if(ret < 0)
			{
				return -1;
			}
			break;
		case DeleteMember:
			ret = deleteMember();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case ViewSomeoneInfromation:
			ret = viewSomeoneInfromation();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case ReturnToPreviousMenu:
			printf("Are you sure you want to return to the previous menu?(Y/N)\n");
			scanf("%c", &loginChoose);
			if(getchar()!='\n');
			if(loginChoose == 'y' || loginChoose == 'Y')
			{
				return 0;
			}
			break;
		default:
			printf("enter error, please try again\n");
			break;
		}
	}
}

int userInterface(void)
{
	int ret;
	char loginChoose;
	while(1)
	{
		printf("-------------------------User Mode-------------------------\n");
		printf("Please enter your choose\n");
		printf("1.View self member\n");
		printf("2.Modify self member\n");
		printf("3.Attendance self record\n");
		printf("4.Attendance\n");
		printf("0.Return to the previous menu\n");
		printf("Enter 'q' to exit system\n>>");
		if(scanf("%d", &member.choose) == 0)
		{
			printf("System exiting....\n");
			return -1;
		}
		if(getchar()!='\n');
		if(member.choose != 0)
		{
			member.choose += 3;
		}
		system("clear");
		switch(member.choose)
		{
		case ViewSelfInfromation:
			ret = viewSelfInfromation();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case ModifySelfInformation:
			modifySelfInfromation();
			break;
		case AttendanceSelfRecord:
			attendanceSelfRecord();
			break;
		case Attendance:
			attendance();
			break;
		case ReturnToPreviousMenu:
			printf("Are you sure you want to return to the previous menu?(Y/N)\n");
			scanf("%c", &loginChoose);
			if(getchar()!='\n');
			if(loginChoose == 'y' || loginChoose == 'Y')
			{
				system("clear");
				return 0;
			}
			break;
		default:
			printf("enter error, please try again\n");
			break;
		}
	}
}

int viewSomeoneInfromation(void)
{
	int ret = -1;
	char loginChoose;
	char name[40] = "";
	memset(name, 0, sizeof(name));
	system("clear");
	printf("Please enter the idnumber or name of the member you want to modify\n");
	scanf("%s", name);
	system("clear");

	while(1)
	{
		returnMemberInfromation(name);
		printf("-------------------------Member Infromation Mode-------------------------\n");
		printf("Please enter your choose\n");
		printf("1.MoOriginal nameOriginal namedify this member\n");
		printf("2.Attendance this member record\n");
		printf("0.Return to the previous menu\n");
		printf("Enter 'q' to exit system\n>>");
		if(scanf("%d", &member.choose) == 0)
		{
			printf("System exiting....\n");
			return -1;
		}
		if(getchar()!='\n');
		if(member.choose != 0)
		{
			member.choose += 7;
		}
		switch(member.choose)
		{
		case ModifyThisMember:
			ret = modifySelfInfromation();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case AttendanceThisMemberRecord:
			attendanceSelfRecord();
			break;
		case ReturnToPreviousMenu:
			printf("Are you sure you want to return to the previous menu?(Y/N)\n");
			scanf("%c", &loginChoose);
			if(getchar()!='\n');
			if(loginChoose == 'y' || loginChoose == 'Y')
			{
				system("clear");
				return 0;
			}
			break;
		default:
			printf("enter error, please try again\n");
			break;
		}
	}
	return 0;
}

int returnMemberInfromation(const char* name)
{
	int ret = -1;
	Numorname non;

	non = numberOrName(name);
	if(non.flag == ISIDNUMBER)
	{
		member.idnumber = non.idnumber;
		do{
			ret = send(sfd, (void*)&member, sizeof(Emif), 0);
		}while(ret < 0 && errno == EINTR);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}
	else{
		strcpy(member.name, non.str);
		do{
			ret = send(sfd, (void*)&member, sizeof(Emif), 0);
		}while(ret < 0 && errno == EINTR);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}

	memset(&member, 0, sizeof(Emif));
	do{
		ret = recv(sfd, (Emif *)&member, sizeof(Emif), 0);
	}while(ret < 0 && errno == EINTR);
	if(ret < 0)
	{
		perror("read");
		return -1; 
	}
	else if(ret == 0)
	{
		fprintf(stderr, "server:%s shutdown!\n", SERVER_ADDR);
		return -1;
	}

	printf("name:%10s idnumber:%d sex:%c", member.name, member.idnumber, member.sex);
	printf("age:%d phone:%11s position:%10s salary:%6.2lf\n", member.age, member.phone,member.position, member.salary);

	return 0;
}

int sendAndRecv(void)
{
	int ret;
	do{
		ret = send(sfd, (void*)&member, sizeof(Emif), 0);
	}while(ret < 0 && errno == EINTR);
	if(ret < 0)
	{
		perror("write");
		return -1;
	}

	memset(&member, 0, sizeof(Emif));
	do{
		ret = recv(sfd, (Emif *)&member, sizeof(Emif), 0);
	}while(ret < 0 && errno == EINTR);
	if(ret < 0)
	{
		perror("read");
		return -1; 
	}
	else if(ret == 0)
	{
		fprintf(stderr, "server:%s shutdown!\n", SERVER_ADDR);
		return -1;
	}
	return 0;
}

int deleteMember()
{
	char choose;
	int ret;

	char name[40] = "";
	printf("Please enter the name or idnumber of the person you want to delete\n");
	scanf("%s", name);
	system("clear");
	ret = returnMemberInfromation(name);
	if(ret < 0)
	{
		return -1;
	}
	printf("Are you sure delete this member?(Y/N)\n");
	scanf("%c", &choose);
	if(getchar()!='\n');
	if(choose == 'y' || choose == 'Y')
	{
		system("clear");
		member.choose = DELETE_CHOOSE;
		ret = sendAndRecv();
		if(ret < 0)
		{
			return -1;
		}
		if(member.choose < 0)
	
		{
			printf("**************\nDelete error\n**************\n");
			sleep(3);
			return 0;
		}
		
		printf("Delete successful\n");
		sleep(3);
		return 0;
	}

	return 0;
}

int addMember()
{
	char loginChoose;
	int ret;
	int choose = 0;
	while(1)
	{
		memset(&member, 0, sizeof(member));
		printf("please enter add member infromation\n");
		printf("member name:\n>>");
		scanf("%s", member.name);
		if(getchar()!='\n');
		printf("member age:\n>>");
		scanf("%d", &member.age);
		if(getchar()!='\n');
		printf("member sex:\n>>");
		scanf("%c", &member.sex);
		if(getchar()!='\n');
		printf("member username:\n>>");
		scanf("%s", member.username);
		if(getchar()!='\n');
		printf("member code:\n>>");
		scanf("%s", member.code);
		if(getchar()!='\n');
		printf("member phone:\n>>");
		scanf("%s", member.phone);
		if(getchar()!='\n');
		printf("member position:\n>>");
		scanf("%s", member.position);
		if(getchar()!='\n');
		printf("member salary:\n>>");
		scanf("%lf", &member.salary);
		if(getchar()!='\n');

		ret = sendAndRecv();
		if(ret < 0)
		{
			return -1;
		}
		system("clear");
		if(member.choose  < 0)
		{
			fprintf(stderr, "**********\nAdd error\n**********\n");
		}else{
			printf("Add successful\n");
		}
		while(1)
		{
			printf("Please enter your choose\n");
			printf("1.Keep adding\n");
			printf("0.Return to the previous menu\n");
			printf("Enter 'q' to exit system\n>>");
			if(scanf("%d", &choose) == 0)
			{
				printf("System exiting....\n");
				return -1;
			}
			if(getchar()!='\n');

			if(choose == 1)
			{
				break;
			}
			else if(choose == 0){
				printf("Are you sure you want to return to the previous menu?(Y/N)\n");
				scanf("%c", &loginChoose);
				if(getchar()!='\n');
				if(loginChoose == 'y' || loginChoose == 'Y')
				{
		
					system("clear");
					return 0;
				}
			}else{
				printf("enter error, please try again\n");
				continue;
			}
		}
	}
}

void attendanceRecord()
{

}

int viewSelfInfromation()
{

	return 0;
}

int modifySelfInfromation(void)
{
	char loginChoose;
	int ret = -1;
	int choose;
	char str[40];
	const int idnumber = member.idnumber;
	if(member.choose == 8)
	{
		while(1)
		{
			printf("Please enter the selection to determine the information you want to modify\n\n");
			printf("1.name 2.age 3.sex 4.salary\n5.username 6.code 7.phone 8.position\n");
			printf("0.Return to the previous menu\n");
			printf("Enter 'q' to exit system\n>>");
			if(scanf("%d", &choose) == 0)
			{
				printf("System exiting....\n");
				return -1;
			}
			if(getchar()!='\n');
			switch(choose)
			{
			case name:
				printf("Original name : %s\n", member.name);
				printf("New name >>");
				memset(str, 0, sizeof(str));
				memset(&member, 0, sizeof(member));
				scanf("%s", str);
				if(getchar()!='\n');
				strcpy(member.name, str);
				member.choose = ModifyName;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case username:
				printf("Original username : %s\n", member.username);
				printf("New username >>");
				memset(str, 0, sizeof(str));
				memset(&member, 0, sizeof(member));
				scanf("%s", str);
				if(getchar()!='\n');
				strcpy(member.username, str);
				member.choose = ModifyUsername;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case code:
				printf("Original code : %s\n", member.code);
				printf("New code >>");
				memset(str, 0, sizeof(str));
				memset(&member, 0, sizeof(member));
				scanf("%s", str);
				if(getchar()!='\n');
				strcpy(member.code, str);
				member.choose = ModifyCode;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case phone:
				printf("Original phone : %s\n", member.phone);
				printf("New phone >>");
				memset(str, 0, sizeof(str));
				memset(&member, 0, sizeof(member));
				scanf("%s", str);
				if(getchar()!='\n');
				strcpy(member.phone, str);
				member.choose = ModifyPhone;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case position:
				printf("Original position : %s\n", member.position);
				printf("New position >>");
				memset(str, 0, sizeof(str));
				memset(&member, 0, sizeof(member));
				scanf("%s", str);
				if(getchar()!='\n');
				strcpy(member.position, str);
				member.choose = ModifyPosition;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case sex:
				printf("Original sex : %c\n", member.sex);
				printf("New sex >>");
				memset(&member, 0, sizeof(member));
				scanf("%c", &member.sex);
				if(getchar()!='\n');
				member.choose = ModifySex;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case age:
				printf("Original age : %d\n", member.age);
				printf("New age >>");
				memset(&member, 0, sizeof(member));
				scanf("%d", &member.age);
				if(getchar()!='\n');
				member.choose = ModifyAge;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case salary:
				printf("Original salary : %.2lf\n", member.salary);
				printf("New salary >>");
				memset(&member, 0, sizeof(member));
				scanf("%lf", &member.salary);
				if(getchar()!='\n');
				member.choose = ModifySalary;
				member.idnumber = idnumber;
				ret = sendAndRecv();
				if(ret < 0)
				{
					return -1;
				}
				printf("modify successful\n");
				break;
			case ReturnToPreviousMenu:
				printf("Are you sure you want to return to the previous menu?(Y/N)\n");
				scanf("%c", &loginChoose);
				if(getchar()!='\n');
				if(loginChoose == 'y' || loginChoose == 'Y')
				{
					system("clear");
					return 0;
				}
				break;
			default:
				printf("enter error, please try again\n");
				break;
			}
		}
	}
	return 0;
}

void attendanceSelfRecord(void)
{

}

Numorname numberOrName(const char* str)
{
	//int i;
	Numorname non;
	//	for(i=0;i<strlen(str);i++)
	//	{
	//if(name[i])
	//	}
	return non;
}

void attendance(void)
{

}
