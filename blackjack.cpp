#include <iostream>
#include "deck.hpp"
using namespace std;

bool hit(int holdVal, int myVal){
  if(myVal < holdVal){return true;}
  else {return false;}
}

int main(int argc, char **argv){
  srand(time(0));

  Hand dealerHand, playerHand;

  vector<Card> player;

  CardDeck *deck = new CardDeck();
  deck->printDeck();
  cout << "Shuffling deck...\n";
  deck->shuffle();
  cout << "Deck Shuffled...\n";
  deck->printDeck();

  /* deal some cards */
  dealerHand.m_hand.push_back(deck->deal());
  playerHand.m_hand.push_back(deck->deal());
  dealerHand.m_hand.push_back(deck->deal());
  playerHand.m_hand.push_back(deck->deal());

  for(int i=0;i<dealerHand.m_hand.size();i++) {
    if(dealerHand.m_hand[i].getFaceVal() == 11) {
      dealerHand.m_aces+=1; 
      cout << "Ace added to dealer hand \n";
    }
    dealerHand.m_val+=dealerHand.m_hand[i].getFaceVal();
  }
  for(int i=0;i<playerHand.m_hand.size();i++) {
    if(playerHand.m_hand[i].getFaceVal() == 11) {
      playerHand.m_aces+=1; 
      cout << "Ace added to player hand \n";
    }
    playerHand.m_val+=playerHand.m_hand[i].getFaceVal();
  }

  cout << "Dealer's cards: ";
  cout << dealerHand.m_hand[0].printCard() << " " << dealerHand.m_hand[1].printBack() << " value: ";
  cout << dealerHand.m_val << endl;

  cout << "Player's cards: ";
  cout << playerHand.m_hand[0].printCard() << " " << playerHand.m_hand[1].printCard() << " value: ";
  cout << playerHand.m_val << endl;

  // dealer logic
  // if(hit(17,dealerHand)) {

  // }

  // player logic
  // if(hit(17,dealerHand)) {
    
  // }

  
  return 0;
}
