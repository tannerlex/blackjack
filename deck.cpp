#include "deck.hpp"
using namespace std;

// CardDeck::CardDeck(){
//   for(int suit = SPADE; suit <= CLUB; suit += SUITROW){
//     for(int face = ACE; face <= KING; ++face){
//       if(face == KNIGHT){++face;}
//       Card card(suit + face);
//       m_deck.push_back(card);
//     }
//   }
// }

string Card::printBack(){
  return BACK;
}

string Card::printCard(){
  // return to_string(m_face) + " of " + to_string(m_suit);
  // return (string) ((char)m_face + (char)m_suit);
  string rtn = BACK;
  switch (m_unicodeVal)
  {
    case SPADE + ACE:
      rtn = "🂡";
      break;
    case SPADE + 2:
      rtn = "🂢";
      break;
    case SPADE + 3:
      rtn = "🂣";
      break;
    case SPADE + 4:
      rtn = "🂤";
      break;
    case SPADE + 5:
      rtn = "🂥";
      break;
    case SPADE + 6:
      rtn = "🂦";
      break;
    case SPADE + 7:
      rtn = "🂧";
      break;
    case SPADE + 8:
      rtn = "🂨";
      break;
    case SPADE + 9:
      rtn = "🂩";
      break;
    case SPADE + 10:
      rtn = "🂪";
      break;
    case SPADE + JACK:
      rtn = "🂫";
      break;
    case SPADE + QUEEN:
      rtn = "🂭";
      break;
    case SPADE + KING:
      rtn = "🂮";
      break;
    case HEART + ACE:
      rtn = "🂱";
      break;
    case HEART + 2:
      rtn = "🂲";
      break;
    case HEART + 3:
      rtn = "🂳";
      break;
    case HEART + 4:
      rtn = "🂴";
      break;
    case HEART + 5:
      rtn = "🂵";
      break;
    case HEART + 6:
      rtn = "🂶";
      break;
    case HEART + 7:
      rtn = "🂷";
      break;
    case HEART + 8:
      rtn = "🂸";
      break;
    case HEART + 9:
      rtn = "🂹";
      break;
    case HEART + 10:
      rtn = "🂺";
      break;
    case HEART + JACK:
      rtn = "🂻";
      break;
    case HEART + QUEEN:
      rtn = "🂽";
      break;
    case HEART + KING:
      rtn = "🂾";
      break;
    case DIAMOND + ACE:
      rtn = "🃁";
      break;
    case DIAMOND + 2:
      rtn = "🃂";
      break;
    case DIAMOND + 3:
      rtn = "🃃";
      break;
    case DIAMOND + 4:
      rtn = "🃄";
      break;
    case DIAMOND + 5:
      rtn = "🃅";
      break;
    case DIAMOND + 6:
      rtn = "🃆";
      break;
    case DIAMOND + 7:
      rtn = "🃇";
      break;
    case DIAMOND + 8:
      rtn = "🃈";
      break;
    case DIAMOND + 9:
      rtn = "🃉";
      break;
    case DIAMOND + 10:
      rtn = "🃊";
      break;
    case DIAMOND + JACK:
      rtn = "🃋";
      break;
    case DIAMOND + QUEEN:
      rtn = "🃍";
      break;
    case DIAMOND + KING:
      rtn = "🃎";
      break;
    case CLUB + ACE:
      rtn = "🃑";
      break;
    case CLUB + 2:
      rtn = "🃒";
      break;
    case CLUB + 3:
      rtn = "🃓";
      break;
    case CLUB + 4:
      rtn = "🃔";
      break;
    case CLUB + 5:
      rtn = "🃕";
      break;
    case CLUB + 6:
      rtn = "🃖";
      break;
    case CLUB + 7:
      rtn = "🃗";
      break;
    case CLUB + 8:
      rtn = "🃘";
      break;
    case CLUB + 9:
      rtn = "🃙";
      break;
    case CLUB + 10:
      rtn = "🃚";
      break;
    case CLUB + JACK:
      rtn = "🃛";
      break;
    case CLUB + QUEEN:
      rtn = "🃝";
      break;
    case CLUB + KING:
      rtn = "🃞";
      break;
  }
  return rtn;
}

