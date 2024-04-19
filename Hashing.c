#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//DO NOT CHANGE DATA STRUCTURE
struct Customer {
    char name[81];
    int transactions;
    int items_purchased;
    float amount_paid;
};

typedef struct Customer customer;

//DO NOT CHANGE FUNCTION PROTOTYPES
void menu();
void printCustomers(customer *, int);
int countCustomers(FILE*);
void readTransactions(FILE*, customer* ,int);
customer* createHashTable();
customer* addCustomer(customer*, customer, int, int*, int);
customer* rehash(customer*, int*, int);
void printTable(customer*, int);
void searchTable(customer*, int, char[], int);
int hashfunction(customer,int*,int );
int hash_key(int,int*,int );
//DO NOT CHANGE THE main FUNCTION
int main() {

    FILE *inFile;
    int no_of_customers = 0;
    customer *customers;
    customer *hashTable;

    inFile = fopen("transactions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }
    no_of_customers = countCustomers(inFile);
    printf("There are %d unique customers\n", no_of_customers);

    customers = (customer*) malloc(no_of_customers * sizeof (customer));
    fclose(inFile);
    inFile = fopen("transactions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }

    readTransactions(inFile, customers, no_of_customers);
    if (customers == NULL) {
        printf("\nCustomers have NOT been read successfully!\n");
        exit(1);
    }
    printCustomers(customers, no_of_customers);
    //Where i start from

    hashTable = createHashTable();
    int hashTableSize = 11;
    char criteria;

    printf("Enter your hashing criteria: ");
    fflush(stdin);
    scanf("%c", &criteria);

    while (criteria != '1' && criteria != '2' && criteria != '3'){
        printf("Error! Please enter a valid hashing criteria: ");
        fflush(stdin);
        scanf("%c", &criteria);
    }
    fflush(stdin);

    int i;
    for (i = 0; i < no_of_customers; i++){
        hashTable = addCustomer(hashTable, customers[i], i, &hashTableSize, criteria - '0');
        printf("%s has been added to the table, the hash table now can be seen below:", customers[i].name);
        printTable(hashTable, hashTableSize);
    }

    char command = 0;
    int exit = 0;
    char searchName[81];
    while (exit != 1) {
        menu();
        printf("\nCommand: ");
        fflush(stdin);
        scanf("%c", &command);

        if (command == '1') {
            printf("Enter the customer's name: ");
            fflush(stdin);
            scanf("%s", searchName);
            searchTable(hashTable, hashTableSize, searchName, criteria - '0');
        }
        else if (command == '2'){
            printTable(hashTable, hashTableSize);
        }
        else if (command == '3'){
            exit = 1;
            printf("Goodbye!\n");
        }
        else{
            printf("Please enter a valid command!\n");
        }
    }
    free(customers);
    free(hashTable);
    return 0;
}

//DO NOT CHANGE THE menu FUNCTION
void menu () {
    printf("Please choose one of the following options:\n"
           "(1) Search a customer\n"
           "(2) Display hashtable\n"
           "(3) Exit\n");
}

//DO NOT CHANGE THE printCustomers FUNCTION
void printCustomers (customer* customers, int no_of_customers) {

    printf("List of customers:\n");
    int i;
    for (i = 0; i < no_of_customers; ++i) {
        printf("Name: %s, number of transactions = %d, number of items = %d, total amount paid = %.2f,"
               " average amount paid per transaction = %.2f\n",
               customers[i].name, customers[i].transactions, customers[i].items_purchased, customers[i].amount_paid,
               customers[i].amount_paid/customers[i].transactions);
    }

}

//DO NOT CHANGE THE countCustomers FUNCTION
int countCustomers (FILE *inFile){
    int no_of_customers = 0;
    char **names;
    char **temp;
    char userName[100];
    int transactionId;
    char transactionTime[100];
    char itemDescription[100];
    int numberOfItemsPurchased;
    float costPerItem;
    char country[100];
    int i, exists = 0;
    char c;
    while ((c = fgetc(inFile)) != EOF) {
        if (c == '\n') {
            break;
        }
    }
    fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
           userName, &transactionId, transactionTime,  itemDescription,
           &numberOfItemsPurchased, &costPerItem, country);
    no_of_customers++;
    names = (char**) malloc (sizeof(char*) * (no_of_customers));
    names[no_of_customers-1] = (char*) malloc(100 * sizeof (char));
    strcpy(names[no_of_customers - 1], userName);

    while (fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
                  userName, &transactionId, transactionTime, itemDescription,
                  &numberOfItemsPurchased, &costPerItem, country) != EOF) {
        exists = 0;
        for ( i = 0; i < no_of_customers; ++i) {
            if (strncmp(names[i], userName, strlen(userName)) == 0){
                exists = 1;
                break;
            }
        }
        if (exists)
            continue;
        temp = (char**) malloc (sizeof(char*) * (no_of_customers));
        for ( i = 0; i < no_of_customers; ++i) {
            temp[i] = (char*) malloc(100 * sizeof (char));
            strcpy(temp[i], names[i]);
        }
        free(names);

        names = (char**) malloc (sizeof(char*) * (no_of_customers + 1));
        for ( i = 0; i < no_of_customers; ++i) {
            names[i] = (char*) malloc(100 * sizeof (char));
            strcpy(names[i], temp[i]);
        }
        free(temp);
        names[no_of_customers] = (char*) malloc(100 * sizeof (char));
        strcpy(names[no_of_customers], userName);
        no_of_customers++;

    }
    return no_of_customers;
}

