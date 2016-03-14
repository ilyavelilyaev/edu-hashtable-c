//Hash table with open adressing
//© 2016 Ilya Velilyaev



#include <stdlib.h>
#include <string.h>

#define SIZE_EXCEEDED ((unsigned int)(-1))
#define TABLE_NOT_CRTD (((unsigned int)(-1)) - 1)
#define DEFAULT_STEPPING 1

typedef struct {
	char *key;
	char *value;
} hashtable_pair_t;

typedef struct {
	hashtable_pair_t *arr;
	unsigned int count;
	unsigned int size;
} hashtable_t;

//public methods

hashtable_t *hashtable_create_table (unsigned int size);

unsigned int hashtable_empty (hashtable_t *hashtable);
unsigned int hashtable_count (hashtable_t *hashtable);
unsigned int hashtable_size (hashtable_t *hashtable);


/*
 add_pair method returns id of pair if it is already added, or adds and returns new id.
 Returns SIZE_EXCEEDED if no more space in table left.
 Can be used with arguments key and value or pair.
 */
unsigned int hashtable_add_pair_by_key_value (hashtable_t *hashtable, char *key, char *value);
unsigned int hashtable_add_pair (hashtable_t *hashtable, hashtable_pair_t pair);

/*
 get_value method returns value by key or id if found.
 If no pair found or no table is created then NULL is returned.
 */
char *hashtable_get_value_by_key (hashtable_t *hashtable, char *key);
char *hashtable_get_value_by_id (hashtable_t *hashtable, unsigned int hash_id);

/*
 get_pair method returns pair by key or id if found.
 If no pair found by key or id then NULL is returned.
 */
hashtable_pair_t *hashtable_get_pair_by_key (hashtable_t *hashtable, char *key);
hashtable_pair_t *hashtable_get_pair_by_id (hashtable_t *hashtable, unsigned int hash_id);

/*
 delete_pair method delets pair by key or id, returns 0 on success.
 Returns 1 if no pair found by key.
 */
unsigned int hashtable_delete_pair_by_key (hashtable_t *hashtable, char *key); //deleting by key
unsigned int hashtable_delete_pair_by_id (hashtable_t *hashtable, unsigned int hash_id); //deleting by id

/*
 create_pair is used to make pair_t typed consts.
 */
hashtable_pair_t hashtable_create_pair (char *nkey, char *nvalue);


/* Delete table and free memory 
 Returns 0 on success. */
unsigned int hashtable_delete_table (hashtable_t *hashtable);


