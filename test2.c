/*
Copyright 2020 DAVA Authors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 3 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


typedef struct{
	int pos;
	int ids[2];
	int upper[2];
	int lower[2];
	int con;
} NODE;

typedef struct{
	int val1;
	int val2;
	int val3;
	int val4;
} MTD_NODE;

int messageID=0;

int numNodes =2;

int updateControl =1;

int RandomID(int upper, int lower) { return rand() % (upper-lower) + lower;}

void sendMessage(int id) { messageID=id; printf("This is the message ID for the receiver 0x%d\n", id); getchar();}

unsigned long long int rdtsc(void)
{
   unsigned long long int x;
   unsigned a, d;

   __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

   return ((unsigned long long)a) | (((unsigned long long)d) << 32);;
}
unsigned long long int rdtscp(void)
{
   unsigned long long int x;
   unsigned a, d;

   __asm__ volatile("rdtscp" : "=a" (a), "=d" (d));

   return ((unsigned long long)a) | (((unsigned long long)d) << 32);;
}

int update(int p, NODE* n) 
{ 
	int x = RandomID(n->upper[p], n->lower[p]); 
	printf("Here is the new ID 0x%d\n", x); 
	getchar();
	return x; 
}

void receiveMessage(NODE* x, NODE* y) 
{
	if(messageID == x->ids[x->pos])
	{
		if(x->con == updateControl)
		{
		printf("I am ecu1 and I have received a message\nI will now broadcast update that ID 0x%d is invalid\n", x->ids[x->pos]);
		getchar();
		x->ids[x->pos] = y->ids[x->pos] = update(x->pos, x);
		x->con =1;
		}
		else
		{
			x->con++;
		}
	}
	else if(messageID == y->ids[y->pos])
	{
		if(y->con == updateControl)
		{
		printf("I am ecu2 and I have received a message\nI will now broadcast update that ID 0x%d is invalid\n", y->ids[y->pos]);
		getchar();
		x->ids[y->pos] = y->ids[y->pos] = update(y->pos, x);
		y->con=1;
		}
		else
		{
			y->con++;
		}
	}
}

int main(int argc, char* argv[])
{
	NODE ecu1, ecu2;
	MTD_NODE mtd_node;
	//this next few lines were for testing the total size a can node would have to store to accomodate this algorithm
	mtd_node.val1 = 1111;
	mtd_node.val2 = 2222;
	mtd_node.val3 = 1000;
	mtd_node.val4 = 2300;
	printf("%lu", sizeof(mtd_node));
	ecu1.con = ecu2.con =1;
	ecu1.pos=0;
	ecu2.pos=1;
	ecu1.ids[0] = ecu2.ids[0] =1;
	ecu1.ids[1] = ecu2.ids[1] =500;
	ecu1.upper[0] = ecu2.upper[0] =499;
	ecu1.upper[1] = ecu2.upper[1] =999;
	ecu1.lower[0] = ecu2.lower[0] =1;
	ecu1.lower[1] = ecu2.lower[1] =500;
	srand(time(0));

	//printf("%lu", sizeof(ecu1));

	//double x = (double)clock();
	unsigned long long x = rdtsc();
	//printf("%llu\n\n", x);
	RandomID(ecu1.upper[0], ecu1.lower[0]);
	//double y = (double)clock();
	unsigned long long y = rdtsc();
	//printf("%llu\n\n", y);
	double z =(double)(((double)y-(double)x)/(double)CLOCKS_PER_SEC);
	printf("%li\n", CLOCKS_PER_SEC);
	printf("this is the average time for a random number to be generated and the update phase to be completed %.10lf seconds\n\n", z);
	
/*	while(1)
	{
		printf("ecu1 is sending a message to ecu2\n");
		getchar();
		sendMessage(ecu1.ids[1]);
		receiveMessage(&ecu1, &ecu2);
		printf("This is ecu2's new ID 0x%d from ecu2 0x%d from ecu1\n It has been updated in both ecus\n", ecu2.ids[1], ecu1.ids[1]);
		getchar();
		printf("ecu2 is sending a message to ecu1\n");
		getchar();
		sendMessage(ecu2.ids[0]);
		receiveMessage(&ecu1, &ecu2);
		printf("This is ecu1's new ID 0x%d from ecu1 0x%d from ecu2\n It has been updated in both ecus\n", ecu1.ids[0], ecu2.ids[0]);
	}*/
	return 0;
}

