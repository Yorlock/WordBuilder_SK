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
#include <bits/stdc++.h>

using namespace std;

#define SERVER_PORT 1112
#define NUMBER_OF_SETS 15

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
    long unsigned int alreadyGuessed = 0;
};

vector<pollfd> pollfdDescr;
vector<client_struct> allClients;
vector<word_struct> allSets;
vector<string> ranking;
client_struct gameMaster;
int currentSet = 0;
int previousSet = 0;
int currentRound = 1;
int currentTime = 0;
int socketServer;
int roundTime = 100;
int roundNumber = 5;
bool isGameStarted = false;
bool isTimeOver = false;

bool endTimer = false;

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

    word[3].letters = "SZOKP";
    word[3].correctWords.push_back(make_pair("SZKOP", false));
    word[3].correctWords.push_back(make_pair("KOSZ", false));
    word[3].correctWords.push_back(make_pair("SZOK", false));
    word[3].correctWords.push_back(make_pair("SZOP", false));
    word[3].correctWords.push_back(make_pair("SOK", false));
    word[3].correctWords.push_back(make_pair("KOS", false));
    word[3].correctWords.push_back(make_pair("OK", false));

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
    word[5].correctWords.push_back(make_pair("POKAZ", false));

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

    word[11].letters = "ORZZA";
    word[11].correctWords.push_back(make_pair("ZORZA", false));
    word[11].correctWords.push_back(make_pair("ORAZ", false));
    word[11].correctWords.push_back(make_pair("ZRAZ", false));
    word[11].correctWords.push_back(make_pair("ZZA", false));
    word[11].correctWords.push_back(make_pair("RAZ", false));

    word[12].letters = "SMATA";
    word[12].correctWords.push_back(make_pair("ASTMA", false));
    word[12].correctWords.push_back(make_pair("MASA", false));
    word[12].correctWords.push_back(make_pair("MATA", false));
    word[12].correctWords.push_back(make_pair("TAMA", false));
    word[12].correctWords.push_back(make_pair("SAM", false));
    word[12].correctWords.push_back(make_pair("TAM", false));
    word[12].correctWords.push_back(make_pair("MAT", false));

    word[13].letters = "SAAKM";
    word[13].correctWords.push_back(make_pair("MASKA", false));
    word[13].correctWords.push_back(make_pair("KASA", false));
    word[13].correctWords.push_back(make_pair("MAKS", false));
    word[13].correctWords.push_back(make_pair("MASA", false));
    word[13].correctWords.push_back(make_pair("SMAK", false));
    word[13].correctWords.push_back(make_pair("SAM", false));
    word[13].correctWords.push_back(make_pair("MAK", false));

    word[14].letters = "ORZZA";
    word[14].correctWords.push_back(make_pair("ZORZA", false));
    word[14].correctWords.push_back(make_pair("ORAZ", false));
    word[14].correctWords.push_back(make_pair("ZRAZ", false));
    word[14].correctWords.push_back(make_pair("ZZA", false));
    word[14].correctWords.push_back(make_pair("RAZ", false));
    
    for(word_struct w : word)
        allSets.push_back(w);
}

int findClientByFd(int fd)
{
    for(unsigned long int i=0; i<allClients.size(); i++)
    {
        if(allClients[i].desc == fd) return i;
    }
    return 0;
}

void disconnectClient(int fd)
{
    int posOfClient = findClientByFd(fd);
    if(!isGameStarted) 
    {
        char message[34] = "Q";
        strcat(message, allClients[posOfClient].nick);
        strcat(message, "@");
        for(int i = 0; i < (int)allClients.size(); i++)
        {
            if(i != posOfClient)
            {
                int tmp = write(allClients[i].desc, message, sizeof(message));
                if (tmp == -1) 
                {
                    auto err = errno;
                    cout <<err<<"\n";
                    if(err == 32) disconnectClient(allClients[i].desc);
                    else error(1, err, "Blad podczas wysylania");
                }
                cout << "Send Q: " << allClients[posOfClient].nick<<"\n";
            }
        }
    }
    allClients.erase(allClients.begin() + posOfClient);
    for (int i = 0; i < (int)pollfdDescr.size(); i++)
    {
        if(pollfdDescr[i].fd == fd) pollfdDescr.erase(pollfdDescr.begin() + i);
    }
    if(close(fd) == -1) error(1, errno, "Blad podczas disconnect");
    cout << "Rozlaczono klienta o fd: " << fd <<"\n";
}

