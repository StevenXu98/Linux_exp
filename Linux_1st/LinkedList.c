#include <stdio.h>
#include <stdlib.h>

// I apologize for my terrible code

typedef struct node
{
	int data;
	struct node *next; 
} Lnode, *Plist;

Plist ListInit_hp();

int ListLength_hp(Plist L);
int ListLength_nhp(Plist L);

Plist ListGet_hp(Plist L ,int n);
Plist ListGet_nhp(Plist L ,int n);

Plist ListLocate_hp(Plist L, int data);
Plist ListLocate_nhp(Plist L, int data);

Plist ListInsert_hp(Plist L, Plist p, int data);
Plist ListInsert_nhp(Plist L, Plist p, int data);

Plist ListDelete_hp(Plist L, int data);
Plist ListDelete_nhp(Plist L, int data);

Plist ListHCreate_hp(Plist L, int* d);
Plist ListHCreate_nhp(Plist L, int* d);

void ListMerge(Plist la, Plist lb, Plist lc);

void ListPrint_h(Plist L);
void ListPrint_nh(Plist L);

void clean_stdin();

int main(int argc, char const *argv[])
{
	Plist phead_h;
	Plist phead_nh;

	Plist phead2_h;
	Plist result_h;

	Plist pget;

	int judge = 1;
	char oper;

	int input,input2;


	int data[5] = { 1, 3, 5, 7, 9};
	int data2[5] = { 0, 2, 4, 6, 8};

	printf("please enter 1:with headnode; 0:without headnode :");
	scanf("%d", &judge);
	clean_stdin();
	if(judge){
		while ((oper=getchar()) != '\n'){
			switch(oper){
				case 'i': phead_h = ListInit_hp();break;
				case 'l': printf("%d \n",ListLength_hp(phead_h));break;
				case 'g': printf("please input the number you want to get:");
						  scanf("%d", &input);pget = ListGet_hp(phead_h, input);break;
				case 'L': printf("please input the data you want to find:");
						  scanf("%d", &input);ListLocate_hp(phead_h, input);break;
				
				case 'I': printf("please input the data you want to insert:");scanf("%d", &input2);
						  printf("please input the number you want to get :");scanf("%d", &input);
						  pget = ListGet_hp(phead_h, input);
						  ListInsert_hp(phead_h, pget, input2);break;
				
				case 'd': printf("please input the data you want to delete:");scanf("%d", &input);
						  ListDelete_hp(phead_h, input);break;
				case 'c': phead_h = ListHCreate_hp(phead_h, data);break;
				case 'p': ListPrint_h(phead_h);break;

				case 'C': phead2_h = ListHCreate_hp(phead2_h, data2);break;     //create list2		
				case 'm': ListMerge(phead_h, phead2_h, result_h);break;			//merge list 1,2	
				case 'P': ListPrint_nh(result_h);break;							//print list(12)	
				default : break;
			}//switch
			clean_stdin();
		}//while
	}else{
		while ((oper=getchar()) != '\n'){
			switch(oper){
				
				case 'l': printf("%d \n",ListLength_nhp(phead_nh));break;
				case 'g': printf("please input the number you want to get:");
						scanf("%d", &input);pget = ListGet_nhp(phead_nh, input);break;
				case 'L': printf("please input the data you want to find:");
						scanf("%d", &input);ListLocate_nhp(phead_nh, input);break;
				
				case 'I': printf("please input the data you want to insert:");scanf("%d", &input2);
						  printf("please input the number you want to get :");scanf("%d", &input);
						  pget = ListGet_nhp(phead_nh, input);
						  ListInsert_nhp(phead_nh, pget, input2);break;
				
				case 'd': printf("please input the data you want to delete:");
						scanf("%d", &input);ListDelete_nhp(phead_nh, input);break;
				case 'c': phead_nh = ListHCreate_nhp(phead_nh, data);break;
				case 'p': ListPrint_nh(phead_nh);break;
				default : break;					
			}//switch
			clean_stdin();
		}//while
	}//else

	return 0;
}//main


Plist ListInit_hp()//ok
{
	Plist L;
	L = (Lnode *)malloc(sizeof(Lnode));
	if(L == NULL){
		printf("malloc fail");
		exit(0);
	}
	L->next = NULL;	
	return L;
}



int ListLength_hp(Plist L)//ok
{
	int length = 0;
	L = L->next;
	while(L != NULL){
		L = L->next;
		length++;
	}
	return length;
}


