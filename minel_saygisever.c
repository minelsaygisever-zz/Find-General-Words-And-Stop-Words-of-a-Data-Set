//150116028 Minel SAYGISEVER

//This program opens and reads text files under the following directories: economy, health and magazin.
//Firstly creates the Master Linked Lists and keeps the words in Master Linked List in ascending order
//And each node of Master Linked List has a Linked List that represents the documents that contain the term in this record of MLL. 
//Main purpose of this program is to find general words and discriminating words of each category.

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>

//stands for list of documents
struct ll {
	int documentID;
	int occur;
	int length;
	char category[20];
	struct ll *next;
};
typedef struct ll ll;

//stands for Master Linked List
struct mll {
	char termName[50];
	int occur;
	struct ll *llhdr;
	struct mll *next;
};
typedef struct mll mll;

//this struct keeps the terms and how many times that term occurs in each category
struct analyse {
	char termName[50];
	int economy;
	int health;
	int magazin;
	struct analyse *next;
};
typedef struct analyse analyse;

//this struct will be used when we want to sort words by order of use. 
//analyse struct keeps the informations we are going to use in this structs
struct sort{
	char termName[50];
	int sortInt; //what we want to sort by? Is it a specific category name? Or is it total occurence?
	struct sort *next;
};
typedef struct sort sort;

FILE *input;
mll *hdr;
analyse *ahdr;
//pointers of structs that used to sort words in order of use.
sort *general, *economy, *health, *magazin;
//pointers of structs that alphabetize the first five of the most commonly used words
sort *sg, *se, *sh, *sm; //sg = sorted general, se = sorted economy..

//declare functions
int insertAllWords(mll **header, char term[], char directory[], int id, int length);
void readFiles( char directory[]);
void analyseWords(mll **header);
void insertAnalyseWord(analyse **header, char termName[], int economy, int health, int magazin);
void sortWords(analyse **header);
void insertNumericOrder(sort **header, char termName[50], int sortInt);
void insertAlphabetical(sort **header, char termName[50], int sortInt);

main(void){
	
	setlocale(LC_ALL, "Turkish");
	ll *l;
	analyse *a;
	sort *g, *e, *h, *m;
	int i;
	
	readFiles("dataset/econ/");
	readFiles("dataset/health/");
	readFiles("dataset/magazin/");
	
	//let's find out how many times words are used in categories
	analyseWords(&hdr);

	//ordered the words in the order they are used most. 
	sortWords(&ahdr);
	g = general;
	e = economy;
	h = health;
	m = magazin;
	
	//most used 5 words...
	
	//...for general
	for(i = 0; i < 5; i++){
		insertAlphabetical(&sg, g->termName, g->sortInt);
		g = g->next;
	}
	//...only used in economy
	for(i = 0; i < 5; i++){
		insertAlphabetical(&se, e->termName, e->sortInt);
		e = e->next;
	}
	//...only used in health
	for(i = 0; i < 5; i++){
		insertAlphabetical(&sh, h->termName, h->sortInt);
		h = h->next;
	}
	//...only used in magazin
	for(i = 0; i < 5; i++){
		insertAlphabetical(&sm, m->termName, m->sortInt);
		m = m->next;
	}
	
	//we have assigned pointers to their sorted version
	g = sg;
	e = se;
	h = sh;
	m = sm;
	
	//print output
	printf("\n STOP WORDS\n");
	printf("_________________\n");
	for(i = 0; i < 5; i++){
		printf("\n %-15s", g->termName);
		g = g->next;
	}
	
	printf("\n\n\n\n GENERAL WORDS\n");
	printf("__________________________________________________");
	printf("\n %-15s| %-15s| %-15s", "Economy", "Health", "Magazin");
	printf("\n________________|________________|________________");
	printf("\n %-15s| %-15s| %-15s", "", "", "");
	for(i = 0; i < 5; i++){
		printf("\n %-15s| %-15s| %-15s", e->termName, h->termName, m->termName);
		e = e->next;
		h = h->next;
		m = m->next;
	}
	printf("\n %-15s| %-15s| %-15s", "", "", "");
}

