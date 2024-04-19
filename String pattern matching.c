//Akinola Adebowale 2545887
/*This assignment aims to help you practice string processing and string matching. Your main task
in this assignment is to develop a simple application for pattern detection in DNA sequences using C
programming language.
Overview
DNA, which stands for deoxyribonucleic acid, is the genetic material found in humans and nearly all
living organisms (see Figure 1). Almost every cell in the human body contains the same DNA. The
information stored in DNA is represented by a code consisting of four chemical bases: Adenine (A),
Guanine (G), Cytosine (C), and Thymine (T).
Figure 1: DNA1
In this assignment, you will need to develop a simple application that reads the DNA sequences in a
given text file, asks the user to enter the patterns, and then lists how many times each pattern
appears in how many sequences.

A sample run is provided below:
Enter the file path containing the DNA sequences: DNAsequences.txt
Enter the number of patterns: 4
Enter the pattern: TGGAT
Enter the pattern: GATG
Enter the pattern: TACCACAAA
Enter the pattern: GTAG
GATG is detected 10 times in 7 DNA sequences
GTAG is detected 5 times in 5 DNA sequences
TGGAT is detected 1 time in 1 DNA sequence
TACCACAAA is detected 0 time in 0 DNA sequence
Note: Please note that these patterns are sorted based on the number times they appear (GATG 10
times, GTAG 5 times, TGGAT 1 time, and TACCACAAA 0 time).


Implementation Requirements:
This application will have a linked list for DNA sequences. Each node in a linked list will keep one
DNA sequence, so it should be created based on the following data structure. When you create your
linked list, do not use a dummy node.
struct node{
char sequence[81];
struct node *next;
};
This application will also have an array of data structures for patterns. Each pattern will be
represented with following data structure. The size of the array will be determined based on the
user input (please see the sample run above)
struct pattern{
char p[81];
int times; //The number of times the pattern appears
int sequences; //The number of sequences the pattern appears.
};
In this assignment, you are given a C file called "template4.c" attached. This will be the template
code which you need to strictly follow. Inside the "template4.c" file, all function prototypes are
declared. You will basically need to fill in the blanks and write the code for the following functions:
In addition to the main function, you will also need to implement the following functions and call
them appropriately in the main function.
• readSequences: This function takes the file path, reads the sequences in the file where
each line shows a different DNA sequence (maximum length can be 80), creates a linked
list and stores these sequences in the linked list. It will then return the linked list of DNA
sequences.
• searchPatterns: This function takes the linked of DNA sequences and the array of
patterns, and then uses the Rabin-Karp Algorithm where d=4 (since there are 4 four
chemical bases) and q=11 to calculate how many times each pattern appears and how
many sequences includes each pattern and store these statistics in the array of patterns.
The general pseudo-code of the Rabin-Karp Algorithm is given below.
RK(T, P, d, q)
    n := length[T];
    m := length[P];
    h := dm-1 mod q;
    p := 0;
    t0 := 0;
    for i := 1 to m do
        p := (dp + P[i]) mod q;
        t0 := (dt0 + T[i]) mod q
    for s := 0 to n – m do
        if p = ts then
            if P[1..m] = T[s+1..s+m] then
                print “pattern occurs with shift s”
        if s < n-m then
            ts+1 := (d(ts – T[s+1]h) + T[s+m+1]) mod q

• sortPatterns: This function takes the array of patterns and sort them according to the
number of times they appear by using the Insertion Sort in descending order.
• printPatterns: This function takes the array of patterns and prints them in the format as shown in
the sample run.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node{
   char sequence[81];
   struct node *next;
};

struct pattern{
    char p[81];
    int times;
    int sequences;
};

struct node * readSequences(char filename[]);
void searchPatterns(struct node *head, struct pattern *patterns, int numberOfPatterns);
void sortPatterns(struct pattern *patterns, int numberOfPatterns);
void printPatterns(struct pattern *patterns, int numberOfPatterns);
void RabinKarp(char [],struct pattern *pattern,int,int);

int main(int argc, char *argv[]){
    struct node * head = NULL;
    struct pattern *patterns = NULL;
    char file[50];
    int pattern_NO;



    printf("\nEnter the file path containing the sequence : ");
    gets(file);

    head=readSequences(file);

    fflush(stdin);
    printf("\nEnter the number of patterns :");
    scanf("%d",&pattern_NO);

    patterns=(struct pattern*)malloc(sizeof (struct pattern) *pattern_NO );
    if(patterns==NULL){
        printf("\nFailed to allocate memory");
        exit(1);
    }

    searchPatterns(head,patterns,pattern_NO);

    fflush(stdin);

    sortPatterns(patterns,pattern_NO);
    printf("\n======================================");
    printPatterns(patterns,pattern_NO);
    // WRITE YOUR CODE HERE

    return 0;
}

struct node * readSequences(char filename[]){
	struct node * head = NULL;
    struct node * temp;
    struct node * temp_last;


    char buffer[100];

    FILE *infile;
    infile=fopen(filename,"r");
    if(infile==NULL){
        printf("\ncannot get to file");
        exit(1);
    }

    while(fgets(buffer,100,infile)){

        temp=(struct node *)malloc(sizeof(struct node));//Create a temporary node to store each sequence copied from the file
        strcpy(temp->sequence,buffer);
        temp->next=NULL;//Set it's next to NULL;

        if(head == NULL){//We check if head is null, then we assign it to the temporary node

            head=temp;
            head->next=NULL;

        }
        else{

            temp_last=head;//If head is not empty we use a pointer to traverse the linked list to get to the end of the list
                            //Then we insert the temporary node to the end of the list
            while(temp_last->next!=NULL){

                temp_last=temp_last->next;
            }
            temp_last->next =temp;

        }

    }
    fclose(infile);

	// WRITE YOUR CODE HERE

	return head;
}

void searchPatterns(struct node *head, struct pattern *patterns, int numberOfPatterns){
    int i,d=4,q=11;
    struct node *temp;

    fflush(stdin);
    for(i=0;i<numberOfPatterns;i++){
        printf("\nEnter the pattern: ");
        gets(patterns[i].p); //Initializing the array structure
        patterns[i].sequences=0;
        patterns[i].times=0;
    }
    fflush(stdin);
    temp=head;
    for(i=0;i<numberOfPatterns;i++){//Here i will send the patterns one after the other
        while (temp != NULL) {//I will send the linked list  and the pattern to the rabin karp function
            RabinKarp(temp->sequence, &patterns[i], d, q);
            temp=temp->next;
            fflush(stdin);
        }
        temp=head;


    }

	// WRITE YOUR CODE HERE

}

void sortPatterns(struct pattern *patterns, int numberOfPatterns){

    int i,j;
    struct pattern key;//Using a structure to store the key value
    for ( i = 1; i < numberOfPatterns; i++) {

        key = patterns[i]; //Stores the value
         j = i - 1;

        while (j >= 0 && patterns[j].times < key.times) {//Here we use the loop to sort the array in descending order
            patterns[j + 1] = patterns[j];
            j = j - 1;
        }

        patterns[j + 1] = key;
    }

	// WRITE YOUR CODE HERE

}

void printPatterns(struct pattern *patterns, int numberOfPatterns){

    int i;
    for(i=0;i<numberOfPatterns;i++){

        printf("\n%s is detected %d times in  %d sequences",patterns[i].p,patterns[i].times,patterns[i].sequences);
    }
	// WRITE YOUR CODE HERE

}
void RabinKarp(char text[],struct pattern *P,int d,int q){

    int n = strlen(text);
    int m = strlen(P->p);
    int h = 1,i,s,j;
    int p = 0,t0 = 0;
    int found = 0;

    for ( i = 0; i < m - 1; i++) {//Loop for calculating the the value h
        h = (h * d) % q;
    }

    for ( i = 0; i < m; i++) { //calculating the spurious hits
        p = (d * p + P->p[i]) % q;
        t0 = (d * t0 + text[i]) % q;
    }

    for ( s = 0; s <= n - m; s++) {//loop for calculating the Ts values and comparing each character from the pattern and text
        if (p == t0) {

            for (j = 0; j < m; j++) {
                if (P->p[j] != text[s + j]) {
                    break;
                }
            }
            if (j == m) {
                //Number of times of that pattern is updated when the pattern is found in the sequence
                P->times++;
                found=1;
            }
        }
        if (s < n - m) {
            t0 = (d * (t0 - text[s] * h) + text[s + m]) % q;
            if (t0 < 0) {
                t0 = (t0 + q);
            }
        }
    }
    if(found!=0){//The sequence is updated if a pattern have been found in that sequence
        P->sequences++;
    }

}