//DO NOT CHANGE THE readTransactions FUNCTION
void readTransactions (FILE* inFile, customer* customers, int no_of_customers) {
    char userName[100];
    int transactionId;
    char transactionTime[100];
    char itemDescription[100];
    int numberOfItemsPurchased;
    float costPerItem;
    char country[100];
    int i,exists = 0;
    int filled = 0;
    char c;

    for ( i = 0; i < no_of_customers; ++i) {
        strcpy(customers[i].name, "unassigned");
        customers[i].amount_paid = 0.0;
        customers[i].items_purchased = 0;
        customers[i].transactions = 0;
    }

    while ((c = fgetc(inFile)) != EOF) {
        if (c == '\n') {
            break;
        }
    }

    while (fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
                  userName, &transactionId, transactionTime, itemDescription,
                  &numberOfItemsPurchased, &costPerItem, country) != EOF) {
        exists = 0; // n
        int i; // n
        for (i = 0; i < no_of_customers; ++i) {
            if(strcmp(userName, customers[i].name) == 0){
                exists = 1;
                break;
            }
        }
        if (exists){
            customers[i].transactions ++;
            customers[i].items_purchased += numberOfItemsPurchased;
            customers[i].amount_paid += (numberOfItemsPurchased * costPerItem);
        }
        else{
            strcpy(customers[filled].name, userName);
            customers[filled].transactions ++;
            customers[filled].items_purchased += numberOfItemsPurchased;
            customers[filled].amount_paid += (numberOfItemsPurchased * costPerItem);
            filled++;
        }
    }
}

customer* createHashTable(){

    int i;
    customer *hashTable;
    //Creates an hashtable of size 11 and checks if it is created successfully
    hashTable = (customer*)malloc(11 * sizeof (customer));
    if(hashTable==NULL){
        printf("Unable to allocate memory!");
    }
    //Initializes the array
    for(i=0;i<11;i++){
        strcpy(hashTable[i].name,"Unassigned");
        hashTable[i].amount_paid = 0.0;
        hashTable[i].transactions = 0;
        hashTable[i].items_purchased = 0;

    }
    //Returns the hashtable
    return hashTable;

    // WRITE YOUR CODE HERE

}

