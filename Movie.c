/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID){

	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	//Check if the User Already Exists
	if(existsUser(userID))
	{
		printf("User with userID: %d Already exists\n", userID);
		return -1;
	}

	user_t *newUser = makeNewUser(userID);
	if(!newUser)
	{
		printf("Malloc failed upon user memory allocation\n");
		return -2;
	} 

	// Generate Key using a call to the hash function
	int key = hash_function(a, b, userID, m, p);

	// Access Last User on appropriate Hash Table cell chain and add newUser
	user_t *user;

	// Check if Hash Table cell is empty
	if(user_hashtable_p[key] == NULL)
	{
		user_hashtable_p[key] = newUser;

		// Print statements
		printf("R %d\n", userID);
		printf("Chain %d of Users:\n   %d\n", key, newUser->userID);
		printf("DONE\n\n");
		return 1;
	}

	// Otherwise find chain tail
	for(user = user_hashtable_p[key]; user->next != NULL; user = user->next);
	user->next = newUser;

	// Print statements
	printf("R %d\n", userID);
	printf("Chain %d of Users:\n", key);
	for(user = user_hashtable_p[key]; user != NULL; user = user->next)
	{
		printf("   %d\n", user->userID);
	}
	printf("DONE\n\n");

	 return 1; // Success
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID){

	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	//Check if the User Already Exists
	if(!existsUser(userID))
	{
		printf("User with userID: %d does not exist. Can't be unregistered\n", userID);
		return -1;
	}

	// Generate Key using a call to the hash function
	int key = hash_function(a, b, userID, m, p);

	// Access Last User on appropriate Hash Table cell chain and add newUser
	user_t *user = user_hashtable_p[key], *prev = user_hashtable_p[key];

	// Identify user
	while(user->userID != userID)
	{
		prev = user;
		user = user->next;
	}
	freeUserHistoryTree(user->history);
	user->history = NULL;;

	if(user == user_hashtable_p[key]) // User is root
	{
		if(user->next == NULL) // User is only score in user chain
		{
			free(user);
			user_hashtable_p[key] = NULL;
		}
		else
		{
			user_hashtable_p[key] = user->next;
			free(user);
		}
	}
	else // User is not root
	{
		if(user->next == NULL) // User is last in chain
		{
			prev->next = NULL;
			free(user);
		}
	}
	 return 1;
 }
 
 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

 int add_new_movie(int movieID, int category, int year){

	// Allocate new memory for newMovie and return 0 upon fialure accordingly
	newMovie_t *newMovie = makeNewMovie(movieID, category, year);
	if(newMovie == NULL)
	{
		printf("Error allocating memory for new movie\n");
		return 0;
	}

	// Check if newMoviesTree is empty
	if(newMoviesTree.root == NULL)
	{
		newMoviesTree.root = newMovie;
		printf("A %d %d %d\n", movieID, category, year);
		printf("New releases Tree:\n");
		printf("   new releases: %d\n", movieID);
		printf("DONE\n\n");
		return 1;
	}

	// Check if the NewMovie Already Exists
	if(existsNewMovie(movieID))
	{
		printf("Movie with MovieID: %d already exists in newMoviesTree\n", movieID);
		return -1;
	}

	// Check if the newMovie Already Exists in the corresponding tree of the category array
	if(newMovieExistsInCategoryArray(movieID, category))
	{
		printf("Movie with MovieID: %d already exists in Category Array Trees\n", movieID);
		return -1;
	}

	// Otherwise add the movie
	newMovie_t *curr_newMovie = newMoviesTree.root, *prev_newMovie = newMoviesTree.root;
	while(curr_newMovie != NULL)
	{
		if(curr_newMovie->Info->movieID > movieID)
		{
			prev_newMovie = curr_newMovie;
			curr_newMovie = curr_newMovie->lc;
		}
		else
		{
			prev_newMovie = curr_newMovie;
			curr_newMovie = curr_newMovie->rc;		
		}
	}
	if(prev_newMovie->Info->movieID > movieID) prev_newMovie->lc = newMovie;
	else prev_newMovie->rc = newMovie;
	printf("A %d %d %d\n", movieID, category, year);
	printf("New releases Tree:\n");
	printf("   new releases: ");
	InOrderPrintNewMovieTree(newMoviesTree.root);
	printf("\n");
	printf("DONE\n\n");
	return 1;
 }
 

 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 int distribute_movies(void){

	if(newMoviesTree.root == NULL)
	{
		printf("There are no movies to distribute\n");
		return 1;
	}
	int err = postOrderDistribute(newMoviesTree.root);
	if(err != 1)
	{
		printf("An error occured while distributing the movies\n");
		return 0;
	}
	// Set newMoviesTree.root to NULL to signify it's empty and not get repeated freeing attempts
	newMoviesTree.root = NULL;

	// Print results upon success
	printf("D\nMovie Category Array:\n");
	printf("   HORROR: ");
	inOrderMoviePrint(categoryArray[0]->movie);
	printf("\n   SCIENCE-FICTION: ");
	inOrderMoviePrint(categoryArray[1]->movie);
	printf("\n   DRAMA: ");
	inOrderMoviePrint(categoryArray[2]->movie);
	printf("\n   ROMANCE: ");
	inOrderMoviePrint(categoryArray[3]->movie);
	printf("\n   DOCUMENTARY: ");
	inOrderMoviePrint(categoryArray[4]->movie);
	printf("\n   COMEDY: ");
	inOrderMoviePrint(categoryArray[5]->movie);
	printf("\nDONE\n\n");
	return 1;
 }
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID,int category, int movieID, int score){

	// return -3 if score is not in range [1, 10]
	if(score < 1 || score > 10) return -3;
	
	// First call userTreeRoot(userID) to return the root of the tree corresponding to the particular user
	user_t *user;
	int succ = userFind(userID, &user);
	if(!succ) return 0; //return 0 when failing to find user

	// Having asserted that both the user exists and having gotten the user tree root edit the existing Movie to account for the new score
	int exists = editeMovie(movieID, score, category);
	if(!exists) return -1; // Return -1 if movie does not exist in categoryArray Trees

	// Movie Edited. Now create new usermovie to add to user history Tree
	succ = addMovieToUser(&user, category, movieID, score);
	if(!succ) return -2; // Return -2 if error occured while adding Movie to User History Tree

	// Print Statements upon success
	printf("W %d ", userID);
	switch (category)
	{
		case 0:
			printf("HORROR ");
			break;
		case 1:
			printf("SCIENCE-FICTION ");
			break;
		case 2:
			printf("DRAMA ");
			break;
		case 3:
			printf("ROMANCE ");
			break;
		case 4:
			printf("DOCUMENTARY ");
			break;
		case 5:
			printf("COMEDY ");
			break;
	}
	printf("%d %d\n", movieID, score);
	printf("History Tree of User %d:\n", userID);
	InOrderPrintHistoryTreeLeaves(user->history);
	printf("DONE\n\n");


	return 1;
 }
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, float score){

	movie_t **helperMovieArray;
	int n_movies = getNumberOfMovies();
	if(n_movies == 0)
	{
		printf("Movie Category Array is empty");
		return 0;
	}

	// Allocate memory for helperMovieArray
	helperMovieArray = (movie_t**)malloc(n_movies * sizeof(movie_t*));
	if(!helperMovieArray) return 0; // Return 0 upon failure to allocate helperMovieArray

	int max_index = 0;
	for(int categ = 0; categ < 6; categ++)
	{
		if(categoryArray[categ] != NULL)
		{
			inOrderAddBasedOnScore(categoryArray[categ]->movie, score, helperMovieArray, &max_index);
		}
	}

	// Having filled the helperMovieArray in up until the max_index, sort it using HeapSort
	heapSort(helperMovieArray, max_index);

	// Print Statements
	printf("F %d %f\n   ", userID, score);
	if(max_index == 0)
	{
		printf("  EMPTY\nDONE\n\n");
		free(helperMovieArray);
		return 1;
	}
	float mvScore;
	int mvID;
	for(int i=0; i < max_index; i++)
	{
		mvScore = (float)helperMovieArray[i]->Info->sumScore / (float)helperMovieArray[i]->Info->watchedCounter;
		mvID = helperMovieArray[i]->Info->movieID;
		printf("{%d %f}, ", mvID, mvScore);
	}
	printf("\nDONE\n\n");
	free(helperMovieArray);

	return 1;
 }
 
