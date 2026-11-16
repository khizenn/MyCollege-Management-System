
#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <iostream>
#include <string>

using namespace std;

class FeedItem
{
public:
    int contentId;
    int authorId;
    string type;
    string title;
    string content;

    void displayFeedItem();
};

#endif
