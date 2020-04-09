#include <iostream>
#include "deck.hpp"
using namespace std;

#define P1HOLD 17
#define DLHOLD 16
#define TOTALROUNDS 1024

void roundResults(Hand &pl, Hand &dl, int &pw, int &dw);

int main(/*int argc, char **argv*/){
  srand(time(0)); /* seed rand with time */
  int rank = 1; /* use MPI communicator rank */

  int playerWins = 0; /* number of rounds player has won */
  int dealerWins = 0; /* number of rounds dealer has won */

  int running = TOTALROUNDS; /* number of rounds left to play */
  while(running--){
    Hand dealerHand, playerHand; /* sets of dealt cards */
    bool playerBusts = false;

    CardDeck deck;
    cout << "Shuffling deck... ";
    deck.shuffle();
    // deck->printDeck();

    /* deal some cards and print to console */
    cout << "Dealing...\n";
    dealerHand.addCard(deck.deal());
    playerHand.addCard(deck.deal());
    dealerHand.addCard(deck.deal());
    playerHand.addCard(deck.deal());
    cout << "Dealer's cards: ";
    dealerHand.printHand();
    cout << "\nPlayer's cards: ";
    playerHand.printHand();
    cout << playerHand.getVal() << "\n";

    /* player play */
    while(playerHand.hit(P1HOLD)) {
      /* player requests the dealer to deal another card */
      playerHand.addCard(deck.deal());
      cout << "Player's cards: ";
      playerHand.printHand();
      cout << "value: " << playerHand.getVal() << "\n";
    }

    /* dealer play */
    while(dealerHand.hit(DLHOLD)) {
      /* deals a card to itself */
      dealerHand.addCard(deck.deal());
    }
    cout << "Dealer's cards: ";
    dealerHand.printHand();
    cout << "value: " << dealerHand.getVal() << "\n";

    /* determine the results of the round */
    roundResults(playerHand, dealerHand, playerWins, dealerWins);
  } /* while there are still rounds to play */

  /* Print out Player 1 results of all rounds */
  cout << "Player " << rank << " has won " << playerWins;
  cout << " round(s) out of " << TOTALROUNDS << "\n";
  cout << "The dealer has won " << dealerWins;
  cout << " round(s) out of " << TOTALROUNDS << "\n";
  cout << "There were " << TOTALROUNDS - playerWins - dealerWins;
  cout << " round(s) that resulted in a push.\n";
  return 0;
} /* main() */

void roundResults(Hand &pl, Hand &dl, int &pw, int &dw){
  /* roundResults analyzes the final hands of a round and records 
     and reports the results including incrementing the number of
     player wins or dealer wins.                                   */
  if(pl.getVal() > BLACKJACK){
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
