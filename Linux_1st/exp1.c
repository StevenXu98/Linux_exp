#include<stdio.h >
#include<stdlib.h >
#include<string.h >

typedef struct stuInfo {
               char stuName[10];  /*学生姓名*/
               int Age;  /*年龄*/
            } ElemType;
typedef struct node {
                Elemtype data；
                struct node *next；
           } ListNode, *ListPtr；
ListPtr CreateList()
{
  //insert your code here
	ListPtr L;
	L = (ListNode *)malloc(sizeof(ListPtr));
	if(L == NULL){
		printf("malloc fail");
		exit(0);
	}
	L->next = NULL;
	return L;
}
void InsertList (NodePtr node)
{
//insert your code here
	ListPtr q, pre;
        q = (ListNode *)malloc(sizeof(ListPtr));
        if(q == NULL){
                printf("malloc fail");
                exit(0);
        }
        q->data = data;

        pre = L;

        while(pre && (pre->next != p)){
                pre = pre->next;
        }

        q->next = pre->next;
        pre->next = q;

        return L;

}
void PrintList(ListPtr node)
{
//insert your code here
}
int main(int argc,char argv[])
{
   while(1)
   {
printf(“1 create list\n”);
printf(“2 printf list\n”);
printf(“3 insert list\n”);
printf(“4 quit\n”);
char command =getchar();
switch(command)
{
  case ‘1’ : ListHead = CreateList();
  break;
  case ‘2’ : PrintList(ListHead);
  break;
  case ‘3’ : InsertList (ListHead);
  break;
}
   }
}
