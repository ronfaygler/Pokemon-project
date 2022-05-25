#include "MaxHeap.h"
#include "LinkedList.h"
#include "BattleByCategory.h"
#include "Pokemon.h"

status castRemovePok(element elem){
	Pokemon *Pok = (Pokemon*) elem;
	return Remove_Pokemon(Pok);
}

status castPrintPok(element elem){
	Pokemon *Pok = (Pokemon*) elem;
	return Print_pokemon(Pok);
}

int equalPokemon(element elem1, element elem2){
	if (elem1 == NULL || elem2 == NULL){
		return -2;
	}
	Pokemon *pok1 = (Pokemon*)elem1;
	Pokemon *pok2 = (Pokemon*)elem2;
	if (pok1->p_bio->attack > pok2->p_bio->attack){
		return 1;
	}
	else if(pok1->p_bio->attack < pok2->p_bio->attack){
		return -1;
	}
	else{
		return 0;
	}
}

element copyPokemon(element elem){
	if (elem == NULL){
		return NULL;
	}
	Pokemon *oldPok = (Pokemon*)elem;
	Pokemon *p_pokemon = (Pokemon*) malloc(sizeof (Pokemon));
	 if (p_pokemon == NULL){
		return NULL;
	 }
	 p_pokemon->p_name = (char *) malloc(strlen(oldPok->p_name)+1);
	 if (p_pokemon->p_name == NULL){
		 free(p_pokemon);
		 return NULL;
	 }
	 strcpy(p_pokemon->p_name, oldPok->p_name);

	 p_pokemon->p_species = (char *) malloc(strlen(oldPok->p_species)+1);
	 if (p_pokemon->p_species == NULL){
		 free(p_pokemon->p_name);
		 free(p_pokemon);
		 return NULL;
	 }
	 strcpy(p_pokemon->p_species, oldPok->p_species);
	 Bio *pbio = Create_Bio(oldPok->p_bio->height, oldPok->p_bio->weight, oldPok->p_bio->attack);
	 if (pbio == NULL){
		 free(p_pokemon->p_name);
		 free(p_pokemon->p_species);
		 free(p_pokemon);
		 return NULL;
	 }
	 p_pokemon->p_bio = pbio;
	 p_pokemon->p_type = oldPok->p_type;
	 p_pokemon->p_type->num ++;
	 return (element)p_pokemon;
}

char *getCategoryPok(element elem){
	if (elem == NULL){
		return NULL;
	}
	Pokemon *pok = (Pokemon*)elem;
	char *newTypeName = (char *) malloc(strlen(pok->p_type->name)+1);
	strcpy(newTypeName, pok->p_type->name);
	return newTypeName;
}

int getAttack(element firstElem , element secondElem, int* attackFirst, int *attackSecond){
	if (firstElem == NULL || secondElem == NULL || attackFirst == NULL || attackSecond == NULL){
		return -1000000000;
	}
	Pokemon *pok1 = (Pokemon*)firstElem;
	Pokemon *pok2 = (Pokemon*)secondElem;
	*attackFirst = pok1->p_bio->attack;
	*attackSecond = pok2->p_bio->attack;
	for (int i=0; i < pok1->p_type->len_me; i++){
		if (strcmp(pok1->p_type->p_against_me[i]->name, pok2->p_type->name) == 0){
			*attackFirst -= 10;
			break;
		}
	}
	for (int i=0; i < pok2->p_type->len_me; i++){
		if (strcmp(pok2->p_type->p_against_me[i]->name, pok1->p_type->name) == 0){
			*attackSecond -= 10;
			break;
		}
	}
	return *attackFirst-*attackSecond;
}

