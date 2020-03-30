#include <iostream>
#include "deck.hpp"
using namespace std;

int main(int argc, char **argv){
  string suits = "♤♣♥♢🂡";
  cout << suits << endl;

  // CardDeck *deck = new CardDeck();
  CardDeck deck;
  // for(int)
  // cout << deck->m_deck[0].printCard() << "\n";
  // cout << deck->m_deck[0].m_unicodeVal << "\n";
  cout << deck.m_deck[0].printCard() << "\n";
  return 0;
}
