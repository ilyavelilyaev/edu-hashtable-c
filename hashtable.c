//Hash table with open adressing
//© 2016 Ilya Velilyaev

#include "hashtable.h"
#include <stdio.h>
#include <assert.h>

/* Interface */

//private methods

unsigned int hashtable_get_hash_from_key (hashtable_t *hashtable, char *key);
unsigned int hashtable_get_hash_from_pair (hashtable_t *hashtable, hashtable_pair_t *pair);
unsigned int hashtable_check_if_pair_is_good (hashtable_pair_t *pair);

/* Implementation */

hashtable_t *hashtable_create_table (unsigned int size) {
	
	//impossible to create table with size 0.
	assert(size >= 1);
	
	//allocating memory for hashtable.
	hashtable_t *hashtable = (hashtable_t *)malloc(sizeof(hashtable_t));
	
	//checking if memory is successfully allocated
	if (hashtable == NULL) {
		perror("Error while allocating memory for hashtable.");
		return NULL;
	}
	
	//initialising count and size fields.
	hashtable->count = 0;
	hashtable->size = size;
	
	//allocating memory for table.
	hashtable->arr = (hashtable_pair_t *)malloc(sizeof(hashtable_pair_t) * size);
	
	//checking if memory is successfully allocated
	if (hashtable->arr == NULL) {
		perror("Error while allocating memory for hashtable array.");
		return NULL;
	}
	
	//initializing all pairs with NULL.
	hashtable_pair_t *array = hashtable->arr;
	for (unsigned int i = 0; i < size; i++) {
		array[i].key = NULL;
		array[i].value = NULL;
	}
	
	return hashtable;
}

unsigned int hashtable_empty (hashtable_t *hashtable) {
	
	//checking arguments.
	assert (hashtable != NULL);
	
	return hashtable->count ? 0 : 1;
}
unsigned int hashtable_count (hashtable_t *hashtable) {
	
	//checking arguments.
	assert (hashtable != NULL);
	
	return hashtable->count;
}
unsigned int hashtable_size (hashtable_t *hashtable) {
	
	//checking arguments.
	assert (hashtable != NULL);
	
	return hashtable->size;
}


unsigned int hashtable_add_pair_by_key_value (hashtable_t *hashtable, char *key, char *value) {
	
	//checking arguments.
	assert (hashtable != NULL);
	assert (key != NULL);
	assert (value != NULL);

	//creating pair and using method add_pair.
	return hashtable_add_pair (hashtable, hashtable_create_pair (key, value));
}

unsigned int hashtable_add_pair (hashtable_t *hashtable, hashtable_pair_t pair) {
	
	//checking arguments.
	assert (hashtable != NULL);
	assert (hashtable_check_if_pair_is_good (&pair));
	
	
	//assigning to new pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	
	//check whether memory is allocated and there is place for one more pair.
	if (array == NULL) return TABLE_NOT_CRTD;
	if (hashtable_count (hashtable) >= hashtable_size (hashtable)) return SIZE_EXCEEDED;
	
	
	//i is first found hash of the key.
	unsigned int i = hashtable_get_hash_from_pair (hashtable, &pair);
	unsigned int hashtbl_size = hashtable_size (hashtable);
	
	
	//looking for first empty place to put new pair.
	while ((array[i].key != NULL) && (strcmp((array[i]).key, pair.key) != 0)) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
	}
	
	//placing pair into array, incrementing counter.
	if (array[i].key == NULL) {
		array[i].key = pair.key;
		array[i].value = pair.value;
		hashtable->count ++;
	}
	
	//returning hash id.
	return i;
}


