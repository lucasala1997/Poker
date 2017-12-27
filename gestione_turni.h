extern const int MAX_SOLDI, NUMERO_CARTE, NUMERO_GIOCATORI, bigBlindValue;
extern int folder, sock;
extern char server_message[2000], client_reply[2000];
extern tavolo *t;
/** Gestisce l'invio e la ricezione delle scelte effettuabili da un giocatore.
 *
 * In base al valore di ::small_who darà possibilità di scelta al client o al
 * server.
 * più precisamente se 0 = server ,se 1 = client.
 * Nel caso del client, invierà un messaggio contenente le possibili scelte
 * effettuabili , cioè: "check" "call" "bet" "fold".
 */
char interazione(char *array,int small_who);

/** Gestisce il tipo di scelta "rilancio".
 *
 * In base al valore di ::small_who gestisce la scelta del server o del client
 * più precisamente se 0 = server ,se 1 = client.
 * Inoltre richiede anche di quanti soldi rilanciare.
 */
int rilancio(int small_who);

/** Costruisce il messaggio da inviare al client.
 *
 * In base al tipo saprà se deve inviare il piatto oppure i soldi dei giocatori
 * ogni mano.
 */
int sendMoney(int tipo, int *soldi);

/** Gestisce l'invio dei soldi.
 *
 * In base ai parametri passati come parametro invoca ::sendMoney()
 * la quale in base al tipo saprà come costruire il messaggio da inviare
 * al client ogni mano.
 */
void sendMoney2(int tipo, int s1,int s2);

/** Gestisce le scelte dei giocatori, uno alla volta.
 *
 * Vengono gestiti all'interno tutti i casi possibili di scelta.
 * in base al turno la scelta potrà variare tra due insiemi di opzioni:
 * il primo (c,b,f) il quale permette di effetuttare call, bet oppure fold,
 * utilizzato nella prima mano oppure dopo che uno ha rilanciato.
 * Il secondo (h,b,f) permette di effettuare check, bet oppure fold,
 * utilizzato in tutte le altre mani.
 */
int gestioneScelta(int s,int big_blind, giocatore *g);

/** Distribuisce le coppie di carte ai giocatori.
 *
 * dopo aver assegnato ai due giocatori le coppie di carte, invia al client un
 * un messaggio contenente queste ultime.
 */
int turnoZero(int &puntatore_mazzo,carta *mazzo,giocatore *g);

/** Scopre le carte sul tavolo
 *
 * In base al valore di ::t.turno scopre sul tavolo 3 o 1 carta.
 * Il primo turno scoprirà 3 carte mentre gli altri una sola fino ad arrivare
 * a 5.
 */
void turnoN(int &puntatore_mazzo,carta *mazzo);

/** Setta i soldi al massimo ai giocatori.
 *
 * Richiamata ad ogni nuova partita per resettare i soldi e creare
 * un nuovo tavolo.
 */
void avviaPartita(carta *mazzo, giocatore *g);
