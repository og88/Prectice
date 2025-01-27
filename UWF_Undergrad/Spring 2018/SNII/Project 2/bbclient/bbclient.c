/*
HTTP Client program. Created to request files from an HTTP server. The program asks user for the server ip adress and port number. Then for their desired file.
Omar Garcia
3/12/18
System and Networks II
*/

#include "Client.h"
#include "fileHandler.c"
#include "informationHandler.c"
#include <pthread.h>

int start(int serverPort, char *clientPort);
void *userOrder(void *args);
int sendConnection();
int recieveConnection(int port);
int userActions();
int socketConnect(int port, int x);

char *fileName;
char *ID;
int portNum; 
int finish;

bool token;
pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	if (argc == 5)
	{
		printf("Program ran fine\n");
		fileName = argv[4];

		if (openFile(fileName, "w+") == 0)
		{
			writeToFile("");
			closeFile();
			ID = argv[3];
			portNum = atoi(ID);
			start(atoi(argv[2]), argv[3]);
		}
	}
	else
	{
		printf("Too few arguments\n");
	}
}

int start(int serverPort, char *clientPort)
{ 
	int socket = socketConnect(serverPort, 1);
	while (socket == 1)
	{
		if (socket == 1)
		{
			socket = socketConnect(serverPort, 1);
		}
	}
	if (socket != 1)
	{
		char insert[CHUNK];
		sprintf(insert, "insert;%s;", clientPort);
		send(socket, insert, strlen(insert) + 1, 0); // send where, what, how much, flags (optional)
	}

	char tcp_server_message[CHUNK];
	read(socket, &tcp_server_message, sizeof(tcp_server_message)); // params: where (socket), what (string), how much - size of the server response, flags (0)
	printf("Server message  %s\n", tcp_server_message);
	if (strcmp(tcp_server_message, "true") == 0)
	{
		printf("valid\n");
	}
	else
	{
		printf("port number is already taken\n");
	}

	read(socket, &tcp_server_message, sizeof(tcp_server_message)); // params: where (socket), what (string), how much - size of the server response, flags (0)
	close(socket);
	sprintf(ID, "%d", order);
	intitialInfo(tcp_server_message);

	if (order == 0)
	{
		token = true;
	}
	else
	{
		token = false;
	}

	int i;
	pthread_t tid;
	for (i = 0; i < 2; i++)
	{
		pthread_create(&tid, NULL, userOrder, (void *)i);
	}
	pthread_exit(NULL);
	return 0;
}

void *userOrder(void *args)
{
	int *id = (int *)args;
	int result = 0;
	finish = 0;
	openFile(fileName, "w+");
	if (id == 0)
	{
		while (result == 0 && finish == 0)
		{
			result = recieveConnection(portNum);
		}
	}
	else
	{
		while (result == 0 && finish == 0)
		{
			if (token)
			{
				result = sendConnection();
			}
		}
	}
	closeFile();
}

