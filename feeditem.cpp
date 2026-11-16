
#include "feeditem.h"

void FeedItem::displayFeedItem()
{
    cout << "\n ******************** \n";

    cout << "Content ID: " << contentId << endl;
    cout << "Author ID: " << authorId << endl;
    cout << "Type: " << type << endl;

    cout << "Title: " << title << endl;
    cout << "Content: " << content << endl;

}