/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){

	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	//Check if the User Already Exists
	if(!existsUser(userID))
	{
		printf("User with userID: %d does not exist\n", userID);
		return -1;
	}

	// Generate Key using a call to the hash function
	int key = hash_function(a, b, userID, m, p);

	user_t *user;
	int succ = userFind(userID, &user);

	int scoreSum = 0, counter = 0;
	inOrderFindStats(user->history, &scoreSum, &counter);
	if(counter == 0)
	{
		printf("User with UserID: %d has not watched any movies\n", userID);
		return -2;
	}

	float m_score = (float)scoreSum / (float)counter;
	printf("Q %d %f\nDONE\n\n", userID, m_score);
	return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){

	// Assert that the passed in category is within the limits
	if(category >= 6 || category < 0) return -1;  // Return -1 for invalid category input argument

	// Check if associated Tree is empty or NULL
	if(categoryArray[category] == NULL) return 0;
	if(categoryArray[category]->movie->Info->movieID == -1) return 0;

	// Otherwise Traverse the Tree to find the movie
	movie_t *curr = categoryArray[category]->movie;
	while(curr->Info->movieID != -1 && curr->Info->movieID != movieID) // While the node is not the sentinel
	{
		if(curr->Info->movieID > movieID) curr = curr->lc; // Move Left
		else curr = curr->rc; // Move Right
	}

	if(curr->Info->movieID != -1)
	{
		printf("I %d ", curr->Info->movieID);
		// Print Genre according to int category
		switch (category) {
			case 0:
				printf("HORROR");
				break;
			case 1:
				printf("SCIENCE-FICTION");
				break;
			case 2:
				printf("DRAMA");
				break;
			case 3:
				printf("ROMANCE");
				break;
			case 4:
				printf("DOCUMENTARY");
				break;
			case 5:
				printf("COMEDY");
				break;
		}
		printf(" %d\nDONE\n\n", curr->Info->year);
		return 1;
	}
	else return 0;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	printf("M\n");
	printf("Movie Category Array:\n");
	for(int i=0; i<6; i++)
	{
		switch (i)
		{
			case 0:
				printf("   HORROR: ");
				break;
			case 1:
				printf("   SCIENCE-FICTION:");
				break;
			case 2:
				printf("   DRAMA: ");
				break;
			case 3:
				printf("   ROMANCE: ");
				break;
			case 4:
				printf("   DOCUMENTARY: ");
				break;
			case 5:
				printf("   COMEDY: ");
				break;
		}

		if(categoryArray[i] != NULL) inOrderMoviePrint(categoryArray[i]->movie);
		printf("\n");
	}
	printf("DONE\n\n");

	return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){

	printf("P\n");
	for(int key=0; key < m; key++)
	{
		printf("Chain %d of Users:\n", key);
		if(user_hashtable_p[key] != NULL)
		{
			for(user_t *user = user_hashtable_p[key]; user != NULL; user = user->next)
			{
				printf("   %d\n   History Tree:\n", user->userID);
				InOrderPrintUserMoviesEd2(user->history);
			}
		}
	}
	printf("DONE\n\n");
	return 1;
 }
 

