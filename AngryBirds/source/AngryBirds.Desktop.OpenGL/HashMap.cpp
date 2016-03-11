#include "stdafx.h"
#include "HashMap.h"
#include <string.h>
#include <malloc.h>


HashMap::HashMap(int totalValues)
{
	numValues = totalValues;
	hashTable = (HashTable)malloc(numValues * sizeof(KeyValue));
	for (int i = 0; i < numValues; i++)
	{
		hashTable[i].value = -1;
	}
}

/**
*	@method hashInsert
*	@desc {HASH_TABLE_OPS} insert the key-value pair in the hash table
*	@param {char *} key 			- string key for storage
*	@param {int} value 				- value to be stored
*/
void HashMap::hashInsert(char *key, int value)
{
	int i, j;
	for (i = 0; i<numValues; i++)
	{
		j = hashProbing(key, i);
		if ((hashTable)[j].value == -1)
		{
			(hashTable)[j].value = value;
			strcpy((hashTable)[j].key, key);
			break;
		}
	}
}

/**
*	@method hashSearch
*	@desc {HASH_TABLE_OPS} searches the hash table for the given key and return the corresponding value
*	@param {char *} key - string key for which the value is to be searched
*	@return int 		- value for given key, -1 if not found
*/
int HashMap::hashSearch(char *key)
{
	int i = 0, j;
	do
	{
		j = hashProbing(key, i);
		if (strcmp((hashTable)[j].key, key) == 0)
			return (hashTable)[j].value;
		i++;
	} while (i < numValues || (hashTable)[j].value == -1);

	return -1;
}

/**
*	@method hashProbing
*	@desc {HASH_TABLE_OPS} probing for the hash value
*	@param {char *} key - string key to be hashed
*	@param {int} index 	- current index of table being probed
*	@return int 		- index
*/
int HashMap::hashProbing(char *key, int index)
{
	return (hash(key) + index) % numValues;
}

/**
*	@method hash
*	@desc {HASH_TABLE_OPS} hash value
*	@param {char *} key - string key to be hashed
*	@return int 		- hash value
*/
int HashMap::hash(char *key)
{
	int i, len = strlen(key), total = 0;
	for (i = 1; i<len; i++)
	{
		total += (i * key[i]);
	}
	return total;
}