void disconnectAllClients()
{
    for(auto client : allClients)
    {
		if(close(client.desc) == -1) error(1, errno, "Blad podczas disconnect");
        cout << "Rozlaczono klienta o fd: " << client.desc <<"\n";
    }
}

int checkCorrectnessOfWord(client_struct client, string word)
{
    
    for(int i=0; i < (int)allSets.at(currentSet).correctWords.size(); i++)
    {
        if(word == allSets.at(currentSet).correctWords[i].first) // takie slowo istnieje
        {
            if(!allSets.at(currentSet).correctWords[i].second) // nie zgadniety jeszcze - gracz dostaje punkt
            {
                allSets.at(currentSet).alreadyGuessed++;
                allSets.at(currentSet).correctWords[i].second = true;
                return 1;
            }
            return 0; // slowo zostalo zgadniete - bez zmian pkt
        }
    }
    return -1; // gracz traci punkt
}

void sendRoundTimeToClient(client_struct client, string typeOfMessage)
{
    char message[5];
    memset(message, 0, 5);
    strcat(message, typeOfMessage.c_str());
    if(isGameStarted) strcat(message, std::to_string(currentTime).c_str());
    else strcat(message, std::to_string(roundTime).c_str());
    strcat(message, "@");
    int tmp = write(client.desc, message, sizeof(message));
    if (tmp == -1) 
    {
        auto err = errno;
        cout <<err<<"\n";
        if(err == 32) disconnectClient(client.desc);
        else error(1, err, "Blad podczas wysylania");
    }
}

void sendRoundNumberToClient(client_struct client, string typeOfMessage)
{
    char message[5];
    memset(message, 0, 5);
    strcat(message, typeOfMessage.c_str());
    if(typeOfMessage == "z") strcat(message, std::to_string(currentRound).c_str());
    else strcat(message, std::to_string(roundNumber).c_str());
    strcat(message, "@");
    int tmp = write(client.desc, message, sizeof(message));
    if (tmp == -1) 
    {
        auto err = errno;
        cout <<err<<"\n";
        if(err == 32) disconnectClient(client.desc);
        else error(1, err, "Blad podczas wysylania");
    }
}

void sendToAllClients(char* message)
{
    for(auto client : allClients)
    {                     
        int tmp = write(client.desc, message, strlen(message));
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(client.desc);
            else error(1, err, "Blad podczas wysylania");
        }
        cout<< "(All) Fd: " << client.desc <<" msg: " << message << endl;
    }
}

void sendToAllClientsWithoutGameMaster(char* message)
{
    for(auto client : allClients)
    {
        if(client.desc != gameMaster.desc) 
        {
            int tmp = write(client.desc, message, strlen(message));
            if (tmp == -1) 
            {       
                auto err = errno;
                       cout <<err<<"\n";
                if(err == 32) disconnectClient(client.desc);
                else error(1, err, "Blad podczas wysylania");
            }
            cout<< "(All) Fd: " << client.desc <<" msg: " << message << endl;
        }
    }
}

void sendToAllClientsWhenWordIsGuessed(client_struct &guesser, string word)
{
    int point = checkCorrectnessOfWord(guesser, word);
    guesser.score += point;
    if(point == 1)
    {
        char message[10] = "";
        strcat(message, "o");
        strcat(message, word.c_str());
        sendToAllClients(strcat(message, "@"));
    }
}

void sendNewNickToPlayersInLobby()
{
    client_struct lastClient = allClients.back();
    for(int i=0; i< (int)allClients.size() - 1; i++)
    {
        char message [32] = "n";
        strcat(message, allClients[i].nick);
        strcat(message, "@");
        int tmp = write(lastClient.desc, message, sizeof(message));
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(lastClient.desc);
            else error(1, err, "Blad podczas wysylania");
        }
        cout << "(last joined) Fd: "<< lastClient.desc << " msg: " << message << endl;
    }
    sendRoundTimeToClient(lastClient, "t");
    sendRoundNumberToClient(lastClient, "r");

    char message [32] = "n";
    strcat(message, lastClient.nick);
    strcat(message, "@");
    sendToAllClients(message);
}