/* Extra functions */

/**
 * @brief Implements the universal hash function given all parameters
 * @return int: the key
*/
int hash_function(int a, int b, int x, int m, int p)
{
	 int k = ((a*x + b) % p) % m;
	 return k;
}


/**
 * @brief Makes a new user of userID
 * @return user_t*: a pointer to the new user struct
*/
user_t* makeNewUser(int userID)
{
	user_t *newUser = (user_t*)malloc(sizeof(user_t));
	newUser->userID = userID;
	newUser->history = NULL;
	newUser->next = NULL;
	return newUser;
}


/**
 * @brief Initializes Hash Table with null Pointers
 * @return 1 on Success 0 on malloc failure
*/
int makeHashTable()
{
	user_hashtable_p = (user_t**)malloc(hashtable_size * sizeof(user_t*));
	if(!user_hashtable_p) return 0;
	return 1;
}


/**
 * @brief Initializes Hash Table with null Pointers
 * @return void
*/
void initHashTable()
{
	for(int i=0; i < hashtable_size; i++) user_hashtable_p[i] = NULL;
	return;
}


/**
 * @brief Deletes Hash Table
 * @return void
*/
void deleteHashTable()
{
	if(user_hashtable_p == NULL) return;
	for(int i=0; i<hashtable_size; i++)
	{
		deleteUserChainIter(user_hashtable_p[i]);
	}
	free(user_hashtable_p);
	return;
}


/**
 * @brief Deletes Hash Table cell chain
 * @return void
*/
void deleteUserChainIter(user_t *user)
{
	if(user == NULL) return;
	deleteUserChainIter(user->next);
	if(user->history != NULL)
	{
		freeUserHistoryTree(user->history);
		user->history = NULL;
	}
	free(user);
	return;
}


