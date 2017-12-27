/**
 * @file
 * File che richiama in modo ordinato le funzioni
 * per il funzionamento di una partita.
 */

/** Funzione che gestisce una singola partita.
 *
 * Setta lo small e il big blind, i quali vengono invertiti ad ogni mano.
 * Viene creato il mazzo, il quale viene azzerato ad ogni mano.
 *
 * Vengono richiamati ::turnoZero() e ::turnoN() per la gestione delle fasi
 * di una singola mano.
 *
 * Viene richiamata ::gestioneScelta() la quale si occupa della gestione
 * di ogni scelta (check,call,bet,fold) dei giocatori.
 *
 * Viene richiamata ::vincita() la quale gestisce sia il caso fold
 * sia il caso di un vincitore, con pareggio o senza.
 *
 * Viene richiamata ::resetVariable() la quale azzera le variabili
 * per poter iniziare una nuova mano
 */
void game(giocatore *g,carta *mazzo,int &puntatore_mazzo,carta *verifica){

    g[0].bBlind=!bigBlind;
    g[1].bBlind=bigBlind;
    #ifdef test
      for(int i=0;i<NUMERO_GIOCATORI;i++){
        cout<<"RIEMPIMENTO ARRAY VERIFICA "<<i<<": ";
        for(int j=0;j<7;j++){
          cout<<"\n["<<j<<"].valore =";
          cin>>verifica[j].valore;
          cout<<"\n["<<j<<"].seme =";
          cin>>verifica[j].seme;
        }
      }
    #else
      creaMazzo(mazzo);
    #endif
    t->turno=0;

    //stampa le prime 9 carte del mazzo
    #ifdef tracing
      cout<<"\nMAZZO: ";
      for(int n=0;n<9;n++){
        cout<<mazzo[n].valore<<mazzo[n].seme<<" ";
    }//#else
    #endif

    puntatore_mazzo=0;
    int fold=0;
    for(int i=0;i<4;i++){
      if(i>0){
        turnoN(puntatore_mazzo,mazzo);
        #ifdef tracing
          cout<<"\nMano "<<g[0].nome<<" : "<<g[0].mano[0].valore<<g[0].mano[0].seme<<" "<<g[0].mano[1].valore<<""<<g[0].mano[1].seme<<endl;
          cout<<"Mano "<<g[1].nome<<" : "<<g[1].mano[0].valore<<g[1].mano[0].seme<<" "<<g[1].mano[1].valore<<""<<g[1].mano[1].seme<<endl;
        #endif

      }else{
        turnoZero(puntatore_mazzo,mazzo,g);
        #ifdef tracing
          cout<<"\nMano "<<g[0].nome<<" : "<<g[0].mano[0].valore<<g[0].mano[0].seme<<" "<<g[0].mano[1].valore<<""<<g[0].mano[1].seme<<endl;
          cout<<"Mano "<<g[1].nome<<" : "<<g[1].mano[0].valore<<g[1].mano[0].seme<<" "<<g[1].mano[1].valore<<""<<g[1].mano[1].seme<<endl;
        #endif
      }

      #ifdef tracing
        cout<<"\nTURNO : "<<t->turno<<endl;
        //stampa tavolo
        cout<<"TAVOLO : ";
        for(int i=0;i<5;i++){
          cout<<t->scoperte[i].valore<<t->scoperte[i].seme<<" ";
        }
        cout<<endl;
      #endif

      if(g[0].bBlind){
        fold=gestioneScelta(1,0,g);
        t->piatto+=(g[0].puntata + g[1].puntata);
      }else{
        fold=gestioneScelta(0,1,g);
        t->piatto+=(g[0].puntata + g[1].puntata);
      }
      if(fold){ //controllo che nessuno abbia foldato ed in caso contrario esco dal ciclo
        break;
      }
      #ifdef tracing
        cout<<"piatto: "<<t->piatto<<endl<<endl;
      #endif

      t->turno++;
    }
    vincita(fold,g,verifica);
    resetVariable(g,mazzo);

    #ifdef tracing
      cout<<"\nFINE MANO.."<<endl;
      //////////////////stampo soldi giocatori a fine mano///////////////////////
      cout<<endl;
      cout<<"----soldi fine mano "<<g[0].nome<<"="<<g[0].n_soldi<<"----"<<endl;
      cout<<"----soldi fine mano "<<g[1].nome<<"="<<g[1].n_soldi<<"----"<<endl;
      cout<<"---------------------------------";
      /////////////////////////////////////////////////////////////////////////
    #endif

    bigBlind=!bigBlind;
}
