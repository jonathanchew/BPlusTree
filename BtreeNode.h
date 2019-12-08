/*
 * Group project B+ Tree
 *
 *
 *
 *
 *
 *
 * Last Modified: 11/25/2019
 *
 *
 *11/27/2019
 * -add utility function,insertion and solve for overflow(neither checked)
 *
 * 12/2/2019
 * -modified insert,solve for overflow,level order traversal and search
 * -initial value to -1 is unnecessary, a node with no parents(disconnected node) is not yet have value stored
 */


#ifndef B_TREE_BTREENODE_H
#define B_TREE_BTREENODE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <list>
#include <sstream>
#include <fstream>
//#include "SDL_Plotter.h"
#include "rectangle.h"
#include "line.h"


using namespace std;

class Bptree {
private:

    //degree of the tree
    int order;
    unsigned int delay;


    struct node {
        point upperLeft,downRight;
        rectangle it;
        node* parent;
        vector<int> key;
        vector<rectangle> keyBlock[7];
        bool isleaf;
        int value;
        vector<node*> children;
        node *leftSib = nullptr,*rightSib = nullptr;

        node() {

            rectangle it;
            parent = nullptr;
            isleaf = true;
            vector<int> key[7];
            vector<rectangle> keyBlock[7];
            vector<node*> children[9];
            leftSib = nullptr;
            rightSib = nullptr;

            value = -1; //value initialized to -1 to show that nothing is stored in the node
            // (Like with the visualization, only positive numbers can be added to the tree)
        }

        ~node(){};
    };

    node* root = nullptr;

    //element that last visited
    node* last_visited = nullptr;

    list<node*> leaves;

public:


    Bptree(int o){
        root = nullptr;
        order = o;
        delay = 5;
    }

    ~Bptree(){ //not implemented

    }

    node* search(int x) {
        node* v = root;
        int i;
        while (v) {
            for (i = 0; i < v->key.size() && x >= v->key[i] ; i++){
                if (x == v->key[i] && v->isleaf) {
                    last_visited = v;
                    return v;
                }
            }

            last_visited = v;
            if(hasChildren(v)){
                v = v->children[i];
            }else{
                return nullptr;
            }

            i = 0;


        }

        return nullptr;

    }





    node* merge(node*, node*);

    size_t findPos(vector<int> arr,int comp) {
        size_t i;
        for (i = 0; i < arr.size() && comp > arr[i]; i++) {}
        return i;
    }

    bool isFull(node* n) {
        return n->key.size() == this->order;
    }

    bool hasChildren(node* n) {
        return n->children.size()!= 0;
    }

//    rectangle LeftSibiling(node* n){
//        if(n != root && n != n->parent->children[0]){
//            return;
//        }
//    }


    void shiftRight(node* n,size_t pos){

        for(size_t i = n->key.size(); i > pos; i--){


            n->key.insert(n->key.begin()+i,n->key[i-1]);
        }
        //adjustGraphR(n);
    }

    void shiftRight(vector<node*> v,size_t pos){
        for(size_t i = v.size(); i > pos; i--){

            v.insert(v.begin()+i,v[i-1]);
        }
    }

