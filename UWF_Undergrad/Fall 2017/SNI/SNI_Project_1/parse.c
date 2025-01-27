
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parse.h"

Param_t* parseTokens();

Param_t* parseTokens()
{

	printf("$ ");                          //input line. reprints after each action

	fgets(input, sizeof(input), stdin);    //Retrieves user input and stores into input.
	token = strtok(input, " \t\n\0");              //Tokenizes input. Separates by what is  in s.
	Param_t* p1 = malloc(sizeof(Param_t)); //Assigns appropriate size for p1
	int i = 0;                             //i keeps count of the number of parameters inserted

	while(token != NULL)
	{

		if(token[0] == '<')
		{
			p1->inputRedirect = token + 1;      //adds the user requested input directory
			i++;
		}
		if(token[0] == '>')
		{
			p1->outputRedirect = token + 1;     //Adds the user requested output directory
			i++;
		}
		if(token[0] == '&')
		{
			p1->background = 1;
			token = strtok(NULL, s);           //Continues to add phrases until it reaches the end of the sentence
		}
		else
		{
			p1->argumentVector[i] = token;
			i++;

			token = strtok(NULL, s);           //Continues to add phrases until it reaches the end of the sentence
		}


	}

	p1->argumentCount = i;                    //Adds the number of arguments into p1
	return p1;

}


