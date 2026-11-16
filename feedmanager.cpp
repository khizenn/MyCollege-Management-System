
#include "feedmanager.h"
#include <fstream>

using namespace std;

void FeedManager::createPost(int authorId)
{
    FeedItem item;

    item.contentId = nextContentId++;
    item.authorId = authorId;
    item.type = "POST";

    cout <<endl<< "Write your post: ";
    getline(cin >> ws, item.content);

    item.title = "Student Post";

    feed.push_back(item);

    saveFeed();

    cout <<"Post published successfully!\n";
}

void FeedManager::createBlog(int authorId)
{
    FeedItem item;

    item.contentId = nextContentId++;
    item.authorId = authorId;
    item.type = "BLOG";

    cout << endl<<"Enter blog title::";
    getline(cin >> ws, item.title);

    cout << "Write your blog::";
    getline(cin, item.content);

    feed.push_back(item);

    saveFeed();

    cout <<"Blog published successfully"<<endl;
}

void FeedManager::viewFeed()
{
    if (feed.empty())
    {
        cout <<endl<< "No posts or blogs available"<<endl;
        return;
    }

    for (int i = feed.size() - 1; i >= 0; i--)
    {
        feed[i].displayFeedItem();
    }
}

void FeedManager::saveFeed()
{
    ofstream file("feed.txt");

    for (int i = 0; i < feed.size(); i++)
    {
        file << feed[i].contentId << endl;
        file << feed[i].authorId << endl;
        file << feed[i].type << endl;
        file << feed[i].title << endl;
        file << feed[i].content << endl;
    }

    file.close();
}

void FeedManager::loadFeed()
{
    ifstream file("feed.txt");

    if (!file)
    {
        return;
    }

    feed.clear();
    nextContentId = 1;

    FeedItem item;

    
while (file >> item.contentId)
{
    file >> item.authorId;
    file.ignore();

    getline(file, item.type);
    getline(file, item.title);
    getline(file, item.content);

    feed.push_back(item);

    if (item.contentId >= nextContentId)
    {
        nextContentId = item.contentId + 1;
    }
}

    file.close();
}
