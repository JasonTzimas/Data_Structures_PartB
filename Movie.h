/*****************************************************
 * @file   Movie.h                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Header File for Data Structures (CS240b)   *
 * Project: Winter 2023						         *
 *****************************************************/
#ifndef MOVIE_FILE
#define MOVIE_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>


typedef enum {
	HORROR,
	SCIFI,
	DRAMA,
	ROMANCE,
	DOCUMENTARY,
	COMEDY
} movieCategory_t;


/**
 * Structure defining the movie info 
*/

typedef struct movie_info
{
	int movieID; 				/* The movie identifier*/
	int year;					/* The year movie released*/
	int watchedCounter;	 		/* How many users rate the movie*/
	int sumScore;				/* The sum of the ratings of the movie*/
}movieInfo_t;


/**
* Structure defining a node of movie binary tree (dendro tainiwn kathgorias)
*/
typedef struct movie{
	movieInfo_t *Info; /* Pointer to the movieInfo_t struct */
	struct movie *lc;	/* Pointer to the node's left child*/
	struct movie *rc;	/* Pointer to the node's right child*/
}movie_t;





/**
 * Structure defining the movie info 
*/

typedef struct movie_info_user
{
	int movieID; 				/* The movie identifier*/
	movieCategory_t category;		/* The movie category enum*/
	int sumScore;				/* The sum of the ratings of the movie*/
}movieInfoUser_t;

/**
* Structure defining movie_category
*/

typedef struct movie_category_array{
	movie_t *movie;
	movie_t *sentinel;
}movieCategoryArray_t;

/**
* Structure defining a node of user_movie for history doubly linked binary 
* tree (dentro istorikou) 
*/
typedef struct user_movie{
	movieInfoUser_t *Info;
	struct user_movie *parent;		/* Pointer to the node's parent*/
	struct user_movie *lc;	/* Pointer to the node's left child*/
	struct user_movie *rc;	/* Pointer to the node's right child*/
}userMovie_t;

/**
* Structure defining a node of users' hashtable (pinakas katakermatismou
* xrhstwn)
*/
typedef struct user {
	int userID; 			/* The user's identifier*/
	userMovie_t *history;	/* A doubly linked binary tree with the movies watched by the user*/
	struct user *next;		/* Pointer to the next node of the chain*/	
}user_t;


///////////////////////// EXTRA STRUCTS ///////////////////////////
// New movies Tree struct
typedef struct new_movie
{
	movieInfo_t *Info;
	movieCategory_t category;
	struct new_movie *lc;
	struct new_movie *rc;
}newMovie_t;


typedef struct new_movies_tree
{
	newMovie_t *root;
}newMoviesTree_t;


typedef struct movies_tree
{
	movie_t *root;
}moviesTree_t;


typedef struct user_movies_tree
{
	userMovie_t *root;
}userMoviesTree_t;


typedef struct heap_tree
{
	movie_t *root;
}heapTree_t;


extern movieCategoryArray_t *categoryArray[6];  /* The categories array (pinakas kathgoriwn)*/
extern user_t **user_hashtable_p;		   /* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
extern newMoviesTree_t newMoviesTree;      /* The new movies Tree struct */
extern int hashtable_size;				   /* The size of the users hashtable, parsed from the command line (>0)*/
extern int max_users;					   /* The maximum number of registrations (users)*/			
extern int max_id;   			   /* The maximum account ID */
extern int primes_g[160];
extern int a;
extern int b;
extern int m;
extern int p;




//////////////////////////////////////////////////////////////////////


/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID);
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID);
 
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

 int add_new_movie(int movieID, int category, int year);
 
 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 1 on success
 *         0 on failure
 */

 int distribute_movies(void);
 
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

 int watch_movie(int userID, int category,int movieID, int score);
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, float score);

/**
 * @brief Find the median score that user rates movies.
 *
 * @param userID The identifier of the user
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID);
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category);
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void);
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void);
 

/* Extra functions */

/**
 * @brief Implements the universal hash function given all parameters
 * @return int: the key
*/

int hash_function(int a, int b, int x, int m, int p);



/**
 * @brief Makes a new user of userID
 * @return user_t*: a pointer to the new user struct
*/
user_t* makeNewUser(int userID);


/**
 * @brief Initializes Hash Table with null Pointers
 * @return 1 on Success 0 on malloc failure
*/
int makeHashTable(void);


/**
 * @brief Initializes Hash Table with null Pointers
 * @return void
*/
void initHashTable(void);


/**
 * @brief Deletes Hash Table
 * @return void
*/
void deleteHashTable(void);


/**
 * @brief Deletes Hash Table cell chain
 * @return void
*/
void deleteUserChainIter(user_t*);


/**
 * @brief Checks if a given User exists
 * @return returns 1 if True, 0 if False
*/
int existsUser(int userID);


/**
 * @brief Initialize newMoviesTree with NULL root
 * @return returns void
*/
void initNewMoviesTree(void);


/**
 * @brief Create New Movie for NewMovies Tree
 * @return returns newMovie_t struct pointer
*/
newMovie_t* makeNewMovie(int movieID, int category, int year);


/**
 * @brief Checks whether the given movie exists or not (using the MovieID)
 * @return returns 1 if True, 0 otherwise
*/
int existsNewMovie(int movieID);



