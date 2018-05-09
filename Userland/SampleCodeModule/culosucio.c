#include <culosucio.h>

static void sleep();
static void swap(int *i,int *j);
static void shuffleDeck(int deck[DECK_SIZE]);
void player(int i);

void initDeck() {
	createMailbox("Deck");
	srand();
	int i;
	int deck[DECK_SIZE];
	for(i = 0; i < DECK_SIZE; i++) {
		deck[i] = i+1;
	}
	shuffleDeck(deck);
	for(i = 0; i < DECK_SIZE; i++) {
		sleep();
		send("Deck",deck + i,sizeof(int));
	}
}

void initPlayers(){
	uint64_t pid[PLAYER_QUANTITY];
	void **argVector = allocateMemory(sizeof(void *) * PLAYER_QUANTITY);
	int mode = FOREGROUND;
	int playerId[PLAYER_QUANTITY];
	*argVector = &mode;
	*(argVector + 1) = "player";
	for (int i = 0; i < PLAYER_QUANTITY; i++) {
		playerId[i] = i;
		*(argVector + 2) = playerId + i;
		createProcess(&player,3,argVector);
	}
	for (int i = 0; i < PLAYER_QUANTITY; i++) {
		waitChild(playerId[i]);
	}

}

static void shuffleDeck(int deck[DECK_SIZE]) {
	int randPosition;
	for(int i=0;i < DECK_SIZE;i++) {
		randPosition = rand()%DECK_SIZE;
		swap(deck + i,deck + randPosition);
	}
}

static void swap(int *i,int *j) {
	int aux;
	aux=*i;
	*i=*j;
	*j=aux;
}

void player(int i) {
	createMailbox("Deck");
	createMailbox("Win");
	int *nobodyWins;
	int *myCard;
	int winner;

	*nobodyWins = 0;

	while(*nobodyWins == 0){
		myCard = (int *) receive("Deck");
		if(*myCard == WINNER_CARD){
			winner = 1;
			send("Win",&winner,sizeof(int));
			printf("PLAYER %d WINS!\n",i);
		}else {
			winner = 0;
			send("Win",&winner,sizeof(int));
			printf("PLAYER %d DOEST WIN. HIS CARD WAS %d\n",i,*myCard);
		}
		nobodyWins = (int *) receive("Win");
	}
}

static void sleep() {
	uint64_t i;
	while(i != 2000000) i++;
}
