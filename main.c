//2014004739 신정식
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION 1000000000ULL

typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
struct AVLNode
{
    uint64_t Key;
    uint64_t Value;
    int Height;
    AVLTree Left;
    AVLTree Right;
};

Position SingleLeft(Position P); //SingleRotateWithLeft 다른 표현
Position SingleRight(Position P);
AVLTree Insert(uint64_t ikey,uint64_t ival, AVLTree T);
AVLTree Find(uint64_t ikey, AVLTree T);
void PrintTree(AVLTree T, FILE* close);
int getHeight(Position P);
uint64_t Max(uint64_t a,uint64_t b);
int notfound, dupli, depth;
uint64_t pre_value;
FILE * close;

struct timespec start, end;
double elapsed_time;

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
    clock_gettime(CLOCK_MONOTONIC, &start);
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
                    fprintf(close,"Found (%lld,%lld) on d=%d with h=%d\n",F->Key,F->Value,depth,getHeight(F)-1);
                    depth=0;
                }
                break;
            case 'P':
                PrintTree(T,close);
                fprintf(close,"\n");
                break;
            case 'Q':
                fclose(open);
                fclose(close);
                // end of file
                break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec) * BILLION;
    printf("Elapsed time: %lf (ns)\n", elapsed_time / 1000000000.0);
    return 0;
}


int getHeight(Position P)
{
    return P == NULL ? 0 : P->Height;
}

uint64_t Max(uint64_t a,uint64_t b)
{
    uint64_t c;
    c=a>b?a:b;
    return c;
}

Position SingleLeft(Position P)
{
    Position R = P->Right;
    Position L = R->Left;

    R->Left = P;
    P->Right = L;

    P->Height = Max(getHeight(P->Left),getHeight(P->Right))+1;
    R->Height = Max(getHeight(R->Left),getHeight(R->Right))+1;

    return R; // new root
}

Position SingleRight(Position P)
{
    Position L = P->Left;
    Position R = L->Right;

    L->Right = P;
    P->Left = R;

    P->Height = Max(getHeight(P->Left),getHeight(P->Right))+1;
    L->Height = Max(getHeight(L->Left),getHeight(L->Right))+1;

    return L; // new root

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
            T->Height = 1;
            T->Left = T->Right = NULL;
            return T;
        }
    }
    else if(ikey < T->Key){
        T->Left = Insert(ikey,ival,T->Left);
    }
    else if(ikey > T->Key){
        T->Right = Insert(ikey,ival,T->Right);
    }
    else{
        pre_value = T->Value;
        T->Value = ival;
        dupli=1;
        return T;
    }

    if(getHeight(T->Left)-getHeight(T->Right) > 1)
    {
        if(ikey < T->Left->Key)
            T=SingleRight(T);
        else{
            T->Left = SingleLeft(T->Left);
            T = SingleRight(T);
        }
    }
    else if(getHeight(T->Left)-getHeight(T->Right) < -1)
    {
        if(ikey > T->Right->Key)
            T=SingleLeft(T);
        else {
            T->Right = SingleRight(T->Right);
            T=SingleLeft(T);
        }
    }
    T->Height = Max(getHeight(T->Left),getHeight(T->Right))+1;
    return T;
}
