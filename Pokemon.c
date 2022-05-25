#include "Pokemon.h"

 Pokemon* Create_Pokemon(char *pname, char *pspecies, Type *ptype, float height, float weight, int attack) {
	 if (pname == NULL || pspecies == NULL || ptype == NULL){
		return NULL;
	}
	 Pokemon *p_pokemon = (Pokemon*) malloc(sizeof (Pokemon));
	 if (p_pokemon == NULL){
		return NULL;
	 }
	 p_pokemon->p_name = (char *) malloc(strlen(pname)+1);
	 if (p_pokemon->p_name == NULL){
		 free(p_pokemon);
		 return NULL;
	 	 }
	 strcpy(p_pokemon->p_name, pname);

	 p_pokemon->p_species = (char *) malloc(strlen(pspecies)+1);
	 if (p_pokemon->p_species == NULL){
		 free(p_pokemon->p_name);
		 free(p_pokemon);
		 return NULL;
	 }
	 Bio *pbio = Create_Bio(height, weight, attack);
	 if (pbio == NULL){
		 free(p_pokemon->p_name);
		 free(p_pokemon->p_species);
		 free(p_pokemon);
		 return NULL;
	 }
	 p_pokemon->p_bio = pbio;
	 strcpy(p_pokemon->p_species, pspecies);
	 p_pokemon->p_type = ptype;
	 p_pokemon->p_type->num ++;
	 return p_pokemon;
}

 status Remove_Pokemon(Pokemon* p_pokemon){
	 if (p_pokemon == NULL){
		 return failure;
	 }
	 p_pokemon->p_type->num --;
	 free(p_pokemon->p_name);
	 free(p_pokemon->p_species);
	 status check;
	 check = Remove_Bio(p_pokemon->p_bio);
	 if (check == failure){
		 return failure;
	 }
	 free(p_pokemon);
	 return success;
 }

 //Initializes a type, allocates memory for the type, and return a pointer to the new type. pass it the type name as in the instructions.
Type* Create_Type(char *pname){
	if (pname == NULL){
		return NULL;
	}
	Type *pType = (Type*) malloc(sizeof (Type));
	if (pType == NULL){
		return NULL;
	}
	//reallocate the memory for the type's name to it's length plus one for /0.
	pType->name = (char *) malloc(strlen(pname)+1);
	if (pType->name == NULL){
		free(pType);
		return NULL;
	}
	strcpy(pType->name, pname);
	pType->p_against_me = (struct Type_t**) malloc(0);
	if (pType->p_against_me == NULL){
		free(pType->name);
		free(pType);
		return NULL;
	}
	pType->p_against_others = (struct Type_t**) malloc(0);
	if (pType->p_against_others == NULL){
		free(pType->p_against_me);
		free(pType->name);
		free(pType);
		return NULL;
	}
	pType->num = 0;
	pType->len_me = 0;
	pType->len_others = 0;
	return pType;
}

status Remove_Type(Type* pType){
	if (pType == NULL){
		return failure;
	}
	free(pType->name);
	free(pType->p_against_me);
	free(pType->p_against_others);
	free(pType);
	return success;
}

Bio* Create_Bio(float height, float weight, int attack){
	Bio *pbio = (Bio*) malloc(sizeof(Bio));
	if (pbio == NULL){
		return NULL;
	}
	pbio->height = height;
	pbio->weight = weight;
	pbio->attack = attack;
	return pbio;
}

status Remove_Bio(Bio* pbio){
	if (pbio == NULL){
		return failure;
	}
	free(pbio);
	return success;
}

status Add_against_me(Type *pA,Type *pB){
	if (pA == NULL || pB == NULL){
		return failure;
	}
	if (pA->len_me == 0){ //the array is empty
		pA->p_against_me = (struct Type_t**) realloc(pA->p_against_me, sizeof(struct Type_t*));
		if (pA->p_against_me == NULL){
			return failure;
		}
		pA->p_against_me[0] = (struct Type_t*)pB;
		pA->len_me ++;
	}
	else{
		//reallocate the memory for the effective against me array, make it bigger for one more type of pokemon.
		pA->p_against_me = (struct Type_t**) realloc(pA->p_against_me, (pA->len_me+1)*sizeof(struct Type_t*));
		if (pA->p_against_me == NULL){
			return failure;
		}
		pA->p_against_me[pA->len_me] = (struct Type_t*)pB;
		pA->len_me++;
	}
	return success;
}

