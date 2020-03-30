#include <iostream>
#include "deck.hpp"
using namespace std;

int main(int argc, char **argv){
  srand(time(0));

  vector<Card> dealerCards;
  vector<Card> playerCards;

  CardDeck *deck = new CardDeck();
  deck->printDeck();
  cout << "Shuffling deck...\n";
  deck->shuffle();
  cout << "Deck Shuffled...\n";
  deck->printDeck();

  /* deal some cards */
  dealerCards.push_back(deck->deal());
  playerCards.push_back(deck->deal());
  dealerCards.push_back(deck->deal());
  playerCards.push_back(deck->deal());

  cout << "Dealer's cards: ";
  cout << dealerCards[0].printCard() << " " << dealerCards[1].printBack() << endl;
  cout << "Player's cards: ";
  cout << playerCards[0].printCard() << " " << playerCards[1].printCard() << " value: ";
  cout << playerCards[0].getFaceVal() + playerCards[1].getFaceVal() << endl;
  
  return 0;
}