int ListLength_nhp(Plist L)//ok
{
	int length = 0;
	while(L != NULL){
		L = L->next;
		length++;
	}
	return length;
}

Plist ListGet_hp(Plist L ,int n)//ok
{
	L = L->next;
	int i = 1;
	while((L != NULL)&&(i < n)){
		L = L->next;
		i++;
	}
	return L;
}

Plist ListGet_nhp(Plist L ,int n)//ok
{
	int i = 1;
	while((L != NULL)&&(i < n)){
		L = L->next;
		i++;
	}
	return L;
}

Plist ListLocate_hp(Plist L, int data)//ok
{
	L = L->next;
	while (L && (L->data != data)){
		L = L->next;
	}
	return L;

}

Plist ListLocate_nhp(Plist L, int data)//ok
{
	while (L && (L->data != data)){
		L = L->next;
	}
	return L;
}

Plist ListInsert_hp(Plist L, Plist p, int data)//ok
{
	Plist q ,pre;
	q = (Lnode *)malloc(sizeof(Lnode));
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

Plist ListInsert_nhp(Plist L, Plist p, int data)//ok
{
	Plist q, pre, *pL;
	pL = &L; 
	q = (Lnode*)malloc(sizeof(Lnode));
	if(q == NULL){
		printf("malloc fail");
		exit(0);
	}
	q->data = data;

	if(p == L){
		q->next = L;
		*pL = q;
		return L;
	}

	pre = L;
	while(pre && (pre->next != p)){
		pre = pre->next;
	}

	q->next = pre->next;
	pre->next = q;

	return L;
}

Plist ListDelete_hp(Plist L, int data)//ok
{
	Plist pre, temp;
	pre = L;
	while(pre && (pre->next->data != data))
		pre = pre->next;

	temp = pre->next;
	if(temp == NULL){
		printf("no found\n");
		return L;
	}else{
		pre->next = pre->next->next;
	}

	free(temp);

	return L;

}

Plist ListDelete_nhp(Plist L, int data)//ok
{
	Plist pre = L, temp;
	Plist *pL;
	pL = &L;

	while(pre && (pre->next->data != data))
		pre = pre->next;

	temp = pre->next;

	if(temp == NULL){
		printf("no found\n");
		return L;
	}else{
		if(temp == L){
			*pL = temp->next;
			return L;
		}else{
			pre->next = pre->next->next;
		}
	}
	free(temp);

	return L;
}

Plist ListHCreate_hp(Plist L, int d[5])//ok
{
	L = (Lnode *)malloc(sizeof(Lnode));
	if(L == NULL){
		printf("malloc fail");
		exit(0);		
	}
	L->next = NULL;

	for(int i = 4; i >= 0; i--){
		Plist p;
		p = (Lnode *)malloc(sizeof(Lnode));
		if(p == NULL){
			printf("malloc fail");
			exit(0);
		}//if
		p->data = d[i];
		p->next = L->next;
		L->next = p;
	}//if
	return L;
}

Plist ListHCreate_nhp(Plist L, int d[5])//ok
{
	L = (Lnode *)malloc(sizeof(Lnode));
	L->data = d[0];
	L->next = NULL;

	for(int i = 4; i > 0; i--){
		Plist p;
		p = (Lnode *)malloc(sizeof(Lnode));
		if(p == NULL){
			printf("malloc fail");
			exit(0);
		}//if
		p->data = d[i];
		p->next = L->next;
		L->next = p;
	}//if
	return L;
}

void ListMerge(Plist la, Plist lb, Plist lc)//ok
{
	Plist pa, pb, pc;
	pa = la->next;
	pb = lb->next;
	lc = la;
	pc = lc;

	while((pa!= NULL)&&(pb!= NULL)){
		if((pa->data <= pb->data)){
			pc->next = pa;
			pc = pa;
			pa = pa->next;
		}else{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}//else
	}//while
	if(pa != NULL) pc->next = pa;
	else pc->next = pb;

	free(pb);
}

void ListPrint_h(Plist L)//ok
{
	L = L->next;
	if(L == NULL){
		printf("no found\n");
		return ;
	}
	while(L != NULL){
		printf("%d ", L->data);
		L = L->next;
	}
	printf("\n");
}

void ListPrint_nh(Plist L)//ok
{

	if(L == NULL){
		printf("no found\n");
		return ;
	}
	while(L != NULL){
		printf("%d ", L->data);
		L = L->next;
	}
	printf("\n");
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
