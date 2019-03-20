#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"

#define TIME 10

struct node** head = NULL;
struct node** tail;

void Delete(int point)
{
	struct node* h = head[point];
	struct node* t = tail[point];

	t = h;
	h = h -> next;
	free(t);
	
	while(h)
	{
	t= h;
	h = h -> next;
	free(t -> task -> name);
	free(t -> task);
	free(t);
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
	int i;

	if(head == NULL)
	{
		head = (struct node**)malloc(sizeof(struct node*)*(MAX_PRIORITY+1));
		tail = (struct node**)malloc(sizeof(struct node*)*(MAX_PRIORITY+1));
		for(i = 0; i <= MAX_PRIORITY; ++i)
			head[i] = tail[i] = NULL;
	}

	if(priority < MIN_PRIORITY || priority > MAX_PRIORITY)
	{
		printf("ERROR: Invalid|\n");

		return;
	}

	if(head[priority] == NULL)
	{
		head[priority] = (struct node*)malloc(sizeof(struct node));
		head[priority] -> task = NULL;
		head[priority] -> next = NULL;
		tail[priority] = head[priority];
	}

	tail[priority] -> next = (struct node*)malloc(sizeof(struct node));
	tail[priority] = tail[priority] -> next;
	tail[priority] -> task = newTask(name, priority, burst);
	tail[priority] -> next = NULL;
}

void scheduleP(int priority)
{
	int s;
	struct node* temp;
	struct node* h = head[priority];
	struct node* t = tail[priority];

	while(h -> next)
	{
		s = (TIME < h -> next -> task -> burst)?TIME: h -> next -> task -> burst;
		run(h -> next -> task, s);

		if(h -> next -> task -> burst == s)
		{
			temp = h -> next;
			h -> next = h -> next -> next;
			removeNode(temp);
		}

		else
		{
			h -> next -> task -> burst -= s;
			t -> next = h -> next;
			t = t -> next;
			h -> next = h -> next -> next;
			t -> next = NULL;
		}
	}

	Delete(priority);
}

void schedule()
{
	int i;

	for(i = MAX_PRIORITY; i >= MIN_PRIORITY; --i)
	{
		if(head[i])
			scheduleP(i);
	}
	free(head);
	free(tail);
}

