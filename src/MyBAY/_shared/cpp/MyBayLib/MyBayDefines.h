

//TODO: MyBayDefines festlegen
/********************************************************************/
/*							PLACEHOLDER								*/
/********************************************************************/
#define PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION L" "				// Platzhalter für den Bieter, wenn ein neues Gebot abgegeben wurde und der Nutzer, der die Nachricht erhält, nicht der Auktionator ist


/********************************************************************/
/*							ERROR DEFINES							*/
/********************************************************************/
#define ERROR_AUCTIONNUMBER_NOT_AVAILABLE 0x80110001L
#define ERROR_USER_ALREADY_INTERESTED 0x8011002L

// bid
#define ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST 0x8011003L
#define ERROR_AUCTION_CLOSED 0x8011004L
#define ERROR_BID_TOO_LOW 0x8011005L

// login
#define ERROR_ALREADY_LOGGED_IN 0x8011006L
#define ERROR_FILE_COULD_NOT_BE_OPENED 0x8011007L
#define ERROR_USERNAME_OR_PASSWORD_WRONG 0x8011008L

#define ERROR_USER_NOT_LOGGED_IN 0x8011009L

// offer
#define ERROR_ARTICLENAME_IS_EMPTY 0x8011010L

// getAuctions
#define NO_AUCTIONS_AVAILABLE 0x8011011L

// details
#define ERROR_USER_IS_NOT_AUCTIONEER 0x8011012L
