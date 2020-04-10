#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <deque>

#include "deck.hpp"

#define DEALTAG 0
#define FACEUPTAG 2
#define HITTAG 1
#define INITDEAL 2
#define MCW MPI_COMM_WORLD
#define NUMDECKS 4
#define TOTALROUNDS 15000

using namespace std;

/* function prototypes */
void dealerPlay(int *wins, int *ties, int size);
void playerPlay(int rank);
int dealCard(int player, CardDeck &deck);
void recvCard(Hand &curHand);
void roundResults(
  Hand &pl, Hand &dl, int &pw, int &dw, int &pt, int &dt);
void printResults(int *wins, int *ties, int size);

int main(int argc, char **argv){
  /* initialize MPI variables */
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  /* algorithm setup */
  int running = TOTALROUNDS; /* number of rounds left to play */
  int wins[size]; /* number of rounds each player has won */
  int ties[size]; /* number of rounds resulting in a push */
  /* initialize counters to zero */
  for(int i = 0; i < size; ++i){wins[i] = 0; ties[i] = 0;}
  srand(time(0));

  while(running--){ /* while there are still rounds left to play */
    if(DEALER == rank){ /* dealer */
      dealerPlay(wins, ties, size);
    } /* end if dealer */

    else { /* if player */
      playerPlay(rank);
    } /* end if player */

    MPI_Barrier(MCW); /* catch up for nice console printing */
  }

  /* notify user of end of play */
  if(DEALER == rank){
    printResults(wins, ties, size);
    cout << "Dealer";
  }else{
    cout << "Player " << rank;
  }
  cout << " is leaving the table.\n";

  MPI_Finalize();
  return 0;
}

void dealerPlay(int *wins, int *ties, int size){
  /* this function executes the dealer's tasks for a round of play */
  CardDeck deck(NUMDECKS); /* deck for the current round */
  Hand dHand; /* dealer's hand for initializing vector of hands */
  vector<Hand> hands; /* set of each players' hand for the round */
  /* hold[rank] = true: player is holding and can be ignored for 
  the rest of the round.
  hold[0] = true means all players are holding                */
  bool hold[size];
  for(int i=0;i<size;i++){ /* initialize all players to hit */
    hold[i] = false;
  }

  /* generate and shuffle deck */
  cout << "Shuffling deck...\n";
  deck.shuffle();
  // deck.printDeck();

  /* initial deal to all players */
  for(int j = 0; j < INITDEAL; ++j){
    dHand.addCard(deck.deal());
  }
  hands.push_back(dHand); /* dealer's hand is index 0 */
  for(int i = 1; i < size; i++){ /* for each player */
    Hand hnd;
    for(int j = 0; j < INITDEAL; ++j){
      Card crd(dealCard(i,deck));
      hnd.addCard(crd);
    }
    hands.push_back(hnd); /* add to set of all hands at the table */
  }

  /* inform players of dealer's face up card */
  int dlrsFaceUp = hands[DEALER].m_hand[1].getUCV();
  for(int i = 1; i < size; ++i){
    MPI_Send(&dlrsFaceUp,1,MPI_INT,i,FACEUPTAG,MCW);
  }

  //   playRound();
  while(!hold[0]){ /* while still dealing round */
    hold[0] = true; /* reset to hold */

    for(int i = 1; i < size; i++){ /* for each player */
      if(!hold[i]){ /* if player has not indicated hold status */
        int hit; /* integer to recieve a hit request */
        MPI_Recv(&hit,1,MPI_INT,i,HITTAG,MCW,MPI_STATUS_IGNORE);

        if(hit){ /* if player has not indicated hold status */
          Card crd(dealCard(i,deck)); /* deal card to player */
          hands[i].addCard(crd); /* add to player's hand */
          hold[0] = false; /* this player just hit, repeat loop */
        }
        hold[i] = !hit; /* update player status */
      }
    } /* end for each player */
  } /* end while() still dealing round */

  /* dealer play */
  while(hands[DEALER].hit(DEALER)) { /* while not holding */
    /* deals a card to itself */
    hands[DEALER].addCard(deck.deal());
  }
  cout << "Dealer's cards: ";
  hands[DEALER].printHand();

  for(int i = 1; i < size; ++i){
    roundResults(
      hands[i], 
      hands[DEALER], 
      wins[i], 
      wins[DEALER], 
      ties[i], 
      ties[DEALER]);
  }
} /* dealerPlay() */