    void solveOverFlow(node* ptr) {
        while (ptr && ptr->key.size() >= order) {
            size_t pivot;
            if(order%2 == 1){
                pivot = order/2;
            }else{
                pivot =(order+1)/2;
            }

            if(!ptr->isleaf){
                //create a right children
                node* rc = new node();
                rc->leftSib = ptr;
                if(ptr->rightSib){
                    rc->rightSib = ptr->rightSib;
                    rc->rightSib->leftSib = rc;
                }

                ptr->rightSib = rc;



                if(root == ptr && !hasChildren(root)){
                    for (int i = pivot; i < ptr->key.size(); i++) {
                        rc->key.push_back(ptr->key[i]);
                    }

                    ptr->key.erase(ptr->key.begin() + pivot, ptr->key.end());

                    node* temp = new node();
                    temp->isleaf = false;
                    rc->parent = temp;

                    //new root has the key that suppose to be pushed up
                    temp->key.push_back(rc->key[0]);


                    //store the children in new root
                    temp->children.push_back(ptr);
                    temp->children.push_back(rc);

//                    point x1;
//                    point x2 = ptr->it.getP2();
//                    x1.x = 750-((order*order)*25);
//                    x1.y = root->it.getP2().y + 30;
//                    x2.x = x1.x + 50;
//                    x2.y = x1.y + 30;
//
//                    rectangle r1(x1,x2);
//                    ptr->keyBlock->push_back(r1);
//                    ptr->it = r1;
//                    r1.draw(g);
//
//                    x2.x = 750+((order*order)*25);
//                    x1.x = x2.x - 50;
//                    rectangle r2(x1,x2);
//                    rc->it = r2;
//                    rc->keyBlock->push_back(r2);
//                    r2.draw(g);





                    this->root = temp;
                    ptr->parent = root;
                    ptr->isleaf = true;


                }else{
                    int pushed = ptr->key[pivot];
                    for (int i = pivot + 1; i < ptr->key.size(); i++) {
                        rc->key.push_back(ptr->key[i]);
                    }

                    ptr->key.erase(ptr->key.begin() + pivot, ptr->key.end());

                    if (hasChildren(ptr)) {
                        for (size_t i = pivot + 1,j = 0; i < ptr->children.size(); i++,j++) {
                            rc->children.push_back(ptr->children[i]);
                            rc->children[j]->parent = rc;
                        }


                        ptr->children.erase(ptr->children.begin() + pivot+1, ptr->children.end());
                    }

                    rc->isleaf = false;
                    ptr->isleaf = false;

                    if (root == ptr) {

                        node* temp = new node();
                        temp->isleaf = false;
                        rc->parent = temp;

                        temp->key.push_back(pushed);
                        //store the children in new root
                        temp->children.push_back(ptr);
                        temp->children.push_back(rc);
                        this->root = temp;
                        ptr->parent = root;

                    }

                    else {
                        size_t pos = findPos(ptr->parent->key, ptr->key[pivot]);
                        shiftRight(ptr->parent, pos);

                        ptr->parent->key.insert(ptr->parent->key.begin()+pos,pushed);
                        shiftRight(ptr->parent->children, pos + 1);

                        //ptr->parent->children.insert(ptr->parent->children.begin()+pos,ptr);
                        ptr->parent->children.insert(ptr->parent->children.begin()+pos+1,rc);
                        rc->parent = ptr->parent;
                    }



                }



            }else{

                size_t pos = findPos(ptr->parent->key, ptr->key[pivot]);

                shiftRight(ptr->parent,pos);
                ptr->parent->key.insert(ptr->parent->key.begin()+pos,ptr->key[pivot]);

                if(pos == 0){

                    node* left = new node();
                    left->rightSib = ptr;
                    ptr->leftSib = left;

                    for(size_t i = 0; i < pivot; i++){
                        left->key.push_back(ptr->key[i]);
                    }
                    left->parent = ptr->parent;
                    size_t i = pivot,j = 0,size = ptr->key.size();
                    for(i,j; i < size; i++,j++){
                        swap(ptr->key[j], ptr->key[i]);
                    }
                    ptr->key.erase(ptr->key.begin()+j,ptr->key.end());


                    shiftRight(ptr->parent->children,0);
                    ptr->parent->children.insert(ptr->parent->children.begin(), left);
                    left->parent = ptr->parent;

                }else{
                    node* right = new node();
                    right->leftSib = ptr;
                    if(ptr->rightSib){
                        right->rightSib = ptr->rightSib;
                        right->rightSib->leftSib = right;
                    }

                    ptr->rightSib = right;


                    for(size_t i = pivot; i < ptr->key.size(); i++){
                        right->key.push_back(ptr->key[i]);

                    }
                    ptr->key.erase(ptr->key.begin()+pivot,ptr->key.end());


//                    rectangle r1 = ptr->it;
//                    point p1 = ptr->it.getP1();
//                    point p2 = ptr->it.getP2();
//                    p1.x -= order*50;
//                    p2.x = p1.x+50;
//                    moveToPosition(ptr->it,p1,p2);



                    shiftRight(ptr->parent->children,pos+1);
                    ptr->parent->children.insert(ptr->parent->children.begin()+pos+1,right);
                    right->parent = ptr->parent;



                }

            }

            ptr = ptr->parent;


        }
    }