/**
 * @brief Checks if a given User exists
 * @return returns 1 if True, 0 if False
*/
int existsUser(int userID)
{
	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	// Generate Hash key
	int key = hash_function(a, b, userID, m, p);

	// If chain associated with key is empty user does not exist.
	if(user_hashtable_p[key] == NULL) return 0;

	// Iterate through the chain untill you either reach NULL or user with identical userID
	user_t *user;
	for(user = user_hashtable_p[key]; user != NULL && user->userID != userID; user=user->next);

	// Return True or False Accordingly
	if(user == NULL) return 0;
	return 1;
}



/**
 * @brief Initialize newMoviesTree with NULL root
 * @return returns void
*/
void initNewMoviesTree()
{
	newMoviesTree.root = NULL;
}


/**
 * @brief Create New Movie for NewMovies Tree
 * @return returns newMovie_t struct pointer
*/
newMovie_t* makeNewMovie(int movieID, int category, int year)
{
	// Allocate newMovie memory
	newMovie_t *newMovie = (newMovie_t *)malloc(sizeof(newMovie_t));
	if(!newMovie) return NULL; // Return NULL upon failure

	// Fill in parameters
	newMovie->category = category;
	newMovie->Info = (movieInfo_t *)malloc(sizeof(movieInfo_t));
	if(!newMovie->Info) return NULL;
	newMovie->Info->movieID = movieID;
	newMovie->Info->year = year;
	newMovie->Info->sumScore = 0;
	newMovie->Info->watchedCounter = 0;
	newMovie->lc = NULL;
	newMovie->rc = NULL;
	return newMovie;
}



/**
 * @brief Checks whether the given movie exists or not (using the MovieID)
 * @return returns 1 if True, 0 otherwise
*/
int existsNewMovie(int movieID)
{
	// If newMoviesTree is empty the movie does not exist
	if(newMoviesTree.root == NULL) return 0;

	// Perform recursive Tree Traversal to check if the requested movies exists
	int found = InOrderFoundNewMovieTree(newMoviesTree.root, movieID);
	if(found) return 1;
	return 0;
}


/**
 * @brief Recursive Function that traverses the newMoviesTree and checks if a given movie exists
 * @return returns 1 if Movie is found, 0 otherwise
*/
int InOrderFoundNewMovieTree(newMovie_t* newMovieNode, int movieID)
{
	if(newMovieNode == NULL) return 0;
	if(newMovieNode->Info->movieID == movieID) return 1;
	int foundLeft = InOrderFoundNewMovieTree(newMovieNode->lc, movieID);
	int foundRight = InOrderFoundNewMovieTree(newMovieNode->rc, movieID);
	return (int)(foundLeft || foundRight);
}


/**
 * @brief InOrder Traversal of newMovieTree that prints the movieIDs
 * @return returns 1 if Movie is found, 0 otherwise
*/
void InOrderPrintNewMovieTree(newMovie_t *newMovieNode)
{
	if(newMovieNode == NULL) return;
	InOrderPrintNewMovieTree(newMovieNode->lc);
	printf(" %d,", newMovieNode->Info->movieID);
	InOrderPrintNewMovieTree(newMovieNode->rc);
	return;
}



/**
 * @brief delete the newMoviesTree
 * @return returns void
*/
void deleteNewMoviesTree()
{
	if(newMoviesTree.root == NULL) return;
	deleteNewMoviesTreeIter(newMoviesTree.root);
	newMoviesTree.root = NULL;
}


/**
 * @brief delete the newMoviesTree recursive PostOrder function
 * @return returns void
*/
void deleteNewMoviesTreeIter(newMovie_t *newMovieNode)
{
	if(newMovieNode == NULL) return;
	deleteNewMoviesTreeIter(newMovieNode->lc);
	deleteNewMoviesTreeIter(newMovieNode->rc);
	if(newMovieNode->Info != NULL)
	{
		free(newMovieNode->Info);
		newMovieNode->Info = NULL;
	}
	free(newMovieNode);
}



/**
 * @brief Initializes the movie Category Array with sentinel nodes
 * @return returns void
*/
int initMovieCategoryArray()
{
	movie_t *sent = makeMovie(-1, 0);
	if(sent == NULL)
	{
		printf("Failure upon movie_t object memory allocation\n");
		return 0;
	}

	for(int  i=0; i<6; i++)
	{
		categoryArray[i] = (movieCategoryArray_t*)malloc(sizeof(movieCategoryArray_t));
		if(categoryArray[i] == NULL)
		{
			printf("Failure upon movieCategory_t object memory allocation\n");
			return 0;
		}

		categoryArray[i]->sentinel = sent;
		categoryArray[i]->movie = sent;
	}
	return 1;
}


