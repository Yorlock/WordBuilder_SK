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
#include <utility>

using namespace std;

#define SERVER_PORT 1112
#define NUMBER_OF_SETS 19
struct client_struct
{
    int desc = 0;
    char nick[30] = "";
    int score = 0;
};

struct word_struct
{
    string letters;
    vector<pair<string, bool>> correctWords;
    bool played = false;
};

vector<client_struct> allClients;
vector<word_struct> allSets;
client_struct gameMaster;
int currentSet = 0;
int socketServer;
int roundTime = 0;
int roundNumber = 0;
bool isGameStarted = false;

void insertAllSets()
{
    word_struct word[NUMBER_OF_SETS];

    word[0].letters = "UMR";
    word[0].correctWords.push_back(make_pair("RUM", false));
    word[0].correctWords.push_back(make_pair("MUR", false));

    word[1].letters = "ETN";
    word[1].correctWords.push_back(make_pair("NET", false));
    word[1].correctWords.push_back(make_pair("TEN", false));

    word[2].letters = "OFSA";
    word[2].correctWords.push_back(make_pair("SOFA", false));
    word[2].correctWords.push_back(make_pair("FOSA", false));
    word[2].correctWords.push_back(make_pair("OSA", false));

    word[3].letters = "TOŁM";
    word[3].correctWords.push_back(make_pair("MŁOT", false));
    word[3].correctWords.push_back(make_pair("ŁOM", false));
    word[3].correctWords.push_back(make_pair("TŁO", false));
    word[3].correctWords.push_back(make_pair("TOM", false));

    word[4].letters = "LABO";
    word[4].correctWords.push_back(make_pair("ALBO", false));
    word[4].correctWords.push_back(make_pair("BOA", false));
    word[4].correctWords.push_back(make_pair("BAL", false));

    word[5].letters = "KOZPA";
    word[5].correctWords.push_back(make_pair("OKAZ", false));
    word[5].correctWords.push_back(make_pair("POKA", false));
    word[5].correctWords.push_back(make_pair("OKAP", false));
    word[5].correctWords.push_back(make_pair("KOZA", false));
    word[5].correctWords.push_back(make_pair("OPAK", false));

    word[6].letters = "DOWWA";
    word[6].correctWords.push_back(make_pair("WDOWA", false));
    word[6].correctWords.push_back(make_pair("OWAD", false));
    word[6].correctWords.push_back(make_pair("WODA", false));
    word[6].correctWords.push_back(make_pair("DWA", false));
    word[6].correctWords.push_back(make_pair("ODA", false));

    word[7].letters = "OSATT";
    word[7].correctWords.push_back(make_pair("TOAST", false));
    word[7].correctWords.push_back(make_pair("TATO", false));
    word[7].correctWords.push_back(make_pair("TOST", false));
    word[7].correctWords.push_back(make_pair("OSA", false));
    word[7].correctWords.push_back(make_pair("STO", false));

    word[8].letters = "RUZAP";
    word[8].correctWords.push_back(make_pair("PAZUR", false));
    word[8].correctWords.push_back(make_pair("URAZ", false));
    word[8].correctWords.push_back(make_pair("ZUPA", false));
    word[8].correctWords.push_back(make_pair("RAZ", false));
    word[8].correctWords.push_back(make_pair("RAP", false));

    word[9].letters = "UMGKA";
    word[9].correctWords.push_back(make_pair("GUMKA", false));
    word[9].correctWords.push_back(make_pair("GUMA", false));
    word[9].correctWords.push_back(make_pair("KUMA", false));
    word[9].correctWords.push_back(make_pair("MAK", false));
    word[9].correctWords.push_back(make_pair("MAG", false));

    word[10].letters = "AAMDL";
    word[10].correctWords.push_back(make_pair("DAMA", false));
    word[10].correctWords.push_back(make_pair("LADA", false));
    word[10].correctWords.push_back(make_pair("LAMA", false));
    word[10].correctWords.push_back(make_pair("DAL", false));
    word[10].correctWords.push_back(make_pair("DLA", false));

    word[11].letters = "AĆAWD";
    word[11].correctWords.push_back(make_pair("DAWAĆ", false));
    word[11].correctWords.push_back(make_pair("WADA", false));
    word[11].correctWords.push_back(make_pair("WDAĆ", false));
    word[11].correctWords.push_back(make_pair("DWA", false));
    word[11].correctWords.push_back(make_pair("DAĆ", false));

    word[12].letters = "SMATA";
    word[12].correctWords.push_back(make_pair("ASTMA", false));
    word[12].correctWords.push_back(make_pair("MASA", false));
    word[12].correctWords.push_back(make_pair("MATA", false));
    word[12].correctWords.push_back(make_pair("TAMA", false));
    word[12].correctWords.push_back(make_pair("SAM", false));
    word[12].correctWords.push_back(make_pair("TAM", false));
    word[12].correctWords.push_back(make_pair("MAT", false));

    word[13].letters = "ĆSPAR";
    word[13].correctWords.push_back(make_pair("SPRAĆ", false));
    word[13].correctWords.push_back(make_pair("SRAĆ", false));
    word[13].correctWords.push_back(make_pair("PRAĆ", false));
    word[13].correctWords.push_back(make_pair("SPAĆ", false));
    word[13].correctWords.push_back(make_pair("RAP", false));
    word[13].correctWords.push_back(make_pair("PAS", false));

    word[14].letters = "ZSŁOK";
    word[14].correctWords.push_back(make_pair("SZKŁO", false));
    word[14].correctWords.push_back(make_pair("KŁOS", false));
    word[14].correctWords.push_back(make_pair("KOSZ", false));
    word[14].correctWords.push_back(make_pair("SZOK", false));
    word[14].correctWords.push_back(make_pair("ZŁO", false));
    word[14].correctWords.push_back(make_pair("KOS", false));
    word[14].correctWords.push_back(make_pair("SOK", false));

    word[15].letters = "AAWZŁ";
    word[15].correctWords.push_back(make_pair("ZAWAŁ", false));
    word[15].correctWords.push_back(make_pair("ŁAWA", false));
    word[15].correctWords.push_back(make_pair("WAZA", false));
    word[15].correctWords.push_back(make_pair("WŁAZ", false));
    word[15].correctWords.push_back(make_pair("ZWAŁ", false));
    word[15].correctWords.push_back(make_pair("WAŁ", false));

    word[16].letters = "SAAKM";
    word[16].correctWords.push_back(make_pair("MASKA", false));
    word[16].correctWords.push_back(make_pair("KASA", false));
    word[16].correctWords.push_back(make_pair("MAKS", false));
    word[16].correctWords.push_back(make_pair("MASA", false));
    word[16].correctWords.push_back(make_pair("SMAK", false));
    word[16].correctWords.push_back(make_pair("SAM", false));
    word[16].correctWords.push_back(make_pair("MAK", false));

    word[17].letters = "ORZZA";
    word[17].correctWords.push_back(make_pair("ZORZA", false));
    word[17].correctWords.push_back(make_pair("ORAZ", false));
    word[17].correctWords.push_back(make_pair("ZRAZ", false));
    word[17].correctWords.push_back(make_pair("ZZA", false));
    word[17].correctWords.push_back(make_pair("RAZ", false));

    word[18].letters = "SZOKP";
    word[18].correctWords.push_back(make_pair("SZKOP", false));
    word[18].correctWords.push_back(make_pair("KOSZ", false));
    word[18].correctWords.push_back(make_pair("SZOK", false));
    word[18].correctWords.push_back(make_pair("SZOP", false));
    word[18].correctWords.push_back(make_pair("SOK", false));
    word[18].correctWords.push_back(make_pair("KOS", false));
    word[18].correctWords.push_back(make_pair("OK", false));
    
    for(word_struct w : word)
    {
        /*
        cout << "letters: " << w.letters <<"\n";
        for(string s : w.correctWords)
        {
            cout << "word: " << s <<"\n";
        }
        */
        allSets.push_back(w);
    }
}