    //insert success or not
    bool insert(int item) {

//        rectangle temp;
//        temp.draw(g);
//        g.update();

        node* y = search(item);
        if (y) {
            return false;
        }
        if (root == nullptr) {
            root = new node();
            root->isleaf = false;
//            root->leftSib = nullptr;
//            root->rightSib = nullptr;
            root->key.push_back(item);
            last_visited = root;
//            point x1(750-(25*(order-1)),400);
//            point x2(x1.x+50,430);
//            rectangle rt1(x1,x2);
//            root->it = rt1;
//            root->it.draw(g);
//            root->keyBlock->push_back(root->it);


        }else if(!hasChildren(root)){
            size_t pos = findPos(root->key,item);
//            rectangle r1 = drawKeyPos(root,item);
            shiftRight(root,pos);
            root->key.insert(root->key.begin()+pos,item);
            //root->it.erase(g);
//            point x1 = root->it.getP1();
//            point x2 = root->it.getP2();
//            x1.x = x2.x;
//            x2.x +=50 ;
//
//            rectangle rec(x1,x2);
//            root->it.setP2(x2);
//
//            root->keyBlock->insert(root->keyBlock->begin()+pos,rec);
//            rec.draw(g);

//            moveToPosition(temp,x1,x2);
//            root->it.draw(g);
//            g.update();
        }
        else {


            int i = findPos(last_visited->key, item);
            //make room for store item at position
            shiftRight(last_visited,i);
            //store the item
            last_visited->key.insert(last_visited->key.begin()+i,item);




        }

         solveOverFlow(last_visited);
    }

    int findPosDelete(vector<int> arr,int comp) {
        int i;
        for (i = 0; i < arr.size() && comp > arr[i]; i++) {}
        if(arr[i]!=comp){
            i = -1;
        }
        return i;
    }

    node* searchDelete(int x, bool &isKey) {
        node* v = root;
        isKey = false;
        node * keyPtr = nullptr;
        int i;
        while (v) {
            for (i = 0; i < v->key.size() && x >= v->key[i] ; i++){
                if (x == v->key[i]) {
                    if(v->isleaf){
                        last_visited = v;
                        return v;
                    }
                    else{
                        isKey = true;
                    }
                }
            }
            last_visited = v;
            if(hasChildren(v)){
                v = v->children[i];
            }else{
                return nullptr;
            }
            i = 0;
        }
        return nullptr;

    }

    bool remove(int item){
        if (!root) {
            return false;
        }else if(!hasChildren(root)){
            int pos = findPosDelete(root->key, item);
            if(pos==-1){
                return false;
            }
            root->key.erase(root->key.begin()+pos);
        }else{
            bool isKey = false;
            node *y = searchDelete(item, isKey);
            if(!y){
                return false;
            }
            int pos = findPosDelete(last_visited->key, item);
            int min = ceil((static_cast<double>(order)/2.0))-1;
            last_visited->key.erase(last_visited->key.begin()+pos);
            if(last_visited->key.size() <min){
                solveUnderFlow(last_visited,item);
            }


            if(isKey){
                //delete the key, replace with child
            }
        }
    }

