#include "include_file.h"



/** Reset di tutti i valori per passare alla mano successiva.
 *
 * Da invocare ogni fine mano in modo da resettare
 * piatto, puntate dei giocatori, mazzo, carte del tavolo
 * e dei giocatori
 */
void resetVariable(giocatore *g, carta *mazzo){
  t->piatto=0;
  g[0].puntata=0;
  g[1].puntata=0;
  //pulisco il tavolo
  for(int i=0;i<5;i++){
    t->scoperte[i].valore=0;
    t->scoperte[i].seme=0;
  }

  //azzeramento mano
  for (int j=0;j<2;j++){      //incremento il numero di carte
    for (int i=0; i<NUMERO_GIOCATORI; i++){  //distribuisco una carta a un giocatore e poi all'altro(x2)(incremento giocatore)
        g[i].mano[j].valore=0;
        g[i].mano[j].seme='0';
    }
  }

  for (int i=0; i<NUMERO_CARTE; i++){
      mazzo[i].valore=0;
      mazzo[i].seme=0;
  }


  t->turno=0;

}

/** Ritorna 1 se il giocatore passato come parametro ha scala reale. */
int scalaReale(int indice,giocatore *g,carta *verifica){
  int num_assi=0;
  int counter=13;
  int conta;
  char seme_asso;

  for(int i=6;i>2;i--){
    if(verifica[i].valore==1)
      num_assi++;
  }

  if(num_assi<1)
    return 0;

  for(int i=6;i>=(7-num_assi);i--){//scorro solo gli assi
    conta=0;
    for(int j=0;j<=i;j++){
      if(verifica[j].seme==verifica[i].seme)
        conta++;
    }
    if(conta>=5){
      seme_asso=verifica[i].seme;
      break;
    }
  }

  if(conta<5)
    return 0;

  conta=1;
  for(int i=0;i<(7-num_assi);i++){
    if(verifica[i].seme==seme_asso){
      if(verifica[i].valore!= counter){
        return 0;
      }
      counter--;
      conta++;
      if(conta>=5){
        return 1;
      }
    }
  }


}//1

/** Ritorna 2 se il giocatore passato come parametro ha scala colore. */
int scalaColore(int indice,giocatore *g,carta *verifica){
  int conta;
  int prec=0;

  for(int i=0;i<3;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].seme==verifica[i].seme){
        if(verifica[j].valore==prec-1){
          conta++;
          prec=verifica[j].valore;
          if(conta>=5){
            //memorizzare le carte in fila per il controllo
            g[indice].controllo[0]=verifica[i].valore;
            return 2;
          }
        }
      }
    }
  }
  return 0;
}//2//2

/** Ritorna 3 se il giocatore passato come parametro ha poker. */
int poker(int indice,giocatore *g,carta *verifica){
  int conta;
  int prec=0;

  for(int i=0;i<4;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec){
        conta++;
        if(conta>=4){
          g[indice].controllo[0]=verifica[i].valore;
          return 3;
        }
      }
    }
  }
  return 0;
}//3

/** Ritorna 4 se il giocatore passato come parametro ha full. */
int full(int indice,giocatore *g,carta *verifica){
  int indici[3];
  int conta=1;
  int prec;
  for(int i=0; i<5;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec){
        indici[conta]=j;
        conta++;
      }
    }
    if(conta>=3){
      indici[0]=i;
        if(verifica[i].valore==1)
          g[indice].controllo[0]=14;
        else
          g[indice].controllo[0]=verifica[i].valore;
      break;
    }
  }
  if(conta<3){
    return 0;
  }
  //controllo coppia
  for(int i=0; i<6;i++){
    if(indici[0]!=i && indici[1]!=i && indici[2]!=i){
      prec=verifica[i].valore;
      conta=1;

      for(int j=i+1; j<7;j++){
        if(indici[0]!=j && indici[1]!=j && indici[2]!=j){
          if(verifica[j].valore==prec){
            conta++;
          }
        }
      }
      if(conta>=2){
        if(verifica[i].valore==1)
          g[indice].controllo[1]=14;
        else
          g[indice].controllo[1]=verifica[i].valore;
        return 4;
      }
    }
  }
  return 0;
}//4

