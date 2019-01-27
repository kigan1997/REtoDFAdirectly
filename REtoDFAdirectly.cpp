/*
Created by Akash Banerjee and Subhojyoti Khastagir
*/
#include <bits/stdc++.h>
using namespace std;
#define M 10
#define N 10

int xyz; //used for numbering leaf nodes
int pt = 0; //keep the count for no of states in dfa table

typedef struct states {
	int data;
	struct states *addr;
} STATES;

typedef struct tree_node{
	char symbol;
	bool nullable;
	int pos;
	int firstpos[10],lastpos[10];	
	struct tree_node * left, * right;
} TREE_NODE;

int indexA(char numset[],int ip,char ch){
	if(numset[ip]==ch)
		return 1;
	else return 0;	
}


//display the tree in pre-order
void display(TREE_NODE *t) {
	if(t == NULL)return;
	printf("(%c,%d), ", t->symbol,t->pos);
	display(t->left);
	display(t->right);
}

int priorityCheck(char a){
	if(a == '*') return 1;
	return 0;
}

void append(int arr[],int data){
	int i = 0;
	while(arr[i] != -1)
		if(arr[i++] == data) return;
	arr[i] = data;
	sort(arr,arr+i+1);
}

//find prefix of a RE and return reverse of that prefix expression
char* prefix(char* re){
	stack<char> mystack;
	int len = strlen(re),i,j = 0;
	char ch, *outputStr = (char*)malloc(50 * sizeof(char));
	for(i = len - 1; i >= 0; i--)
	{
		ch = re[i];
		if((ch >= 'a' && ch <= 'z') || (ch == '#') || (ch == 'E') )	
			outputStr[j++] = ch;
		else if (ch == '('){
			while(mystack.top() != ')'){
				outputStr[j++] = mystack.top();
				mystack.pop();
			}
			mystack.pop();
		}
		else if(ch == ')')
			mystack.push(ch);
		else{
			while(mystack.size() && priorityCheck(mystack.top())){
				outputStr[j++] = mystack.top();
				mystack.pop();
			}
			mystack.push(ch);
		}
	}
	while(mystack.size()){
		outputStr[j++] = mystack.top();
		mystack.pop();
	}
	outputStr[j] = '\0';
	return outputStr;
}