    void solveUnderFlow(node* ptr,int item){
        int min = ceil((static_cast<double>(order)/2.0))-1;
        bool newRoot = false;
        while(!newRoot && (ptr->key.size() < min || ptr->children.size() == ptr->key.size())){
            this->levelOrder(cout);
            int x = findPos(ptr->key,item);
            if(ptr->key.size() != 0 && ptr->key[x] == item){
                ptr->key.erase(ptr->key.begin()+x);
            }




            if(ptr->leftSib && ptr->leftSib->key.size() > min){

                ptr->parent->key.insert(ptr->parent->key.begin(),ptr->leftSib->key[ptr->leftSib->key.size()-1]);
                ptr->leftSib->key.erase(ptr->leftSib->key.end());
                ptr->key.insert(ptr->key.begin(),ptr->parent->key[ptr->parent->key.size()-1]);
                ptr->parent->key.erase(ptr->parent->key.end());
                if(hasChildren(ptr->leftSib)){
                    ptr->children.insert(ptr->children.begin(),ptr->leftSib->children[ptr->leftSib->children.size()-1]);
                    ptr->children[0]->parent = ptr;
                    ptr->children.erase(ptr->children.end());
                }
            }

            else if(ptr->rightSib && ptr->rightSib->key.size() > min){
                if(ptr->rightSib->isleaf){
                    ptr->parent->key.push_back(ptr->rightSib->key[1]);
                    ptr->key.insert(ptr->key.end(),ptr->parent->key[0]);
                    ptr->rightSib->key.erase(ptr->rightSib->key.begin());
                    ptr->parent->key.erase(ptr->parent->key.begin());
                }else{
                    ptr->parent->key.push_back(ptr->rightSib->key[0]);
                    ptr->key.insert(ptr->key.end(),ptr->parent->key[0]);
                    ptr->rightSib->key.erase(ptr->rightSib->key.begin());
                    ptr->parent->key.erase(ptr->parent->key.begin());
                    if(hasChildren(ptr->rightSib)){
                        ptr->children.push_back(ptr->rightSib->children[0]);
                        ptr->children[ptr->children.size()-1]->parent = ptr;
                        ptr->rightSib->children.erase(ptr->rightSib->children.begin());
                    }
                }


            }else if(ptr->isleaf){
                if(ptr->leftSib && ptr->leftSib->parent == ptr->parent){
                    for(int i = 0; i < ptr->leftSib->key.size(); i++){
                        ptr->key.insert(ptr->key.begin()+i,ptr->leftSib->key[i]);
                    }
                    int pos;
                    for( pos = 0; pos < ptr->parent->children.size() && ptr->parent->children[pos]!=ptr->leftSib; pos++){}
                    ptr->parent->children.erase(ptr->parent->children.begin()+pos);

                    if(ptr->leftSib->leftSib){
                    ptr->leftSib = ptr->leftSib->leftSib;
                    }

                    ptr->leftSib->rightSib = ptr;


                }else if(ptr->rightSib && ptr->rightSib->parent == ptr->parent){


                    int p = ptr->rightSib->key[0];
                    for(int i = 0; i < ptr->rightSib->key.size(); i++){
                        ptr->key.push_back(ptr->rightSib->key[i]);
                    }
                    int pos;
                    for( pos = 0; pos < ptr->parent->children.size() && ptr->parent->children[pos]!=ptr->rightSib; pos++){}
                    ptr->parent->children.erase(ptr->parent->children.begin()+pos);
                    int po = findPos(ptr->parent->key,p);
                    if(ptr->key.size() != 0 && ptr->key[po] == p){
                        ptr->parent->key.erase(ptr->parent->key.begin()+po);
                    }

                    if(ptr->rightSib->rightSib){
                        ptr->rightSib = ptr->rightSib->rightSib;
                    }

                    ptr->rightSib->leftSib = ptr;
                }

            }else{
                if(ptr->leftSib && ptr->leftSib->parent == ptr->parent){
                    int i;
                    for(i = 0; i < ptr->parent->children.size() && ptr->leftSib != ptr->parent->children[i]; i++){}
                    int k = ptr->parent->key[i];
                    ptr->parent->key.erase(ptr->parent->key.begin()+i);
                    ptr->parent->children.erase(ptr->parent->children.begin()+i);
                    ptr->key.push_back(k);
                    for(int j = 0; j < ptr->leftSib->key.size(); j++){
                        ptr->key.insert(ptr->key.begin()+j,ptr->rightSib->key[j]);
                    }
                    for(int k = 0; k < ptr->leftSib->children.size(); k++){
                        ptr->children.insert(ptr->children.begin()+k,ptr->leftSib->children[k]);
                    }

                    if(ptr->leftSib->leftSib){
                    ptr->leftSib = ptr->leftSib->leftSib;
                    }

                    ptr->leftSib->rightSib = ptr;

                    for(int o = 0; o < ptr->children.size();o++){
                        ptr->children[o]->parent = ptr;
                    }


                }else if(ptr->rightSib && ptr->rightSib->parent == ptr->parent){
                    int i;
                    for(i = 0; i < ptr->parent->children.size() && ptr->rightSib != ptr->parent->children[i]; i++){}
                    int k = ptr->parent->key[i-1];
                    ptr->parent->key.erase(ptr->parent->key.begin()+i-1);
                    ptr->parent->children.erase(ptr->parent->children.begin()+i);
                    ptr->key.push_back(k);
                    for(int j = 0; j < ptr->rightSib->key.size(); j++){
                        ptr->key.push_back(ptr->rightSib->key[j]);
                    }
                    for(int k = 0; k < ptr->rightSib->children.size(); k++){
                        ptr->children.push_back(ptr->rightSib->children[k]);
                    }
                    if(ptr->rightSib->rightSib){
                    ptr->rightSib = ptr->rightSib->rightSib;
                    }
                    ptr->rightSib->leftSib = ptr;

                    for(int o = 0; o < ptr->children.size();o++){
                        ptr->children[o]->parent = ptr;
                    }

                }





            }


            if(ptr->parent == root && root->key.empty()){
                ptr->parent = nullptr;
                root = ptr;
                newRoot = true;

            }
            ptr = ptr->parent;

        }

    }


