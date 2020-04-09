#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <deque>

#include "deck.hpp"

#define DEALER 0
#define DEALTAG 0
#define DLHOLD 16
#define FACEUPTAG 2
#define HITTAG 1
#define INITDEAL 2
#define MCW MPI_COMM_WORLD
#define NUMDECKS 4
#define TOTALROUNDS 5

using namespace std;

/* function prototypes */
void dealerPlay(int *wins, int size);
void playerPlay(int rank);
int dealCard(int player, CardDeck &deck);
void recvCard(Hand &curHand);
void roundResults(Hand &pl, Hand &dl, int &pw, int &dw);

int main(int argc, char **argv){
  /* initialize MPI variables */
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  /* algorithm setup */
  int running = TOTALROUNDS; /* number of rounds left to play */
  int wins[size]; /* number of rounds each player has won */
  for(int i = 0; i < size; ++i){wins[i] = 0;}
  srand(time(0));

  while(running--){ /* while there are still rounds left to play */
    if(DEALER == rank){ /* dealer */
      dealerPlay(wins, size);
    } /* end if dealer */

    else { /* if player */
      playerPlay(rank);
    } /* end if player */

    MPI_Barrier(MCW); /* catch up for nice console printing */
  }

  /* notify user of end of play */
  if(DEALER == rank){
    cout << "Dealer";
  }else{
    cout << "Player " << rank;
  }
  cout << " is leaving the table.\n";

  MPI_Finalize();
  return 0;
}

void dealerPlay(int *wins, int size){
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
    dHand.m_hand.push_back(deck.deal());
  }
  hands.push_back(dHand); /* dealer's hand is index 0 */
  for(int i = 1; i < size; i++){
    Hand hnd;
    for(int j = 0; j < INITDEAL; ++j){
      Card crd(dealCard(i,deck));
      hnd.m_hand.push_back(crd);
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
          hands[i].m_hand.push_back(crd); /* add to player's hand */
          hold[0] = false; /* this player just hit, repeat loop */
        }
        hold[i] = !hit; /* update player status */
      }
    } /* end for each player */
  } /* end while() still dealing round */

  /* dealer play */
  while(hands[DEALER].hit(DLHOLD)) { /* while not holding */
    /* deals a card to itself */
    hands[DEALER].m_hand.push_back(deck.deal());
    hands[DEALER].update();
  }
  cout << "Dealer's cards: ";
  hands[DEALER].printHand();

  for(int i = 1; i < size; ++i){
    roundResults(hands[i], hands[DEALER], wins[i], wins[DEALER]);
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

  int hit = myHand.hit(16); /* determine whether to hit */
  MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW); /* tell dealer */

  while(hit){ /* if a hit is called for */
    recvCard(myHand); /* receive the next card */
    hit = myHand.hit(16); /* determine whether to hit again */
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
  curHand.m_hand.push_back(recvCard); /* add card to hand */
  curHand.update(); /* update hand numbers */
}

void roundResults(Hand &pl, Hand &dl, int &pw, int &dw){
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
  } else if (dl.getVal() > pl.getVal()){
    cout << "Dealer wins.\n";
    dw++; /* chalk one up for the dealer */
  } else {
    cout << "Player wins.\n";
    pw++; /* chalk one up for the player */
  }
} /* roundResults() */

// TODO:
// round results
// strategy for each player