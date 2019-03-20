#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"

#define TIME 10

struct node* head = NULL;
struct node* tail;

void Delete()
{
	tail = head;
	head = head -> next;
	free(tail);

	while(head)
	{
	tail = head;
	head = head -> next;
	free(tail -> task -> name);
	free(tail -> task);
	free(tail);
	}
}

void removeNode(struct node* n)
{
	free(n -> task -> name);
	free(n -> task);
	free(n);
}

Task* newTask(char* name, int priority, int burst)
{
	Task* n_job = (Task*)malloc(sizeof(Task));
	n_job -> name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(n_job -> name, name);
	n_job -> priority = priority;
	n_job -> burst = burst;
	n_job -> tid = 0;

	return n_job;
}

void add(char* name, int priority, int burst)
{
	if(head == NULL)
	{
	head = (struct node*)malloc(sizeof(struct node));
	head -> next = NULL;
	head -> task = NULL;
	tail = head;
	}

	tail -> next = (struct node*)malloc(sizeof(struct node));
	tail = tail -> next;
	tail -> task = newTask(name, priority, burst);
	tail -> next = NULL;
}

void schedule()
{
	int s;

	struct node* temp;

	while(head -> next)
	{
	s = (TIME < head -> next -> task -> burst)?TIME:head -> next -> task -> burst;
	run(head -> next -> task, s);
		if(head -> next -> task -> burst == s)
		{
		temp = head -> next;
		head -> next = head -> next -> next;
		removeNode(temp);
		}

		else
		{
		head -> next -> task -> burst -= s;
		tail -> next = head -> next;
		tail = tail -> next;
		head -> next = head -> next -> next;
		tail -> next = NULL;
		}	
	}

	Delete();
}