customer* addCustomer(customer* hashTable, customer addedCustomer, int hashTableCapacity, int *hashTableSize, int criteria){

    float load_factor;
    int i=0,key,length,j,initialtablesize;
    //This calculates the length of the customer name
    length = strlen(addedCustomer.name);
    initialtablesize = *hashTableSize;

    // We assume all the names start with a Upper case letter!,The '-32' changes the Ascii code value of the lower case to the value of the uppercase
    key = ((addedCustomer.name[length - 1] - 32) - (addedCustomer.name[0])); //Check the ASCII values of the first and last character of the customer name if they are not equal.
    if (key <0){
        key = key / -1;//If it is a negative number it converts to positive
    }
    hashTableCapacity++;//Increments in order to get the appropriate load factor

    load_factor = (float) hashTableCapacity  /  (*hashTableSize);//Calculates the load factor
    fflush(stdin);

    if (load_factor > 0.50){
        fflush(stdin);
        //If load factor is greater than zero it sends it to the rehash function for rehashing
        hashTable = rehash(hashTable,hashTableSize,criteria);
    }
    //This if statement ensures that after rehashing the value is still stored and it does not leave the function. That is why it is not an else statement.
    //initialtablesize stores the size of the previous hashtable size.
    if(load_factor < 0.50 || *hashTableSize > initialtablesize){

        if (criteria == 1) {

            fflush(stdin);

            key = key % *hashTableSize; //This calculate the hash(key) function
            int temp_key = key; //Temp key is created to maintain the hash(key) function, for future use
            fflush(stdin);
            j = key;//The customer value is needed to be stored in the exact position so the array element is our starting index

             while(j < *hashTableSize){//This loops till we get to our destination in the array structure

                    if (strcmp(hashTable[j].name, "Unassigned") != 0) {//This condition checks if there is a customer in its key array.
                        i++;
                        //if there is we would increment by 1.
                        key = ((temp_key + i) % *hashTableSize);
                        j=key;
                    }
                    else {//If there isn't we put it in the array! and break;
                        strcpy(hashTable[j].name, addedCustomer.name);
                        hashTable[j].transactions = addedCustomer.transactions;
                        hashTable[j].amount_paid = addedCustomer.amount_paid;
                        hashTable[j].items_purchased = addedCustomer.items_purchased;
                        break;
                    }

            }

        }
        else if(criteria == 2){//Quadratic probing

            key = key % *hashTableSize;
            int temp_key = key; //Temp key is created to maintain the hash(key) function
            j = key;
            while(j < *hashTableSize){

                //if(j == key){
                if(strcmp(hashTable[j].name, "Unassigned") != 0){//This condition checks if there is a customer in its key array.
                    i++;
                    //Uses quadratic probing with the formula : key = (value % tablesize + f(i square))
                    key = ((temp_key + (i * i)) % *hashTableSize);
                    j=key;
                }
                else{//If it is empty we allocate that array element to that customer
                    strcpy(hashTable[j].name, addedCustomer.name);
                    hashTable[j].transactions = addedCustomer.transactions;
                    hashTable[j].amount_paid = addedCustomer.amount_paid;
                    hashTable[j].items_purchased = addedCustomer.items_purchased;
                    break;
                }

            }
        }
        else if (criteria == 3){//Double Hashing

            key = key % *hashTableSize;
            int temp_key = key; //Temp key is created to maintain the hash(key) function

            j = key;
            while(j < *hashTableSize){

                //if(j == key){
                    if(strcmp(hashTable[j].name, "Unassigned") != 0){//This condition checks if there is a customer in its key array.
                        i++;
                        //Uses double hashing with the formula : key = (value % tablesize + f(i))
                        key = (temp_key + (i * (7 - (temp_key % 7)))) % *hashTableSize;
                        j=key;             //here indicates the f(i) given in the assignment.
                    }
                    else{//If it is empty we allocate that array element to that customer
                        strcpy(hashTable[j].name, addedCustomer.name);
                        hashTable[j].transactions = addedCustomer.transactions;
                        hashTable[j].amount_paid = addedCustomer.amount_paid;
                        hashTable[j].items_purchased = addedCustomer.items_purchased;
                        break;
                    }

            }
        }
    }

    return hashTable;
    // WRITE YOUR CODE HERE

}

customer* rehash(customer* hashTable, int *hashTableSize, int criteria){
    int i,j,k;
    customer* new_hashtable;

    int oldtablesize = *hashTableSize;//Store my previous hashtable for future purpose

    (*hashTableSize) = (*hashTableSize) * 2; //Doubles it to  get the new hashtable size

    for ( k = 2; k * k <= (*hashTableSize); k++) {//This loops check if our new hashtablesize is a prime number
        if ((*hashTableSize) % k == 0) {
            (*hashTableSize) +=1;  // number is divisible by k, hence not a prime number
        }
    }

    new_hashtable = (customer*)malloc ((*hashTableSize) * sizeof (customer));

    fflush(stdin);

    if(new_hashtable==NULL){//Checks if the new hashtable was created successfully or not!
        printf("\nUnable to allocate memory for new hashtable");
        exit(1);
    }
    //This initializes the values of the new hashtable to 0 and "Unassigned"
    for(i=0;i<*hashTableSize;i++){
        strcpy(new_hashtable[i].name,"Unassigned");
        new_hashtable[i].amount_paid = 0.0;
        new_hashtable[i].transactions = 0;
        new_hashtable[i].items_purchased = 0;

    }

    fflush(stdin);
    for(j=0;j < oldtablesize;j++){
        //This loop takes the new hashtable and sends it to the addcustomer function and then rehashes the the values from the old hashtable to the new one.
        //With the size of the previous hashtable.
        new_hashtable = addCustomer(new_hashtable,hashTable[j], j, hashTableSize, criteria );
    }

    free(hashTable);//Freeing up the hashtable!
    //After rehashing from the old hashtable it returns the new hashtable to the add customer
    return new_hashtable;

    // WRITE YOUR CODE HERE

}