/**
 * @brief Makes a regular movie_t struct object
 * @return returns 1 upon success, 0 upon malloc failure
*/
movie_t* makeMovie(int movieID, int year)
{
	movie_t *movie = (movie_t*)malloc(sizeof(movie_t));
	if(movie == NULL) return NULL;
	movieInfo_t *m_info = (movieInfo_t*)malloc(sizeof(movieInfo_t));
	if(m_info == NULL)
	{
		free(movie);
		return NULL;
	}
	movie->Info = m_info;
	movie->Info->movieID = movieID;
	movie->Info->sumScore = 0;
	movie->Info->watchedCounter = 0;
	movie->Info->year = year;
	movie->lc = NULL;
	movie->rc = NULL;

	return movie;
}


/**
 * @brief Performs inOrder newMovieTree traversal and for each node it visits it calls the function distsributeMovie()
 * that is responsible for creating and placing the corresponding movie in the appropriate BST of the movieCategoryArray
 * @note: IF A MOVIE TRANSFER FAILS WE HAVE TO STOP THE PROCESS. OTHERWISE WE MIGHT END UP WITH A CORRUPTED TREE 
 * @return returns void
*/
int postOrderDistribute(newMovie_t *newMovie)
{
	if(newMovie == NULL) return 1;
	int successLeft = postOrderDistribute(newMovie->lc);
	int successRight = postOrderDistribute(newMovie->rc);
	int successful_transfer;
	if(successLeft && successRight)
	{
		successful_transfer = DistributeMovie(newMovie);
		if(!successful_transfer)
		{
			printf("Movie Distributing Process has stopped at the node with MovieID: %d\n", newMovie->Info->movieID);
			newMovie->lc = NULL;
			newMovie->rc = NULL;
			return 0;
		} 
		else
		{
			if(newMovie->Info != NULL)
			{
				free(newMovie->Info);
				newMovie->Info = NULL;
			}
			free(newMovie);
			return 1;
		}
	}
	else return 0;
}



/**
 * @brief Distributes the given movie to the appropriate place and tree of the movieCategoryArray
 * @return returns 1 upon success, 0 upon failure
*/
int DistributeMovie(newMovie_t *newMovie)
{
	int category = newMovie->category;
	int movieID = newMovie->Info->movieID;
	int year = newMovie->Info->year;
	movie_t *temp = makeMovie(movieID, year);
	if(!temp) return 0;
	movie_t *root = categoryArray[category]->movie;
	movie_t *sentinel = categoryArray[category]->sentinel;

	// the BST for this category is empty. Create movie_t node and assign it to BST root
	if(root == sentinel)
	{
		categoryArray[category]->movie = temp;
		temp->lc = sentinel;
		temp->rc = sentinel;
		return 1;
	}

	// Otherwise traverse the Tree until you hit a sentinel
	movie_t *curr = root, *prev = root;
	while(curr != sentinel)
	{
		if(curr->Info->movieID > movieID)
		{
			prev = curr;
			curr = curr->lc;
		}
		else
		{
			prev = curr;
			curr = curr->rc;
		}
	}

	// Having traversed the tree we place the new Node
	if(prev->Info->movieID > movieID) // Place on the left
	{
		prev->lc = temp;
		temp->lc = sentinel;
		temp->rc = sentinel;
		return 1;
	}
	else // place on the right
	{
		prev->rc = temp;
		temp->rc = sentinel;
		temp->lc = sentinel;
		return 1;
	}

}


/**
 * @brief Checks if the movie already exists in the movie category array trees (Assuming movieID-category pair is correct)
 * @return returns 1 if True, 0 if False
*/
int newMovieExistsInCategoryArray(int movieID, int category)
{
	movie_t *root = categoryArray[category]->movie;
	movie_t *sentinel = categoryArray[category]->sentinel;

	// If the tree is empty the movie does not exist in it
	if(root == sentinel) return 0;

	// Otherwise traverse the tree until you either hit a sentinel or a movieID match node
	movie_t *curr = root;
	while(curr != NULL && curr->Info->movieID != movieID)
	{
		if(curr->Info->movieID > movieID)
		{
			curr = curr->lc;
		}
		else
		{
			curr = curr->rc;
		}
	}

	// Having traversed the tree check if curr is sentinel or a movieID match
	if(curr == sentinel) return 0; // It does not exist
	else return 1;
}