int generateRandomSet()
{
    while(1)
    {
        int set = rand()%NUMBER_OF_SETS;
        if(!allSets.at(set).played)
        {
            allSets.at(set).played = true;
            previousSet = currentSet;
            currentSet = set;
            return set;
        }
    }
}

void sendRandomSet(int set)
{
    char buffor[10] = "";
    strcat(buffor, "q");
    strcat(buffor, allSets.at(set).letters.c_str());
    strcat(buffor, "@");
    sendToAllClients(buffor);
}

void sendRandomSet(client_struct client, int set)
{
    char buffor[10] = "";
    strcat(buffor, "q");
    strcat(buffor, allSets.at(set).letters.c_str());
    strcat(buffor, "@");
    int tmp = write(client.desc, buffor, strlen(buffor));
    if (tmp == -1) 
    {
        auto err = errno;
        cout <<err<<"\n";
        if(err == 32) disconnectClient(client.desc);
        else error(1, err, "Blad podczas wysylania");
    }
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

bool compareTwoClients(client_struct a, client_struct b)
{
    return a.score > b.score;
}

vector<string> generateRanking()
{
    vector<string> result;
    sort(allClients.begin(), allClients.end(), compareTwoClients);
    for(client_struct client : allClients)
    {
        string helper;
        helper.append(client.nick);
        helper.append(" ");
        helper.append(to_string(client.score));
        cout << "Ranking: " << helper <<"\n";
        result.push_back(helper);
    }
    return result;
}

void sendWholeRankingToClient(client_struct client, vector<string> ranking)
{
    for(string s : ranking)
    {
        char message[34] = "";
        strcat(message, "c");
        strcat(message, s.c_str());
        strcat(message, "@");
        int tmp = write(client.desc, message, sizeof(message));
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(client.desc);
            else error(1, err, "Blad podczas wysylania");
        }
    }
}

void sendWholeRankingAfterGameToClient(client_struct client, vector<string> ranking)
{
    for(string s : ranking)
    {
        char message[34] = "";
        strcat(message, "C");
        strcat(message, s.c_str());
        strcat(message, "@");
        int tmp = write(client.desc, message, sizeof(message));
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(client.desc);
            else error(1, err, "Blad podczas wysylania");
        }
    }
}

void sendSolvedWordsToClient(client_struct client, word_struct words)
{
    for(pair word : words.correctWords)
    {
        char message[10] = "";
        strcat(message, "p");
        strcat(message, word.first.c_str());
        strcat(message, "@");
        int tmp = write(client.desc, message, sizeof(message));
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(client.desc);
            else error(1, err, "Blad podczas wysylania");
        }
    }
}

void sendGuessedWords(client_struct client, int set)
{
    for(pair word : allSets[set].correctWords)
    {
        if(word.second)
        {
            char message[10] = "";
            strcat(message, "o");
            strcat(message, word.first.c_str());
            strcat(message, "@");
            int tmp = write(client.desc, message, sizeof(message));
            if (tmp == -1) 
            {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(client.desc);
            else error(1, err, "Blad podczas wysylania");
            }
        }
    }
}

void addPlayerToGame(client_struct client)
{
    pollfd tmp;
    tmp.fd = client.desc;
    tmp.events = POLLIN;
    pollfdDescr.push_back(tmp);
    sendRandomSet(client, currentSet);
    sendGuessedWords(client, currentSet);
    sendRoundTimeToClient(client, "x");
    sendRoundNumberToClient(client, "z");
    sendWholeRankingToClient(client, ranking);
    if(currentRound > 1) sendSolvedWordsToClient(client, allSets.at(previousSet));
    char message [34] = "c";
    string helper;
    helper.append(client.nick);
    helper.append(" ");
    helper.append(to_string(client.score));
    ranking.push_back(helper);
    strcat(message, helper.c_str());
    strcat(message, "@");
    sendToAllClients(message);
}

