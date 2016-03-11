#ifndef HASHMAP_H
#define HASHMAP_H

// maintains sprite state variables like texture UV coordinates, index number for the renderer and the string name of the state 
typedef struct
{
	const char* spriteState;
	float uLeft;
	float uRight;
	float vTop;
	float vBottom;

	int rendererIndex;
}SpriteStateData;

// physics and sprite data of an object (birds, pigs or blocks)
typedef struct
{
	const char* objectType;

	float density;
	float restitution;
	float friction;

	int health;

	SpriteStateData *spriteData;
	int numSprites;
}ObjectData;

// key value pair for a hashtable
typedef struct{
	char key[30];
	int value;
}KeyValue;
typedef KeyValue* HashTable;

/**
 *	@class HashMap
 *	@desc string to integer Hash table
 */
class HashMap
{
	public:
		HashMap(int totalValues);
		void hashInsert(char *key, int value);
		int hashSearch(char *key);
		int hashProbing(char *key, int index);
		int hash(char *key);

	private:
		int numValues;
		HashTable hashTable;
};

#endif