void checkCorrectnessOfWord(client_struct client, string word)
{
    for(pair w : allSets.at(currentSet).correctWords)
    {
        if(word == w.first) // takie slowo istnieje
        {
            if(!w.second) //nie zgadniety jeszcze - gracz dostaje punkt
            {
                return;
            }
            //slowo zostalo zgadniete - bez zmian pkt
            return;
        }
    }
    // gracz traci punkt
}

void sendRoundTimeToClient(client_struct client)
{
    char message[5];
    memset(message, 0, 5);
    strcat(message, "t");
    strcat(message, std::to_string(roundTime).c_str());
    strcat(message, "@");
    write(client.desc, message, sizeof(message));
}

void sendRoundNumberToClient(client_struct client)
{
    char message[5];
    memset(message, 0, 5);
    strcat(message, "r");
    strcat(message, std::to_string(roundNumber).c_str());
    strcat(message, "@");
    write(client.desc, message, sizeof(message));
}

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
    sendRoundTimeToClient(lastClient);
    sendRoundNumberToClient(lastClient);

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
            else
            {
                write(client.desc, "l3@", 3);
                // wysłanie info o dolaczeniu gracza
            } 
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

    insertAllSets();
    srand(time(0));

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
                else if(message[0] == 'g') // wysylamy 1 zestaw liter???
                {
                    isGameStarted = true;

                    int firstSet = rand()%NUMBER_OF_SETS;
                    allSets.at(firstSet).played = true;

                    char firstRound[7] = "";
                    strcat(firstRound, "g");
                    strcat(firstRound, allSets.at(firstSet).letters.c_str());
                    sendToAllClients(strcat(firstRound, "@"));
                    //zrobienie watków do wszystkich graczy
                    break; //wyjscie z petli
                }

                strcat(message, "@");
                sendToAllClientsWithoutGameMaster(message);
            }


        }
    }

    while(1) // rozpoczecie watku od gry;
    {

    }

}