char *hashtable_get_value_by_key (hashtable_t *hashtable, char *key) {
	
	//checking arguments.
	assert (hashtable != NULL);
	assert (key != NULL);
	
	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	assert (array != NULL);
	
	//getting first hash from key and saving it for future use (in order not to count hash again).
	unsigned int i = hashtable_get_hash_from_key (hashtable, key);
	unsigned int hash = i;
	
	//getting size of hashtable.
	unsigned int hashtbl_size = hashtable_size (hashtable);
	
	//var to understand when we cycled through all table.
	unsigned int runned_through_cycle = 0;
	
	//looking for value.
	while ((array[i].key != NULL) && (strcmp((array[i]).key, key) != 0) && runned_through_cycle == 0) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
		if (i == hash) runned_through_cycle = 1;
	}
	
	//returning NULL in cases of not found.
	if (runned_through_cycle == 1) return NULL;
	if (array[i].key == NULL) return NULL;
	
	
	/* Creating copy of Value to return. */
	
	//asserting value in array is not NULL.
	assert ((array[i]).value != NULL);
	
	//allocating memory for new value instance. Size is incremented because of '\0' character.
	char *value_copy = (char *) malloc (sizeof(char) * (strlen((array[i]).value) + 1));
	
	//checking if memory is successfully allocated.
	if (value_copy == NULL) {
		perror("Error while allocating memory for value.");
		return NULL;
	}
	
	//copying value.
	value_copy = strcpy(value_copy, (array[i]).value);
	
	return value_copy;
}

char *hashtable_get_value_by_id (hashtable_t *hashtable, unsigned int hash_id) {
	
	//checking arguments
	assert (hashtable != NULL);
	assert (hash_id < hashtable_size (hashtable));
	
	//creating array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	
	//check if array memory is allocated and there is pair for hash_id.
	if (array == NULL) return NULL;
	if (array[hash_id].key == NULL) return NULL;
	
	/* Creating copy of Value. */
	
	//asserting value in array is not NULL.
	assert ((array[hash_id]).value != NULL);
	
	//allocating memory.
	char *value_copy = (char *) malloc (sizeof(char) * (strlen((array[hash_id]).value) + 1));
	
	//checking if memory is successfully allocated.
	if (value_copy == NULL) {
		perror("Error while allocating memory for value.");
		return NULL;
	}
	
	//copying value.
	value_copy = strcpy(value_copy, (array[hash_id]).value);
	
	return value_copy;
}


hashtable_pair_t *hashtable_get_pair_by_key (hashtable_t *hashtable, char *key) {
	
	/* Using get_value_by_key method then creating pair. */
	
	//retreiving value.
	char *nvalue = hashtable_get_value_by_key (hashtable, key);
	if (nvalue == NULL) return NULL;

	//copying key.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(key) + 1));
	if (key_copy == NULL) {
		perror ("Error while creating copy of key instance.");
		return NULL;
	}
	key_copy = strcpy(key_copy, key);

	//creating pair to return.
	hashtable_pair_t *pair = (hashtable_pair_t *)malloc(sizeof(hashtable_pair_t));
	if (pair == NULL) {
		perror ("Error while allocating memory for new pair.");
		return NULL;
	}
	pair->key = key_copy;
	pair->value = nvalue;
	
	return pair;

}

hashtable_pair_t *hashtable_get_pair_by_id (hashtable_t *hashtable, unsigned int hash_id) {
	
	/* Using get_value_by_id method then creating pair. */

	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	assert (array != NULL);
	
	//getting value.
	char *nvalue = hashtable_get_value_by_id (hashtable, hash_id);
	if (nvalue == NULL) return NULL;
	
	//getting key.
	char *key = (array[hash_id]).key;
	if (key == NULL) return NULL;
	
	//copying key instance.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(key) + 1));
	if (key_copy == NULL) {
		perror ("Error while creating key copy");
		return NULL;
	}
	key_copy = strcpy(key_copy, key);
	
	//creating pair to return.
	hashtable_pair_t *pair = (hashtable_pair_t *)malloc(sizeof(hashtable_pair_t));
	if (pair == NULL) {
		perror ("Error while allocating memory for new pair.");
		return NULL;
	}
	pair->key = key_copy;
	pair->value = nvalue;
	
	return pair;
}


