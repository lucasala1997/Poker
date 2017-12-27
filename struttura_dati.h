const int MAX_SOLDI = 10000; /**< Soldi di tutti i giocatore ogni nuova partita */
const int NUMERO_CARTE=52;  /**< Dimensione di un mazzo di carte da poker standard */
const int NUMERO_GIOCATORI=2; /**< Numero dei giocatori partecipanti ad ogni partita */
bool bigBlind=false;  /**< Variabile per il controllo di: small blind e big blind */
const int bigBlindValue= 100; /**< Valore del big blind */
const int smallBlindValue = 50; /**< Valore dello small blind */
char  server_message[2000],
      client_message[2000],
      client_reply[2000],
      server_reply[2000]; /**< Buffer utilizzati per lo scambio di messaggi tra client e server */
int folder; /**< Variabile utilizzata per memorizzare il giocatore che ha scelto "fold" */
int sock; /**< Variabile nella quale viene memorizzato il socket per la comunicazione
           * memorizzato dopo l'avvenuta connessione
           */
const int righe=4; /**< righe della matrice utilizzata per la creazione del mazzo */
const int colonne=13; /**< colonne della matrice utilizzata per la creazione del mazzo  */

/**
 * Struttura di una singola carta.
 *
 * Ogni singola carta contiene un campo seme di tipo char il quale
 * potrà essere 'c' 'q' 'f' 'p' , quattro lettere che rappresentano i
 * possibili semi.
 * Oltre al seme la carta contiene un campo valore nel quale sarà
 * memorizzato un numero compreso tra 1 e 13.
 */
struct carta{
  char seme;  /**< Campo seme della carta */
  int valore; /**< Campo valore della carta */
};

/**
 * Struttura che rappresenta un singolo giocatore.
 *
 */
struct giocatore{
  char nome[20];  /**< Variabile per la memorizzazione del nome */
  int n_soldi;  /**< Soldi del giocatore */
  carta* mano=new carta[2]; /**< Puntatore ad un array di carte di due
                             * elementi contenente la mano del giocatore
                             */
  bool bBlind;     /**< Se true il giocatore ha il big blind,
                    * altrimenti ha lo small blind
                    */
  int puntata;      /**< Utilizzata per tenere in memoria la puntata corrente */
  int *controllo;   /**< Puntatore che punta a una o due celle di memoria
                     * in base al tipo di mano del giocatore, se il giocatore ha:
                     * full o doppiaCoppia due celle altrimenti una.
                     * Questo meccanismo serve per gestire il caso di parità*/
};

/**
 * Struttura che rappresenta le carte sul tavolo.
 */
struct tavolo{
  carta* scoperte=new carta[5];/**< Puntatore ad un array di carte di cinque
                                * elementi contenente le carte del tavolo
                                */
  int turno;                    /**< Variabile contenente il numero del turno
                                 * utilizzata per gestire le varie parti di una
                                 * mano.
                                 * Nel turno zero vengono distribuite le coppie
                                 * di carte ai due giocatori.
                                 * Nel turno uno si scoprono le prime 3 carte
                                 * sul tavolo.
                                 * Nei turni maggiori di uno viene scoperta una
                                 * carta alla volta.
                                 */
  int piatto;                    /**< Aggiornato ogni mano con il big,
                                  * lo small blind e la somma delle puntate
                                  * dei due giocatori.
                                  * Viene poi sommato ai soldi del vincitore
                                  * della mano.
                                  */
};
tavolo *t;