/** Ritorna 5 se il giocatore passato come parametro ha colore. */
int colore(int indice,giocatore *g,carta *verifica){
  char seme;
  int conta;
  g[indice].controllo[0]=0;
  for(int i=0;i<3;i++){
    conta=0;
    seme=verifica[i].seme;
    //cout<<endl;
    for(int j=i;j<7;j++){
      if(verifica[j].seme==seme){
        //cout<<"dentro colore "<<verifica[j].valore<<verifica[j].seme<<endl;
        conta++;
      }
      if(conta>=5){
        for(int k=0;k<7;k++){
          if(verifica[k].seme==seme){
            if(verifica[k].valore>g[indice].controllo[0])
              g[indice].controllo[0]=verifica[k].valore;
            if(verifica[k].valore==1)
              g[indice].controllo[0]=14;
          }
        }
        return 5;
      }
    }
  }
  return 0;
}//5

/** Ritorna 6 se il giocatore passato come parametro ha scala. */
int scala(int indice,giocatore *g,carta *verifica){
  int conta=0;
  int prec=0;
  if(verifica[6].valore==1){//SCALA MAX ASSO=14
      prec=14;
      conta=1;
      for(int i=0;i<4;i++){
        if(verifica[i].valore==prec-1){
          conta++;
          prec=verifica[i].valore;
          if(conta>=5){
            g[indice].controllo[0]=14;
            return 6;
          }
        }else{
          break;
        }
      }
  }
  for(int i=0;i<3;i++){//ALTRI TIPI DI SCALA, SCALA MINIMA AUTOGESTITA
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec-1){
        conta++;
        prec=verifica[j].valore;
        if(conta>=5){
          g[indice].controllo[0]=verifica[i].valore;
          return 6;
        }
      }
    }
  }
  return 0;
}//6

/** Ritorna 7 se il giocatore passato come parametro ha tris. */
int tris(int indice,giocatore *g,carta *verifica){
  int conta=1;
  int prec;
  for(int i=0; i<5;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec){
        conta++;
      }
      if(conta>=3){
        if(verifica[i].valore==1)
          g[indice].controllo[0]=14;
        else
          g[indice].controllo[0]=verifica[i].valore;
        return 7;
      }
    }
  }
  return 0;
}//7

/** Ritorna 8 se il giocatore passato come parametro ha doppia coppia. */
int doppiaCoppia(int indice,giocatore *g,carta *verifica){
  int indici[3];
  int conta=1;
  int prec;
  for(int i=0; i<6;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec){
        indici[conta]=j;
        conta++;
      }
    }
    if(conta>=2){
      indici[0]=i;
      if(verifica[i].valore==1)
        g[indice].controllo[0]=14;
      else
        g[indice].controllo[0]=verifica[i].valore;
      break;
    }
  }
  if(conta<2){
    return 0;
  }
  //controllo seconda coppia
  int second_couple=0;
  for(int i=0; i<6;i++){
    if(indici[0]!=i && indici[1]!=i && indici[2]!=i){
      prec=verifica[i].valore;
      conta=1;

      for(int j=i+1; j<7;j++){
        if(indici[0]!=j && indici[1]!=j && indici[2]!=j){
          if(verifica[j].valore==prec){
            conta++;
          }
        }
      }
      if(conta>=2){
        if(verifica[i].valore==1)
          g[indice].controllo[1]=14;
        else
          g[indice].controllo[1]=prec;
        return 8;
      }
    }
  }
  return 0;
}//8