void playerPlay(int rank){
  Hand myHand;

  /* receive 1st 2 cards */
  recvCard(myHand);
  recvCard(myHand);

  /* receive dealer's face up card */
  int dlrsFaceUp;
  MPI_Recv(&dlrsFaceUp,1,MPI_INT,0,FACEUPTAG,MCW,MPI_STATUS_IGNORE);
  Card dfu(dlrsFaceUp);

  int hit = myHand.hit(rank); /* determine whether to hit */
  MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW); /* tell dealer */

  while(hit){ /* if a hit is called for */
    recvCard(myHand); /* receive the next card */
    hit = myHand.hit(rank); /* determine whether to hit again */
    MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW); /* tell dealer */
  }

  cout << "\nPlayer " << rank << " hand: ";
  myHand.printHand();
}

int dealCard(int player, CardDeck &deck){
  /* deal a card to player */
  Card sendCard(deck.deal()); /* pull card from deck */
  int dealtCard = sendCard.getUCV(); /* unicode val of dealt card */
  MPI_Send(&dealtCard,1,MPI_INT,player,DEALTAG,MCW); /* send card */
  return dealtCard; /* used if tracking which cards were dealt */
}

void recvCard(Hand &curHand){
  /* recieve a card into hand */
  int card; /* unicode val of card being received */
  MPI_Recv(&card,1,MPI_INT,0,DEALTAG,MCW,MPI_STATUS_IGNORE);
  Card recvCard(card); /* create a card object from unicode val */
  curHand.addCard(recvCard); /* add card to hand */
}

void roundResults(Hand &pl, Hand &dl, /* player and dealer hands */
                  int &pw, int &dw, /* player and dealer wins */
                  int &pt, int &dt) /* player and dealer ties */ {
  /* roundResults analyzes the final hands of a round and records 
     and reports the results including incrementing the number of
     player wins or dealer wins.                                   */
  if((INITDEAL == pl.m_hand.size()) && (BLACKJACK == pl.getVal())){
    cout << "Player has Blackjack!.\n";
    pw++; /* chalk one up for the player */
  } else if(pl.getVal() > BLACKJACK){
    cout << "Player busts.\n";
    dw++; /* chalk one up for the dealer */
  } else if (dl.getVal() > BLACKJACK){
    cout << "Dealer busts.\n";
    pw++; /* chalk one up for the player */
  } else if (dl.getVal() == pl.getVal()){
    cout << "It's a push.\n";
    pt++;
    dt++;
  } else if (dl.getVal() > pl.getVal()){
    cout << "Dealer wins.\n";
    dw++; /* chalk one up for the dealer */
  } else {
    cout << "Player wins.\n";
    pw++; /* chalk one up for the player */
  }
} /* roundResults() */

void printResults(int *wins, int *ties, int size){
  /* print dealer results */
  cout << "Results of " << TOTALROUNDS << " rounds:\n";
  cout << "The dealer won " << wins[DEALER] << " rounds (";
  cout << wins[DEALER]*100.0/TOTALROUNDS/(size-1.0) << "%), and had ";
  cout << ties[DEALER] << " rounds (";
  cout << ties[DEALER]*100.0/TOTALROUNDS/(size-1.0);
  cout << "%) end in a push.\n";

  /* print player results */
  for(int i = 1; i < size; ++i){
    cout << "Player " << i << " won " << wins[i] << " rounds (";
    cout << wins[i]*100.0/TOTALROUNDS << "%), and had ";
    cout << ties[i] << " rounds (" << ties[i]*100.0/TOTALROUNDS;
    cout << "%) end in a push using a strategy of hit on ";
    cout << i + LWSTHOLD - 1 << ".\n";
  }
}

// TODO:
// strategy for each player