/**
 * @brief Recursive Function that traverses the newMoviesTree and checks if a given movie exists
 * @return returns 1 if Movie is found, 0 otherwise
*/
int InOrderFoundNewMovieTree(newMovie_t* newMovieNode, int movieID);


/**
 * @brief InOrder Traversal of newMovieTree that prints the movieIDs
 * @return returns 1 if Movie is found, 0 otherwise
*/
void InOrderPrintNewMovieTree(newMovie_t *newMovieNode);


/**
 * @brief delete the newMoviesTree
 * @return returns void
*/
void deleteNewMoviesTree();


/**
 * @brief delete the newMoviesTree recursive PostOrder function
 * @return returns void
*/
void deleteNewMoviesTreeIter(newMovie_t *newMovieNode);


/**
 * @brief Initializes the movie Category Array with sentinel nodes
 * @return returns void
*/
int initMovieCategoryArray();


/**
 * @brief Makes a regular movie_t struct object
 * @return returns 1 upon success, 0 upon malloc failure
*/
movie_t* makeMovie(int movieId, int year);


/**
 * @brief Performs inOrder newMovieTree traversal and for each node it visits it calls the function distsributeMovie()
 * that is responsible for creating and placing the corresponding movie in the appropriate BST of the movieCategoryArray
 * @note: IF A MOVIE TRANSFER FAILS WE HAVE TO STOP THE PROCESS. OTHERWISE WE MIGHT END UP WITH A CORRUPTED TREE 
 * @return returns void
*/
int postOrderDistribute(newMovie_t *newMovie);


/**
 * @brief Distributes the given movie to the appropriate place and tree of the movieCategoryArray
 * @return returns 1 upon success, 0 upon failure
*/
int DistributeMovie(newMovie_t *newMovie);


/**
 * @brief Checks if the movie already exists in the movie category array trees (Assuming movieID-category pair is correct)
 * @return returns 1 if True, 0 if False
*/
int newMovieExistsInCategoryArray(int movieID, int category);


/**
 * @brief Prints the BSTs of the movie category array Inorder
 * @return returns void
*/
void inOrderMoviePrint(movie_t *movie);


/**
 * @brief destroy movieCategoryArray and Subtrees
 * @return returns void
*/
void deleteMovieCategoryArray();


/**
 * @brief Recursively Frees BST nodes using PostOrder Traversal
 * @return returns void
*/
void PostOrderMovieDelete(movie_t *movie);


/**
 * @brief Assigns the user of UID for the given user  parameter.
 * @return returns 1 if user exists, 0 otherwise
*/
int userFind(int userID, user_t **user);



/**
 * @brief edits the Movie with movieID given the user's score
 * @return returns 1 if movie exists, 0 otherwise
*/
int editeMovie(int movieID, int score, int category);


/**
 * @brief adds the given movie to the user History Tree
 * @return returns 1 on success, 0 otherwise
*/
int addMovieToUser(user_t** user, int category, int movieID, int score);


/**
 * @brief makes a new usermovie_t object and fills it in
 * @return returns 1 on success, 0 otherwise
*/
userMovie_t *makeNewUserMovie(int movieID, int category, int score);


/**
 * @brief Makes a copy user movie object from the passed in object
 * @return returns NULL on failure, userMovie_t* on success
*/
userMovie_t* userMovieCopy(userMovie_t *userMovie);


/**
 * @brief Frees the History tree associated with one user using PostOrder Tree Traversal
 * @return returns NULL on failure, userMovie_t* on success
*/
void freeUserHistoryTree(userMovie_t *userMovie);



/**
 * @brief Traverse the History Tree Inorder and prints the leaves
 * @return returns NULL on failure, userMovie_t* on success
*/
void InOrderPrintHistoryTreeLeaves(userMovie_t *root);


/**
 * @brief Heapifies movieHelperArray with size d from index i
 * @return returns void
*/
void heapify(movie_t **helperMovieArray, int d, int i);


/**
 * @brief heapSorts a given movie_t pointer Array based on their scores
 * @return returns void
*/
void heapSort(movie_t **helperMovieArray, int d);


/**
 * @brief Traverses a movie_t tree InOrder and adds a movie to helperMovieArray if movie_score exceeds the provided score
 * @return returns void
*/
void inOrderAddBasedOnScore(movie_t *movie, float score, movie_t **helperMovieArray, int *index);


/**
 * @brief Returns the total amount of movies found in the movieCategoryArray Trees
 * @return the number of movies
*/
int getNumberOfMovies(void);


/**
 * @brief Returns the total amount of movies found in the movieCategoryArray Trees
 * @return the number of movies
*/
void inOrderCountMovies(movie_t *movie, int *n_movies);


/**
 * @brief Edits scoreSum and counter int pointers according to the sum of scores of the user and the movie count
 * @return returns void
*/
void inOrderFindStats(userMovie_t *movie, int *scoreSum, int *counter);


/**
 * @brief Prints Movies in the history tree of user UserID
 * @return returns 1 on Success, 0 on failure
*/
int PrintUserMovies(int userID);


/**
 * @brief InOrder Print user History Tree movies
 * @return returns void
*/
void InOrderPrintUserMovies(userMovie_t *movie);


/**
 * @brief InOrder Print user History Tree movies
 * @return returns void
*/
void InOrderPrintUserMoviesEd2(userMovie_t *movie);



 #endif


