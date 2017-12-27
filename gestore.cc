/**
 * @mainpage gestore
 *
 * Programma che simula una partita di poker. Gestore permette di stabilire e
 * gestire la connessione tra client e server in modo da poter far giocare a
 * due utenti una partita di poker.
 *
 * Vedere la documentazione della funzione ::main per maggiori dettagli
 * sulle funzionalità e sulla loro implementazione.
 *
 * @author Matteo Vanzini, Luca Sala
 */

 /**
  * @file
  * File contenente l'intero programma.
  */

//sudo /etc/init.d/networking restart

#include <iostream>
#include <stdio.h>
#include <string.h> // strlen
#include <stdlib.h> // strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> // write
#include <pthread.h> // for threading, link with lpthread
#include <math.h>
#include "struttura_dati.cc"
#include "gestione_mazzo.cc"
#include "gestione_turni.cc"
#include "gestione_vincita.cc"
#include "gestione_game.cc"
using namespace std;
//definizione macro per testing e tracing e modserver del Programma
//per attivare modalità testing -D test
//per attivare tracing -D tracing
//per attivare modserver -D modserver

//per compilare con sdl #include<SDL2/SDL.h> e poi a tempo di compilazione-lSDL2

void *connection_handler(void *);
int start_server();
int start_client();

/** Avvio del programma come server.
 *
 * Crea un socket e mette il programma in ascolto sulla porta 8080,
 * fino a quando dalla funzione ::start_client() qualcuno non cerca
 * di connettersi su di essa.
 * in caso di connessione riuscita, lancia su un thread la funzione
 * ::connection_handler() la quale fungerà da main del server.
 */
int start_server(){
  int socket_server, client_sock, c;
	struct sockaddr_in server, client;
	// Create socket

	socket_server = socket(AF_INET, SOCK_STREAM, 0);
  #ifdef bob
    close(socket_server);
  #endif
	if(socket_server == -1) {
		cout<<"Could not create socket"<<endl;
	}
	cout<<"Socket created"<<endl;
	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8080);
	// Bind
	if(bind(socket_server,(struct sockaddr *)&server, sizeof(server)) < 0) {
		// print the error message
		perror("bind failed. Error");
		return 1;
	}
 cout<<"bind done"<<endl;
	// Listen
	listen(socket_server, 3);
	// Accept and incoming connection
	cout<<"Waiting for incoming connections..."<<endl;
	c = sizeof(struct sockaddr_in);
	// Accept and incoming connection
	cout<<"Waiting for incoming connections..."<<endl;
	c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	while((client_sock = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c))) {
		cout<<"Connection accepted"<<endl;
		if(pthread_create( &thread_id, NULL,  connection_handler, (void*) &client_sock) < 0) {
			perror("could not create thread");
			return 1;
		}
		// Now join the thread, so that we dont terminate before the thread
		// pthread_join( thread_id, NULL);
		cout<<"Handler assigned"<<endl;
	}
	if (client_sock < 0) {
		perror("accept failed");
		return 1;
	}
}

/** Main del server.
 *
 * In qualità di main del server si occupa di richiamare in modo
 * sequenziale le funzioni base per costruire l'ambiente di gioco.
 */
void *connection_handler(void *socket_server) {
	// Get the socket descriptor

	sock = *(int*)socket_server;
	int read_size;

  giocatore *g = new giocatore[NUMERO_GIOCATORI];
  carta *mazzo=new carta[NUMERO_CARTE];
  int puntatore_mazzo;
  carta *verifica= new carta[7];


  memset(client_reply,0,2000);
  memset(client_message,0,2000);
  //richiesta nome client
  while (true){
    if( (read_size = recv(sock, client_reply, 2000, 0)) > 0 ){
      //copio nome dentro nome
      for(int i=0;i<strlen(client_reply);i++){
        g[1].nome[i] = client_reply[i];
      }
      cout<<"----------------client--------"<<g[1].nome<<endl;
      break;
    }
  }
  //richiesta nome server
  cout<<"nome srv: ";
  cin>>g[0].nome;
  cout<<"-----------------server-----------"<<g[0].nome<<endl;

  int counter=0;
  do{
    memset(client_reply,0,2000);
    if(counter == 0){
      avviaPartita(mazzo, g);
      counter++;
    }
    game(g,mazzo,puntatore_mazzo,verifica);
  }while( (read_size = recv(sock, client_reply, 2000, 0)) > 0 );

	if(read_size == 0) {
		cout<<"Client disconnected"<<endl;
		fflush(stdout);
		close(sock);
	}
	return 0;
}

