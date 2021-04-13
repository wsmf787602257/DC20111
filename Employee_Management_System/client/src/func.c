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
			system("clear");
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
			printf("%s\n", member.attendanceRecord);
			printf("You still have %d chances\n", number--);
			sleep(3);
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
	printf("localtime %dyear%dmonth%dday week%d %d:%d:%d\n\n", nowtime->tm_year+1900, nowtime->tm_mon+1, nowtime->tm_mday, nowtime->tm_wday+1, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec);
}

int adminInterface(void)
{
	int ret;
	char loginChoose;
	while(1)
	{
			system("clear");
		printf("-------------------------Admin Mode-------------------------\n");
		memset(&member, 0, sizeof(member));
		member.choose = NOWMEMBER;
		ret = sendAndRecv();
		if(ret < 0)
		{
			printf("%s\n", member.attendanceRecord);
			return -1;
		}
		printf("Now member:\n%s\n", member.attendanceRecord);

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
			printf("%s\n", member.attendanceRecord);
			}
			break;
		case DeleteMember:
			ret = deleteMember();
			if(ret < 0)
			{
				return -1;
			printf("%s\n", member.attendanceRecord);
			}
			break;
		case ViewSomeoneInfromation:
			ret = viewSomeoneInfromation();
			if(ret < 0)
			{
				return -1;
			printf("%s\n", member.attendanceRecord);
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
		system("clear");
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
			ret = modifySelfInfromation();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case AttendanceSelfRecord:
			ret = attendanceRecord();
			if(ret < 0)
			{
				return -1;
			}
			break;
		case Attendance:
			ret = attendance();
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
			system("clear");
		ret = returnMemberInfromation(name);
		if(ret < 0)
		{
			printf("%s\n", member.attendanceRecord);
			sleep(3);
			return 0;
		}
		printf("-------------------------Member Infromation Mode-------------------------\n");
		printf("Please enter your choose\n");
		printf("1.Modify this member\n");
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
			ret = modifySomeoneInfromation();
			if(ret < 0)
			{
			printf("%s\n", member.attendanceRecord);
			}
			break;
		case AttendanceThisMemberRecord:
			ret = attendanceRecord();
			if(ret < 0)
			{
			printf("%s\n", member.attendanceRecord);
			}
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
	

	member.choose = RETURNINFROMATION;
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
	if(member.choose < 0)
	{
		return -1;
	}

	printf("name:%s idnumber:%d sex:%c", member.name, member.idnumber, member.sex);
	printf(" age:%d phone:%11s position:%s salary:%6.2lf\n", member.age, member.phone,member.position, member.salary);

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
		perror("send");
		return -1;
	}

	memset(&member, 0, sizeof(Emif));
	do{
		ret = recv(sfd, (Emif *)&member, sizeof(Emif), 0);
	}while(ret < 0 && errno == EINTR);
	if(ret < 0)
	{
		perror("recv");
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
	Numorname non;
	char choose1;
	int ret;

	char name[40] = "";
	printf("Please enter the name or idnumber of the person you want to delete\n");
	scanf("%s", name);
	system("clear");

	non = numberOrName(name);
	strcpy(member.name, non.str);
	member.idnumber = non.idnumber;

	printf("Are you sure delete this member?(Y/N)\n>>");
	if(getchar()!='\n');
	choose1 = getchar();
	if(getchar()!='\n');
	printf("%c\n", choose1);
	if(choose1 == 'y' || choose1 == 'Y')
	{
		printf("lalala\n");
		system("clear");
		ret = sendAndRecv();
		if(ret < 0)
		{
			printf("%s\n", member.attendanceRecord);
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
	while(1){
			system("clear");
		printf("Do you want to add members or admin\n");
		printf("1.member\n2.admin\n");
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
		case ADDMember:
			memset(&member, 0, sizeof(member));
			printf("please enter add member infromation\n");
			printf("member name:\n>>");
			scanf("%s", member.name);
			if(getchar()!='\n');

			do{
				printf("member age:\n>>");
				ret = scanf("%d", &member.age);
			if(getchar()!='\n');
				if(ret == 0)
				{
					printf("enter error,please try again\n");
				}
			}while(ret == 0);

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

			do{
				printf("member salary:\n>>");
				ret = scanf("%lf", &member.salary);
			if(getchar()!='\n');
				if(ret == 0)
				{
					printf("enter error,please try again\n");
				}
			}while(ret == 0);

			member.choose = ADD_CHOOSE; 
			member.identifier = 'm';

			ret = sendAndRecv();
			if(ret < 0)
			{
			printf("%s\n", member.attendanceRecord);
				return -1;
			}
			system("clear");
			if(member.choose  < 0)
			{
				fprintf(stderr, "**********\nAdd error\n**********\n");
			}else{
				printf("Add successful\n");
			}
			break;
		case ADDAdmin:
			memset(&member, 0, sizeof(member));
			printf("please enter add admin infromation\n");
			printf("admin username:\n>>");
			scanf("%s", member.username);
			if(getchar()!='\n');
			printf("admin code:\n>>");
			scanf("%s", member.code);
			if(getchar()!='\n');
			member.identifier = ADMIN;
			member.choose = ADD_CHOOSE;
			ret = sendAndRecv();
			if(ret < 0)
			{
				printf("%s\n",member.attendanceRecord);
				return -1;
			}
			system("clear");
			if(member.choose  < 0)
			{
				fprintf(stderr, "**********\nAdd error\n**********\n");
			}else{
				printf("Add successful\n");
			}
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

int todayAttendance(void)
{
	int ret;
	member.choose = AttendanceThisMemberRecord;
	ret = sendAndRecv();
	if(ret < 0)
	{
		return -1;	
	}

	return 0;
}

int attendanceRecord(void)
{
	char loginChoose;
	int choose;
	int ret = -1;
	while(1)
	{
	system("clear");
	ret = todayAttendance();
	if(ret < 0)
	{
		return -1;
	}
	printf("********Today's attendance record*********\n");
	printf("%s", member.attendanceRecord);
		printf("Please enter your choose\n");
		printf("1.Show the attendance record of the last 30 days\n");
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
		case SHOWLAST30RECORD:
			member.choose = ATTENDANCE_30;
			ret = sendAndRecv();
			if(ret < 0)
			{
			printf("%s\n", member.attendanceRecord);
				return -1;
			}
			system("clear");
			printf("********Last 30 days attendance record*********\n");
			printf("%s", member.attendanceRecord);
			printf("Please enter your choose\n");
			printf("0.Return to the previous menu\n");
			printf("Enter 'q' to exit system\n>>");
			if(scanf("%d", &choose) == 0)
			{
				printf("System exiting....\n");
				return -1;
			}
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

int viewSelfInfromation()
{
	int choose;
	int ret = -1;
	ret = returnMemberInfromation(member.name);
	if(ret < 0)
	{
		printf("%s\n", member.attendanceRecord);
		return -1;
	}
	printf("Please enter your choose\n");
	printf("0.Return to the previous menu\n");
	printf("Enter 'q' to exit system\n>>");
	if(scanf("%d", &choose) == 0)
	{
		printf("System exiting....\n");
		return -1;
	}
	return 0;
}

int modifySelfInfromation(void)
{
	char loginChoose;
	int ret = -1;
	int choose;
	char str[40];
	const int idnumber = member.idnumber;
		while(1)
		{
			system("clear");
			printf("Please enter the selection to determine the information you want to modify\n\n");
			printf("1.username 2.code 3.phone\n");
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
			case username-4:
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
				break;
			case code-4:
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
				break;
			case phone-4:
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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

int modifySomeoneInfromation(void)
{
	char loginChoose;
	int ret = -1;
	int choose;
	char str[40];
	const int idnumber = member.idnumber;
		while(1)
		{
			system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
			printf("%s\n", member.attendanceRecord);
					return -1;
				}
				printf("modify successful\n");
				sleep(2);
				system("clear");
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
				sleep(2);
				system("clear");
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
				sleep(2);
				system("clear");
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

Numorname numberOrName(const char* str)
{
	int i;
	char name[40] = "";
	char number[40] = "";
	char *na = name;
	char *nu = number;
	Numorname non;
	for(i=0;i<strlen(str);i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
		{
			*nu++ = str[i];
		}
		*na++ = str[i];
	}
	*nu = '\0';
	*na = '\0';
	if(strlen(number) == 0)
	{
		strcpy(non.str, name);
		non.flag = ISNAME;
	}
	else
	{
		non.str[0] = '\0';
		non.idnumber = atoi(number);
		non.flag = ISIDNUMBER;
	}

	return non;
}

int attendance(void)
{
	int choose;
	int ret = -1;
	ret = sendAndRecv();
	if(ret < 0)
	{
		return -1;
	}
	if(member.choose < 0)
	{
		printf("Attendance Error, please try again!\n");
	}else
	{
		printf("Attendance successful!\nAttendance time is : %s\n", member.attendanceRecord);
	}
	printf("Please enter your choose\n");
	printf("0.Return to the previous menu\n");
	printf("Enter 'q' to exit system\n>>");
	if(scanf("%d", &choose) == 0)
	{
		printf("System exiting....\n");
		return -1;
	}
	if(getchar()!='\n');
	return 0;
}