/**
 * @brief Prints the BSTs of the movie category array Inorder
 * @return returns void
*/
void inOrderMoviePrint(movie_t *movie)
{
	if(movie->Info->movieID == -1) return;
	inOrderMoviePrint(movie->lc);
	printf("%d, ", movie->Info->movieID);
	inOrderMoviePrint(movie->rc);
	return;
}


/**
 * @brief destroy movieCategoryArray and Subtrees
 * @return returns void
*/
void deleteMovieCategoryArray()
{
	for(int i=0; i<6; i++)
	{
		// If categoryArra[i] already freed or is empty skip for now
		if(categoryArray[i] == NULL) continue;

		// Otherwise
		PostOrderMovieDelete(categoryArray[i]->movie);
		categoryArray[i]->movie = NULL;
	}

	if(categoryArray[0]->sentinel != NULL)
	{
		if(categoryArray[0]->sentinel->Info != NULL)
		{
			free(categoryArray[0]->sentinel->Info);
			categoryArray[0]->sentinel->Info = NULL;
		}
		free(categoryArray[0]->sentinel);
	}
	for(int i=0; i<6; i++)
	{
		categoryArray[i]->sentinel = NULL;
		if(categoryArray[i] != NULL)
		{
			free(categoryArray[i]);
			categoryArray[i] = NULL;
		}
	} 
}



/**
 * @brief Recursively Frees BST nodes using PostOrder Traversal
 * @return returns void
*/
void PostOrderMovieDelete(movie_t *movie)
{
	if(movie->Info->movieID == -1) return;
	PostOrderMovieDelete(movie->lc);
	PostOrderMovieDelete(movie->rc);
	if(movie->Info != NULL)
	{
		free(movie->Info);
		movie->Info = NULL;
	}
	if(movie != NULL)
	{
		free(movie);
	}
	return;
}


/**
 * @brief Assigns the user of UID for the given user  parameter.
 * @return returns 1 if user exists, 0 otherwise
*/
int userFind(int userID, user_t **user)
{
	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	//Check if the User Already Exists
	if(!existsUser(userID))
	{
		printf("User with userID: %d does not exist\n", userID);
		return 0;
	}

	// Generate Key using a call to the hash function
	int key = hash_function(a, b, userID, m, p);
	for((*user) = user_hashtable_p[key]; (*user)->userID !=userID; (*user) = (*user)->next)
	{
		printf("userID: %d, Key: %d\n", (*user)->userID, key);
	}
	return 1;
}


/**
 * @brief edits the Movie with movieID given the user's score
 * @return returns 1 if movie exists, 0 otherwise
*/
int editeMovie(int movieID, int score, int category)
{
	movie_t *root = categoryArray[category]->movie;
	movie_t *sentinel = categoryArray[category]->sentinel;

	// If the tree is empty the movie does not exist in it
	if(root == sentinel) return 0;

	// Otherwise traverse the tree until you either hit a sentinel or a movieID match node
	movie_t *curr = root;
	while(curr->Info->movieID != -1 && curr->Info->movieID != movieID)
	{
		if(curr->Info->movieID > movieID)
		{
			curr = curr->lc;
		}
		else
		{
			curr = curr->rc;
		}
	}

	// Having traversed the tree check if curr is sentinel or a movieID match
	if(curr == sentinel) return 0; // It does not exist
	else // Edit the movie
	{
		curr->Info->sumScore += score;
		curr->Info->watchedCounter += 1;
		return 1;
	}
}


/**
 * @brief adds the given movie to the user History Tree
 * @return returns 1 on success, 0 otherwise
*/
int addMovieToUser(user_t** user, int category, int movieID, int score)
{
	// Create new user movie object
	userMovie_t *movie = makeNewUserMovie(movieID, category, score);
	if(!movie) return 0;

	userMovie_t *root = (*user)->history;

	// Check if the root is empty in which case add root
	if(root == NULL)
	{
		(*user)->history = movie;
		return 1;
	}

	// Otherwise
	userMovie_t *curr = root, *prev = root;
	int isLeft = 0, isRight = 0;
	while(curr != NULL)
    {
        if(curr->Info->movieID >= movieID)
        {
            isLeft = 1;
            isRight = 0;
			prev = curr;
            curr = curr->lc;
        }
        else
        {
            isRight = 1;
            isLeft = 0;
			prev = curr;
            curr = curr->rc;
        }
    }
    if(prev->Info->movieID != movieID)
    {
        if(isLeft)
        {
			userMovie_t *parent_copy = userMovieCopy(prev);
			if(!parent_copy) return 0;
            prev->lc = movie;
			movie->parent = prev;
			prev->rc = parent_copy;
			parent_copy->parent = prev;
        }
        else
        {
			userMovie_t *parent_copy = userMovieCopy(prev);
			if(!parent_copy) return 0;
            prev->rc = movie;
			movie->parent = prev;
			prev->lc = parent_copy;
			parent_copy->parent = prev;
        }
    }
	else  // If we hit the same movie on the users watch history just edit the score
	{
		prev->Info->sumScore = score;
	}
	return 1;
}