void printTable(customer *hashTable, int hashTableSize){
    int i;
    // WRITE YOUR CODE HERE
    printf("\n****************************************************************");
    printf("\nIndex\tname\tTransactions\tItems\tPaid_amount");
    for(i=0;i<hashTableSize;i++){

        printf("\n%d",i);
        if(strcmp(hashTable[i].name,"Unassigned")!=0){
            printf("\t%s\t\t%d\t%d\t%.2f",hashTable[i].name,hashTable[i].transactions,hashTable[i].items_purchased,hashTable[i].amount_paid);
        }

    }
    printf("\n****************************************************************\n");

}

void searchTable(customer *hashTable, int hashTableSize, char searchName[], int criteria){

   int i=0,j,found=0,length = strlen(searchName);
   int count;

    //Here we get the key!
    int key = ((searchName[length - 1] - 32) - (searchName[0]));
    //The idea is to use the key to find the customer  we are looking for.
    //Since we used the key to arrange them in the structure we would also use it to find them.

    if(criteria == 1){ //Linear probing!

            key = key % hashTableSize;

            fflush(stdin);
            //This loop starts from exactly the key index and uses linear search after that!
            for (j = key; j < hashTableSize; j++) {

                    if (strcmp(hashTable[j].name,searchName) == 0)  {

                        printf("\nInformation for customer %s :", searchName);
                        printf("\n\tNumber of transactions = %d",hashTable[j].transactions);
                        printf("\n\tItems purchased = %d",hashTable[j].items_purchased);
                        printf("\n\tTotal Amount Paid = %.2f\n",hashTable[j].amount_paid);
                        found=1;
                        break;//We break assuming the customer names are unique
                    }

            }
            if(found==0){//If an element is not found then it prints the error message
                printf("\nUser does not exist in the table!\n ");
            }
    }
    else if(criteria == 2){ //Quadratic probing

        key = key % hashTableSize;
        int temp_key = key; //Temp key is created to maintain the hash(key) function
        //for(k = key; k < hashTableSize; k++)
        j = key;
        count = j;
        while(j < hashTableSize){

                fflush(stdin);
                if(strcmp(hashTable[j].name, searchName)==0) {

                    printf("Information for customer %s :", searchName);
                    printf("\n\tNumber of transactions = %d",hashTable[j].transactions);
                    printf("\n\tItems purchased = %d",hashTable[j].items_purchased);
                    printf("\n\tTotal Amount Paid = %.2f\n",hashTable[j].amount_paid);
                    found =1;
                    break;//We break assuming the customer names are unique
                }
                else{
                    i++;
                    key = (temp_key + (i * i)) % hashTableSize;
                    j = key;
                }
                if(count==j){//This if statement checks if the an element has been checked twice!
                    break;
                }
        }
        if(found==0){//If an element is not found then it prints the error message
            printf("User does not exist in the table!\n");
        }
    }
    else {
        key = key % hashTableSize;
        int temp_key = key; //Temp key is created to maintain the hash(key) function
        j = key;
        count = j;
        while(j < hashTableSize){

            fflush(stdin);
            if(strcmp(hashTable[j].name, searchName)==0) {

                printf("Information for customer %s :", searchName);
                printf("\n\tNumber of transactions = %d",hashTable[j].transactions);
                printf("\n\tItems purchased = %d",hashTable[j].items_purchased);
                printf("\n\tTotal Amount Paid = %.2f\n",hashTable[j].amount_paid);
                found =1;
                break;//We break assuming the customer names are unique
            }
            else{
                i++;
                key = (temp_key + (i * (7 - (temp_key % 7)))) % hashTableSize;
                j = key;
            }
            if(count==j){//This condition checks if an array element has been checked twice!.
                //If it has then it has checked all possible locations for that customer, it breaks out of the loop.
                break;
            }
        }
        if(found==0){//If an element is not found then it prints the error message
            printf("User does not exist in the table!\n");
        }
    }
    // WRITE YOUR CODE HERE

}