void newRound()
{
    ranking = generateRanking();
    sendRandomSet(generateRandomSet());
    for(client_struct client : allClients)
    {
        sendRoundTimeToClient(client, "x");
        sendRoundNumberToClient(client, "z");
        sendWholeRankingToClient(client, ranking);
        sendSolvedWordsToClient(client, allSets.at(previousSet));
    }
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
        int count = read(client.desc, client.nick, 32);
        if(count == -1) error(1, errno, "Blad read'a");
        else if (count == 0) disconnectClient(client.desc);
        else{
            if(checkNick(client.nick))
            {
                if(allClients.empty()) gameMaster = client;
                printf("Dodano %s do gry\n", client.nick);
                allClients.push_back(client);
                //1 - klient może dołączyć do gry i gra nie jest rozpoczeta i jest gamemasterem
                if(!isGameStarted && allClients.size() == 1)
                {
                    int tmp = write(client.desc, "l1@", 3);
                    if (tmp == -1) 
                    {
                        auto err = errno;
                        cout <<err<<"\n";
                        if(err == 32) disconnectClient(client.desc);
                        else error(1, err, "Blad podczas wysylania");
                    }
                    sendNewNickToPlayersInLobby();
                }
                //2 - klient może dołączyć do gry i gra nie jest rozpoczeta
                else if(!isGameStarted)
                {
                    int tmp = write(client.desc, "l2@", 3);
                    if (tmp == -1) 
                    {
                        auto err = errno;
                        cout <<err<<"\n";
                        if(err == 32) disconnectClient(client.desc);
                        else error(1, err, "Blad podczas wysylania");
                    }
                    sendNewNickToPlayersInLobby();
                }
                //3 - klient może dołączyć do gry i gra jest rozpoczeta
                else
                {
                    int tmp = write(client.desc, "l3@", 3);
                    if (tmp == -1) 
                    {
                        auto err = errno;
                        cout <<err<<"\n";
                        if(err == 32) disconnectClient(client.desc);
                        else error(1, err, "Blad podczas wysylania");
                    }
                    addPlayerToGame(client);
                }
            }
            else
            {
                printf("Nick: %s jest juz zajety\n", client.nick);
                //4 - klient nie moze dolaczyc do rozrywki
                int tmp = write(client.desc, "l4@", 3);
                if (tmp == -1) 
                {
                    auto err = errno;
                    cout <<err<<"\n";
                    if(err == 32) disconnectClient(client.desc);
                    else error(1, err, "Blad podczas wysylania");
                }
                close(client.desc);
            }
        }
    }
}

void disconnectGameMaster()
{
    disconnectClient(gameMaster.desc);
    if(!allClients.empty()) 
    {
        gameMaster = allClients[0];
        char message[3] = "";
        strcat(message, "a");
        strcat(message, "@");
        int tmp = write(gameMaster.desc, message, 3);
        if (tmp == -1) 
        {
            auto err = errno;
            cout <<err<<"\n";
            if(err == 32) disconnectClient(gameMaster.desc);
            else error(1, err, "Blad podczas wysylania");
        }
    }
    else gameMaster.desc = 0;
}

void inTheWaitingRoom()
{
    while(1)
    {
        if(gameMaster.desc)
        {
            char message[5];
            memset(message, 0, 5);
            int count = read(gameMaster.desc, message, 5);
            if(count == -1) error(1, errno, "Blad read'a");
            else if (count == 0) disconnectGameMaster();          
            else if(count > 0)
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
                    char startGame[3] = "";
                    strcat(startGame, "g");
                    sendToAllClients(strcat(startGame, "@"));
                    sendRandomSet(generateRandomSet());
                    ranking = generateRanking();
                    for(client_struct client : allClients)
                    {
                        sendRoundTimeToClient(client, "x");
                        sendRoundNumberToClient(client, "z");
                        sendWholeRankingToClient(client, ranking);
                    }
                    isGameStarted = true;
                    //zrobienie watków do wszystkich graczy
                    break; //wyjscie z petli
                }

                strcat(message, "@");
                sendToAllClientsWithoutGameMaster(message);
            }
        }
    }
}

void sendToAllClientsThatGameIsOver()
{
    char message[3] = "E";
    strcat(message, "@");
    sendToAllClients(message);
    ranking = generateRanking();
    for(client_struct client : allClients)
        sendWholeRankingAfterGameToClient(client, ranking);
}

void endGameTooLittlePlayers()
{
    cout << "Gra została zakończona. Za mało graczy.\n";
    char message[3] = "L";
    strcat(message, "@");
    sendToAllClients(message);
}