    node* merge(node* ptr){
        int min = ceil((static_cast<double>(order)/2.0))-1;
        node * temp;
        size_t i;
        bool done = false;
        while(ptr->key.size() < min){
                temp = ptr->parent;
                for(i = 0; i < temp->children.size() && temp->children[i]!=ptr; i++){}
                if(!hasChildren(ptr)){ //ptr must be a leaf to try borrowing
                    //try to borrow from right
                    cout << "Children key size: " << temp->children[i+1]->key.size() << " min = " << min << endl;
                    if((i+1) < temp->children.size() && temp->children[i+1]->key.size()-1 >= min){
                        ptr->key.push_back(temp->children[i+1]->key.front());
                        temp->children[i+1]->key.erase(temp->children[i+1]->key.begin());
                        temp->key[i]=temp->children[i+1]->key.front();
                        done = true;
                    }
                        //try to borrow from left
                    else if( i!= 0 && temp->children[i-1]->key.size()-1 >= min){
                        ptr->key.insert(ptr->key.begin(), temp->children[i-1]->key.back());
                        temp->children[i-1]->key.pop_back();
                        temp->key[i-1]=ptr->key.front();
                        done = true;
                    }
                }
                if(!done){ //this means borrowing was not successful
                    if(i+1<temp->children.size() && ptr->isleaf){ //then we will merge with right sibling
                        cout << "hi" << endl;
                        for(int j = 0; j < temp->children[i+1]->key.size(); j++){
                            ptr->key.insert(ptr->key.end(),temp->children[i+1]->key[j]);
                        }

                        temp->children.erase(temp->children.begin()+i+1);
                        temp->key.erase(temp->key.begin()+i);
                    }else if((i+1) < temp->children.size()){
                        if(temp->children.size()-1 == 0 ){
                            temp->key.push_back(ptr->rightSib->key[0]);
                        }else if(temp->children.size()-1 <= temp->key.size()){

                        }
                    }

                    else{ //then merge with left sibling
                        for(int j = 0; j < ptr->key.size(); i++){
                            temp->children[i-1]->key.insert(temp->children[i-1]->key.end(),ptr->key[i]);
                        }

                        ptr = temp->children[i-1];
                        temp->children.erase(temp->children.begin()+i);
                        temp->key.erase(temp->key.begin()+i-1);
                    }

                    //if(temp->key.size() < temp->children.size()){
                    //    done = true;
                    //}

                }
                ptr = temp;
            }


    }



