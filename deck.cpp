#include <algorithm>
#include "deck.hpp"
#include <time.h> /* used to seed rand with current time */
using namespace std;

void CardDeck::shuffle(){
  random_shuffle(m_deck.begin(), m_deck.end());
}

string Card::printBack(){
  return BACK;
}

string Card::printCard(){
  return cardCh.at(m_unicodeVal);
}

void CardDeck::printDeck(){
  cout << "Deck:\n";
  for(int i = 0; i < m_deck.size(); ++i){ /* for each card */
    cout << m_deck[i].printCard() << " "; /* print to console */
  }
  cout << endl;
}

const map<int, string> Card::cardCh = {
  {SPADE + ACE, "🂡"},
  {SPADE + 2, "🂢"},
  {SPADE + 3, "🂣"},
  {SPADE + 4, "🂤"},
  {SPADE + 5, "🂥"},
  {SPADE + 6, "🂦"},
  {SPADE + 7, "🂧"},
  {SPADE + 8, "🂨"},
  {SPADE + 9, "🂩"},
  {SPADE + 10, "🂪"},
  {SPADE + JACK, "🂫"},
  {SPADE + QUEEN, "🂭"},
  {SPADE + KING, "🂮"},
  {HEART + ACE, "🂱"},
  {HEART + 2, "🂲"},
  {HEART + 3, "🂳"},
  {HEART + 4, "🂴"},
  {HEART + 5, "🂵"},
  {HEART + 6, "🂶"},
  {HEART + 7, "🂷"},
  {HEART + 8, "🂸"},
  {HEART + 9, "🂹"},
  {HEART + 10, "🂺"},
  {HEART + JACK, "🂻"},
  {HEART + QUEEN, "🂽"},
  {HEART + KING, "🂾"},
  {DIAMOND + ACE, "🃁"},
  {DIAMOND + 2, "🃂"},
  {DIAMOND + 3, "🃃"},
  {DIAMOND + 4, "🃄"},
  {DIAMOND + 5, "🃅"},
  {DIAMOND + 6, "🃆"},
  {DIAMOND + 7, "🃇"},
  {DIAMOND + 8, "🃈"},
  {DIAMOND + 9, "🃉"},
  {DIAMOND + 10, "🃊"},
  {DIAMOND + JACK, "🃋"},
  {DIAMOND + QUEEN, "🃍"},
  {DIAMOND + KING, "🃎"},
  {CLUB + ACE, "🃑"},
  {CLUB + 2, "🃒"},
  {CLUB + 3, "🃓"},
  {CLUB + 4, "🃔"},
  {CLUB + 5, "🃕"},
  {CLUB + 6, "🃖"},
  {CLUB + 7, "🃗"},
  {CLUB + 8, "🃘"},
  {CLUB + 9, "🃙"},
  {CLUB + 10, "🃚"},
  {CLUB + JACK, "🃛"},
  {CLUB + QUEEN, "🃝"},
  {CLUB + KING, "🃞"}
};