void *timer(void *)
{
    for(int i=0; i<roundTime; i++)
    {
        if(endTimer || allSets.at(currentSet).alreadyGuessed == allSets.at(currentSet).correctWords.size())
        {
            endTimer = false;
            return nullptr;
        }
        currentTime--;
        sleep(1);
    }
    endTimer = false;
    isTimeOver = true;
    return nullptr;
}

void *listenClientsInGame(void *)
{
    for(unsigned long int i=0; i<allClients.size(); i++)
    {
        pollfd tmp;
        tmp.fd = allClients[i].desc;
        tmp.events = POLLIN;
        pollfdDescr.push_back(tmp);
    }

    while(1)
    {
        int ready = poll(&pollfdDescr[0], pollfdDescr.size(), 100);
        if(!isGameStarted) return nullptr;
        if(ready == -1){
            error(0, errno, "Poll failed\n");
            closeServer(0);
        }
        for(unsigned long int i=0; i<allClients.size() && ready>0; i++)
        {
            if(!isGameStarted) return nullptr;
            if(pollfdDescr[i].revents & (POLLERR|POLLHUP|POLLRDHUP)) {
                error(0, errno, "Poll failed\n");
                closeServer(0);
            }
            if(pollfdDescr[i].revents & POLLIN)
            {
                char buffer[255] = "";
                ssize_t received = read(pollfdDescr[i].fd, buffer, 255);
                if(received == -1) error(1, errno, "Blad read'a\n");
                else if(received == 0) disconnectClient(pollfdDescr[i].fd);
                else
                {
                    cout << "Odebrano slowo: " << buffer <<"\n";
                    int posOfClient = findClientByFd(pollfdDescr[i].fd);
                    sendToAllClientsWhenWordIsGuessed(allClients[posOfClient], string(buffer));
                }
            }
        }
    }
    return nullptr;
}

void inTheGameWindow()
{
    isTimeOver = false;
    currentTime = roundTime;

    pthread_t threadTimer;
    int threadTimerId = pthread_create(&threadTimer, NULL, timer, NULL);
    if(threadTimerId) error(1, errno, "Blad watku\n");
    
    pthread_t threadListen;
    int threadListenId = pthread_create(&threadListen, NULL, listenClientsInGame, NULL);
    if(threadListenId) error(1, errno, "Blad watku\n");

    while(1)
    {
        if(allClients.size() < 2)
        {
            endGameTooLittlePlayers();
            endTimer = true;
            return;
        } 
        if(isTimeOver || allSets.at(currentSet).alreadyGuessed == allSets.at(currentSet).correctWords.size())
        {
            if(!isTimeOver) endTimer = true;
            isTimeOver = true;
            currentRound++;
            if(roundNumber < currentRound) break;
            isTimeOver = false;
            currentTime = roundTime;
            newRound();
            int threadTimerId = pthread_create(&threadTimer, NULL, timer, NULL);
            if(threadTimerId) error(1, errno, "Blad watku\n");
        }
    }
    sendToAllClientsThatGameIsOver();   
}

void resetServer()
{
    isGameStarted = false;
    pollfdDescr.clear();
    allSets.clear();
    insertAllSets();
    disconnectAllClients();
    allClients.clear();
    gameMaster.desc = 0;
    currentSet = 0;
    previousSet = 0;
    currentRound = 1;
    currentTime = 0;
    roundTime = 100;
    roundNumber = 5;
    isTimeOver = false;
}

int main(int argc, char ** argv)
{
    printf("Start serwera\n");

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(socketServer == -1) error(1, errno, "Blad socket Server\n");

    setReuseAddr(socketServer);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, closeServer);

    sockaddr_in serverAddr{
		.sin_family=AF_INET, 
		.sin_port=htons(SERVER_PORT),
		.sin_addr={INADDR_ANY}
	};

    int serverBind = bind(socketServer, (sockaddr*) &serverAddr, sizeof(serverAddr));
	if(serverBind) error(1, errno, "Blad server Bind\n");

    serverBind = listen(socketServer, 4);
    if(serverBind) error(1, errno, "Blad listen\n");

    insertAllSets();
    srand(time(0));

    pthread_t accepting;
    int threadAccepting = pthread_create(&accepting, NULL, acceptingClients, NULL);
    if(threadAccepting) error(1, errno, "Blad watku\n");

    while(1)
    {
        inTheWaitingRoom();
        inTheGameWindow();
        resetServer();
    }
}