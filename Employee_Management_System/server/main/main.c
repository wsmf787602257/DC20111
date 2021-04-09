#include "../include/func.h"

int main(int argc, const char *argv[])
{
	//1.创建套接字
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0)
	{
		perror("socket");
		return -1;
	}


	//允许本地端口快速重用
	int reuse = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	//2.绑定
	//填充服务器信息
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.s_addr = inet_addr("0");

	if(bind(sfd, (struct sockaddr*)&sin, sizeof(sin))<0)
	{
		perror("bind");
		exit(1);
	}
	printf("bind successful\n");

	//3.监听
	if(listen(sfd, 1024)<0)
	{
		perror("listen");
		exit(1);
	}

	//4.接收客户端的连接
	socklen_t len = sizeof(cin);

	//创建集合
	FD_ZERO(&readfds);
	FD_ZERO(&temp);
	FD_SET(sfd, &readfds);
	FD_SET(0, &readfds);
	

	int ret = -1;
	maxfd = sfd;
	char quit;
	printf("if you want quit server : enter 'q'\n");
	while(1)
	{
		temp = readfds;
		ret = select(maxfd+1, &temp, NULL, NULL, NULL);
		if(ret < 0)
		{
			perror("select");
			exit(1);
		}
		else if(0 == ret)
		{
			fprintf(stderr, "time out\n");
			continue;
		}

		int i = 0;
		for(i=0; i < maxfd+1; i++)
		{
			if(!FD_ISSET(i, &temp))
			{
				continue;
			}

			if(0 == i)
			{
				//键盘事件
				int ret = scanf("%c", &quit);
				while(getchar()!='\n');
				
				if(ret < 0)
				{
					fprintf(stderr, "enter error\n");
					continue;
				}
				
				if(quit == 'q')
				{
					printf("server exiting....\n");
					break;
				}

			}
			else if(sfd == i)
			{
				//客户端连接事件
				int newfd = accept(sfd, (void*)&cin, &len);
				if(newfd < 0)
				{
					perror("accept");
					exit(1);
				}
				
				//打印连接成功的客户端信息
				cli_info(newfd, cin);

				//将newfd添加到readfds中
				FD_SET(newfd, &readfds);
				maxfd = maxfd>newfd?maxfd:newfd;
			}
			else
			{
				ret = clientInteraction(i);
				//客户端交互事件
			}
		}
	}

	close(sfd);
	return 0;
}

