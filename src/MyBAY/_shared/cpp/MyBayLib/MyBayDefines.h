

//TODO: MyBayDefines festlegen
/********************************************************************/
/*							PLACEHOLDER								*/
/********************************************************************/
#define PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION L" "				// Platzhalter für den Bieter, wenn ein neues Gebot abgegeben wurde und der Nutzer, der die Nachricht erhält, nicht der Auktionator ist


/********************************************************************/
/*							ERROR DEFINES							*/
/********************************************************************/
#define ERROR_AUCTIONNUMBER_NOT_AVAILABLE 0xA0110001L
#define ERROR_USER_ALREADY_INTERESTED 0xA0110002L

// bid
#define ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST 0xA0110003L
#define ERROR_AUCTION_CLOSED 0xA0110004L
#define ERROR_BID_TOO_LOW 0xA0110005L
#define ERROR_BID_NEGATIVE 0xA0110013L

// login
#define ERROR_ALREADY_LOGGED_IN 0xA0110006L
#define ERROR_FILE_COULD_NOT_BE_OPENED 0xA0110007L
#define ERROR_USERNAME_OR_PASSWORD_WRONG 0xA0110008L

#define ERROR_USER_NOT_LOGGED_IN 0xA0110009L

// offer
#define ERROR_ARTICLENAME_IS_EMPTY 0xA0110010L

// getAuctions
#define ERROR_NO_AUCTIONS_AVAILABLE 0xA0110011L

// details
#define ERROR_USER_IS_NOT_AUCTIONEER 0xA0110012L
