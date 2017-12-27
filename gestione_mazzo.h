extern const int righe;
extern const int colonne;
/** Richiamata da ::creaMazzo().
 *
 * Ogni volta che viene creato il mazzo, viene richiamata per mescolare
 * le carte.
 */
void mescolaMazzo(int mazzo_ordinato[][colonne], carta *mazzo);

/** Richiamata da ::game().
 *
 * Ogni mano viene richiamata per creare un nuovo mazzo il quale verrà
 * successivamente mescolato da ::mescolaMazzo() .
 */
int creaMazzo(carta *mazzo);