int main(int argc, char *argv[])
{
	char *numberOfTypes = argv[1];
	char *p_num_types;
	long num_of_types;
	num_of_types = strtol(numberOfTypes, &p_num_types, 10);
	char *capacityInType = argv[2];
	char *p_capacity;
	long capacityType;
	capacityType = strtol(capacityInType, &p_capacity, 10);
	FILE *fp;
	char row[300];
	fp = fopen(argv[3], "r");
	char str[1];
	str[0] = 'n';
	char limit[] = ",\n\r";
	char limit2[] = "\t\r\n-:, ";
	char limit3[] = "\r\n,";
	char *p_typename;
	char *curr;
	char *p_typename_B;
	char *who;
	Type *p_typeA;
	Type *p_typeB;
	Type* types_arr[num_of_types];
	Type **type_arr = types_arr;
	char categories[300];
	int type_arr_len = num_of_types;
	Battle myBattle = NULL;
	char *p_pok_name;
	char *p_pok_species;
	float pok_height;
	float pok_weight;
	int pok_attack;
	char *p_pok_type_name;
	Type *p_pok_type;
	Pokemon *Pok;
	int pok_word = 0;
	status check;
	int curr_pok = 0;

	while (fgets(row, 300, fp)){
		if (str[0] == 'n'){
			if (strcmp(row, "Types\r\n") == 0 || strcmp(row, "Types\n") == 0){
				str[0] = 't'; //"t" = the next row is types
				continue;
			}
		}
		if (str[0] == 't'){ //we're in the line of the types names
			if(strcmp(row, "Pokemons\r\n") == 0 || strcmp(row, "Pokemons\n") == 0 || strcmp(row, "Pokemons") == 0){
				str[0] = 'p'; //"p" = the next row is Pokemons
				continue;
			}
			strcpy(categories, row);
			p_typename = strtok(row, limit);
			int curr_type = 0;
			while(p_typename != NULL){ //create types
				p_typeA = Create_Type(p_typename);
				if (p_typeA == NULL){ //memory problem
					if (curr_type != 0){ // types got memory before this one
						for (int i=0; i<curr_type; i++){
							check = Remove_Type(type_arr[i]);
							if (check == success){
								type_arr[i] = NULL;
							}
						}
					}
					fclose(fp);
					return 0;
				}
				type_arr[curr_type] = p_typeA;
				p_typename = strtok(NULL, limit);
				curr_type++;
			}
			myBattle = createBattleByCategory(capacityType, num_of_types, categories, equalPokemon,
					copyPokemon, castRemovePok, getCategoryPok, getAttack, castPrintPok);
			if (myBattle == NULL){
				for (int i=0; i<type_arr_len; i++){ //delete types
					check = Remove_Type(type_arr[i]);
					if (check == success){
						type_arr[i] = NULL;
					}
				}
				printf("No memory available.\n");
				fclose(fp);
				return 0;
			}
			str[0] = 'e'; // the next row if effective
			continue;
		}
		else if(strcmp(row, "Pokemons\r\n") == 0 || strcmp(row, "Pokemons\n") == 0){
			str[0] = 'p'; //"p" = the next row is Pokemons
			continue;
		}
		else if(str[0] == 'e'){
			curr = strtok(row, limit2);
			p_typename = curr;
			int param = 2;
			while(curr != NULL){
				if (param == 5){
					who = curr;
					curr = strtok(NULL, limit2);
				}
				else if(param > 5){
					p_typename_B = curr; //the type we need to add to array
					for (int j = 0; j<type_arr_len; j++){
						if (strcmp(p_typename,type_arr[j]->name) == 0){
							p_typeA = type_arr[j];
							break;
						}
					}
					for (int j = 0; j<type_arr_len; j++){
						if (strcmp(p_typename_B, type_arr[j]->name)==0){
							p_typeB = type_arr[j];
							break;
						}
					}
					if (strcmp(who, "me") == 0){
						check = Add_against_me(p_typeA, p_typeB);
						if (check == failure){ //memory problem
							check = destroyBattleByCategory(myBattle); //delete battle
							if (check == success){
								myBattle = NULL;
							}
							for (int i=0; i<type_arr_len; i++){ //delete types
								check = Remove_Type(type_arr[i]);
								if (check == success){
									type_arr[i] = NULL;
								}
							}
							printf("No memory available.\n");
							fclose(fp);
							return 0;
						}
					}
					else{
						check = Add_against_others(p_typeA, p_typeB);
						if (check == failure){ //memory problem
							check = destroyBattleByCategory(myBattle); //delete battle
							if (check == success){
								myBattle = NULL;
							}
							for (int i=0; i<type_arr_len; i++){ //delete types
								check = Remove_Type(type_arr[i]);
								if (check == success){
									type_arr[i] = NULL;
								}
							}
							printf("No memory available.\n");
							fclose(fp);
							return 0;
						}
					}
					curr = strtok(NULL, limit2);
				}
				else{
					curr = strtok(NULL, limit2);
				}
				param++;
			}
		}
		else if(str[0] == 'p'){
			curr = strtok(row, limit3);
			p_pok_name = curr;
			while(curr != NULL){

				if (pok_word == 1){
						p_pok_species = curr;
					}
				if (pok_word == 2){
					pok_height = atof(curr);
				}
				if (pok_word == 3){
					pok_weight = atof(curr);
				}
				if (pok_word == 4){
					pok_attack = atoi(curr);
				}
				if (pok_word == 5){
					p_pok_type_name = curr;
				}
				curr = strtok(NULL, limit3);
				pok_word++;
			}
			for (int i=0; i<type_arr_len; i++){
				if (strcmp(type_arr[i]->name, p_pok_type_name) == 0){
					p_pok_type = type_arr[i];
					break;
				}
			}
			Pok = Create_Pokemon(p_pok_name, p_pok_species, p_pok_type, pok_height, pok_weight, pok_attack);
			check = insertObject(myBattle, Pok);
			if (check == failure){ //memory problem
				check = destroyBattleByCategory(myBattle); //delete battle
				if (check == success){
					myBattle = NULL;
				}
				for (int i=0; i<type_arr_len; i++){ //delete types
					check = Remove_Type(type_arr[i]);
					if (check == success){
						type_arr[i] = NULL;
					}
				}
				printf("No memory available.\n");
				fclose(fp);
				return 0;
			}

			check = Remove_Pokemon(Pok);
			if (check == success){
				Pok = NULL;
			}
			pok_word = 0;
			curr_pok ++;
		}
	}
	fclose(fp);

	int number = 1;
	char ch = '1';
	char pokTypeName[300];
	Type *type;
	char pokName[300];
	char pokSpecies[300];
	float pokHeight;
	float pokWeight;
	int pokAttack;
	Pokemon *newPok;

	Type *type_exist(char *type_name, Type* type){ //check if type exist in the program
		for (int i=0; i<type_arr_len; i++){
			if (find_type(type_arr[i], type_name)){
				type = type_arr[i];
				return type;
			}
		}
		return NULL;
	}

	while (number != 6){
		printf("Please choose one of the following numbers:\n");
		printf("1 : Print all Pokemons by types\n");
		printf("2 : Print all Pokemons types\n");
		printf("3 : Insert Pokemon to battles training camp\n");
		printf("4 : Remove strongest Pokemon by type\n");
		printf("5 : Fight\n");
		printf("6 : Exit\n");
		scanf("%s[1]", &ch);
		number = atoi(&ch);
		switch( number )
		{
			case 1: //print all pokemons
					displayObjectsByCategories(myBattle);
					break;

			case 2: //print all pokemons types
					for (int i=0; i<type_arr_len; i++){
						Print_type(type_arr[i]);
					}
					break;

			case 3: //Insert Pokemon to battles training camp
					printf("Please enter Pokemon type name:\n");
					scanf("%s", pokTypeName);
					if (type_exist(pokTypeName, type) == NULL){ //type doesn't exist
						printf("Type name doesn't exist.\n");
						break;
					}
					else{ //type exist
						type = type_exist(pokTypeName, type);
						if(type->num == capacityType){
							printf("Type at full capacity.\n");
							break;
						}
						printf("Please enter Pokemon name:\n");
						scanf("%s", pokName);
						printf("Please enter Pokemon species:\n");
						scanf("%s", pokSpecies);
						printf("Please enter Pokemon height:\n");
						scanf("%f", &pokHeight);
						printf("Please enter Pokemon weight:\n");
						scanf("%f", &pokWeight);
						printf("Please enter Pokemon attack:\n");
						scanf("%d", &pokAttack);
						newPok = Create_Pokemon(pokName, pokSpecies, type, pokHeight, pokWeight, pokAttack);
						check = insertObject(myBattle, newPok);
						if (check == failure){ //memory problem
							check = destroyBattleByCategory(myBattle); //delete battle
							if (check == success){
								myBattle = NULL;
							}
							for (int i=0; i<type_arr_len; i++){ //delete types
								check = Remove_Type(type_arr[i]);
								if (check == success){
									type_arr[i] = NULL;
								}
							}
							printf("No memory available.\n");
							return 0;
						}
						printf("The Pokemon was successfully added.\n");
						Print_pokemon(newPok);
						check = Remove_Pokemon(newPok);
						if (check == success){
							newPok = NULL;
						}
					}
					break;

			case 4: //Remove strongest Pokemon by type
					printf("Please enter type name:\n");
					scanf("%s", pokTypeName);
					if (type_exist(pokTypeName, type) == NULL){ //type doesn't exist
						printf("Type name doesn't exist.\n");
						break;
					}
					else{ //type exist
						type = type_exist(pokTypeName, type);
						if (type->num == 0){
							printf("There is no Pokemon to remove.\n");
							break;
						}
						element maxPokByType = removeMaxByCategory(myBattle, pokTypeName);
						if (maxPokByType == NULL){ //memory problem
							check = destroyBattleByCategory(myBattle); //delete battle
							if (check == success){
								myBattle = NULL;
							}
							for (int i=0; i<type_arr_len; i++){ //delete types
								check = Remove_Type(type_arr[i]);
								if (check == success){
									type_arr[i] = NULL;
								}
							}
							printf("No memory available.\n");
							return 0;
						}
						printf("The strongest Pokemon was removed:\n");
						Print_pokemon((Pokemon *) maxPokByType);
						check = Remove_Pokemon((Pokemon *) maxPokByType);
						if (check == success){
							maxPokByType = NULL;
						}
					}
					break;

			case 5: //Fight
					printf("Please enter Pokemon type name:\n");
					scanf("%s", pokTypeName);
					if (type_exist(pokTypeName, type) == NULL){ //type A doesn't exist
						printf("Type name doesn't exist.\n");
						break;
					}
					else{ //type exist
						type = type_exist(pokTypeName, type);
						printf("Please enter Pokemon name:\n");
						scanf("%s", pokName);
						printf("Please enter Pokemon species:\n");
						scanf("%s", pokSpecies);
						printf("Please enter Pokemon height:\n");
						scanf("%f", &pokHeight);
						printf("Please enter Pokemon weight:\n");
						scanf("%f", &pokWeight);
						printf("Please enter Pokemon attack:\n");
						scanf("%d", &pokAttack);
						newPok = Create_Pokemon(pokName, pokSpecies, type, pokHeight, pokWeight, pokAttack);
						element selected = fight(myBattle, (element)newPok);
						if (selected != NULL){
							check = Remove_Pokemon((Pokemon *)selected);
							if (check == success){
								selected = NULL;
							}
						}
						else{
							int numOfPoks = 0;
							for (int i=0; i < num_of_types; i++){
								numOfPoks += getNumberOfObjectsInCategory(myBattle, type_arr[i]->name);
							}
							if (numOfPoks != 0){ //memory problem
								check = destroyBattleByCategory(myBattle); //delete battle
								if (check == success){
									myBattle = NULL;
								}
								for (int i=0; i<type_arr_len; i++){ //delete types
									check = Remove_Type(type_arr[i]);
									if (check == success){
										type_arr[i] = NULL;
									}
								}
								printf("No memory available.\n");
								return 0;
							}
							printf("There are no Pokemons to fight against you.\n");
							check = Remove_Pokemon((Pokemon *)newPok);
							if (check == success){
								newPok = NULL;
							}
						}
					}
					break;

			case 6: //Exit
					check = destroyBattleByCategory(myBattle); //delete battle
					if (check == success){
						myBattle = NULL;
					}
					for (int i=0; i<type_arr_len; i++){ //delete types
						check = Remove_Type(type_arr[i]);
						if (check == success){
							type_arr[i] = NULL;
						}
					}

					printf("All the memory cleaned and the program is safely closed.\n");
					break;

			default:
					printf("Please choose a valid number.\n");
					break;
		}
	}
		return 0;
}


