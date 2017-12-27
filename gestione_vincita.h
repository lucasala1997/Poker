extern const int NUMERO_CARTE, NUMERO_GIOCATORI;
extern char server_message[2000], client_reply[2000];
extern int folder, sock;
extern tavolo *t;


/** Reset di tutti i valori per passare alla mano successiva.
 *
 * Da invocare ogni fine mano in modo da resettare
 * piatto, puntate dei giocatori, mazzo, carte del tavolo
 * e dei giocatori
 */
void resetVariable(giocatore *g, carta *mazzo);

/** Ritorna 1 se il giocatore passato come parametro ha scala reale. */
int scalaReale(int indice,giocatore *g,carta *verifica);

/** Ritorna 2 se il giocatore passato come parametro ha scala colore. */
int scalaColore(int indice,giocatore *g,carta *verifica);

/** Ritorna 3 se il giocatore passato come parametro ha poker. */
int poker(int indice,giocatore *g,carta *verifica);

/** Ritorna 4 se il giocatore passato come parametro ha full. */
int full(int indice,giocatore *g,carta *verifica);

/** Ritorna 5 se il giocatore passato come parametro ha colore. */
int colore(int indice,giocatore *g,carta *verifica);

/** Ritorna 6 se il giocatore passato come parametro ha scala. */
int scala(int indice,giocatore *g,carta *verifica);

/** Ritorna 7 se il giocatore passato come parametro ha tris. */
int tris(int indice,giocatore *g,carta *verifica);

/** Ritorna 8 se il giocatore passato come parametro ha doppia coppia. */
int doppiaCoppia(int indice,giocatore *g,carta *verifica);

/** Ritorna 9 se il giocatore passato come parametro ha coppia. */
int coppia(int indice,giocatore *g,carta *verifica);

/** Ritorna 10 se il giocatore passato come parametro ha carta alta. */
int cartaAlta(int indice,giocatore *g,carta *verifica);

/** Ritorna 0/1 nel caso vinca il primo (0) o secondo giocatore (1),
 *  2 in caso di pareggio.
 */
int verificaVittoria(int numVittoria[]);

/** Richiamata in caso di pareggio.
 *
 * Ritorna 0/1 se vince un sono giocatore,
 * 2 se bisogna spartire il piatto, cioè pareggio effettivo.
 */
int pari(int numVittoria[],giocatore *g);

/** Richiamata per ordinare ::verifica per poter controllare il tipo di mano
 *  del giocatore
 */
void ins_sort(carta *a, int n);

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
void winner(giocatore *g,carta *verifica);

/** Invia al client in vincitore.
 *
 * viene gestito anche il caso di fold.
 */
void sendWinner(int fold, int winner);

/** Richiamata ogni fine mano per decretare il vincitore di essa.
 *
 * Nel caso ci la mano sia finita in seguito ad un fold,
 * viene sommato il contenuto del piatto ai soldi del giocatore che
 * non ha foldato.
 * Se non vi è stato un fold, si richiama la funzione ::winner() la quale
 * decreterà il vincitore.
 */
void vincita(bool fold,giocatore *g,carta *verifica);