/**
 * @brief makes a new usermovie_t object and fills it in
 * @return returns 1 on success, 0 otherwise
*/
userMovie_t *makeNewUserMovie(int movieID, int category, int score)
{
	userMovie_t *userMovie = (userMovie_t*)malloc(sizeof(userMovie_t));
	if(!userMovie) return NULL;
	userMovie->Info = (movieInfoUser_t*)malloc(sizeof(movieInfoUser_t));
	if(!userMovie->Info)
	{
		free(userMovie);
		return NULL;
	}
	userMovie->Info->category = category;
	userMovie->Info->movieID = movieID;
	userMovie->Info->sumScore = score;
	userMovie->lc = NULL;
	userMovie->rc = NULL;
	userMovie->parent = NULL;
	return userMovie;
}



/**
 * @brief Makes a copy user movie object from the passed in object
 * @return returns NULL on failure, userMovie_t* on success
*/
userMovie_t* userMovieCopy(userMovie_t *userMovie)
{
	userMovie_t *movie = makeNewUserMovie(userMovie->Info->movieID, userMovie->Info->category, userMovie->Info->sumScore);
	return movie;
}


/**
 * @brief Frees the History tree associated with one user using PostOrder Tree Traversal
 * @return returns NULL on failure, userMovie_t* on success
*/
void freeUserHistoryTree(userMovie_t *userMovie)
{
	if(userMovie == NULL) return;
	freeUserHistoryTree(userMovie->lc);
	freeUserHistoryTree(userMovie->rc);
	if(userMovie->Info != NULL)
	{
		free(userMovie->Info);
		userMovie->Info = NULL;
	}
	free(userMovie);
	return;
}


/**
 * @brief Traverse the History Tree Inorder and prints the leaves
 * @return returns NULL on failure, userMovie_t* on success
*/
void InOrderPrintHistoryTreeLeaves(userMovie_t *root)
{
	if(root == NULL) return;
	InOrderPrintHistoryTreeLeaves(root->lc);
	InOrderPrintHistoryTreeLeaves(root->rc);
	if(root->lc == NULL && root->rc == NULL)
	{
		printf("   %d, %d\n", root->Info->movieID, root->Info->sumScore);
	}
	return;
}



/**
 * @brief Heapifies movieHelperArray with size d from index i
 * @return returns void
*/
void heapify(movie_t **helperMovieArray, int d, int i)
{
    // Given the specific parent index extract the associated children indices
    int largest_parent = i;
    int left_child = 2*i + 1;
    int right_child = 2*i + 2;
	int score_left, score_right;
    if (left_child < d) score_left = (float)helperMovieArray[left_child]->Info->sumScore / (float)helperMovieArray[left_child]->Info->watchedCounter;
    if(right_child < d) score_right = (float)helperMovieArray[right_child]->Info->sumScore / (float)helperMovieArray[right_child]->Info->watchedCounter;
	int score_largest_parent = (float)helperMovieArray[largest_parent]->Info->sumScore / (float)helperMovieArray[largest_parent]->Info->watchedCounter;
	
    // If the left child index is not out of array boundaries and the associated left child score is larger than the parent score make it largest
    if(left_child < d && score_left > score_largest_parent)
    {
        largest_parent = left_child;
		score_largest_parent = score_left;
    }

    // If the left child index is not out of array boundaries and the associated left child  is larger than the previous largest
    // score make it largest
    if(right_child < d && score_right > score_largest_parent)
    {
        largest_parent = right_child;
		score_largest_parent = score_right;
    }

    if(largest_parent != i) // If any swapping occured, recursively perfrom heapifications begining from the swapped child index
    {
        movie_t *temp = helperMovieArray[i];
        helperMovieArray[i] = helperMovieArray[largest_parent];
        helperMovieArray[largest_parent] = temp;

        // Recursive heapification call
        heapify(helperMovieArray, d, largest_parent);
    }
}


