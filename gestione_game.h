extern const int NUMERO_GIOCATORI;
extern bool bigBlind;
extern tavolo *t;
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
void game(giocatore *g,carta *mazzo,int &puntatore_mazzo,carta *verifica);