//This function inserts the nodes (which keep informations about words) in alphabetic order
int insertAllWords(mll ** header, char term[], char directory[], int id, int length){
	//q stands for wandering
	//r is place holder
	mll *p, *q, *r;
	//l stands for wandering
	//m is place holder
	ll *k, *l, *m;
	
	//create spaces for nodes and assign values
	p = malloc(sizeof(mll));
	k = malloc(sizeof(ll));
	
	strcpy(p->termName, term);
	p->next = NULL;
	p->occur = 0;
	
	k->documentID = id;
	k->occur = 0;
	k->length = length;
	
	//decide for category
	if(strcmp(directory, "dataset/econ/") == 0){
		strcpy(k->category, "economy");
	}
	else if(strcmp(directory, "dataset/health/") == 0){
		strcpy(k->category, "health");
	}
	else if(strcmp(directory, "dataset/magazin/") == 0){
		strcpy(k->category, "magazin");
	}
	
	k->next = NULL;
	
	//if MLL is empty, p will be the first node.
	if(*header == NULL){
		*header = p;
		p->llhdr = k;
		k->occur++;
		p->occur++;
	}
	else{	
		q = *header;
			
		//we're visiting the list until find the appropriate place by alphabetical order.
		while(q != NULL && strcmp(q->termName, p->termName) < 0){
			r = q;
			q = q->next;
		}
		
		//if we find the same word..
		if(q != NULL && strcmp(q->termName, p->termName) == 0){
			l = q->llhdr;
			
			//if it matches with first node of linked list..
			if( (strcmp(l->category, k->category) == 0) && (l->documentID == k->documentID) ){
				l->occur++;
				return 0;
			}
			//else check for other nodes
			else{
				//we're moving as long as we don't find another node with the same file information.
				while(l != NULL && !(strcmp(l->category, k->category) == 0 && l->documentID == k->documentID) ){
					m = l;
					l = l->next;
				} 
				
				//if this word is in the same file once more, we increase the number of occurence
				if(l != NULL && (strcmp(l->category, k->category) == 0) && (l->documentID == k->documentID) ){
					l->occur++;
					return 0;
				}
				
				//If we have passed the last node, we are adding a new node.
				if(l == NULL){
					q->occur++;
					k->occur++;
					m->next = k;
				}
				
				//If we're still on the first node, we're assigning the current node to the first node.
				if(l == q->llhdr){
					q->llhdr = k;
					k->occur++;
				}
			}
			return 0;
		}

		//We're putting the node in the right place.
		if(q != NULL){
			p->next = q;
			
		}
		
		if(q == *header){
			*header = p;
			p->llhdr = k;
			k->occur++;
			p->occur++;
		}
		else {
			r->next = p;
			p->llhdr = k;
			k->occur++;
			p->occur++;
		}
	}
}

//This function reads all files in the directory and calls insertAllWords function for each word.
void readFiles(char directory[]){
	char file[50];
	char word[50];
	int fileNumber = 1;
	int length = 0; //length of the document;
	
	snprintf(file, sizeof file, "%s%d.txt", directory, fileNumber);
	
	while(fopen(file, "r")){
		input = fopen(file, "r");
		
		//calculates the length of the document
		while(1){
			fscanf(input, "%s", word);
			length++;
			if(feof(input)){
				break;
			}
		}
		fclose(input);
		
		//opening the same file one more time to insert words
		input = fopen(file, "r");
		
		while(1){
			fscanf(input, "%s", word);
			insertAllWords(&hdr, word, directory, fileNumber, length);
			if(feof(input)){
				break;
			}
		}
		fclose(input);
		length = 0;	
		fileNumber++;
		memset(file, 0, sizeof file);
		snprintf(file, sizeof file, "%s%d.txt", directory, fileNumber);
	}
}

