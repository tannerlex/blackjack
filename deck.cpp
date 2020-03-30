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
  {0x1F0A1, "🂡"},
  {0x1F0A2, "🂢"},
  {0x1F0A3, "🂣"},
  {0x1F0A4, "🂤"},
  {0x1F0A5, "🂥"},
  {0x1F0A6, "🂦"},
  {0x1F0A7, "🂧"},
  {0x1F0A8, "🂨"},
  {0x1F0A9, "🂩"},
  {0x1F0AA, "🂪"},
  {0x1F0AB, "🂫"},
  {0x1F0AD, "🂭"},
  {0x1F0AE, "🂮"},
  {0x1F0B1, "🂱"},
  {0x1F0B2, "🂲"},
  {0x1F0B3, "🂳"},
  {0x1F0B4, "🂴"},
  {0x1F0B5, "🂵"},
  {0x1F0B6, "🂶"},
  {0x1F0B7, "🂷"},
  {0x1F0B8, "🂸"},
  {0x1F0B9, "🂹"},
  {0x1F0BA, "🂺"},
  {0x1F0BB, "🂻"},
  {0x1F0BD, "🂽"},
  {0x1F0BE, "🂾"},
  {0x1F0C1, "🃁"},
  {0x1F0C2, "🃂"},
  {0x1F0C3, "🃃"},
  {0x1F0C4, "🃄"},
  {0x1F0C5, "🃅"},
  {0x1F0C6, "🃆"},
  {0x1F0C7, "🃇"},
  {0x1F0C8, "🃈"},
  {0x1F0C9, "🃉"},
  {0x1F0CA, "🃊"},
  {0x1F0CB, "🃋"},
  {0x1F0CD, "🃍"},
  {0x1F0CE, "🃎"},
  {0x1F0D1, "🃑"},
  {0x1F0D2, "🃒"},
  {0x1F0D3, "🃓"},
  {0x1F0D4, "🃔"},
  {0x1F0D5, "🃕"},
  {0x1F0D6, "🃖"},
  {0x1F0D7, "🃗"},
  {0x1F0D8, "🃘"},
  {0x1F0D9, "🃙"},
  {0x1F0DA, "🃚"},
  {0x1F0DB, "🃛"},
  {0x1F0DD, "🃝"},
  {0x1F0DE, "🃞"}
};
