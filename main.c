//2014004739 신정식
#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
struct AVLNode
{
    uint64_t Key;
    uint64_t Value;
    uint64_t Height;
    AVLTree Left;
    AVLTree Right;
};

Position SingleLeft(Position P); //SingleRotateWithLeft 다른 표현
Position SingleRight(Position P);
Position DoubleLeft(Position P);
Position DoubleRight(Position);
AVLTree Insert(uint64_t ikey,uint64_t ival, AVLTree T);
AVLTree Find(uint64_t ikey, AVLTree T);
void PrintTree(AVLTree T, FILE* close);
int getHeight(Position P);
int Max(uint64_t a,uint64_t b);
int notfound, dupli, depth;
uint64_t pre_value;
FILE * close;

// PrintTree - root left right
void PrintTree(AVLTree T, FILE* close)
{
    if(T!=NULL){
        fprintf(close,"(%lld,%lld) ",T->Key,T->Value);
        PrintTree(T->Left, close);
        PrintTree(T->Right, close);
    }
}


AVLTree Find(uint64_t ikey, AVLTree T)
{
    if(ikey==NULL){printf("check input\n"); return T;}
    if(T==NULL){notfound=1; return T;}
    if(T->Key > ikey){
        depth++;
        return Find(ikey,T->Left);
    }
    else if(T->Key < ikey){
        depth++;
        return Find(ikey,T->Right);
    }
    else if(T->Key == ikey){
        return T;
    }
}

int main(int argc, char * argv[])
{
    char infi; // insert, find
    uint64_t key,val;
    FILE *open = fopen("input.txt","r");
    close = fopen("output.txt","w");
    if(open==NULL){puts("err : file read"); return 0;}
    AVLTree T = NULL;
    AVLTree F = NULL;
    while(fscanf(open,"%c",&infi) != EOF)
    {
        switch(infi)
        {
            case 'I':
                fscanf(open, "%lld%lld", &key, &val);
                T = Insert(key,val,T);
                if(dupli==1){
                    fprintf(close,"Found (%lld,%lld) update v=%lld\n",key,pre_value,val);
                    dupli=0;
                }
                else fprintf(close,"Inserted (%lld,%lld)\n",key,val);
                break;
            case 'F':
                fscanf(open,"%lld",&key);
                F = Find(key,T);
                if(notfound==1){
                    fprintf(close,"Not Found\n");
                    notfound=0;
                    depth=0;
                }
                else{
                    fprintf(close,"Found (%lld,%lld) on d=%d with h=%d\n",F->Key,F->Value,depth,getHeight(F));
                    depth=0;

                }
                break;
            case 'P':
                PrintTree(T,close);
                fprintf(close,"\n");
            case 'Q':
                // end of file
                break;
        }
    }
    return 0;
}


int getHeight(Position P)
{
    if(P==NULL)
        return -1;
    else
        return P->Height;
}

int Max(uint64_t a,uint64_t b)
{
    uint64_t c;
    c=a>b?a:b;
    return c;
}

Position SingleLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max(getHeight(K2->Left),getHeight(K2->Right))+1;
    K1->Height = Max(getHeight(K1->Left),K2->Height)+1;

    return K1; // new root
}

Position SingleRight(Position K2)
{
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left;
    K1->Left = K2;

    K2->Height = Max(getHeight(K2->Right),getHeight(K2->Left))+1;
    K1->Height = Max(getHeight(K1->Right),K2->Height)+1;

    return K1; // new root

}

Position DoubleLeft(Position K3)
{
    K3->Left = SingleRight(K3->Left);
    return SingleLeft(K3);
}

Position DoubleRight(Position K3)
{
    K3->Right = SingleLeft(K3->Right);
    return SingleRight(K3);
}

AVLTree Insert(uint64_t ikey,uint64_t ival, AVLTree T)
{
    if(T == NULL){
        T=(AVLTree)malloc(sizeof(struct AVLNode));
        if(T==NULL)
            printf("space error\n");
        else{
            T->Key = ikey;
            T->Value = ival;
            T->Height = 0;
            T->Left = T->Right = NULL;
            return T;
        }
    }
    else if(ikey < T->Key){
        T->Left = Insert(ikey,ival,T->Left);
        if(getHeight(T->Left)-getHeight(T->Right) == 2)
        {
            if(ikey < T->Left->Key)
                T=SingleLeft(T);
            else
                T=DoubleLeft(T);
        }
    }
    else if(ikey > T->Key){
        T->Right = Insert(ikey,ival,T->Right);
        if(getHeight(T->Right)-getHeight(T->Left) == 2)
        {
            if(ikey > T->Right->Key)
                T=SingleRight(T);
            else
                T=DoubleRight(T);
        }
    }
    else if(ikey == T->Key){
        pre_value = T->Value;
        T->Value = ival;
        dupli=1;
        return T;
    }
    //로테이션이 없이 Insert 될 때 Height 를 조절
    T->Height = Max(getHeight(T->Left),getHeight(T->Right))+1;

    return T;
}
