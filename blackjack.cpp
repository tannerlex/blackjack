#include <iostream>
#include "deck.hpp"
using namespace std;

int main(int argc, char **argv){
  srand(time(0));

  CardDeck *deck = new CardDeck();
  deck->printDeck();
  cout << "Shuffling deck...\n";
  deck->shuffle();
  cout << "Deck Shuffled...\n";
  deck->printDeck();

  return 0;
}