/** Ritorna 9 se il giocatore passato come parametro ha coppia. */
int coppia(int indice,giocatore *g,carta *verifica){
  int conta=1;
  int prec;
  for(int i=0; i<6;i++){
    prec=verifica[i].valore;
    conta=1;
    for(int j=i+1; j<7;j++){
      if(verifica[j].valore==prec){
        conta++;
      }
      if(conta>=2){
        if(verifica[i].valore==1)
          g[indice].controllo[0]=14;
        else
          g[indice].controllo[0]=verifica[i].valore;
        return 9;
      }
    }
  }
  return 0;
}//9

/** Ritorna 10 se il giocatore passato come parametro ha carta alta. */
int cartaAlta(int indice,giocatore *g,carta *verifica){
    if(verifica[6].valore)
      g[indice].controllo[0]=14;
    else
      g[indice].controllo[0]=verifica[0].valore;
    return 10;
} //si fa dentro lo switch//10

/** Ritorna 0/1 nel caso vinca il primo (0) o secondo giocatore (1),
 *  2 in caso di pareggio.
 */
int verificaVittoria(int numVittoria[]){
  if(numVittoria[0]>=numVittoria[1]){
    if(numVittoria[0]==numVittoria[1])
      return 2;
    else
      return 0;
  }else{
      return 1;
  }
}

/** Richiamata in caso di pareggio.
 *
 * Ritorna 0/1 se vince un sono giocatore,
 * 2 se bisogna spartire il piatto, cioè pareggio effettivo.
 */
int pari(int numVittoria[],giocatore *g){
    switch(numVittoria[0]){
      case 2 : case 3 : case 5: if(g[0].controllo[0]>g[1].controllo[0])
                return 0;
              else
                return 1;
              break;
      case 4 : case 8: if(g[0].controllo[0]>=g[1].controllo[0]){//TRIS
                        if(g[0].controllo[0]==g[1].controllo[0]){//TRIS
                          if(g[0].controllo[1]>=g[1].controllo[1]){//COPPIA
                            if(g[0].controllo[1]==g[1].controllo[1])//COPPIA
                              return 2;
                            else
                              return 0;
                          }else{
                            return 1;
                          }
                        }else{
                          return 0;
                        }
                      }else
                        return 1;
                      break;
      case 6 : case 7 : case 9 : case 10: if(g[0].controllo[0]>=g[1].controllo[0])
                                            if(g[0].controllo[0]==g[1].controllo[0])
                                              return 2;
                                            else
                                            return 0;
                                          else
                                            return 1;
                                          break;
    }
}

/** Richiamata per ordinare ::verifica per poter controllare il tipo di mano
 *  del giocatore
 */
void ins_sort(carta *a, int n){
  for (int i = 1 ; i < n ; i++) {
    int attuale = a[i].valore; ;//memorizzo valore attuale
    char seme = a[i].seme;
    int j ;//creo nuovo indice
    for (j = i ; j > 0 && a[j-1].valore < attuale ; j--){
      a[j].valore = a[j-1].valore ;
      a[j].seme = a[j-1].seme;
    }
    a[j].valore = attuale ;
    a[j].seme = seme;
  }
}

/** Richiamata per decretare il vincitore.
 *
 * Vengono richiamate in ordine le funzioni per l'assegnamento del tipo
 * di mano di ogni giocatore, le quali sono:
 * ::scalaReale ::scalaColore ::poker ::full ::colore ::scala ::tris
 * ::doppiaCoppia ::coppia ::cartaAlta ,
 * in modo da poter comparare le mani dei due giocatori.
 *
 * Viene richiamata la funzione ::verificaVittoria() e
 * in caso di pareggio viene richiamata la funzione ::pari() , la quale verifica
 * se è una reale situazione di pareggio.
 */
