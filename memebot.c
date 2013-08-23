#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include "memebot.h"

char buf[513];
char sbuf[513];
//char* roomName = "#brogrammers";
int sockID;

void raw(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(sbuf,512,fmt,ap);
	va_end(ap);
	//printf("<< %s\n", sbuf);
	write(sockID,sbuf,strlen(sbuf));
}

int main(int argc, char **argv)
{
	
	if ( argc != 3 )
	{
		printf( "usage: %s [IRC Server URL] [roomname]\nExample: %s irc.test.org test\n", argv[0], argv[0] );
		exit(0);
	}

	char* connectStr = (char*)malloc(sizeof(argv[1])+1);
	char* roomName = (char*)malloc(sizeof(argv[2])+2);
	connectStr = argv[1];
	roomName[0] = '#';	//Set the # for IRC room names
	strcpy(roomName+1,argv[2]);
	printf("%s\n", roomName);

	srand(time(NULL));
	struct addrinfo hints , *res;
	int i,j,l,status,start,wordcount,say;
	int connected =0;
	int fuckCount = 0;
	char *nick = "memebot";
	char *port = "6667";

	char *user,*command,*where,*message,*target;

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;


	getaddrinfo(connectStr,port,&hints,&res);

	sockID = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

//	bind(sockID,res->ai_addr,res->ai_addrlen);
	
	if(connect(sockID,res->ai_addr,res->ai_addrlen)==-1)
	{
		printf("Socket connect Didn't work\n");
		return 1;
	}

	raw("USER %s 0 * :%s\r\n",nick,nick);
	printf("Connecting...\n");
	raw("NICK %s\r\n",nick);
	printf("Connected! with nick %s\n",nick);
	i=0;
	while((status = read(sockID,sbuf,512)))
	{
		srand(time(NULL));
		strcpy(buf,sbuf);
	//	printf("%s\n\n",buf);

				if((strncmp(buf, "PING",4))==0)
				{
				//	printf("%s\n",buf);
					raw("PONG 0\r\n");
				}
				else if(buf[0] == ':')
				{
					wordcount = 0;
					user=command=where=message=NULL;
					for(j=1; j<strlen(buf);j++)
					{
						if(buf[j] ==' ')
						{
							
							buf[j] == '\0';
							wordcount++;
							switch(wordcount)
							{
								case 1: 
								user = buf +1;
								break;
								case 2: 
								command = buf+start;
								break;
								case 3:
								where = buf+start;
								break;
							}
						if(j== l-1)
						{ 
						continue;
						}
						start = j+1;
						
						}
						else if(buf[j] == ':' && wordcount == 3)
						{
							if(j<l-1)
							{
								message == buf+j+1;
							}
							break;
						}
					}//for loop
					if(wordcount < 3)
					{
						continue;
					}
					if(!strncmp(command,"001",3) && roomName!=NULL)
					{
						raw("JOIN %s\r\n", roomName);
						i=1;
						raw("PRIVMSG %s :in this channel we will discuss %s\r\n",roomName,getMeme(NULL));
					}
					else if(!strncmp(command, "PRIVMSG", 7))// || !strncmp(command, "NOTICE", 6))
					{
					//	printf("Command: %s\n",command);
						if(i)
						{
						//raw("PRIVMSG %s :/me LIVES\r\n",roomName);
							
							say = rand()%15;
							if(say==2)
							{
								raw("PRIVMSG %s :%s\r\n",roomName,getMeme(NULL));
							}
							else
							{   
								if(strstr(command,"crashed")!=NULL)
								{
									raw("PRIVMSG %s :%s\r\n",roomName,"sauce");
								}
                                if(strstr(user,"enders")!=NULL&&(strstr(command,
"http:")!=NULL||strstr(command,"www.")!=NULL)){
								raw("PRIVMSG %s :%s\r\n",roomName,"OLD");

                                }
								else if((strstr(command, "fuck off memebot")!=NULL||strstr(command, "FUCK OFF MEMEBOT")!=NULL) && fuckCount <5)
								{
								fuckCount++;
								}
								else if(fuckCount>=5)
								{
								raw("PRIVMSG %s :%s\r\n",roomName,"No, YOU fuck off!");
								fuckCount = 0;
								}
							}
							//else
							//raw("PING 0\r\n");
						}
					}
					
				}//else if
			//if(i && rand()%10==1)
			//raw("PING 0\r\n");
			//write(sockID,"PING 0\r\n",8);
	}//while
		

	return 0; 
} //main

char* getMeme(int* selection)
{
	int num;
	if(selection == NULL)
	num = rand()%16;
	switch(num)
	{
	case 0:
	return "as long as";
	break;
	case 1:
	return "rough";
	break;
	case 2:
	return "pretty entry level dude...";
	break;
	case 3:
	return "so";
	break;
	case 4:
	return "FUCK IT, WE'LL DO IT LIVE";
	break;
	case 5:
	return "Iz good...but not very good";
	break;
	case 6:
	return "clear?";
	break;
	case 7:
	return "sauce";
	break;
	case 8:
	return "that's what she said";
	break;
	case 9:
	return "Given given given given";
	break;
	case 10:
	return "woah!";
	break;
	case 11:
	return "free as in freedom";
	break;
	case 12: 
	return "/me is securing funding";
	break;
	case 13:
	return "FUUUUUUUUUUUUUUUUUUUUU";
	break;
	case 14:
	return "OLD";
	break;
	case 15:
	return "KA-BOOOOOOOOOOOOOOOOOM";
	}	
}