//construct tree
TREE_NODE* createTree(char *re) {
	stack<TREE_NODE*> treeStack;
	char ch;
	for( int i=0; i < strlen(re);i++) {
		ch = re[i];
		if((ch >= 'a' && ch <= 'z') || ch == '#') {
			TREE_NODE *node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
			node->symbol = ch;
			node->nullable = false;
			node->left = NULL;
			node->right = NULL;
			for(int k = 0; k < 10; k++)
				node->firstpos[k] = node->lastpos[k] = -1;
			node->firstpos[0] = xyz;
			node->lastpos[0] = xyz;
			node->pos = xyz--;
			treeStack.push(node);
		} else if(ch == 'E') {
			TREE_NODE *node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
			node->symbol = ch;
			node->nullable = false;
			node->left = NULL;
			node->right = NULL;
			for(int k = 0; k < 10; k++)
				node->firstpos[k] = node->lastpos[k] = -1;
			node->firstpos[0] = -1;
			node->lastpos[0] = -1;
			node->pos = -1;
			treeStack.push(node);
		} else if(ch == '*') {
			TREE_NODE *node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
			node->symbol = ch;
			node->nullable = true;
			node->left = treeStack.top();
			treeStack.pop();
			node->right = NULL;
			for(int k = 0; k < 10; k++)
				node->firstpos[k] = node->lastpos[k] = -1;
			for(int j = 0; node->left->firstpos[j] != -1; j++)
				append(node->firstpos,node->left->firstpos[j]);
			for(int j = 0; node->left->lastpos[j] != -1; j++)
				append(node->lastpos,node->left->lastpos[j]);
			node->pos = -1;
			treeStack.push(node);
		} else if(ch == '+') {
			TREE_NODE *node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
			node->symbol = ch;
			node->left = treeStack.top();
			treeStack.pop();
			node->right = treeStack.top();
			treeStack.pop();
			node->nullable = node->left->nullable | node->right->nullable;
			for(int k = 0; k < 10; k++)
				node->firstpos[k] = node->lastpos[k] = -1;
			for(int j = 0; node->left->firstpos[j] != -1; j++)
				append(node->firstpos,node->left->firstpos[j]);
			for(int j = 0; node->right->firstpos[j] != -1; j++)
				append(node->firstpos,node->right->firstpos[j]);
			for(int j = 0; node->left->lastpos[j] != -1; j++)
				append(node->lastpos,node->left->lastpos[j]);
			for(int j = 0; node->right->lastpos[j] != -1; j++)
				append(node->lastpos,node->right->lastpos[j]);
			node->pos = -1;
			treeStack.push(node);
		} else if(ch == '.') {
			TREE_NODE *node = (TREE_NODE*)malloc(sizeof(TREE_NODE));
			node->symbol = ch;
			node->left = treeStack.top();
			treeStack.pop();
			node->right = treeStack.top();
			treeStack.pop();
			node->nullable = node->left->nullable & node->right->nullable;
			for(int k = 0; k < 10; k++)
				node->firstpos[k] = node->lastpos[k] = -1;
			if(node->left->nullable == true) {
				for(int j = 0; node->left->firstpos[j] != -1; j++)
					append(node->firstpos,node->left->firstpos[j]);
				for(int j = 0; node->right->firstpos[j] != -1; j++)
					append(node->firstpos,node->right->firstpos[j]);
			} else {
				for(int j = 0; node->left->firstpos[j] != -1; j++)
					append(node->firstpos,node->left->firstpos[j]);
			}
			if(node->right->nullable == true) {
				for(int j = 0; node->left->lastpos[j] != -1; j++)
					append(node->lastpos,node->left->lastpos[j]);
				for(int j = 0; node->right->lastpos[j] != -1; j++)
					append(node->lastpos,node->right->lastpos[j]);
			} else {
				for(int j = 0; node->right->lastpos[j] != -1; j++)
					append(node->lastpos,node->right->lastpos[j]);
			}
			node->pos = -1;
			treeStack.push(node);
		}
	}
	return treeStack.top();
}

//find followpos of a node
void find_followpos(int followpos[][15],TREE_NODE* root){
	if(root == NULL) return;
	TREE_NODE * c = root;
	int k = 0;
	if(c->symbol == '.')
		for(int i = 0; c->left->lastpos[i] != -1; i++)
			for(int j = 0; c->right->firstpos[j] != -1; j++)
				append(followpos[c->left->lastpos[i]],c->right->firstpos[j]);
	else if(c->symbol == '*')
		for(int i = 0; c->lastpos[i] != -1; i++)
			for(int j = 0; c->firstpos[j] != -1; j++)
				append(followpos[c->lastpos[i]],c->firstpos[j]);

	find_followpos(followpos,root->left);
	find_followpos(followpos,root->right);
}

//check if two state are same or not
//if same return 0
//else return 1
int same(int a[],int b[]){
	int i=0,j=0;
	while(a[i]!=-1 && b[i]!=-1){
		if(a[i] != b[i])
			return 1;
		i++;j++;
	}
	if(a[i]==-1 && b[i]==-1) return 0;
	return 1;
}


//check if a state is new state or not 
//if it is same with an existing state then return the index of that state
//else return -1
int multiple_same(int states[][N],int a[]){
	int j;
	for(j = 0; j <= pt; j++){
		if(!same(states[j],a))
			break;
	}
	if (j == pt + 1) 
		return -1;
	else
		return j;
}