    int xDimension(){
        queue<node *> queue1, queue2;
        int count = 0;
        if(!root){
            return count;
        }
        queue1.push(root);
        while(!queue1.empty() || !queue2.empty()){
            while(!queue1.empty()){
                count+=10;
                if(hasChildren(queue1.front())){
                    for (auto i : queue1.front()->children) {
                        queue2.push(i);
                    }
                }
                for(auto i : queue1.front()->key){
                    count+=60;
                }
                queue1.pop();
            }
            if(!queue2.empty()){
                count=0;
            }
            while(!queue2.empty()){
                count+=10;
                if(hasChildren(queue2.front())){
                    for (auto i : queue2.front()->children) {
                        queue1.push(i);
                    }
                }
                for(auto i : queue1.front()->key){
                    count+=60;
                }
                queue2.pop();
            }
            if(!queue1.empty()){
                count = 0;
            }
        }
        return count;
    }

    void printDigit(SDL_Plotter &g, int a, point P, color color1, int scale) {
        ifstream input;
        int rows, cols;
        switch(a){
            case 0:
                input.open("0.txt");
                break;
            case 1:
                input.open("1.txt");
                break;
            case 2:
                input.open("2.txt");
                break;
            case 3:
                input.open("3.txt");
                break;
            case 4:
                input.open("4.txt");
                break;
            case 5:
                input.open("5.txt");
                break;
            case 6:
                input.open("6.txt");
                break;
            case 7:
                input.open("7.txt");
                break;
            case 8:
                input.open("8.txt");
                break;
            case 9:
                input.open("9.txt");
                break;
            default:
                break;
        }
        input >> rows >> cols;
        char letter[rows][cols];
        for(int r = 0; r < rows; r++){
            for(int c = 0; c < cols; c++){
                input >> letter[r][c];
            }
        }
        for(int y = 0; y < rows; y++){
            for(int x = 0; x < cols; x++){
                for(int r=0; r<scale; r++){
                    for(int c=0; c<scale; c++){
                        if(letter[y][x]=='1'){
                            g.plotPixel(P.x+scale*x+r, P.y+scale*y+c, color1.R, color1.G, color1.B);
                        }
                    }
                }
            }
        }
        input.close();

    }

    void plotNumber(const string message, int scale, point P, SDL_Plotter &g, color C) {
        size_t n=0;
        while(n<message.length()){
            switch(message[n]){
                case '0':
                    printDigit(g, 0, P, C, scale);
                    break;
                case '1':
                    printDigit(g, 1, P, C, scale);
                    break;
                case '2':
                    printDigit(g, 2, P, C, scale);
                    break;
                case '3':
                    printDigit(g, 3, P, C, scale);
                    break;
                case '4':
                    printDigit(g, 4, P, C, scale);
                    break;
                case '5':
                    printDigit(g, 5, P, C, scale);
                    break;
                case '6':
                    printDigit(g, 6, P, C, scale);
                    break;
                case '7':
                    printDigit(g, 7, P, C, scale);
                    break;
                case '8':
                    printDigit(g, 8, P, C, scale);
                    break;
                case '9':
                    printDigit(g, 9, P, C, scale);
                    break;
                default:
                    break;
            }
            n++;
            P.x+=(18*scale);
        }
    }

