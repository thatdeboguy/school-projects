/*Adebowale Akinola 2545887*/

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
void heapify(customer*, int, int, int);
void heapSort(customer*, int,int);
/****************************/

//Helper functions
void build_heap(customer*, int,int);
void swap( customer*,customer*);

//DO NOT CHANGE THE main FUNCTION
int main() {
    FILE *inFile;
    int no_of_customers = 0;
    customer *customers;

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

    char command = 0;
    int exit = 0;
    while (exit != 1) {
        menu();
        printf("\nCommand: ");
        fflush(stdin);
        scanf("%c", &command);

        if (command == '1' || command == '2' || command == '3' || command == '4') {
            heapSort(customers, no_of_customers, command - '0');
            printCustomers(customers, no_of_customers);
        }
        else if (command == '5'){
            exit = 1;
            printf("Goodbye!\n");
        }
        else{
            printf("Please enter a valid command!\n");
        }
    }
    free(customers);
    return 0;
}

//DO NOT CHANGE THE menu FUNCTION
void menu () {
    printf("Please choose one of the following options:\n"
           "(1) Display customers sorted by number of transactions\n"
           "(2) Display customers sorted by number of items purchased\n"
           "(3) Display customers sorted by total amount paid\n"
           "(4) Display customers sorted by average amount paid per transaction\n"
           "(5) Exit\n");
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

int countCustomers (FILE *inFile){
    //WRITE YOUR CODE HERE

    char buffer[100];
    int count=0,found=0,i=0,j;
    char name[81];
    char customers_name[80][81];

    while(fgets(buffer,100,inFile)){
    	
    	//This code skips the first line in the file.
    	if(i>=1){

    		sscanf(buffer,"%[^;]",name);  //Read file from file till the semicolon.
			
    		for(j=0;j<count;j++){    //Checks if we have encountered the name in the customers array

    			if(strcmp(name,customers_name[j])==0){
    				found=1;
    				break;
				}
			}
						
			if(found==0){
				strcpy(customers_name[j],name);//Copies the name checked to the customers array, if it was not there
				count++;
				
			}
		}

		if(found==1){
			found=0;
		}

		i++;

	}
	return count;

}

void readTransactions (FILE* inFile, customer* customers, int no_of_customers) {

    //WRITE YOUR CODE HERE
	char buffer[100];
	int count=0,found=0,i=0,j,x=0;
	char temp_name[81];
	int temp_itemspurchased;
	float temp_costperitem;
	
    for ( i = 0; i < no_of_customers; i++) {
        strcpy(customers[i].name, "NULL");
        customers[i].transactions = 0;
        customers[i].items_purchased = 0;
        customers[i].amount_paid = 0;
    }
    i=0;
	while(fgets(buffer,100,inFile)){
			
		
    		if(i>=1){//skips the first line
    		
    		sscanf(buffer,"%[^;];%*[^;];%*[^;];%*[^;];%d;%f;",temp_name, &temp_itemspurchased, &temp_costperitem);
						
    		for(j=0;j<count;j++){    //Checks if we have encountered the name in the customers array
				
    			if(strcmp(temp_name,customers[j].name)==0){
    				
    				customers[j].amount_paid+= temp_itemspurchased * temp_costperitem;
    				customers[j].transactions++;//This is correct as it is.  
					customers[j].items_purchased +=  temp_itemspurchased;//Increments the items purchased after converting string to integer.    				
    				found=1;
    				break;
				}
			}
			
			if(found==0){ //Copy the details of each line in the file to our structure.
			
				strcpy(customers[count].name,temp_name);//Copies the name checked to the customers array, if it was not there				
                customers[count].amount_paid+=temp_itemspurchased * temp_costperitem;
				customers[count].transactions=1; //Increments the number of transactions.
				customers[count].items_purchased +=  temp_itemspurchased; //Increments the items purchased after converting string to integer.				
				count++;

			}
		}
			if(found==1){
				found=0;
			}
		i++;
	}
	
}

void swap(customer *x ,customer *y){
	//These swap the pointers of the structures.
	customer z;
	z=*y;
	*y=*x;
	*x=z;
	
	
}
void heapify(customer* customers, int no_of_customers, int i, int criteria) {

    //WRITE YOUR CODE HERE
    int largest=i;			//Initializing them this way because our indexing starts from 0.
    int left_node= 2*i +1;
    int right_node=2*i + 2;    
	
	
	if(criteria==1){
		
		if(left_node<no_of_customers && customers[left_node].transactions > customers[largest].transactions){
			
			largest = left_node;
		}

		if(right_node<no_of_customers && customers[right_node].transactions > customers[largest].transactions){
			
			largest=right_node;
		}
		if(largest!=i){
			swap(&customers[i],&customers[largest]);
			heapify(customers,no_of_customers,largest,criteria);
		}	
	}
	if(criteria==2){	
	
		if(left_node<no_of_customers && customers[left_node].items_purchased > customers[largest].items_purchased){
			largest = left_node;
		}

		if(right_node<no_of_customers && customers[right_node].items_purchased > customers[largest].items_purchased){
			largest=right_node;
		}
		if(largest!=i){
			swap(&customers[i],&customers[largest]);
			heapify(customers,no_of_customers,largest,criteria);
		}
	}
	if(criteria==3){
		
		if(left_node<no_of_customers && customers[left_node].amount_paid > customers[largest].amount_paid){
			largest = left_node;
		}

		if(right_node<no_of_customers && customers[right_node].amount_paid > customers[largest].amount_paid){
			largest=right_node;
		}
		if(largest!=i){
			swap(&customers[i],&customers[largest]);
			heapify(customers,no_of_customers,largest,criteria);
		}
	}
	if(criteria==4){
		
		if(left_node<no_of_customers && customers[left_node].amount_paid/customers[left_node].transactions > customers[largest].amount_paid/customers[largest].transactions){
			
			largest = left_node;
		}

		if(right_node<no_of_customers && customers[right_node].amount_paid/customers[right_node].transactions > customers[largest].amount_paid/customers[largest].transactions){
			
			largest=right_node;
		}
		if(largest!=i){
			swap(&customers[i],&customers[largest]);
			heapify(customers,no_of_customers,largest,criteria);
		}	
	}
	
 	
}
void build_heap(customer* customers,int no_of_customers, int criteria){
	
	int i=(no_of_customers-2)/2;
	
	while(i>=0){
		
		heapify(customers,no_of_customers,i,criteria);
		i--;
	}
}
void heapSort(customer* customers, int no_of_customers, int criteria) {
	
	//WRITE YOUR CODE HERE
	int i;
	
	build_heap(customers,no_of_customers,criteria);
	
	for(i=no_of_customers-1;i>0;i--){
		
		swap(&customers[i],&customers[0]);
		
		heapify(customers,i,0,criteria);
	}
}