#include "include_file.h"



/** Richiamata da ::creaMazzo().
 *
 * Ogni volta che viene creato il mazzo, viene richiamata per mescolare
 * le carte.
 */
void mescolaMazzo(int mazzo_ordinato[][colonne], carta *mazzo){
  char p_seme[]={'c','q','f','p'};//semi possibili(cuori,quadri,fiori,picche)
  srand(time(NULL));
  int conta_carte_piazzate=0,rseme=0,rval=0;
  int crighe=righe+1,ccolonne=colonne+1;
  while(true){
    rseme = (rand()%righe);     //da 0 a righe
    rval = (rand()%colonne);  //da 0 a colonne
    if(mazzo_ordinato[rseme][rval]!=-1){
        mazzo[conta_carte_piazzate].seme=p_seme[rseme];
        mazzo[conta_carte_piazzate].valore=mazzo_ordinato[rseme][rval];
        conta_carte_piazzate++;
        mazzo_ordinato[rseme][rval]=-1;//se il valore è settato a -1 allora l'elemento è gia stato utilizzato
    }

    #ifdef tracing
    for(int i=0;i<righe;i++){
      for(int j=0;j<colonne;j++)
        cout<<mazzo_ordinato[i][j]<<" ";
      cout<<endl;
    }
    cout<<endl<<conta_carte_piazzate<<endl;
    #endif
    if(conta_carte_piazzate==52 /*&& controllo_mazzo(mazzo_ordinato)*/){
      break;
    }
  }
}

/** Richiamata da ::game().
 *
 * Ogni mano viene richiamata per creare un nuovo mazzo il quale verrà
 * successivamente mescolato da ::mescolaMazzo() .
 */
int creaMazzo(carta *mazzo){
  int mazzo_ordinato[righe][colonne];
  for(int i=0;i<righe;i++)
    for(int j=0;j<colonne;j++)
      mazzo_ordinato[i][j]=j+1;
  #ifdef tracing
  for(int i=0;i<righe;i++){
    for(int j=0;j<colonne;j++)
      cout<<mazzo_ordinato[i][j]<<" ";
    cout<<endl;
  }
  #endif
  mescolaMazzo(mazzo_ordinato,mazzo);
  return 0;
}

#ifdef tracing
  /** Controllo del mazzo.
   *
   * Funzione utilizzata solo in fase di testing per controllare
   * la creazione corretta del mazzo.
   */
  int controllo_mazzo(int mazzo_ordinato[][colonne]){
    for(int i=0;i<righe;i++){
      for(int j=1;j<colonne;j++) {
        cout<<mazzo_ordinato[i][j]<<" ";
        if(mazzo_ordinato[i][j]!=-1)
          return 0;
      }
      cout<<endl;
    }
    return 1;
  }
#endif