unsigned int hashtable_delete_pair_by_key (hashtable_t *hashtable, char *key) {
	
	//checking arguments
	if (hashtable == NULL) return TABLE_NOT_CRTD;
	assert (key != NULL);
	
	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	if (array == NULL) return TABLE_NOT_CRTD;
	
	/* Looking for pair in the same manner as when adding */
	
	unsigned int i = hashtable_get_hash_from_key (hashtable, key);
	unsigned int hash = i;
	unsigned int hashtbl_size = hashtable_size (hashtable);
	unsigned int runned_through_cycle = 0;
	
	while ((array[i].key != NULL) && (strcmp((array[i]).key, key) != 0) && runned_through_cycle == 0) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
		if (i == hash) runned_through_cycle = 1;
	}
	
	//if no pair found, return 1
	if (runned_through_cycle == 1) return 1;
	if (array[i].key == NULL) return 1;
	
	//freeing allocated memory and deleting.
	free(array[i].key);
	free(array[i].value);
	array[i].key = NULL;
	array[i].value = NULL;
	hashtable->count --;
	
	return 0;
}


unsigned int hashtable_delete_pair_by_id (hashtable_t *hashtable, unsigned int hash_id) {
	
	//checking arguments.
	if (hashtable == NULL) return TABLE_NOT_CRTD;
	assert (hash_id < hashtable_size (hashtable));
	
	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	if (array == NULL) return TABLE_NOT_CRTD;
	
	//if there is no pair by id, return 1.
	if (array[hash_id].key == NULL) return 1;
	
	//freeing allocated memory and deleting.
	free(array[hash_id].key);
	free(array[hash_id].value);
	array[hash_id].key = NULL;
	array[hash_id].value = NULL;
	hashtable->count --;
	
	return 0;

}


hashtable_pair_t hashtable_create_pair (char *nkey, char *nvalue) {
	
	//checking arguments.
	assert (nkey != NULL);
	assert (nvalue != NULL);
	
	//allocating memory for copies of key and value.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(nkey) + 1));
	char *value_copy = (char *) malloc (sizeof(char) * (strlen(nvalue) + 1));
	if (key_copy == NULL || value_copy == NULL) {
		perror ("Error while allocating memory for copy of key or value while creating pair.");
	}
	
	//copying key and value.
	key_copy = strcpy(key_copy, nkey);
	value_copy = strcpy(value_copy, nvalue);
	
	//creating pair.
	hashtable_pair_t a = {key_copy, value_copy};
	
	return a;
}


unsigned int hashtable_get_hash_from_pair (hashtable_t *hashtable, hashtable_pair_t *pair) {
	
	//checking arguments.
	assert (hashtable != NULL);
	assert (hashtable_check_if_pair_is_good (pair));
	
	//just getting hash from key
	return hashtable_get_hash_from_key (hashtable, pair->key);
}

				 

unsigned int hashtable_get_hash_from_key (hashtable_t *hashtable, char *key) {
	
	//checking arguments.
	assert (hashtable != NULL);
	assert (key != NULL);
	
	/* Counting hash value using polynom 2*x^2 - x + 3. 
	 hashval must be less then size of hashtable. */
	unsigned int hashval = 0;
	unsigned int i = 0;
	while (key[i] != 0) {
		hashval += key[i]*key[i]*2 - key[i] + 3;
		i++;
	}
	hashval %= hashtable_size(hashtable);
	
	return hashval;
}


unsigned int hashtable_check_if_pair_is_good (hashtable_pair_t *pair) {
	assert (pair != NULL);
	assert (pair->key != NULL);
	assert (pair->value != NULL);
	return 1;
}

unsigned int hashtable_delete_table (hashtable_t *hashtable) {
	//checking arguments.
	assert (hashtable != NULL);
	assert (hashtable->arr != NULL);
	
	hashtable_pair_t *array = hashtable->arr;
	
	//freeing all keys-value pairs.
	unsigned int size = hashtable_size (hashtable);
	for (int i = 0; i < size; i++) {
		if ((array[i]).key != NULL) {
			free ((array[i]).key);
			free ((array[i]).value);
			(array[i]).key = NULL;
			(array[i]).value = NULL;
		}
	}
	
	//free memory allocated for array.
	free (array);
	hashtable->arr = NULL;
	
	//free memory allocated for hashtable.
	free (hashtable);
	
	return 0;
}