//find move of a state with respect to a symbol
void move(int states[],int followpos[][15],int temp[],char numset[], char ch){
	int i=0,op,k;
	for(i = 0; states[i] != -1; i++){
		op = indexA(numset,states[i],ch);
		k=0;
		if(op)
			while(followpos[states[i]][k] != -1)
				append(temp,followpos[states[i]][k++]);
	}
}

//check a DFA state is final state or not
int check_final(int states[],int final){
	if(states[0] == -1) return 2;
	int k = 0;
	while(states[k]!=-1){
		if(states[k] == final)
			return 1;
		k++;
	}
	return 0;
}


//main
int main(void){
	char symbol[50],inStr[50], *inStr2, numset[25];
	printf("Enter symbol set:");
	//scanf("%s",symbol);
	cin >> symbol;
	printf("Enter any Regular Expression:");
	// scanf("%s",inStr); 
	cin >> inStr;

	//insert '.#' at the end of input string
	int k = strlen(inStr);
	inStr[k++] = '.';
	inStr[k++] = '#';
	inStr[k] = '\0';
	printf("\nInput RE:%s\n",inStr);

	for(int i = 0,k = 0; i < strlen(inStr); i++)
		if((inStr[i] >= 'a' && inStr[i] <= 'z') || inStr[i] == '#')
			numset[k++] = inStr[i];
	numset[k] = '\0';
	

	//for numbering of leaf nodes
	xyz = strlen(numset) - 1;

	//find prefix expression of string
	inStr2 = prefix(inStr);
	printf("Prefix of RE:%s\n", inStr2);


	//generate the tree 
	TREE_NODE *root = createTree(inStr2);
	printf("\nTree: ");
	display(root);
	printf("END\n");

	int ep = strlen(numset);
	int followpos[ep][15];
	for(int i = 0; i < ep; i++)
		for(int j = 0; j < 15; j++)
			followpos[i][j] = -1;


	//fine followpos
	find_followpos(followpos,root);

	printf("\nThe followpos table is as follows:\n");
	for(int i = 0; i < ep; i++){
		for(int j = 0; followpos[i][j] != -1; j++)
			printf("%d,",followpos[i][j]);
		printf("end\n");
	}

	int states[M][N],dfa[M][N],temp[N];
	for(int i = 0; i < M; i++)
		for(int j = 0; j < N; j++)
			states[i][j] = dfa[i][j] = -1;

	memset(temp, -1, sizeof(temp));
	
	int i = 0,j,sym = strlen(symbol);
	while(root->firstpos[i] != -1){
		append(states[0],root->firstpos[i++]);
	}

	for(i = 0;i <= pt; i++){
		for(j = 0; j < sym; j++){
			memset(temp, -1, sizeof(temp));
			move(states[i],followpos,temp,numset,symbol[j]);

			int abc = multiple_same(states,temp);
			if(abc != -1)//for old state
				dfa[i][j] = abc;
			else{	//for new state, put that state into list of states
				++pt;
				int k=0;
				while(temp[k] != -1)
					append(states[pt],temp[k++]);
				dfa[i][j] = pt;
			}	
		}
	}


	printf("\nThe equivalent DFA transition table is as follows:\n");

	for(int p = 0; p < sym; p++)
		printf("\t%c",symbol[p]);
	printf("\n");

	int a2 = strlen(numset) - 1;

	for(int p = 0; p <= pt; p++){
		if(!p && check_final(states[p],a2) == 1) printf("(S%d)<-:\t",p);
		else if(!p && check_final(states[p],a2) == 0) printf("S%d<--:\t",p);
		else if(check_final(states[p],a2) == 1) printf("(S%d):\t",p);
		else if(check_final(states[p],a2) == 2) printf("S%d(R):\t",p);
		else printf("S%d:\t",p);
		for(int q = 0; q < sym; q++)
		{
			printf("S%d\t", dfa[p][q]);
		}
		printf("\n");
	}
	
}
