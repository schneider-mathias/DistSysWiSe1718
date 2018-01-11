

//TODO: MyBayDefines festlegen
/********************************************************************/
/*							PLACEHOLDER								*/
/********************************************************************/
#define PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION L" "				// Platzhalter für den Bieter, wenn ein neues Gebot abgegeben wurde und der Nutzer, der die Nachricht erhält, nicht der Auktionator ist


/********************************************************************/
/*							ERROR DEFINES							*/
/********************************************************************/
#define ERROR_AUCTIONNUMBER_NOT_AVAILABLE 0xA0110001L
#define ERROR_USER_ALREADY_INTERESTED 0xA011002L

// bid
#define ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST 0xA011003L
#define ERROR_AUCTION_CLOSED 0xA011004L
#define ERROR_BID_TOO_LOW 0xA011005L
#define ERROR_BID_NEGATIVE 0xA011013L

// login
#define ERROR_ALREADY_LOGGED_IN 0xA011006L
#define ERROR_FILE_COULD_NOT_BE_OPENED 0xA011007L
#define ERROR_USERNAME_OR_PASSWORD_WRONG 0xA011008L

#define ERROR_USER_NOT_LOGGED_IN 0xA011009L

// offer
#define ERROR_ARTICLENAME_IS_EMPTY 0xA011010L

// getAuctions
#define ERROR_NO_AUCTIONS_AVAILABLE 0xA011011L

// details
#define ERROR_USER_IS_NOT_AUCTIONEER 0xA011012L
