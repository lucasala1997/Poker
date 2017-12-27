/**
 * @file
 * File contenente tutte le funzioni per la gestione dei turni di gioco.
 */

/** Gestisce l'invio e la ricezione delle scelte effettuabili da un giocatore.
 *
 * In base al valore di ::small_who darà possibilità di scelta al client o al
 * server.
 * più precisamente se 0 = server ,se 1 = client.
 * Nel caso del client, invierà un messaggio contenente le possibili scelte
 * effettuabili , cioè: "check" "call" "bet" "fold".
 */
char interazione(char *array,int small_who){
  char scelta='0';
  if(!small_who){
    cout<<"Scelta : ";
    for(int i=1;i<4;i++)
      cout<<array[i]<<", ";
    cout<<"\n Cosa fare?   ";
    cin>>scelta;
  }else{
    send(sock,array,strlen(array),0);
    int read_size=-1;
    if( (read_size = recv(sock, client_reply, 2000, 0)) < 0 ){
      cout<<"Errore ricezione client_reply, pos : fun. interazione";
    }else{
      scelta=client_reply[0];
      cout<<client_reply[0];
      cout<<endl<<"Client : "<<scelta<<endl;
    }
  }
  memset(client_reply,0,2000);
  return scelta;
}

/** Gestisce il tipo di scelta "rilancio".
 *
 * In base al valore di ::small_who gestisce la scelta del server o del client
 * più precisamente se 0 = server ,se 1 = client.
 * Inoltre richiede anche di quanti soldi rilanciare.
 */
int rilancio(int small_who){
  int alzata=0;
  int comodo=0;
  int decremento=0;
  int m=0;

  if(small_who){
    cout<<" rilanciare di: ";
    cin>>alzata;
  }else{
    char raise[]={"3rilanciare di : "};
    send(sock,raise,strlen(raise),0);
    int read_size;
    while (true){
      if( (read_size = recv(sock, client_reply, 2000, 0)) > 0 ){
        comodo=(int)client_reply[0] -'0';
        decremento = comodo;

        cout<<"Messaggie: "<<client_reply<<endl;
        for(int k=1;k<=comodo;k++,decremento --){
          m=pow(10,decremento)/10;
          alzata += ((int)(client_reply[k]) - '0')*m;
        }

        cout<<"Client : "<<alzata<<endl;
        memset(client_reply,0,2000);
        break;
      }
    }
  }
  return alzata;
}

/** Costruisce il messaggio da inviare al client.
 *
 * In base al tipo saprà se deve inviare il piatto oppure i soldi dei giocatori
 * ogni mano.
 */
int sendMoney(int tipo, int *soldi){
  int contatore=0;
  int len = 0, esp = 0 , m = 0 , comodo = 0;
  server_message[contatore]='1';
  contatore++;
  server_message[contatore]=(char)tipo +'0';
  contatore++;
  for(int i=0;i<tipo;i++){
    if(soldi[i]>=10000){
      server_message[contatore]='5';
    }else{
      if(soldi[i]>=1000){
        server_message[contatore]='4';
      }else{
        if(soldi[i]>=100){
          server_message[contatore]='3';
        }else{
          if(soldi[i]>=10){
            server_message[contatore]='2';
          }else{
            server_message[contatore]='1';
            contatore++;
            server_message[contatore]=(char)soldi[i]+'0';
            contatore++;
          }
        }
      }
    }
    len = esp = (int)server_message[contatore]-'0';
    if(len<=1){
      break;
    }
    contatore++;
    esp--;
    cout<<"soldi: "<<soldi[i]<<" , esp :"<<esp<<endl;
    for(int c = 0 ; c < len ; c++ , esp-- ){
      m = pow(10,esp);
      comodo = soldi[i]/m;
      server_message[contatore]=(char)comodo +'0';
      soldi[i] -= comodo*m;
      cout<<"esp : "<<esp<<" , comodo : "<<comodo<<" , m : "<<m<<" , soldi["<<i<<"] : "<<soldi[i]<<" , server_message["<<contatore<<"] : "<<server_message[contatore]<<endl;
      contatore++;

    }
  }
  server_message[contatore] = '\0';
  contatore++;
  send(sock,server_message,strlen(server_message),0);
  memset(server_message,0,2000);
  while(true){
    int read_size=0;
    if( read_size = recv(sock , client_reply, 2000 , 0) > 0){
        cout<<endl<<"esco da sendMoney : "<<client_reply[0];
        memset(client_reply,0,2000);
        return 0;
    }
  }
}

/** Gestisce l'invio dei soldi.
 *
 * In base ai parametri passati come parametro invoca ::sendMoney()
 * la quale in base al tipo saprà come costruire il messaggio da inviare
 * al client ogni mano.
 */