    void drawNode(SDL_Plotter& g, node * ptr, point P, color C){
        string arr;
        int num;
        stringstream ss;
        for (int i : ptr->key) {
            num = i;
            if(0<=num && num<10){
                ss << "00";
            }else if(10<=num && num<100){
                ss << "0";
            }
            ss << num;
            ss >> arr;
            ss.clear();
            rectangle rec(P, point(P.x+60, P.y+25));
            rec.drawOutline(g,RED);
            plotNumber(arr, 1, point(P.x+5, P.y+2), g, C);
            P.x+=60;
        }
    }

//    void levelOrderInSDL(ostream& os){
//        queue<node *> queue1;
//        queue1.push(root);
//
//        while(!queue1.empty()){
//            node* temp = queue1.front();
//            queue1.pop();
//
//            if (temp == nullptr) {
//                os << "NULL ";
//            }else {
//                temp->it.draw(g);
//                g.update();
//                for(int i = 0; i < temp->children.size(); i++){
//                    queue1.push(temp->children[i]);
//                }
//            }
//        }
//        os << endl;
//    }


    void levelOrder(ostream& os){
        queue<node *> queue1, queue2;
        if(!root){
            os << "NULL" << endl;
            return;
        }
        queue1.push(root);
        while(!queue1.empty() || !queue2.empty()){
            while(!queue1.empty()){
                if(hasChildren(queue1.front())){
                    for (auto i : queue1.front()->children) {
                        queue2.push(i);
                    }
                }
                for(int i = 0; i<queue1.front()->key.size(); i++){
                    if(!queue1.front()->isleaf && queue1.front()!=root){
                        os << "K: ";
                    }
                    os << queue1.front()->key[i] << " ";
                }
                queue1.pop();
                os << "   ";
            }
            os << endl;
            while(!queue2.empty()){
                if(hasChildren(queue2.front())){
                    for (auto i : queue2.front()->children) {
                        queue1.push(i);
                    }
                }
                for(int i = 0; i<queue2.front()->key.size(); i++){
                    if(!queue2.front()->isleaf && queue2.front()!=root){
                        os << "K: ";
                    }
                    os << queue2.front()->key[i] << " ";
                }
                queue2.pop();
                os << "   ";
            }
            os << endl;
        }
        os << endl;



    }

    void drawGraph(SDL_Plotter &plotter){
        plotter.clear();
        queue<node *> queue1, queue2;
        queue1.push(root);
        int x=5, y=5;
        while(!queue1.empty() || !queue2.empty()){
            while(!queue1.empty()){
                if(hasChildren(queue1.front())){
                    for (auto i : queue1.front()->children) {
                        queue2.push(i);
                    }
                }
                drawNode(plotter, queue1.front(), point(x, y), color(0, 0, 0));
                x+=queue1.front()->key.size()*60+10;
                queue1.pop();
            }
            y+=50;
            x=5;
            while(!queue2.empty()){
                if(hasChildren(queue2.front())){
                    for (auto i : queue2.front()->children) {
                        queue1.push(i);
                    }
                }
                drawNode(plotter, queue2.front(), point(x, y), color(0, 0, 0));
                x+=queue2.front()->key.size()*60+10;
                queue2.pop();
            }
            y+=50;
            x = 5;
        }
        plotter.update();
    }



    void setSpeed(unsigned int dl){
        delay = dl;
    }

    void findSib(node* n){
        if(n == root){
            return;
        }else if(n->isleaf){

        }
    }

    int findCpos(node* n){
        int i = 0;
        for(i ; n != n->parent->children[i] && i < n->parent->children.size(); i++){}
        return i;
    }




    rectangle drawKeyPos(node*,int );

    rectangle findPosition(node*);

    void conncetNode(node*,node*);

//    void draw(){
//        root->it.draw(g);
//    }

    void moveToPosition(rectangle& ,point&, point&);

    void adjustGraphL(node*);




};

#endif //B_TREE_BTREENODE_H