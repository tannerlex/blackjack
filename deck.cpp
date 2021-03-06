#include <algorithm>
#include "deck.hpp"
#include <time.h> /* used to seed rand with current time */
using namespace std;

#define DLHOLD 16
#define DLSFTHLD 17

/* unicode values for playing card characters */
#define BACK "🂠"
#define HEART 0x1F0B0
#define DIAMOND 0x1F0C0

void CardDeck::shuffle(){ /* randomized the order of cards in deck */
  random_shuffle(m_deck.begin(), m_deck.end());
}

string Card::printBack(){ /* prints unicode card back character */
  return BACK;
}

string Card::printCard(){ /* prints card using unicode character */
  return cardCh.at(m_unicodeVal);
}

void CardDeck::printDeck(){ /* prints each card of deck in order */
  cout << "Deck:\n";
  for(int i = 0; i < m_deck.size(); ++i){ /* for each card */
    cout << m_deck[i].printCard() << " "; /* print to console */
  }
  cout << "\n";
}/* printDeck() */

void Hand::addCard(Card c){ /* adds a card to this hand */
  m_hand.push_back(c); /* add card to vector of cards */
  update(); /* update the hand metrics */
}/* addCard() */

void Hand::printHand(){ /* prints each card of hand in order */
  for(int i = 0; i < m_hand.size(); ++i){ /* for each card */
    cout << m_hand[i].printCard() << " "; /* print to console */
  }
  cout << "value: ";
  if(m_softVal){cout << "soft ";}
  cout << m_val << "\n";
}

void CardDeck::deck52(){ /* create 52 cards to add to deck */
  for(int suit = SPADE; suit <= CLUB; suit += SUITROW){/*ea. suit*/
    for(int face = ACE; face <= KING; ++face){ /* foreach face */
      if(face == KNIGHT){++face;}/* knight not used in blackjack */
      Card card(suit + face); /* determine which card */
      m_deck.push_back(card); /* add card to the deck */
    } /* end for each face */
  } /* end for each suit */
}

Card CardDeck::deal(){ /* deal a single card from the deck */
  Card rtn = m_deck.back();
  m_deck.pop_back();
  return rtn;
}/* deal() */

bool Hand::hit(int rank){ /* determine whether to hit or not */
  bool hit;
  switch (rank) /* determine hit strategy based on rank */
  {
    case DEALER:{ /* hit strategy for dealer */
      if(m_softVal){ /* if it has a soft value */
        hit = m_val < DLSFTHLD; /* use the soft value threshold */
      } else {/* hard value */
        hit = m_val < DLHOLD; /* use hard value threshold */
      }
      break;
    }
    default:{ /* hit strategy for any player other than the dealer */
      /* use the -np param and LWSTHOLD to get strategies results */
      hit = m_val < (LWSTHOLD + rank);/* get a spread of strategy */
      break;
    }
  } /* end switch on rank */
  return hit;
}/* hit() */

void Hand::update(){
  /* The update function for the Hand class is used to track the
     value of the hand, whether that is a hard or soft value and
     the number of aces. It must be used every time a hand is
     changed (e.g. a card is dealt) for these numbers to be 
     accurate.                                                     */
  m_val = 0;
  m_aces = 0;
  m_softVal = false;
  for(int i = 0; i < m_hand.size(); ++i){
    int val = m_hand[i].getFaceVal();
    m_val += val;
    if(val == ACEVAL){++m_aces;}
  }
  m_val = aceException(m_aces, m_val);
}/* update() */

int Hand::aceException(int aces, int handVal){
  /* The aceException function in the Hand class solves the hand
     value by accounting for the alternative values of aces. It
     does this by a recursive method. It also determines whether
     the hand value is soft or hard                                */
  if(aces){
    if(handVal > BLACKJACK){
      handVal = aceException(aces-1, handVal - (ACEVAL - ACE));
    } else {
      m_softVal = true;
    }
  }
  return handVal;
} /* aceException() */

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
