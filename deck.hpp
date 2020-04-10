#include <iostream>
#include <map>
#include <time.h> /* used to seed rand with current time */
#include <vector>
using namespace std;

#define BLACKJACK 21
#define ACEVAL 11
#define DEALER 0
#define LWSTHOLD 12

/* unicode values for playing card characters */
#define SPADE 0x1F0A0
#define CLUB 0x1F0D0
#define ACE 0x1
#define FACECRD 10
#define JACK 0xB
#define KNIGHT 0xC
#define QUEEN 0xD
#define KING 0xE
#define SUITROW 0x10

class Card {
  /* Card class includes properties and functions unique to a
     playing card.                                                 */
  public:
  Card();
  explicit Card(int unicodeVal)
    : m_unicodeVal(unicodeVal)
  {
    switch (unicodeVal % 0x10)
    {
    case ACE:
      m_faceValue = ACEVAL; /* default value for ace */
      break;
    case JACK:
    case QUEEN:
    case KING:
      m_faceValue = FACECRD;
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
  int getFaceVal(){return m_faceValue;}
  int getUCV(){return m_unicodeVal;}

  private:
  int m_unicodeVal; /* unique identifier for each card in a deck */
  static const map<int, string> cardCh; /* unicode value, character*/
  int m_faceValue; /* face value of each playing card (ace = 11) */
};

class CardDeck {
  /* CardDeck class is comprised of a vector of cards and
     functions necessary to the management of a 52 card deck for
     blackjack                                                     */
  public:
  CardDeck(){
    /* initialize a 52 card deck */
    deck52();
  } /* end constructor */

  explicit CardDeck(const int numdecks){
    /* initialize large deck of multiples of 52 cards from which
       to play                                                     */
    for(int i = 0; i < numdecks; ++i){ /* for each deck to add */
      deck52(); /* add 52 cards to deck */
    }
  }

  void deck52();
  void shuffle();
  void printDeck();
  Card deal();
  vector<Card> m_deck; /* deck of cards from which cards are dealt */
};

class Hand {
  public: 
  Hand()
    : m_aces(0)
    , m_val(0)
    , m_softVal(false)
  {}

  void addCard(Card c);
  int aceException(int aces, int handVal);
  int getVal(){return m_val;}
  int getSoft(){return m_softVal;}
  int getAces(){return m_aces;}
  bool hit(int rank);
  void printHand();
  vector<Card> m_hand; /* cards dealt to a player in round */

  private:
  void update();
  int m_aces; /* number of aces */
  int m_val; /* value of hand */
  bool m_softVal; /* value is soft due to presence of Ace = 11 */
};