int userActions()
{
	printf("Please make a selection\n1. Read\n2. Write\n3. List\n4. Exit\n");

	int selection;
	bool done = false;
	while (!done)
	{

		scanf("%d", &selection);
		if (selection == 1) //Read
		{
			int tcp_client_socket = socketConnect(nextPort, 1);
			long filesize = fileSize();
			char *fileContent = malloc(filesize);
			fileContent = readFile();
			printf("%s", fileContent);
			fflush(stdout);
			char message[CHUNK];
			sprintf(message, "token;");
			send(tcp_client_socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
			close(tcp_client_socket);
			token = false;
			return 0;
		}
		else if (selection == 2) //write
		{
			printf("input message you would like to write\n");
			getchar();
			int tcp_client_socket = socketConnect(nextPort, 1);
			char buff[128];
			scanf("%[^\n]s", &buff);
			writeToFile(buff);
			char filemessage[CHUNK]; 
			sprintf(filemessage, "token;");
			send(tcp_client_socket, filemessage, strlen(filemessage) + 1, 0); // send where, what, how much, flags (optional)
			close(tcp_client_socket);
			token = false;
			return 0;
		}
		else if (selection == 3) //List
		{
			printf("input message # you would like to read\n");
			getchar();
			char buff[128];
			scanf("%[^\n]s", &buff);
			listMessage(atoi(buff));
			
			int tcp_client_socket = socketConnect(nextPort, 1);
			char filemessage[CHUNK];
			sprintf(filemessage, "token;");
			send(tcp_client_socket, filemessage, strlen(filemessage) + 1, 0); // send where, what, how much, flags (optional)
			close(tcp_client_socket);
			token = false;
			return 0;
		}
		else if (selection == 4) //Exit
		{
			int tcp_client_socket = socketConnect(nextPort, 1);
			token = false;
			char message[CHUNK];
			if(portNum == previousPort)
			{
			sprintf(message, "Bye");
			}
			else{
			sprintf(message, "exit;%i;%i;", portNum, previousPort);
			}
			send(tcp_client_socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
			send(tcp_client_socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
			if(portNum != previousPort)
			{
				int tcp_client_socket = socketConnect(portNum, 1);
				send(tcp_client_socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
			}
			close(tcp_client_socket);
			finish = 1;
			return 1;
		}
	}
	return 1;
}

int sendConnection()
{
		int result = userActions();
		return result;
}

int recieveConnection(int port)
{
	int socket = socketConnect(port, 2);

	if (socket != 1)
	{
		char tcp_client_message[CHUNK];
		memset(tcp_client_message, 0, strlen(tcp_client_message));
		read(socket, &tcp_client_message, sizeof(tcp_client_message)); // params: where (socket), what (string), how much - size of the server response, flags (0)

		char c;
		int i = 0, j = 0;
		char header[CHUNK];
		memset(header, 0, strlen(header));
		c = tcp_client_message[i];
		while (c != ';')
		{
			header[j] = c;
			j++;
			i++;
			c = tcp_client_message[i];
		}
		if (strcmp(header, "token") == 0)
		{
			token = true;
			close(socket);
		}

		if (strcmp(header, "Bye") == 0)
		{
			printf("Bye\n");
			finish = 1;
			close(socket);
			return 1;
		}

		if (strcmp(header, "insert") == 0)
		{
			printf("handling insert\n\n");
			i++;
			j = 0;
			char content[CHUNK / 2];
			c = tcp_client_message[i];
			while (c != ';' && c != '\0' && c != ',')
			{
				content[j] = c;
				j++;
				content[j] = '\0';
				i++;
				c = tcp_client_message[i];
			}
			int newPort = atoi(content);
		printf("new member %i\n", newPort);
			if (socket != 1)
			{
				char message[CHUNK / 2];
				sprintf(message, "next=%d;prev=%d;order=%i,%i;", portNum, previousPort, size, size++);
				write(socket, "true", strlen("true") + 1); // send where, what, how much, flags (optional)
				send(socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
			}
			close(socket);
			socket = socketConnect(previousPort, 1);
			while (socket == 1)
			{
				if (socket == 1)
				{
					socket = socketConnect(previousPort, 1);
				}
			}
			if (socket != 1)
			{
			//	printf("Connected \n");
				char message[CHUNK / 2];
				sprintf(message, "update;%s", content);
				send(socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
				close(socket);
			}
		}

		if (strcmp(header, "update") == 0)
		{
			printf("Processing update\n");
			i++;
			j = 0;
			char content[CHUNK];
			c = tcp_client_message[i];
			while (c != ';' && c != '\0' && c != ',')
			{
				content[j] = c;
				j++;
				content[j] = '\0';
				i++;
				c = tcp_client_message[i];
			}
			printf("%s\n", content);
			nextPort = atoi(content);
			printf("new neighbor%i\n\n", nextPort);
		}

		if (strcmp(header, "exit") == 0)
		{
			if (previousPort != nextPort)
			{
				printf("Processing exit\n");
				i++;
				j = 0;
				char content[CHUNK];
				c = tcp_client_message[i];
				while (c != ';' && c != '\0' && c != ',')
				{
					content[j] = c;
					j++;
					content[j] = '\0';
					i++;
					c = tcp_client_message[i];
				}

				if (atoi(content) == previousPort)
				{
				//	printf("previous client exited\n\n");
					i++;
					j = 0;
					memset(content, 0, strlen(content));
					c = tcp_client_message[i];
					while (c != ';' && c != '\0' && c != ',')
					{
						content[j] = c;
						j++;
						content[j] = '\0';
						i++;
						c = tcp_client_message[i];
					}
					previousPort = atoi(content);
				//	printf("new pre port %i\n\n", previousPort);
					int socket = socketConnect(previousPort, 1);
					while (socket == 1)
					{
						if (socket == 1)
						{
							socket = socketConnect(previousPort, 1);
						}
					}
					if (socket != 1)
					{
					//	printf("Connected \n");
						char message[CHUNK];
						sprintf(message, "update;%i", portNum);
						send(socket, message, strlen(message) + 1, 0); // send where, what, how much, flags (optional)
					}
				}
				token = true;
			}
			else
			{
				token = true;
				nextPort = portNum;
				previousPort = portNum;
			}
		}
		close(socket);
		return 0;
	}
	else
	{
		close(socket);
		return 0;
	}
	close(socket);
	return 1;
}

int socketConnect(int port, int x)
{
	//printf("nextPort %i, previousPort %i, order %i\n", nextPort, previousPort, order);

	char ipAdress[CHUNK] = "127.0.0.1"; //String to hold IP address

	//creating the TCP socket
	int tcp_client_socket;								 //Socket descriptor
	tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0); //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

	//Check whether the connectio to the ip address is valid
	struct in_addr ip;
	if (inet_pton(AF_INET, ipAdress, &ip) != 1)
	{
		perror("inet_pton");
		exit(1);
	}

	//specify address and port of the remote socket
	struct sockaddr_in tcp_server_address;   //declaring a structure for the address
	tcp_server_address.sin_family = AF_INET; //Structure Fields' definition: Sets the address family of the address the client would connect to
	tcp_server_address.sin_addr = ip;		 //Connecting to 127.0.0.1

	if (x == 1)
	{
		//printf("%i\n", port);

		tcp_server_address.sin_port = htons(port); //Specify and pass the port number to connect - converting in right network byte order

		//connecting to the remote socket
		int connection_status = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); //params: which socket, cast for address to the specific structure type, size of address
		if (connection_status == -1)																							//return value of 0 means all okay, -1 means a problem
		{
			printf(" Problem connecting to the socket! Sorry!! \n");
			close(tcp_client_socket);
			return 1;
		}
		else
		{
			return tcp_client_socket;
		}
	}
	else if (x == 2)
	{

		tcp_server_address.sin_port = htons(portNum); //Specify and pass the port number to connect - converting in right network byte order
													  // binding the socket to the IP address and port

		bind(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); //Params: which socket, cast for server address, its size
																									 //listen for simultaneous connections

		listen(tcp_client_socket, 1);

		//Create a client socket
		int tcp_preClient_socket;
		tcp_preClient_socket = accept(tcp_client_socket, NULL, NULL); // server socket to interact with client, structure like before - if you know - else NULL for local connection
		close(tcp_client_socket);
		return tcp_preClient_socket;
	}
	else
		return 1;
}