void winner(giocatore *g,carta *verifica){
  //uno alla volta unisco le carte di un giocatore con quelle del tavolo
  //per poter fare tutti i controlli (tramite la struttura vincente)
  int *numVittoria=new int[NUMERO_GIOCATORI];
  int tmp;
  int (*funzioni[10])(int indice,giocatore *gioc,carta *verifica)={scalaReale,scalaColore,poker,full,colore,scala,tris,doppiaCoppia,coppia,cartaAlta};
  //assegnamento statico delle carte e richiamo puntatore a funzioni
  int i;
  for(i=0;i<NUMERO_GIOCATORI;i++){
    #ifdef tracing
    //stampa prima di ordinamento
    cout<<endl<<"PRIMA : \n";
    for(int j=0; j<7; j++){
      cout<<verifica[j].valore;
      cout<<verifica[j].seme;
      cout<<" ";
    }
    #endif
    ins_sort(verifica,7);
    #ifdef tracing
    //stampa array di verifica dopo ordinamento
    cout<<endl<<"DOPO : \n";
    for(int j=0; j<7; j++){
      cout<<verifica[j].valore;
      cout<<verifica[j].seme;
      cout<<" ";
    }
    #endif
    //qua dentro vengono richiamate le funzioni per l'assegnamento del tipo di mano
    g[i].controllo=new int[2];//array utilizzato in caso di parità
    numVittoria[i]=0;
    for(int l=0; numVittoria[i]<=0 ;l++){
      numVittoria[i]=funzioni[l](i,g,verifica);
    }
    #ifdef tracing
    //stampa di controllo
    cout<<endl<<g[i].nome<<" :   tipo mano : "<<numVittoria[i]<<" , controllo  1-->"<<g[i].controllo[0]<<" / 2-->"<<g[i].controllo[1]<<endl;
    #endif
  }

  //controllo vittoria o pareggio
  tmp=verificaVittoria(numVittoria);
  #ifdef tracing
  cout<<"\ntmp : "<<tmp;
  #endif

  if(tmp>1){
    tmp=pari(numVittoria,g);
    if(tmp>1){

      #ifdef tracing
      cout<<endl<<endl<<"PARI"<<endl<<endl;
      #endif

      for(int i=0;i<NUMERO_GIOCATORI;i++)
        g[i].n_soldi+=(t->piatto)/2;
    }else{
      #ifdef tracing
      cout<<endl<<endl<<g[tmp].nome<<" ha vinto la mano."<<endl<<endl;
      #endif
      g[tmp].n_soldi+=t->piatto;
    }
  }else{
    #ifdef tracing
    cout<<endl<<endl<<g[tmp].nome<<" ha vinto la mano."<<endl<<endl;
    #endif
    g[tmp].n_soldi+=t->piatto;
  }
  #ifdef tracing
  for(int i=0;i<NUMERO_GIOCATORI;i++)
    cout<<"\n"<<g[i].nome<<" : "<<g[i].n_soldi;
  #endif
}

/** Invia al client in vincitore.
 *
 * viene gestito anche il caso di fold.
 */
void sendWinner(int fold, int winner = 0){
  server_message[0] = '4';
  server_message[1] = (char)fold +'0';
  if(fold){
    cout<<"FOLDER: "<<folder<<endl;
    server_message[2] = (char)folder +'0';
    server_message[3] = '\0';
    send(sock,server_message,strlen(server_message),0);
  }else{
    server_message[2] = (char)winner +'0';
    server_message[3] = '\0';
    send(sock,server_message,strlen(server_message),0);
  }
  memset(server_message,0,2000);
  int read_size;
  while (true){
    if( (read_size = recv(sock, client_reply, 2000, 0)) > 0 ){
      memset(client_reply,0,2000);
      break;
    }
  }
}

/** Richiamata ogni fine mano per decretare il vincitore di essa.
 *
 * Nel caso ci la mano sia finita in seguito ad un fold,
 * viene sommato il contenuto del piatto ai soldi del giocatore che
 * non ha foldato.
 * Se non vi è stato un fold, si richiama la funzione ::winner() la quale
 * decreterà il vincitore.
 */
void vincita(bool fold,giocatore *g,carta *verifica){
  if(fold){
    g[!folder].n_soldi+=t->piatto;

    sendWinner(1);
    sendMoney2(2,g[0].n_soldi,g[1].n_soldi);
  }else{
    winner(g,verifica);
    // sendWinner(0);
  }
}
