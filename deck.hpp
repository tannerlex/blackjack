#include <iostream>
#include <map>
#include <time.h> /* used to seed rand with current time */
#include <vector>
using namespace std;

// #define DECKSIZE 52

/* unicode values for suit characters */
// #define SPADECH "♤"
// #define HEARTCH "♥"
// #define DIAMONDCH "♢"
// #define CLUBCH "♣"

/* unicode values for playing card characters */
#define BACK "🂠"
#define SPADE 0x1F0A0
#define HEART 0x1F0B0
#define DIAMOND 0x1F0C0
#define CLUB 0x1F0D0
#define ACE 0x1
#define JACK 0xB
#define KNIGHT 0xC
#define QUEEN 0xD
#define KING 0xE
#define SUITROW 0x10

class Card {
  /* This "Card" class */
  public:
  Card();
  explicit Card(int unicodeVal)
    : m_unicodeVal(unicodeVal)
  {
    switch (unicodeVal % 0x10)
    {
    case ACE:
      m_faceValue = 11;
      break;
    case JACK:
    case QUEEN:
    case KING:
      m_faceValue = 10;
      break;
    
    /* error cases - invalid cards */
    case 0:
    case KNIGHT:
    case 0xF:
      cout << "Error creating Card - invalid unicode value\n\n";
      break;

    /* cases 2-10 */
    default:
      m_faceValue = unicodeVal % 0x10;
      break;
    }
  }

  string printCard();
  string printBack();
  int m_unicodeVal;
  int getFaceVal(){return m_faceValue;};

  private:
  static const map<int, string> cardCh;
  int m_faceValue;
  // string m_suit; 
};

class CardDeck {
  public:
  CardDeck(){
    // int i = 0;
    /* initialize a 52 card deck */
    for(int suit = SPADE; suit <= CLUB; suit += SUITROW){
      for(int face = ACE; face <= KING; ++face){
        if(face == KNIGHT){++face;}
        Card card(suit + face);
        m_deck.push_back(card);
        // cout << m_deck[i++].printCard() << endl;
      } /* end for each face */
    } /* end for each suit */
  } /* end constructor */

  void shuffle();
  void printDeck();
  Card deal();
  vector<Card> m_deck;

  private:
};

class Hand {
  public: 
  Hand(){
    m_aces = 0;
    m_val = 0;
  }
  vector <Card> m_hand;
  int m_aces, m_val;
};
