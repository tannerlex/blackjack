#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <deque>

#include "deck.hpp"

#define MCW MPI_COMM_WORLD
#define DEALER 0
#define DEALTAG 0
#define HITTAG 1
#define DLHOLD 16
#define TOTALROUNDS 5

using namespace std;

void dealerPlay(CardDeck &deck, int size);
void playerPlay(int rank);

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

int main(int argc, char **argv){
  int rank, size;
  srand(time(0));
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  int running = TOTALROUNDS; /* number of rounds left to play */

  while(running--){ /* while there are still rounds left to play */
    CardDeck deck; /* deck for the current round */

    if(DEALER == rank){ /* dealer */
      dealerPlay(deck, size);
    } /* end if dealer */

    else { /* if player */
      playerPlay(rank);
    } /* end if player */

    MPI_Barrier(MCW); /* catch everyone up for nice console printing */
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

void dealerPlay(CardDeck &deck, int size){
  /* hold[rank] = true: player is holding and can be ignored for 
  the rest of the round.
  hold[0] = true means all players are holding                */
  Hand myHand;
  bool hold[size];
  for(int i=0;i<size;i++){ /* initialize all players to hit */
  hold[i] = false;
  }

  /* generate and shuffle deck */
  cout << "Shuffling deck... ";
  deck.shuffle();
  deck.printDeck();

  /* initial deal to all players */
  for(int i = 1; i < size; i++){
    dealCard(i,deck);
  }
  myHand.m_hand.push_back(deck.deal());
  for(int i = 1; i < size; i++){
    dealCard(i,deck);
  }
  myHand.m_hand.push_back(deck.deal());

  //   playRound();
  while(!hold[0]){ /* while still dealing round */
    hold[0] = true; /* reset to hold */

    for(int i = 1; i < size; i++){ /* for each player */
      if(!hold[i]){ /* if player has not indicated hold status */
        int hit; /* integer to recieve a hit request */
        MPI_Recv(&hit,1,MPI_INT,i,HITTAG,MCW,MPI_STATUS_IGNORE);

        if(hit){ /* if player has not indicated hold status */
          dealCard(i,deck); /* deal card to player */
          hold[0] = false; /* this player just hit, repeat loop */
        }
        hold[i] = !hit; /* update player status */
      }
    } /* end for each player */
  } /* end while() still dealing round */

  /* dealer play */
  while(myHand.hit(DLHOLD)) { /* while not holding */
    /* deals a card to itself */
    myHand.m_hand.push_back(deck.deal());
    myHand.update();
  }
  cout << "Dealer's cards: ";
  myHand.printHand();
  cout << "value: " << myHand.getVal() << "\n";
  //deck.printDeck();
} /* dealerPlay() */

void playerPlay(int rank){
  Hand myHand;

  /* receive 1st 2 cards */
  recvCard(myHand);
  recvCard(myHand);

  int hit = myHand.hit(16); /* determine whether to hit */
  MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW); /* tell dealer */

  while(hit){ /* if a hit is called for */
    recvCard(myHand); /* receive the next card */
    hit = myHand.hit(16); /* determine whether to hit again */
    MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW); /* tell dealer */
  }

  cout << "\nPlayer " << rank << " hand: ";
  myHand.printHand();
  cout << "value: " << myHand.getVal() << "\n";
}
// TODO:
// round results
// strategy for each player