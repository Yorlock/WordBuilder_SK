#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <poll.h>
#include <thread>
#include <vector>
#include <signal.h>
#include <string.h>
#include <iostream>

using namespace std;

#define SERVER_PORT 1112

struct client_struct
{
    int desc = 0;
    char nick[30] = "";
    int score = 0;
};


std::vector<client_struct> allClients;
client_struct gameMaster;
int socketServer;
int roundTime = 0;
int roundNumber = 0;
bool isGameStarted = false;

void sendToAllClients(char* message)
{
    for(auto client : allClients)
    {
        write(client.desc, message, strlen(message));
        cout<< "(All) Fd: " << client.desc <<" msg: " << message << endl;
    }
}

void sendToAllClientsWithoutGameMaster(char* message)
{
    for(auto client : allClients)
    {
        if(client.desc != gameMaster.desc) 
        {
            write(client.desc, message, strlen(message));
            cout<< "(All) Fd: " << client.desc <<" msg: " << message << endl;
        }
    }
}


void sendNewNickToPlayersInLobby()
{
    client_struct lastClient = allClients.back();
    for(long unsigned int i=0; i<allClients.size() - 1; i++)
    {
        char message [32] = "n";
        strcat(message, allClients[i].nick);
        strcat(message, "@");
        write(lastClient.desc, message, sizeof(message));
        cout << "(last joined) Fd: "<< lastClient.desc << " msg: " << message << endl;
    }
    char message [32] = "n";
    strcat(message, lastClient.nick);
    strcat(message, "@");
    sendToAllClients(message);
}

void setReuseAddr(int sock)
{
	const int one = 1;
	int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	if(res) error(1,errno, "setsockopt failed");
}

void closeServer(int)
{
    for(auto client : allClients)
		close(client.desc);
	close(socketServer);
	printf("Zamykanie serwera\n");
	exit(0);
}

bool checkNick(char* nick)
{
    for(auto client : allClients)
    {
        if(strcmp(client.nick, nick) == 0) return false;
    }
    return true;
}

void *acceptingClients(void *)
{
    while(1)
    {
        client_struct client;
        sockaddr_in clientAddr{0};
		socklen_t clientAddrSize = sizeof(clientAddr);
        client.desc = accept(socketServer, (sockaddr*) &clientAddr, &clientAddrSize);
        if(client.desc == -1) error(1, errno, "Blad przy polaczeniu klienta");
        printf("Nowe polaczenie od: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), client.desc);
        
        int count = read(client.desc, client.nick, 255);
        if(count == -1) error(1, errno, "Blad read'a");
        if(checkNick(client.nick))
        {
            if(allClients.empty()) gameMaster = client;
            printf("Dodano %s do gry\n", client.nick);
            allClients.push_back(client);
            //1 - klient może dołączyć do gry i gra nie jest rozpoczeta i jest gamemasterem
            if(!isGameStarted && allClients.size() == 1)
            {
                write(client.desc, "l1@", 3);
                sendNewNickToPlayersInLobby();
            }
            //2 - klient może dołączyć do gry i gra nie jest rozpoczeta
            else if(!isGameStarted)
            {
                write(client.desc, "l2@", 3);
                sendNewNickToPlayersInLobby();
            }
            //3 - klient może dołączyć do gry i gra jest rozpoczeta
            else write(client.desc, "l3@", 3);
        }
        else
        {
            printf("Nick: %s jest juz zajety\n", client.nick);
            //4 - klient nie moze dolaczyc do rozrywki
            write(client.desc, "l4@", 3);
            close(client.desc);
        }
    }
}

int main(int argc, char ** argv)
{
    printf("Start serwera\n");

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(socketServer == -1) error(1, errno, "Blad socket Server");

    setReuseAddr(socketServer);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, closeServer);

    sockaddr_in serverAddr{
		.sin_family=AF_INET, 
		.sin_port=htons(SERVER_PORT),
		.sin_addr={INADDR_ANY}
	};

    int serverBind = bind(socketServer, (sockaddr*) &serverAddr, sizeof(serverAddr));
	if(serverBind) error(1, errno, "Blad server Bind");

    serverBind = listen(socketServer, 4);
    if(serverBind) error(1, errno, "Blad listen");

    pthread_t accepting;
    int threadAccepting = pthread_create(&accepting, NULL, acceptingClients, NULL);
    if(threadAccepting) error(1, errno, "Blad watku");

    while(1)
    {
        if(gameMaster.desc)
        {
            char message[5];
            memset(message, 0, 5);
            int count = read(gameMaster.desc, message, 5);
            //cout << count << "\n";  //if 0 gamemaster sie zapewne rozlaczyl :)
            if(count == -1) error(1, errno, "Blad read'a");
            
            if(count > 0) //do wyjebania jak ogarniemy* discconecta gamemastera     *jeśli
            {
                if(message[0] == 'r') 
                {
                    char round[3];
                    strcpy(round, message + 1);
                    roundNumber = atoi(round);
                }
                else if(message[0] == 't') 
                {
                    char round[4];
                    strcpy(round, message + 1);
                    roundTime = atoi(round);
                }

                strcat(message, "@");
                sendToAllClientsWithoutGameMaster(message);
            }


        }
    }
}