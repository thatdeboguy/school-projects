	///Adebowale/Akinola 2545887
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct StudentTable{
	char name[50];
	char surname[50];
	float lab;
	float Assignment;
	float Midterm;
	float Final;
	double Total;
};
typedef struct StudentTable Student_T;

/*This function counts the number of lines in the array and return the integer!*/
int row_count(FILE *incount,char *filename){

    int count;
    char point[100];
    incount=fopen(filename,"r");
    if(incount==NULL){
            //Opens the stream to read the file and report if it fails to open.
        printf("could not reach file:");
        exit(1);
    }
    //Read the file line by line till it gets to no line and counts after reading the line.
    while(fgets(point,100,incount)){

            count++;
    }
    fclose(incount);
//Returns the value of count after every increament.
    return count;
}

/*This functions loads the details of the file read and populate them into an array*/
Student_T *load_studentTable(Student_T *studentTable,FILE *infile,char *User_filename,int *Lines){
    int i;
    infile=fopen(User_filename,"r");//reads the file from and checks if its accessible
    if(infile==NULL){
        printf("couldn't open file");
        exit(1);
    }
	//This creates the size of the array and checks if it can be allocated
   	studentTable=(Student_T*)malloc(sizeof(Student_T)* (*Lines));
	if(studentTable == NULL){
        printf("Failed to allocate memory");
        exit(1);
	}
	/*This scans the information fom the file and into the structure array!*/
   for(i=0;i<*Lines;i++){
    fscanf(infile,"%s %s %f %f %f %f",studentTable[i].name,studentTable[i].surname,&studentTable[i].lab,&studentTable[i].Assignment,&studentTable[i].Midterm,&studentTable[i].Final);
   }
    fclose(infile);
    /*Printing the information from the array to the user!*/
    for(i=0;i<*Lines;i++){
    studentTable[i].Total=(studentTable[i].lab)+(studentTable[i].Assignment)+(0.3 *(studentTable[i].Midterm))+(0.35 *(studentTable[i].Final));
   }
   printf("Student records file (%s) has been successfully loaded!",User_filename);
   /*Returning the array to the main function*/
    return studentTable;
}

/*This functions displays the array of student table. It will be used to display the array structure in the code.*/
void Display_StudentTable(Student_T *studentTable,int lines){
    int i;

    printf("\nName\t\tSurname\t\tLab\tAssignment\tMidterm\t\tFinal\tTotal\n");

    for(i=0;i<lines;i++){
        printf("%-8s\t%-8s\t%.2f\t%.2f\t\t%.2f\t\t%.2f\t%.2lf\t\t\n",studentTable[i].name,studentTable[i].surname,studentTable[i].lab,studentTable[i].Assignment,studentTable[i].Midterm,studentTable[i].Final,studentTable[i].Total);
    }

}

/*This function is used to search for information the uer wishes to find*/
int search_studentTable(Student_T *studentTable,int Lines,char *searchname){
    int i,found=0;
    char tmap1_name[50];//This is a temporary array for the variable stored in order to check and compare the name input by the user and the name in the array!
    //A loop to scan through the array of structures
    for(i=0;i<Lines && !found;i++){
		//This statement is check if the name input coresponds with the name in the structured array.if so it returns the position in the array.
         if(strcmp(studentTable[i].name,searchname)==0){
                found=1;
            return i;
        }
        //This statement is check if the surname input coresponds with the name in the structured array.if so it returns the position in the array.
        if(strcmp(studentTable[i].surname,searchname)==0){
        	    found=1;
            return i;
		}
		strcpy(tmap1_name,studentTable[i].name);//Copying the variable to the array!
		//This strcat below copies and replace the spacebar with the value in my temporary array and the second strcat replaces any variable if found after the spacebar and compares it with the search name. If found it returns the position.
		if(strcmp(searchname,(strcat(strcat(tmap1_name," "),studentTable[i].surname)))==0){
    		found=1;
    		return i;
		}
    }
    //If No name is found it returns a negative value.
    if(found==0){

        return -1;
    }

}

