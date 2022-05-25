/*Pokemon.h - Pokemon module header file
 this module defines a Pokemon and implements some Pokemon manipulation functions.*/
#ifndef POKEMON_H
#define POKEMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

typedef struct Type_t
{
	char *name;
	int num;
	struct Type_t **p_against_me;
	struct Type_t **p_against_others;
	int len_me;
	int len_others;
} Type;


typedef struct Bio_t
{
	float height;
	float weight;
	int attack;
} Bio;


typedef struct Pokemon_t
{
	char *p_name;
	char *p_species;
	Type *p_type;
	Bio *p_bio;
} Pokemon;

//creates pokemon and returns pointer to the pokemon's object, if fails returns NULL
Pokemon* Create_Pokemon(char *pname, char *pspecies, Type *ptype, float height, float weight, int attack);
//deletes pokemon and return if it has succeed
status Remove_Pokemon(Pokemon* p_pokemon);
//creates type of pokemon and returns pointer to the type object, if fails returns NULL
Type* Create_Type(char *pname);
//deletes type of pokemon and returns if it has succeed
status Remove_Type(Type* pType);
//creates biological info to pokemon and returns pointer to the biological info object. if fails returns NULL
Bio* Create_Bio(float height, float weight, int attack);
//deletes biological info of pokemon and returns if it has succeed
status Remove_Bio(Bio* pbio);
//adding one type of pokemon to the effective against me array of another type
status Add_against_me(Type *pA,Type *pB);
//adding one type of pokemon to the effective against others array of another type
status Add_against_others(Type *pA,Type *pB);
//returns if the type is the same as the type we're looking for
bool find_type(struct Type_t *type, char *pnameB);
//removes a type of pokemon from some other type's array "effective against me" and returns a status if it has succeed
status Remove_against_me(Type *pA, char *pnameB);
//removes a type of pokemon from some other type's array "effective against others" and returns a status if it has succeed
status Remove_against_others(Type *pA, char *pnameB);
//prints the details of the pokemon and returns a status if it has succeed
status Print_pokemon(Pokemon *p_pokemon);
//prints the details of the pokemon's type and returns a status if it has succeed
status Print_type(Type *ptype);

#endif
