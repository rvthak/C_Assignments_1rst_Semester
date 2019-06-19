#define WHITE 'w'
#define BLACK 'b'

void defaults(char *n, char *d, char *b, char *s);
char start(char argc, char *argv[], char *running);
void onestore(char *n, char *d, char *b, char *s);
void showstate(char n, char **table);
void spacer(char n);
void bricks(char n, char offset);
char flip(char b);
void save(char n, char turn, char **table);
void load(char *n, char *b, char **table, char **tag, char **undo);
char **tballoc(char n, char n1, char init);
void tbfree(char **table, char n);
char reccheck(char **table, char **tag, char n, char i, char j, char player);
char check(char **table, char **tag, char n, char i, char j, char player);
void zero(char **tag, char n);
char maxlen(char **table, char **tag, char n, char player);
char reclen(char **table, char **tag, char n, char i, char j, char player);
char levels(char **table, char n, char player);
void swap(char **table, char n, char player);