/*This functions sorts the arrays in terms of their final grades or total grades depending on the user.*/
void  sort_studentTable(Student_T *studentTable,int Lines){
    char sort_option;
    int j,k,i;
    double temp_total;//These are temporary arrays and variables used to store my values and then replace them.
    float temp_lab;
    float temp_assignment;
    float temp_Midterm;
    float temp_Final;
    char Temporary_name[50];
    char Temporary_surname[50];

    printf("Sort by (F: Final, T: Total): ");

    scanf(" %c",&sort_option);
	//Asks the user to input their choice!
    if(sort_option=='T'){//Using bubble sort, Creating two arrays, one for the whole structure array and the other for the first and second variable.
        for(j=0;j<Lines;j++){

            for( k=0;k<(Lines-1);k++){
				//First checks if the value in the highr position of the array is less than or not then enters the if statement.
                if(studentTable[k].Total < studentTable[k+1].Total )
                    {
                    	//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_total=studentTable[k].Total;
                    studentTable[k].Total=studentTable[k+1].Total;
                    studentTable[k+1].Total=temp_total;
                    
					//This copies  the character in the struct array into the temporary array,copy the higher variable into the smaller variable and then replace the temporary variable with smaller array.
                    strcpy(Temporary_name,studentTable[k].name);
                    strcpy(studentTable[k].name,studentTable[k+1].name);
                    strcpy(studentTable[k+1].name,Temporary_name);
                    
					//This copies  the character in the struct array into the temporary array,copy the higher variable into the smaller variable and then replace the temporary variable with smaller array.
                    strcpy(Temporary_surname,studentTable[k].surname);
                    strcpy(studentTable[k].surname,studentTable[k+1].surname);
                    strcpy(studentTable[k+1].surname,Temporary_surname);
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_lab=studentTable[k].lab;
                    studentTable[k].lab=studentTable[k+1].lab;
                    studentTable[k+1].lab=temp_lab;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_assignment=studentTable[k].Assignment;
                    studentTable[k].Assignment=studentTable[k+1].Assignment;
                    studentTable[k+1].Assignment=temp_assignment;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_Midterm=studentTable[k].Midterm;
                    studentTable[k].Midterm=studentTable[k+1].Midterm;
                    studentTable[k+1].Midterm=temp_Midterm;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_Final=studentTable[k].Final;
                    studentTable[k].Final=studentTable[k+1].Final;
                    studentTable[k+1].Final=temp_Final;
                }
        }
    }
    //Displays the new array
    Display_StudentTable(studentTable,Lines);
}
else if(sort_option=='F'){
	
      for(j=0;j<Lines;j++){

            for( k=0;k<(Lines-1);k++){

                if(studentTable[k].Final < studentTable[k+1].Final )
                    {
                    	//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_Final=studentTable[k].Final;
                    studentTable[k].Final=studentTable[k+1].Final;
                    studentTable[k+1].Final=temp_Final;
                    
					//This copies  the character in the struct array into the temporary array,copy the higher variable into the smaller variable and then replace the temporary variable with smaller array.
                    strcpy(Temporary_name,studentTable[k].name);
                    strcpy(studentTable[k].name,studentTable[k+1].name);
                    strcpy(studentTable[k+1].name,Temporary_name);
                    
					//This copies  the character in the struct array into the temporary array,copy the higher variable into the smaller variable and then replace the temporary variable with smaller array.
                    strcpy(Temporary_surname,studentTable[k].surname);
                    strcpy(studentTable[k].surname,studentTable[k+1].surname);
                    strcpy(studentTable[k+1].surname,Temporary_surname);
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_lab=studentTable[k].lab;
                    studentTable[k].lab=studentTable[k+1].lab;
                    studentTable[k+1].lab=temp_lab;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_assignment=studentTable[k].Assignment;
                    studentTable[k].Assignment=studentTable[k+1].Assignment;
                    studentTable[k+1].Assignment=temp_assignment;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_Midterm=studentTable[k].Midterm;
                      studentTable[k].Midterm=studentTable[k+1].Midterm;
                    studentTable[k+1].Midterm=temp_Midterm;
                    
					//This stores the variable in my struct array to my temporary variable,replace the value in the higher array then returns the value in the temporary variable to the smaller array.
                    temp_Final=studentTable[k].Total;
                    studentTable[k].Total=studentTable[k+1].Total;
                    studentTable[k+1].Total=temp_Final;
                }
        }
    }
    //Displays the new array
    Display_StudentTable(studentTable,Lines);
}
else{
	printf("Invalid option!\n");
}
}
int main(int argc, char *argv[]) {
	


//	if(argc ==1){
//		printf("there is no file detected:");
//		exit(1);
//	} 
	while(strcmp(argv[1],"cng140.txt")!=0){//campares from command linee and asks the user to input the filename if arguments are not detected.
        printf("\nThis file does not exist,");
		printf("please enter again: ");
		gets(argv[1]);
	}

    FILE *infile;
    char filename[50];
    char User_filename[50];
    int found=0;
    int option;
	int student_rows;
    char searchname[50];
    char searchsurname[50];
    Student_T *studentTable;
	FILE *incount;
    strcpy(filename,argv[1]);
    //Copies the commandline to the file in order to pass the filename.

    student_rows=row_count(incount,filename);
    
    studentTable=load_studentTable(studentTable,infile,filename,&student_rows);
	
	printf("\nFollowing records have been loaded:\n");
    Display_StudentTable(studentTable,student_rows);
    do{
		//A loop in order to ask the user for his options.        

        int value,seen=0;

        printf("\n\n1 for search, 2 for sort and 3 for exit: ");
        scanf("%d",&option);
		//A switch case for the options chosen by the user!
        switch (option){
        case 1:

           while(!seen) {
            printf("Enter the name of the student:");
            fflush(stdin);
            gets(searchname);//The value represents the number that it returns from the search and prints the value if students are found or not!
            value=search_studentTable(studentTable,student_rows,searchname);
            if(value>=0){
                printf("%s %s has %3.2f from lab, %3.2f from Assignments, %3.2f from Midterm, %3.2f from Final with the total of %3.2f\n",studentTable[value].name,studentTable[value].surname,studentTable[value].lab,studentTable[value].Assignment,studentTable[value].Midterm,studentTable[value].Final,studentTable[value].Total);
                seen=1;
            }
            else{
                printf("That student is unknown! Please try again!");
                seen=0;
            }
           }
           break;
        case 2:
			//Calls the sort function and sort the values out!
            sort_studentTable(studentTable,student_rows);
            break;
        case 3:
            printf("\nBye!!!!");
            exit(1);
        }
    }while(option!=3);

	return 0;
}
