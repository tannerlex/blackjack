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
#define DEALTAG 0
#define HITTAG 1
#define DLHOLD 16

using namespace std;

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
  Hand myHand;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  if(rank==0){ /* dealer */
    /* hold[rank] = true: player is holding and can be ignored for 
       the rest of the round.
       hold[0] = true means all players are holding                */
    bool hold[size];
    for(int i=0;i<size;i++){ /* initialize all players to hit */
      hold[i] = false;
    }

    /* generate and shuffle deck */
    CardDeck deck; 
    cout << "Shuffling deck... ";
    deck.shuffle();
    deck.printDeck();

    /* initial deal to all players */
    myHand.m_hand.push_back(deck.deal());
    myHand.m_hand.push_back(deck.deal());
    for(int i = 1; i < size; i++){
      cout << "dealing 2 cards to player " << i << "\n";
      dealCard(i,deck);
      dealCard(i,deck);
    }

    while(!hold[0]){ /* while still dealing round */
      hold[0] = true; /* reset to hold */
      for(int i = 1; i < size; i++){ /* for each player */
        if(!hold[i]){ /* if player has not indicated hold status */
          cout << "check player " << i << " for hit\n";
          int hit; /* integer to recieve a hit request */
          MPI_Recv(&hit,1,MPI_INT,i,HITTAG,MCW,MPI_STATUS_IGNORE);
          if(hit){ /* if player has not indicated hold status */
            /* deal card to player */
            cout << "dealing card to player: " << i << "\n";
            dealCard(i,deck);
            hold[0] = false; /* this player just hit, repeat loop */
          }
          hold[i] = !hit;
        }
      }
      cout << "Hold[0] while loop: " << hold[0] << endl;
    } /* end while() still dealing round */

    /* dealer play */
    cout << "Dealer is going to play now.\n";
    while(myHand.hit(DLHOLD)) {
      /* deals a card to itself */
      myHand.m_hand.push_back(deck.deal());
      myHand.update();
    }
    cout << "Dealer's cards: ";
    myHand.printHand();
    cout << "value: " << myHand.getVal() << "\n";
  } /* end if dealer */
  else { /* if player */
    recvCard(myHand);
    recvCard(myHand);

    myHand.printHand();
        
    int hit = myHand.hit(16);
    MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW);

    while(hit){
      MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW);
      recvCard(myHand);
      hit = myHand.hit(16);
    }

    cout << "Rank: " << rank << " recieved hand: " << endl;
    myHand.printHand();
    cout << "value: " << myHand.getVal() << "\n";
  } /* end if player */

  cout << rank << " about to finalize \n";

  MPI_Finalize();
  return 0;
}