#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"

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

	struct node* current;
	struct node* previous;

	previous = head;
	current = head -> next;

	while(current)
	{
		if(priority >= current -> task -> priority)
		{
		previous = current;
		current = current -> next;
		}

		else
			break;
	}

	previous -> next = (struct node*)malloc(sizeof(struct node));
	previous = previous -> next;
	previous -> next = current;
	previous -> task = newTask(name, priority, burst);
}
void schedule()
{
	struct node* p = head -> next;

	while(p)
	{
	run(p -> task, p -> task -> burst);
	p = p -> next;
	}

	Delete();
}