void sendMoney2(int tipo, int s1,int s2 = 0){
  int soldi[2];
  soldi[0] = s1;
  soldi[1] = s2;
  if(tipo == 1){
    sendMoney(1,soldi);
  }else{
    sendMoney(2,soldi);
  }
}

/** Gestisce le scelte dei giocatori, uno alla volta.
 *
 * Vengono gestiti all'interno tutti i casi possibili di scelta.
 * in base al turno la scelta potrà variare tra due insiemi di opzioni:
 * il primo (c,b,f) il quale permette di effetuttare call, bet oppure fold,
 * utilizzato nella prima mano oppure dopo che uno ha rilanciato.
 * Il secondo (h,b,f) permette di effettuare check, bet oppure fold,
 * utilizzato in tutte le altre mani.
 */
int gestioneScelta(int s,int big_blind, giocatore *g){//FUNZIONE CHE RITORNA 0 PER FOLD, 1 PER ALTRO
    int small_who = s; /**< Variabile nella quale viene memorizzato chi deve effettuare la scelta */
    char scelta_zero[]={'2','c','b','f','\0'};//call/bet/fold
    char scelta_uno[]={'2','h','b','f','\0'};//check/bet/fold
    char scelta;//0 per turno0, c per call, h per check, b per bet, f per fold
    int punt; // variabile di comodo per prendere la puntata in input

    if(t->turno==0){
      scelta='0';
      //setto le puntate iniziali e il piatto iniziale(fatto per spiegare il meccanismo
      //in realta' per il turno zero basta : t->piatto=(bigBlindValue + smallBlindValue;)
      g[small_who].puntata=smallBlindValue;
      g[small_who].n_soldi-= smallBlindValue;
      g[big_blind].puntata=bigBlindValue;
      g[big_blind].n_soldi-= bigBlindValue;
      t->piatto=0;

      #ifdef tracing
      //////////////////stampo puntate iniziali e piatto///////////////////////
      cout<<endl;
      cout<<"----soldi "<<g[small_who].nome<<"="<<g[small_who].n_soldi<<endl;
      cout<<"----soldi "<<g[big_blind].nome<<"="<<g[big_blind].n_soldi<<endl;
      cout<<g[small_who].nome<<" puntata: "<<g[small_who].puntata<<endl;
      cout<<g[big_blind].nome<<" puntata: "<<g[big_blind].puntata<<endl;
      cout<<"piatto: "<<t->piatto<<endl<<endl;
      /////////////////////////////////////////////////////////////////////////
      #endif
    }else{
      scelta='h';
    }
    if(t->turno!=0){
      g[small_who].puntata=0;
      g[big_blind].puntata=0;
    }
    sendMoney2(2,g[0].n_soldi,g[1].n_soldi);
    sendMoney2(1,t->piatto);
    char enter=scelta;
    for(int i=0;true;i++){
      switch(scelta){
        case '0': scelta=interazione(scelta_zero,small_who);
                  if(scelta=='c'){
                    enter='h';
                    punt=smallBlindValue;
                    g[small_who].n_soldi-=punt;
                    g[small_who].puntata+=punt;
                  }
                  if(scelta=='b'){
                    punt=rilancio(small_who);
                    g[small_who].n_soldi-=(punt+(g[!small_who].puntata - g[small_who].puntata));
                    g[small_who].puntata+=(punt+(g[!small_who].puntata - g[small_who].puntata));
                  }
                  break;

        case 'c': if(!t->turno && i==1){
                    scelta=interazione(scelta_uno,small_who);
                    if(scelta=='b'){
                      punt=rilancio(small_who);
                      g[small_who].n_soldi-=(punt+(g[!small_who].puntata - g[small_who].puntata));
                      g[small_who].puntata+=(punt+(g[!small_who].puntata - g[small_who].puntata));
                    }
                  }else{
                    return 0;
                  }
                  break;

        case 'h': //cout<<endl<<enter<<endl;
                  if(enter='h' && i==2){
                    return 0;
                  }else{
                    scelta=interazione(scelta_uno,small_who);
                    if(scelta=='b'){
                      punt=rilancio(small_who);
                      g[small_who].n_soldi-=(punt+(g[!small_who].puntata - g[small_who].puntata));
                      g[small_who].puntata+=(punt+(g[!small_who].puntata - g[small_who].puntata));
                    }
                  }
                  break;

        case 'b':
                scelta=interazione(scelta_zero,small_who);
                  if(scelta=='b'){
                    punt=rilancio(small_who);
                    g[small_who].n_soldi-=(punt+(g[!small_who].puntata - g[small_who].puntata));
                    g[small_who].puntata+=(punt+(g[!small_who].puntata - g[small_who].puntata));

                  }
                  if(scelta=='c'){
                    g[small_who].n_soldi-=punt;
                    g[small_who].puntata+=punt;
                  }
                  break;
      }
      // se il giocatore attuale ha foldato, memorizzo il suo indice in folder ed esco dalla funzione
      cout<<"Small_who : "<<small_who<<endl;
      if(scelta == 'f'){
        folder=small_who;
        return 1;
      }
      //inverto giocatore
      small_who=(!small_who);
      #ifdef tracing
      ////////////////stampo puntate iniziali e piatto///////////////////////
      cout<<"----soldi "<<g[0].nome<<"="<<g[0].n_soldi<<endl;
      cout<<"----soldi "<<g[1].nome<<"="<<g[1].n_soldi<<endl;
      cout<<g[0].nome<<" puntata: "<<g[0].puntata<<endl;
      cout<<g[1].nome<<" puntata: "<<g[1].puntata<<endl;
      ///////////////////////////////////////////////////////////////////////
      #endif
    }
    return 0;
}