status Add_against_others(Type *pA,Type *pB){
	if (pA == NULL || pB == NULL){
		return failure;
	}
	if(pA->len_others == 0){ //the array is empty
		pA->p_against_others = (struct Type_t**) realloc(pA->p_against_others, sizeof(struct Type_t*));
		if (pA->p_against_others == NULL){
			return failure;
		}
		pA->p_against_others[0] = (struct Type_t*)pB;
		pA->len_others ++;
	}
	else{
		//reallocate the memory for the effective against others array, make it bigger by one more type of pokemon.
		pA->p_against_others = (struct Type_t**) realloc(pA->p_against_others, (pA->len_others+1)*sizeof(struct Type_t*));
		if (pA->p_against_others == NULL){
			return failure;
		}
		pA->p_against_others[pA->len_others] = (struct Type_t*)pB;
		pA->len_others++;
	}
	return success;
}


bool find_type(struct Type_t *type, char *pnameB){
	if (type == NULL || pnameB == NULL){
		return false;
	}
	if (strcmp(type->name, pnameB) == 0){
		return true;
	}
		return false;
}


status Remove_against_me(Type *pA, char *pnameB){
	if (pA == NULL || pnameB == NULL){
		return failure;
	}
	//in case the array (effective against me) is empty, we know that the name we got is not in there
	if (pA->len_me == 0){
		return failure;
	}
	for (int i=0; i < pA->len_me; i++){
		//check if the name we got is the same as the name of the current type in the array
		if (find_type(pA->p_against_me[i], pnameB)){
			if (pA->len_me == 1){
				//we delete the only type from the array, so we can let free the memory of this array
				free(pA->p_against_me);
				pA->len_me = 0;
			}
			else{
				/*there is more then one type in the array, so we take the types that are after the one we want to delete,
				one step back in the array*/
				for (int j=i; j <  pA->len_me-1; j++){
				pA->p_against_me[j] = pA->p_against_me[j+1];
				}
				pA->len_me--;
				//reallocate the memory for the array, make it smaller by one, because we have deleted an object
				pA->p_against_me = (struct Type_t**) realloc(pA->p_against_me, (pA->len_me)*sizeof(struct Type_t*));
				if (pA->p_against_me == NULL){
					return memory;
				}
			}
			return success;
		}
	}
	return failure;

}

status Remove_against_others(Type *pA, char *pnameB){
	if (pA == NULL || pnameB == NULL){
		return failure;
	}
	//in case the array (effective against others) is empty, we know that the name we got is not in there
	if (pA->len_others == 0){
			return failure;
	}
	for (int i=0; i < pA->len_others; i++){
		//check if the name we got is the same as the name of the current type in the array
		if (find_type(pA->p_against_others[i], pnameB)){
			if (pA->len_others == 1){
				//we delete the only type from the array, so we can let free the memory of this array
				free(pA->p_against_others);
				pA->len_others = 0;

			}
			else{
				/*there is more then one type in the array, so we take the types that are after the one we want to delete,
				one step back in the array*/
				for (int j=i; j <  pA->len_others-1; j++){
					pA->p_against_others[j] = pA->p_against_others[j+1];
				}
				pA->len_others--;
				//reallocate the memory for the array, make it smaller by one, because we have deleted an object
				pA->p_against_others = (struct Type_t**) realloc(pA->p_against_others, (pA->len_others)*sizeof(struct Type_t*));
				if (pA->p_against_others == NULL){
					return memory;
				}
			}
			return success;
		}
	}
	return failure;
}

status Print_pokemon(Pokemon *p_pokemon){
	if (p_pokemon == NULL){
		return failure;
	}
	printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n",
			p_pokemon->p_name, p_pokemon->p_species, p_pokemon->p_type->name, p_pokemon->p_bio->height,
			p_pokemon->p_bio->weight, p_pokemon->p_bio->attack);
	return success;
}

status Print_type(Type *ptype){
	if (ptype == NULL){
		return failure;
	}
	printf("Type %s -- %d pokemons",ptype->name, ptype->num);
	if (ptype->len_me != 0){ //the array isn't empty
		printf("\n\tThese types are super-effective against %s:%s", ptype->name, ptype->p_against_me[0]->name);
		for (int i=1; i < ptype->len_me; i++){
			printf(" ,%s", ptype->p_against_me[i]->name);
		}
	}
	if (ptype->len_others != 0){ //the array isn't empty
		printf("\n\t%s moves are super-effective against:%s", ptype->name, ptype->p_against_others[0]->name);
		for (int i=1; i < ptype->len_others; i++){
			printf(" ,%s", ptype->p_against_others[i]->name);
		}
	}
	printf("\n\n");
	return success;
}







