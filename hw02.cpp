/*
 * CS202-HW02
 * hw02.cpp
 * Çağdaş Öztekin 21201651 Section 1
 */

#include <stdio.h>

#include "hw02.h"

using namespace std;

// initialize an empty WordTree, only constructor
WordTree::WordTree(){
    root = NULL;
}
// deletes the whole tree, deleteTree is a recursive function that calls itself for the left and right children
WordTree::~WordTree(){
    deleteTree(root);
}

void WordTree::deleteTree(WordNode*& node) throw(TreeException)
{
    // nothing to delete if root is null, and the best part here is root is always deleted at the end after all the
    // other nodes are deleted!
    if(root == NULL)
    {
        throw TreeException("Delete failed");
    }
    // go as far as you can go to find all the leaves and delete from leaves to the root
    else if (node != NULL){
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
        node = NULL;
    }
}

/* I can't really fathom why there should be a TreeExcetpion for addWord, there is no explanation in the assignment */
void WordTree::addWord(string& newWord)
{
    /* as the assignment sheet dictates, capitalization should be ignored so I choose to convert all given words
     * to lower case to avoid any conflict
     */
    
    /* I can't fathom how C++ can't have a proper function to toggle case, so I have to initialize a char array here
     * it's length is given word's length
     */
    
    char s[newWord.length()];
    // tolower only applies to chars so I convert each letter to lower case one by one in a for loop
    for(int i = 0; i < newWord.length(); i++)
    {
        s[i] = tolower(newWord[i]);
    }
    
    // this will be the given word converted to lower case
    string str = "";
    
    // each lower case is appended to str because somehow char array s can also reach the original words letters unless stopped
    for(int i = 0; i < newWord.length(); i++)
    {
        str += s[i];
    }
    
    // if tree is empty the first word to come becomes root node's word
    if(root == NULL)
    {
        root = new WordNode(str, NULL, NULL);
        // cout<<"root is "<<str<<endl;
    }
    else
    {
        WordNode* ptr = root;
        
        // if root is already occupied, the most suitable node is found by comparing next nodes' words alphabetically
        while(ptr != NULL)
        {
            // means given word already added to the tree
            if(str.compare(ptr->word) == 0)
            {
                // in this case word count is incremented
                ptr->count++;
                // cout<<str<<" appears again"<<endl;
                break;
            }
            // means given word alphabetically appears earlier in a dictionary than the one it's compared now
            else if(str.compare(ptr->word) < 0)
            {
                // if current node's left child is empty than current word could be inserted
                if(ptr->leftChild == NULL)
                {
                    ptr->leftChild = new WordNode(str, NULL, NULL);
                    // cout<<"left child is "<<str<<endl;
                    break;
                }
                // else further comparisons will be made with current node's left child
                else
                {
                    ptr = ptr->leftChild;
                }
            }
            // means given word alphabetically appears later in a dictionary than the one it's compared now
            else
            {
                // if current node's right child is empty than current word could be inserted
                if(ptr->rightChild == NULL)
                {
                    ptr->rightChild = new WordNode(str, NULL, NULL);
                    // cout<<"right child is "<<str<<endl;
                    break;
                }
                // else further comparisons will be made with current node's right child
                else
                {
                    ptr = ptr->rightChild;
                }
            }
        }
    }
}

void WordTree::printWordCounts()
{
    // prints word counts for each node in pre-order
    WordNode* ptr = root;
    // only root's info is printed here so the output can be as it is on the assignment
    printf("\n\"%s\" appears %d time(s)\n", ptr->word.c_str(), ptr->count);
    ptr->leftChild->printCount();
    ptr->rightChild->printCount();
}

int WordTree::getTotalWordCount()
{
    // traverses each node summing up their word counts
    int count = 0;
    WordNode* ptr = root;
    ptr->countWords(count);
    return count;
}

bool WordTree::isComplete()
{
    // status 2 means every node with height h-1 has two children so far
    int height = 0;
    int status = 2;
    // height is found here
    height = root->check(height);
    int curHeight = 1;
    // count is used to check if all nodes with height h-1 apply the rules
    int count = 0;
    return root->checkComplete(curHeight, status, height, count);
}

// main function
int main(int argc, char **argv)
{
    WordTree wordTree;
    char word[1000];
    FILE * pFile;
    if (! (pFile = fopen("input.txt","r"))){
        return 1;
    }
    while (fscanf(pFile, "%s", word) != EOF)
    {
        string s(word);
        wordTree.addWord(s);
        printf("%s ", word);
    }
    fclose(pFile);
    printf("\nTotal word count:%d", wordTree.getTotalWordCount());
    wordTree.printWordCounts();
    printf("Word tree is complete: %s", wordTree.isComplete() ? "Yes" :
           "No");
    return 0;
}