/** Distribuisce le coppie di carte ai giocatori.
 *
 * dopo aver assegnato ai due giocatori le coppie di carte, invia al client un
 * un messaggio contenente queste ultime.
 */
int turnoZero(int &puntatore_mazzo,carta *mazzo,giocatore *g){
  //distribuisci coppia di carte ai giocatori

  puntatore_mazzo=0;
  for (int j=0;j<2;j++){      //incremento il numero di carte
    for (int i=0; i<NUMERO_GIOCATORI; i++){  //distribuisco una carta a un giocatore e poi all'altro(x2)(incremento giocatore)
        g[i].mano[j].valore=mazzo[puntatore_mazzo].valore;
        g[i].mano[j].seme=mazzo[puntatore_mazzo].seme;

        mazzo[puntatore_mazzo].valore=0; //metto il valore 0 a tutte le carte estratte dal mazzo in modo da facilitarmi il contollo
        mazzo[puntatore_mazzo].seme=0;  //metto il seme 0 a tutte le carte estratte dal mazzo in modo da facilitarmi il contollo
        puntatore_mazzo++;//incremento puntatore per scorrimento mazzo
    }
  }
  while(1){
    int contatore=0;
    server_message[contatore]='0';
    contatore++;
    for(int i=0;i<NUMERO_GIOCATORI;i++){
      if(g[1].mano[i].valore>9){
        server_message[contatore]='2';
        contatore++;
        server_message[contatore]=((char)(g[1].mano[i].valore-10))+'0';
        contatore++;
        server_message[contatore]=g[1].mano[i].seme;
        contatore++;
      }else{
        server_message[contatore]='1';
        contatore++;
        server_message[contatore]=((char)g[1].mano[i].valore)+'0';
        contatore++;
        server_message[contatore]=g[1].mano[i].seme;
        contatore++;
      }
      server_message[contatore]='\0';
      contatore++;
    }
    send(sock,server_message,strlen(server_message),0);
    memset(server_message,0,2000);
    if( recv(sock , client_reply, 2000 , 0) < 0)
      {
          cout<<"errore ricezione da client"<<endl;
          break;
      }else{
        if(client_reply[0] = '0'){//TUTTO OK
          cout<<"esco da turno zero"<<endl;
          memset(client_reply,0,2000);
          return 0;
        }
      }
  }
}

/** Scopre le carte sul tavolo
 *
 * In base al valore di ::t.turno scopre sul tavolo 3 o 1 carta.
 * Il primo turno scoprirà 3 carte mentre gli altri una sola fino ad arrivare
 * a 5.
 */
void turnoN(int &puntatore_mazzo,carta *mazzo){
  if(t->turno==0)
    t->turno++;
  if(t->turno==1){
    for(int x=0;x<3;x++,puntatore_mazzo++){
      t->scoperte[x].valore=mazzo[puntatore_mazzo].valore;
      t->scoperte[x].seme=mazzo[puntatore_mazzo].seme;

      mazzo[puntatore_mazzo].valore=0; //metto il valore 0 a tutte le carte estratte dal mazzo in modo da facilitarmi il contollo
      mazzo[puntatore_mazzo].seme=0;  //metto il seme 0 a tutte le carte estratte dal mazzo in modo da facilitarmi il contollo
    }
  }else{
    t->scoperte[(t->turno+1)].valore=mazzo[puntatore_mazzo].valore;
    t->scoperte[(t->turno+1)].seme=mazzo[puntatore_mazzo].seme;
    puntatore_mazzo++;
  }
}

/** Setta i soldi al massimo ai giocatori.
 *
 * Richiamata ad ogni nuova partita per resettare i soldi e creare
 * un nuovo tavolo.
 */
void avviaPartita(carta *mazzo, giocatore *g){
  g[1].n_soldi=MAX_SOLDI;//setto valore max per i soldi
  g[0].n_soldi=MAX_SOLDI;
  t=new tavolo();
}
