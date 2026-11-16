
#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include "feeditem.h"
#include <vector>

using namespace std;

class FeedManager
{
private:
    vector<FeedItem> feed;
    int nextContentId = 1;

public:
    void createPost(int authorId);
    void createBlog(int authorId);
    void viewFeed();

    void saveFeed();
    void loadFeed();
};

#endif
