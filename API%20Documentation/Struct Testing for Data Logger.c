#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * Sensor Struct Test
 * Original C for DataLoggerSD C++ Library as tested in CodeRunner
 * Author Brian Young 
 * 6/21/16.
 * Version 0.2 (Added as part of documentation)
 */
typedef enum { false, true } bool; //C does not have boolean type, Arduino does

typedef struct SensorInfo SensorInfo;
struct SensorInfo {
	char *_sensorName;
	char *_units;
	float _sensorReading;
	SensorInfo *next;
};



SensorInfo* newSensorInfo()
{
	SensorInfo* foo = (SensorInfo *)(malloc(sizeof(SensorInfo)));
	foo -> _sensorName = NULL;
	foo -> _units = NULL;
	foo -> _sensorReading = 0;
	foo -> next = NULL;
	return foo;
	
}

SensorInfo* addSensor(char* sensorName, char *units)
{
	SensorInfo* addSensor = newSensorInfo();
	addSensor -> _sensorName = sensorName;
	addSensor -> _units = units;
	return addSensor;
}

void addNodeToList(SensorInfo *head, SensorInfo *node)
{
	SensorInfo *n = head;
	while (n->next != NULL) {
		n = n->next;
	}
	n -> next = node;
}

void printlist(SensorInfo *head)
{
	SensorInfo *n;
	int count = 0;
	for(n = head; n != NULL; (n = n->next))
	{
		printf("Node %d -> Name: %s, Units: %s\n", count, n->_sensorName, n->_units);	
		count++;
	}
	
}

SensorInfo* searchList(char* senseName, SensorInfo *root)
{
	SensorInfo *n = root;
	
	while(strcmp((n->_sensorName), senseName) != 0)
	{
		if (n -> next == NULL)
		{
			printf("Sensor named %s not found. %s will be returned.\n", senseName, root -> _sensorName);
			return root;
		}
		n = n-> next;
	}
	return n;
}



int main(int argc, char *argv[]) {

SensorInfo *head = addSensor("Solar Panel", "V");
SensorInfo *node1 = addSensor("Test", "N/A");
SensorInfo *node2 = addSensor("Time",":");
SensorInfo *node3 = addSensor("Date", "\\");
addNodeToList(head, node1);
addNodeToList(head, node2);
addNodeToList(head, node3);
printlist(head);
printf("Size of SensorInfo: %lu bytes.\n\n", sizeof(*head));
SensorInfo *find = searchList("Date", head);
printf("Node Found: %s\n", find->_sensorName);
return 0;	
}