/**
 * @brief heapSorts a given movie_t pointer Array based on their scores
 * @return returns void
*/
void heapSort(movie_t **helperMovieArray, int d)
{
    // Begin by heapifying the entire array
    for(int i = d/2 -1; i >= 0; i--)
    {
        heapify(helperMovieArray, d, i);
    }

    // Then the associated tree Root gets swapped with the last array score and heapification is repeated for the reduced heap
     for(int i = d-1; i>0; i--)
     {
        movie_t *temp = helperMovieArray[0];
        helperMovieArray[0] = helperMovieArray[i];
        helperMovieArray[i] = temp;

        // Reheapification of reduced heap 
        heapify(helperMovieArray, i, 0);
     }
}


/**
 * @brief Traverses a movie_t tree InOrder and adds a movie to helperMovieArray if movie_score exceeds the provided score
 * @return returns void
*/
void inOrderAddBasedOnScore(movie_t *movie, float score, movie_t **helperMovieArray, int *index)
{
	if(movie->Info->movieID == -1) return;
	inOrderAddBasedOnScore(movie->lc, score, helperMovieArray, index);
	inOrderAddBasedOnScore(movie->rc, score, helperMovieArray, index);

	// Check if current movie has a greater score than score parameter 
	//and add at the helperMovieArray at the given index location while increasing index by 1
	float mv_score = (float)movie->Info->sumScore / (float)movie->Info->watchedCounter;
	if(mv_score >= score)
	{
		helperMovieArray[(*index)] = movie;
		(*index) = (*index) + 1;
	}
	return;
}


/**
 * @brief Returns the total amount of movies found in the movieCategoryArray Trees
 * @return the number of movies
*/
int getNumberOfMovies(void)
{
	int n_movies = 0;
	for(int categ = 0; categ < 6; categ++)
	{
		if(categoryArray[categ] != NULL)
		{
			inOrderCountMovies(categoryArray[categ]->movie, &n_movies);
		}
	}

	return n_movies;
}


/**
 * @brief Returns the total amount of movies found in the movieCategoryArray Trees
 * @return the number of movies
*/
void inOrderCountMovies(movie_t *movie, int *n_movies)
{
	if(movie->Info->movieID == -1) return;
	inOrderCountMovies(movie->lc, n_movies);
	inOrderCountMovies(movie->rc, n_movies);

	// Increase the movie count by 1
	(*n_movies) = (*n_movies) + 1;
	return;
}


/**
 * @brief Edits scoreSum and counter int pointers according to the sum of scores of the user and the movie count
 * @return returns void
*/
void inOrderFindStats(userMovie_t *movie, int *scoreSum, int *counter)
{
	if(movie == NULL) return;
	inOrderFindStats(movie->lc, scoreSum, counter);
	inOrderFindStats(movie->rc, scoreSum, counter);
	(*scoreSum) = (*scoreSum) + movie->Info->sumScore;
	(*counter) = (*counter) + 1;
	return;
}


/**
 * @brief Prints Movies in the history tree of user UserID
 * @return returns 1 on Success, 0 on failure
*/
int PrintUserMovies(int userID)
{
	// Return 0 if userId > max_id
	if(userID > max_id)
	{
		printf("UserID: %d is larger than max_id: %d\n", userID, max_id);
		return 0;
	} 

	//Check if the User Already Exists
	if(!existsUser(userID))
	{
		printf("User with userID: %d does not exist\n", userID);
		return -1;
	}

	// Generate Key using a call to the hash function
	int key = hash_function(a, b, userID, m, p);

	// Access Last User on appropriate Hash Table cell chain and add newUser
	user_t *user;


	// Otherwise find chain tail
	for(user = user_hashtable_p[key]; user->next != NULL; user = user->next);
	printf("User %d: ", user->userID);
	InOrderPrintUserMovies(user->history);
	printf("\n\n");

	return 1; // Success
}


/**
 * @brief InOrder Print user History Tree movies
 * @return returns void
*/
void InOrderPrintUserMovies(userMovie_t *movie)
{
	if(movie == NULL) return;
	InOrderPrintUserMovies(movie->lc);
	InOrderPrintUserMovies(movie->rc);
	if(movie->rc == NULL && movie->lc == NULL) 	printf("%d, ", movie->Info->movieID);
	return;
}


/**
 * @brief InOrder Print user History Tree movies
 * @return returns void
*/
void InOrderPrintUserMoviesEd2(userMovie_t *movie)
{
	if(movie == NULL) return;
	InOrderPrintUserMoviesEd2(movie->lc);
	InOrderPrintUserMoviesEd2(movie->rc);
	if(movie->rc == NULL && movie->lc == NULL) 	printf("      %d %d\n", movie->Info->movieID, movie->Info->sumScore);
	return;
}