/** Ricezione delle mani dei giocatori. */
void ricezioneZero(){
  //ASCOLTO
  int v1,v2;
  char s1,s2;
  v1 = server_reply[2] - '0' ;
  v2 = server_reply[5] - '0' ;
  if(server_reply[1]=='2')
     v1 += 10;
  if(server_reply[4]=='2')
    v2 += 10;
  s1 = server_reply[3];
  s2 = server_reply[6];
  memset(server_reply,0,2000);

  //RISPONDO
  client_message[0]='0';
  client_message[1]='\0';
  send(sock,client_message,strlen(client_message),0);
  memset(client_message,0,2000);
}

/** Ricezione dei soldi.
 *
 * In base ad un secondo identificatore inserito all'interndo del messaggio
 * la funzione riconoscerà se si tratta dei soldi dei giocatori o dei soldi
 * presenti sul piatto.
 */
void ricezioneSoldi(){
  //ASCOLTO
  int len = strlen(server_reply);
  int tipo = (int) server_reply[1] - '0' ;
  int m=0;
  cout<<"Len = "<<len<<" ,Tipo = "<<tipo<<" Message = "<<server_reply<<endl;
  //message example 124990049950
  //                0123456789
  if(tipo>1){
    int soldi=0;
    int esp;//esponente per potenza
    for(int i=2;i<len;){
      int n = (int)server_reply[i] - '0' ;
      i++;
      esp = (n-1) ;
      cout<<"n : "<<n<<" , esp : "<<esp<<" , i : "<<i<<" , soldi :"<<soldi<<endl;
      for(int c = 0; c < n ; c++ , esp-- , i++){
        m = pow(10,esp);
        soldi += ((int)server_reply[i] - '0')*m;
      }

      cout<<"g[] : "<<soldi<<endl;
      soldi=0;
    }
  }else{
    int piatto=0;
    int n= (int)server_reply[2] - '0';
    for(int i=3, c=n-1 ; i<len; i++, c--){
      m=pow(10,c);
      piatto += ((int)server_reply[i] - '0')*m;
    }
    cout<<"piatto: "<<piatto<<endl;
  }
  memset(server_reply,0,2000);

  //RISPONDO
  client_message[0] = '0';
  client_message[1]='\0';
  send(sock,client_message,strlen(client_message),0);
  memset(client_message,0,2000);
}

/** Ricezione della scelta.
 *
 * Questa funzione riceve le possibili scelte effettuabili nel turno corrente
 * del client e risponde al server inviandogli la scelta effettuata.
 */
void ricezioneScelta(){
  //ASCOLTO
  int len = strlen(server_reply);
  cout<<"Scelta : ";
  cout<<"Len = "<<len<<"  Message = "<<server_reply<<endl;
  for(int i = 1; i< len; i++)
      cout<<server_reply[i]<<" ";
  cout<<endl<<": ";
  memset(server_reply,0,2000);

  //RISPONDO
  cin>>client_message[0];

  client_message[1]='\0';
  send(sock, client_message, strlen(client_message),0);
  memset(client_message,0,2000);
}

/** Ricezione del rilancio.
 *
 * Nel caso in cui la scelta sia bet, questa funzione serve per prendere in
 * input la quantità di soldi da rilanciare e poi la invia al server per le
 * successive operazioni da eseguire.
 */