//This function calculates the occurence in each category for every word and calls insertAnalyseWord function.
void analyseWords(mll **header){
	mll *p;
	ll *m;	
	p = malloc(sizeof(mll));	
	p = *header;		
	char termName[50];
	int economy = 0, health = 0, magazin = 0;
	
	while(p != NULL){
		m = p->llhdr;	
		strcpy(termName, p->termName);
		
		while(m != NULL){
			
			if(strcmp(m->category, "economy") == 0){
				economy += m->occur;
			}
			else if(strcmp(m->category, "health") == 0){		
				health += m->occur;
			}
			else if(strcmp(m->category, "magazin") == 0){
				magazin += m->occur;
			}	
			
			m = m->next;
		}				
		
		//we added the terms with occurence of each category informations to another linked list.
		insertAnalyseWord(&ahdr, termName, economy, health, magazin);
	
		economy = 0;
		health = 0;
		magazin = 0;
		
		p = p->next;
	}	
	
}

//This function creates new linked list and inserts analyse words to it.
void insertAnalyseWord(analyse **header, char termName[], int economy, int health, int magazin){
	analyse *k, *l, *m;
	k = malloc(sizeof(analyse));
	
	strcpy(k->termName, termName);
	k->economy = economy;
	k->health = health;
	k->magazin = magazin;
	k->next = NULL;

		
	if(*header == NULL){
		*header = k;
	}
	else{
		l = *header;
		
		while(l != NULL){
			m = l;
			l = l->next;
		}
		if(l == *header){
			*header = k;
		}
		else{
			m->next = k;
		}
		
	}
}

//This functon calls insertNumericOrder function, so it provides to sort words by occurence
void sortWords(analyse **header){
	
	analyse *p;
	p = *header;
	
	int sortInt = 0;
	
	//Sort for stop words
	while(p != NULL){
		sortInt = p->economy + p->health + p->magazin;
		insertNumericOrder(&general, p->termName, sortInt);
		sortInt = 0;
		p = p->next;
	}
	
	
	
	p = *header;
	
	//Sort for category -> economy 
	while(p != NULL){
		sortInt = p->economy;
		//if this word occurs only economy...
		if(p->health == 0 && p->magazin == 0){
			//insert the linked list whose header pointer's name is economy
			insertNumericOrder(&economy, p->termName, sortInt);
		}
		sortInt = 0;
		p = p->next;
	}
	
	p = *header;
	
	//Sort for category -> health
	while(p != NULL){
		sortInt = p->health;
		if(p->economy == 0 && p->magazin == 0){
			insertNumericOrder(&health, p->termName, sortInt);
		}
		sortInt = 0;
		p = p->next;
	}
	
	p = *header;
	
	//Sort for category -> magazin
	while(p != NULL){
		sortInt = p->magazin;
		if(p->economy == 0 && p->health == 0){
			insertNumericOrder(&magazin, p->termName, sortInt);
		}
		sortInt = 0;
		p = p->next;
	}
	
}

//inserts the node to the linked list comparing the given number information sortInt
void insertNumericOrder(sort **header, char termName[50], int sortInt){
	sort *p, *q, *r;
	
	p = malloc(sizeof(sort));
	
	strcpy(p->termName, termName);
	p->sortInt = sortInt;
	p->next = NULL;
	
	if (*header == NULL){
   		*header=p;
    } 
    else{
    	
    	q = *header;
    	
    	while (q != NULL && q->sortInt >= p->sortInt) {
    		r=q;
	   		q=q->next;
	 	}
	 	
	 	if (q != NULL){
	 		p->next=q;
	 	} 
	 	
	 	if (q == *header){
     		*header=p;
	 	}
	 	else r->next=p;
	}
}

//inserts the node to the linked list alphabetical order
//sortInt parameter is not necessary, I just does not want to lose the sortInt information
void insertAlphabetical(sort **header, char termName[50], int sortInt){ 
	sort *p, *q, *r;
	
	p = malloc(sizeof(sort));
	
	strcpy(p->termName, termName);
	p->sortInt = sortInt;
	p->next = NULL;
	
	if(*header == NULL){
		*header = p;
	}
	else{	
		q = *header;
			
		while(q != NULL && strcmp(q->termName, p->termName) < 0){
			r = q;
			q = q->next;
		}
		
		if(q != NULL){
			p->next = q;
		}
		
		if(q == *header){
			*header = p;
		}
		else {
			r->next = p;
		}	
	}
	return;
}
