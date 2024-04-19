//Akinola Adebowale
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct graphVertex Vertex;
struct graphVertex
{
    Vertex* next;
    char team[81];
    int inDegree;
    int outDegree;
    int isVisited;
    struct graphEdge* firstArc;
};

typedef struct graphEdge Edge;
struct graphEdge
{
    Vertex* destination;
    int weight;
    Edge* nextArc;
};

typedef struct graphHead graphHead;
struct graphHead
{
    int count;
    Vertex* first;
};

graphHead* readTeams(FILE*);
void readMatches(FILE*, graphHead*);
graphHead* createGraph(void);
void createVertex(graphHead*, char[]);
void createEdge(graphHead*, char[], char[], int);
void printGraph(graphHead*);
void getMostWins(graphHead*);
void getMostLosses(graphHead*);
void getMaxGoals(graphHead*);
void getMinGoals(graphHead*);
int checkwinChain(graphHead*, char[], char[]);
int checkPath(graphHead*, char[], char[]);
int traversal(graphHead*,char[],char[]);

int main(int argc, char *argv[]){
    FILE *inFile;
    graphHead *graph;
    inFile = fopen("teams.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }
    printf("Teams File has been opened successfully; the graph with no edges can be seen below:\n");
    graph = readTeams(inFile);
    printGraph(graph);
    printf("***************************************************************************************************************\n");

    fclose(inFile);
    inFile = fopen("matches.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }
    printf("Matches File has been opened successfully; the graph with edges can be seen below:\n");
    readMatches(inFile, graph);
    printGraph(graph);
    printf("***************************************************************************************************************\n");

    getMostWins(graph);
    getMostLosses(graph);
    getMaxGoals(graph);
    getMinGoals(graph);
    printf("***************************************************************************************************************\n");
    if (argc < 3){
        printf("Two teams are not given in the command line arguments!");
    }
    else{
        char str1[81], str2[81];
        int i = 0;
        while (argv[1][i] != '\0'){
            if (argv[1][i] == '_')
                str1[i] = ' ';
            else
                str1[i] = argv[1][i];
            i++;
        }
        str1[i] = '\0';
        i = 0;
        while (argv[2][i] != '\0'){
            if (argv[2][i] == '_')
                str2[i] = ' ';
            else
                str2[i] = argv[2][i];
            i++;
        }
        str2[i] = '\0';

        int result = checkwinChain(graph, str1, str2);
        if (result)
            printf("%s have beaten a team that beat %s.\n", str1, str2);

        else
            printf("%s have NOT beaten a team that beat %s.\n", str1, str2);

        int result2 = checkPath(graph, str1, str2);
        if (result2)
            printf("There is a path from %s to %s", str1, str2);
        else
            printf("There is no path from %s to %s", str1, str2);
    }
    return 0;
}

graphHead* createGraph(void){
    graphHead *graph;
    graph = (graphHead*)malloc(sizeof(graphHead));
    if(graph == NULL)
    {
        printf("Memory allocation is failed.");
        exit(1);
    }
    graph->count = 0;
    graph->first = NULL;
    return graph;
}

void createVertex(graphHead *graph, char teamName[]){
    Vertex *vertex = (struct graphVertex *) malloc(sizeof(struct graphVertex));
    vertex->next = NULL;
    strcpy(vertex->team, teamName);
    vertex->inDegree = 0;
    vertex->outDegree = 0;
    vertex->firstArc = NULL;
    vertex->isVisited = 0;

    graph->count++;

    if(graph->first == NULL){
        graph->first = vertex;
    }
    else {
        struct graphVertex *temp = graph->first;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = vertex;
    }
}

graphHead* readTeams(FILE* inFile){
    graphHead* temp_graph;
    temp_graph = createGraph();

    char data[100];
    char buffer[100];

    while(fgets(buffer,100,inFile)){//This line reads every hundred characters of each line in the file

        strcpy(data,buffer);//This then stores the team name in a temporary data.
        data[strcspn(data,"\n")]='\0'; //Here I remove the new character line in order not to cause confusion
        createVertex(temp_graph,data);//Send the graph and the data to the create vertex function

      }
    return temp_graph;//Returns the graph!
    // WRITE YOUR CODE HERE
}

void readMatches(FILE* inFile, graphHead *graph){

//These are my temporary variables
    int count = 0;
    char name[50];
    char buffer[100];
    int temp_SOTY = 0;
    int temp_Wk = 0;
    char temp_date[50];
    char temp_Home[50];
    int temp_HomeGoals = 0;
    int temp_AwayGoals = 0;
    char temp_Away[50];
    char tempFTR;

    while(fgets(buffer,100,inFile)){
        fflush(stdin);

        if(count>=1){//This skips the first line in the file!
       //Here I read and store each line in the file to the temporary variables
            sscanf(buffer,"%d;%d;%[^;];%[^;];%d;%d;%[^;];%c" ,&temp_SOTY,&temp_Wk,temp_date,temp_Home,&temp_HomeGoals,&temp_AwayGoals,temp_Away,&tempFTR);
            //printf("\nYOLO1");
            if(tempFTR=='H'){
                createEdge(graph,temp_Home,temp_Away,temp_HomeGoals-temp_AwayGoals);
                //printf("\nYOLO");
            }
            else if(tempFTR=='A'){
                createEdge(graph,temp_Away,temp_Home,temp_AwayGoals-temp_HomeGoals);
            }
        }
        count++;
    }
    // WRITE YOUR CODE HERE
}

void createEdge(graphHead* graph, char winningTeam[], char losingTeam[], int goalDiff){

    Vertex* from_vertex;//create pointers to the from and to vertices
    Vertex *to_vertex;
    Edge* toedge = (struct graphEdge*)malloc(sizeof(struct graphEdge));//Create our edge

    if(graph->count==0){//If the graph is empty it prints the following message
        printf("\nthis graph is empty\n");
        exit(1);
    }
    else{

        from_vertex=graph->first;// Here we traverse the graph from the first graph
        fflush(stdin);

        while(from_vertex->next!=NULL){//Here we look for the vertex of the winning team.

            if(strcmp(from_vertex->team,winningTeam)==0){//Once the team is found it breaks from the loop
                break;
            }
            from_vertex=from_vertex->next;//If it is not found it goes to the next vertex
        }
        if(from_vertex->firstArc==NULL){// Here we create an edge for the from vertex, if it has zero adges
            from_vertex->firstArc=toedge;
            from_vertex->outDegree++;
            toedge->nextArc=NULL;
        }
        else{// If it has more than one edge it traverses the edges till we get to the last one.
            Edge* temp_edge = from_vertex->firstArc;
            while (temp_edge->nextArc!=NULL){
                temp_edge= temp_edge->nextArc;
            }
            temp_edge->nextArc=toedge;//Updates the outdegree
            from_vertex->outDegree++;
        }
        fflush(stdin);

        to_vertex=graph->first;//We check for the to vertex

        while(to_vertex->next!=NULL){//This looks for the vertex of the losing team

            if(strcmp(to_vertex->team,losingTeam) ==0){//Once the team is found it breaks from the loop
                break;
            }

            to_vertex=to_vertex->next;
        }
        toedge->destination=to_vertex;
        toedge->weight=goalDiff;
        toedge->nextArc=NULL;
        to_vertex->inDegree++;
    }
    // WRITE YOUR CODE HERE
}

void printGraph(graphHead* graph){

    Vertex* temp_vertex;//Create temporary pointers for my vertices and edges
    Edge *temp_edge;

    if(graph->count==0){//Check if the graph is empty
        printf("\nThere are no teams in this file!");
        exit(1);
    }
    else{

        temp_vertex=graph->first;//Assign the temp vertex to teh first graph
        while(temp_vertex!=NULL){// Using the temp_vertex pointer to traverse the graph

            printf("\n%s ->", temp_vertex->team );//Print each vertex
            if(temp_vertex->firstArc!=NULL){//If a vertex have an edge it traverse the edges and then prints them.
                temp_edge=temp_vertex->firstArc;
                while(temp_edge!=NULL){
                    printf("%s | %d ",temp_edge->destination->team,temp_edge->weight);
                    temp_edge=temp_edge->nextArc;
                    printf("->");
                }
            }
            temp_vertex = temp_vertex->next;
            printf("\n____________________________________________________________________\n");
        }
    }
    // WRITE YOUR CODE HERE
}

void getMostWins(graphHead *graph){

    Vertex *temp_vertex;
    char name[50];//Temporary string to store the name of the team with the most wins
    int won;

    if(graph->count==0){//Check if the graph is empty
        printf("\nYou have no vertices in this graph");
        exit(1);
    }
    else{
        temp_vertex=graph->first;
        strcpy(name,temp_vertex->team);//Copies the first team to the temp variable and then traverses the graph
        won=temp_vertex->outDegree;//Here we use temporary variable 'won' to store the outdegree since the outdegree stores the number of matches won
        while(temp_vertex->next!=NULL){

            if(temp_vertex->outDegree >won){
                strcpy(name,temp_vertex->team);
                won=temp_vertex->outDegree;//Each time it checks a vertex with more outdegrees the value changes
            }
            temp_vertex=temp_vertex->next;
        }
    }
    printf("\nTeam with the most victories:");
    printf("\n%s have won %d matches.\n",name,won);
    // WRITE YOUR CODE HERE
}

void getMostLosses(graphHead *graph){

    Vertex *temp_vertex;
    char name[50];
    int lost;

    if(graph->count==0){
        printf("\nYou have no vertices in this graph");
        exit(1);
    }
    else{

        temp_vertex=graph->first;
        strcpy(name,temp_vertex->team);
        lost=temp_vertex->inDegree;//Here we use temporary variable 'lost' to store the indegree since the indegree stores the number of matches lost
        while(temp_vertex->next!=NULL){

           if( temp_vertex->inDegree > lost){
                strcpy(name,temp_vertex->team);
                lost=temp_vertex->inDegree;//Each time it checks a vertex with more indegrees the value changes
            }
            temp_vertex=temp_vertex->next;
        }
    }
    printf("\nTeam with the most losses:");
    printf("\n%s have lost %d matches.\n", name, lost);
    // WRITE YOUR CODE HERE

}

void getMaxGoals(graphHead *graph){

    Vertex *temp_vertex,*from_vertex;//Two temporary gointers to check for the matches won and the matches lost
    Edge *temp_edge,*from_edge;
    int goal_for=0,totalgoaldiff=0,goal_against=0;
    char name[50];

    if(graph->count==0){
        printf("\nThe graph seems to be empty");
        exit(1);
    }
    else{
        temp_vertex=graph->first;
        from_vertex=graph->first;//We start the pointers from the first graph in order to traverse the graph of both of them
        while(temp_vertex!=NULL){//Loop through the vertices

            if(temp_vertex->firstArc!=NULL){//Loop through it's edges: The matches it has won
                temp_edge=temp_vertex->firstArc;

                while(temp_edge!=NULL){//Sum up all the goal difference of the matches won

                    goal_for += temp_edge->weight;
                    temp_edge=temp_edge->nextArc;
                }
                while(from_vertex!=NULL){//Loop for other vertices in order to know the matches the team has lost

                    if(from_vertex->firstArc!=NULL){

                        from_edge=from_vertex->firstArc;
                        while(from_edge!=NULL){//Search for the matches the team has lost and calculate the sum of the goal difference of that match

                            if(strcmp(from_edge->destination->team,temp_vertex->team)==0){
                                goal_against+=from_edge->weight;
                            }
                            from_edge=from_edge->nextArc;
                        }
                    }
                    from_vertex=from_vertex->next;
                }
            }
            //Calculate the overall goal difference: goal difference of won matches - goal difference of matches lost
            goal_for = goal_for - goal_against;

            if(goal_for > totalgoaldiff){//Here we store the team with the highest goal difference after traversal
                strcpy(name,temp_vertex->team);
                totalgoaldiff=goal_for;
            }
            goal_for=0;//Initialize the goal difference for winning the match and losing
            goal_against=0;
            from_vertex=graph->first;//Return this pointer to the source vertex in order to loop through it again
            temp_vertex=temp_vertex->next;
        }
    }
    printf("\nTeam with the highest goal difference:");
    printf("\n%s have a goal difference of %d\n",name,totalgoaldiff);

    // WRITE YOUR CODE HERE
}

void getMinGoals(graphHead *graph){
//Same code from getMostgoals function but the ">" is changed to "<".
    Vertex *temp_vertex,*from_vertex;
    Edge *temp_edge,*from_edge;
    int goal_for=0,totalgoaldiff=0,goal_against=0;
    char name[50];

    if(graph->count==0){
        printf("\nThe graph seems to be empty");
        exit(1);
    }
    else{
        temp_vertex=graph->first;
        from_vertex=graph->first;
        while(temp_vertex!=NULL){

            if(temp_vertex->firstArc!=NULL){
                temp_edge=temp_vertex->firstArc;

                while(temp_edge!=NULL){

                    goal_for += temp_edge->weight;
                    temp_edge=temp_edge->nextArc;
                }
                while(from_vertex!=NULL){

                    if(from_vertex->firstArc!=NULL){

                        from_edge=from_vertex->firstArc;
                        while(from_edge!=NULL){

                            if(strcmp(from_edge->destination->team,temp_vertex->team)==0){
                                goal_against+=from_edge->weight;
                            }
                            from_edge=from_edge->nextArc;
                        }
                    }
                    from_vertex=from_vertex->next;
                }
            }

            goal_for = goal_for - goal_against;

            if(goal_for < totalgoaldiff){//The sign is changed in order to get the team with the lowest goal difference
                strcpy(name,temp_vertex->team);
                totalgoaldiff=goal_for;
            }
            goal_for=0;
            goal_against=0;
            from_vertex=graph->first;
            temp_vertex=temp_vertex->next;
        }
    }
    printf("\nTeam with the lowest goal difference:");
    printf("\n%s have a goal difference of %d\n",name,totalgoaldiff);

    // WRITE YOUR CODE HERE
}

int checkwinChain(graphHead *graph, char team1[], char team2[]){

    Vertex *temp_vertex,*temp2_vertex;
    Edge *temp_edge,*temp2_edge;
    char temp_team[50];
    int found=0;

    if(graph->count==0){
        printf("\nGraph is empty!");
        exit(1);
    }
    else{
        temp_vertex=graph->first;
        temp2_vertex=graph->first;
        while(temp_vertex->next!=NULL){//Check for the first team

            if(strcmp(temp_vertex->team,team1)==0){//Looks for the first team vertex in the graph

                if(temp_vertex->firstArc!=NULL){//Check if it's not NULL, if not it assigns an edge to it's first arc

                    temp_edge=temp_vertex->firstArc;
                    while(temp_edge!=NULL){//Traverses the edges of the first team vertex and then copies the name of its verteces directed to
                        strcpy(temp_team,temp_edge->destination->team);
                        while(temp2_vertex!=NULL){//This loop checks through the graph to find the team vertex that team 1 beat.

                            if(strcmp(temp2_vertex->team,temp_team)==0){//It checks through the edges of the beaten vertex to find the second team.
                                if(temp2_vertex->firstArc!=NULL){
                                    temp2_edge=temp2_vertex->firstArc;
                                    while(temp2_edge!=NULL){
                                        if(strcmp(temp2_edge->destination->team,team2)==0){
                                            found=1;
                                            break;
                                        }
                                        temp2_edge=temp2_edge->nextArc;
                                    }
                                }
                            }
                            temp2_vertex=temp2_vertex->next;
                        }
                        temp2_vertex=graph->first;
                        temp_edge=temp_edge->nextArc;
                    }
                }
            }
            temp_vertex=temp_vertex->next;
        }
    }
    if(found!=0){
        return 1 ;
    }
    else{
        return 0;
    }
    // WRITE YOUR CODE HERE
}

int checkPath(graphHead *graph, char team1[], char team2[]){

    int found;

    if(graph->count==0){//Check if the graph is empty
        printf("\nGraph is empty!");
        exit(1);
    }
    else{//Send the graph to a function and then traverses
        found=traversal(graph,team1,team2);
    }
    return found;
    // WRITE YOUR CODE HERE
}


int traversal(graphHead *graph, char team1[],char team2[]){
    //This is a recursive function that traverses the edges of each vertex one after the other
    Vertex *temp_vertex;
    Edge *temp_edge;
    int temp_found=0;//Used as a temporary found to check if a path has been found.
    temp_vertex=graph->first;
    while(temp_vertex->next!=NULL){//Start traversing in order to check for team 1.

        if(strcmp(temp_vertex->team,team1)==0){
            //It then finds team 1 and marks it as visited.
            if(temp_vertex->isVisited==0){

                temp_vertex->isVisited=1;
                temp_edge=temp_vertex->firstArc;//We begin to travers its edges one after the other
                while(temp_edge!=NULL){//If it's edge is directed to team2 vertex the found becomes one and the loop breaks
                    if(strcmp(temp_edge->destination->team,team2)==0){

                        temp_found=1;
                        break;
                    }//the traverse function is called again
                    else{
                        temp_found= traversal(graph,temp_edge->destination->team,team2);
                        if(temp_found)//It checks if temp_found is 1,a path has been found, it breaks from the loop
                            break;
                    }
                    temp_edge=temp_edge->nextArc;
                }
            }
            else{
                //It has been vertex has been visited,it leaves the function.
                return temp_found;
            }
        }
        //If path is found it leaves the loop.
        if(temp_found)
            break;
        temp_vertex=temp_vertex->next;
    }
    return temp_found;
}