void ricezioneRilancio(){
  //ASCOLTO
  int len = strlen(server_reply);
  int valore = 0;
  int contatore;
  int comodo;
  for(int i = 1; i < len; i++){
    cout<<server_reply[i];
  }
  cout<<endl<<": ";
  memset(server_reply,0,2000);

  //RISPONDO
  cin>>valore;

  if(valore>=10000)
    comodo=5;
  else
    if(valore>=1000)
      comodo=4;
    else
      if(valore>=100)
        comodo=3;
      else
        if(valore>=10)
          comodo=2;
        else
          comodo=1;
  contatore = 0;
  client_message[contatore]=(char)comodo + '0';
  contatore++;
  int soldi_rimanenti = valore;
  int m=0;
  for(int k=0;k<=comodo;k++,comodo--){
   m=pow(10,comodo)/10;
   client_message[contatore] = ((char)(soldi_rimanenti/m)) + '0';
   soldi_rimanenti -= ((soldi_rimanenti/m)*m);
   contatore++;
  }
  client_message[contatore]=(char)(soldi_rimanenti) + '0';
  contatore++;
  client_message[contatore] = '\0';
  contatore++;
  send(sock,client_message,strlen(client_message),0);
  memset(client_message,0,2000);
}

/** Ricezione del fold. */
void ricezioneFold(giocatore *client_g){
  //ASCOLTO
  int winner;
  if(server_reply[1]){//qualcuno ha foldato
    int folder=(int)server_reply[2] - '0'; //che è il folder
    cout<<"Folder : "<<folder<<endl;
    if(folder == 0) {
      cout<<"Fold di "<<client_g[0].nome<<endl;
    }else{
      cout<<"Hai foldato"<<endl;
    }
  }else{
    winner=(int)server_reply[2] - '0';
    if(winner == 0) {
      cout<<"ha vinto il server"<<endl;
    }else{
      cout<<"Hai vinto"<<endl;
    }
  }
  memset(server_reply,0,2000);

  //RISPONDO
  client_message[0] = '0';
  client_message[1]='\0';
  send(sock,client_message,strlen(client_message),0);
  memset(client_message,0,2000);
}

/** Main del client.
 *
 * Specificatogli l'indirizzo sul quale tentare una connessione si occupa di
 * stabilirla e di creare un socket di comunicazione.
 * Il socket di comunicazione è memorizzato all'interno della variabile ::sock .
 *
 * In qualità di main del client, si occupa della ricezione dei messaggi
 * ed in base ad un indentificatore definito in ogni messaggio di richiamare
 * una diversa funzione.
 */
int start_client(){
  struct sockaddr_in server;

  giocatore *client_g = new giocatore[NUMERO_GIOCATORI];//posizione  per client, posizione 0 per server
  //tavolo *client_t;

  memset(server_reply,0,2000);
  memset(client_message,0,2000);

  //Creazione socket
  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1)
  {
      cout<<"Impossibile creare soket"<<endl;
  }
  cout<<"Socket creato"<<endl;

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8080 );

  //connessione al server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
      perror("Connessione fallita. E : " );
      return 1;
  }

  cout<<"Connessione stabilita!"<<endl<<endl;

  //setting nome client
  cout<<"Enter name : ";
  cin>>client_g[1].nome;

  send(sock,client_g[1].nome,strlen(client_g[1].nome),0);


  memset(client_message,0,2000);
  while(1)
  {
      if( recv(sock , server_reply , 2000 , 0) < 0)
      {
          cout<<"errore ricezione messaggio server"<<endl;
          break;
      }else{
        switch(server_reply[0]){
          case '0':  ricezioneZero();
                    break;
          case '1':  ricezioneSoldi();
                    break;
          case '2':  ricezioneScelta();
                    break;
          case '3':  ricezioneRilancio();
                    break;
          case '4':  ricezioneFold(client_g);
                    break;
          default: cout<<"errore di comunicazione. Err : "<<server_reply[0]<<endl;
                    break;
        }
      }
      memset(server_reply,0,2000);
    }
    close(sock);
    return 0;
}


/* Funzione principale
 *
 */
int main(){
  int scelta;
  cout<<"0-->server    1-->client";
  cin>>scelta;
  switch(scelta){//1
    case 1 : cout<<"Starting client............";
        start_client();
        break;
    case 0 :  cout<<"Starting server............";
        start_server();
        break;
  }
  return 0;
}
