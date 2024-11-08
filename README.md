
# Movie Streaming Service - Data Structures Project

## Table of Contents
- [Overview](#overview)
- [Data Structures](#data-structures)
- [Functional Requirements](#functional-requirements)
- [Execution](#execution)
- [Events and Operations](#events-and-operations)
- [Development Guidelines](#development-guidelines)

## Overview

This project implements a simplified movie streaming service, categorizing movies into specific themes and allowing users to register, watch movies, and conduct filtered searches.

### Key Features
- **Movie Categorization**: Movies are categorized into six themes: Horror, Science Fiction, Drama, Romance, Documentary, and Comedy. Each category is structured as a binary search tree.
- **User Management**: Users can register, maintain a watch history, and rate movies.
- **Data Storage**: Movie data and user information are stored using binary trees and hash tables.

## Data Structures

### Movie Data Structures
- **Category Array**: An array of six binary search trees, each representing a movie category.
- **Binary Search Trees**:
  - Each category tree node holds a `movie` struct containing the movie ID, release year, watch counter, and total score.
  - The "New Releases" list is a separate binary search tree without a sentinel node.

### User Data Structures
- **User Hash Table**: Users are stored in a hash table, where collisions are managed by chaining.
- **User History**:
  - Each user has a history tree, storing movies they've watched and rated.
  - The history tree is a doubly linked leaf-oriented binary search tree.

## Functional Requirements

### Movie Operations
1. **Add Movie**: Insert new movies into the New Releases tree with fields for ID, year, and category.
2. **Distribute Movies**: Distribute movies from New Releases into category trees based on their genre.
3. **Search Movies**: Locate movies within a specific category by movie ID.

### User Operations
1. **Register User**: Add a user to the hash table.
2. **Unregister User**: Remove a user and clear their history.
3. **Watch and Rate Movie**: Track user ratings and watch counts for specific movies.
4. **Filtered Search**: Allow users to search for movies by minimum score within specific categories.

## Execution

First compile the .c code using the following command

```bash
gcc main.c Movies.c -o main
```

Then run the executable by running

```bash
main.exe <input_file> # for Windows
```

or

```bash
./main <input_file> # for Linux
```
## Events and Operations

### Event List
- **Register User (`R <userID>`)**: Registers a new user with a unique ID.
- **Unregister User (`U <userID>`)**: Removes an existing user.
- **Add New Movie (`A <movieID> <category> <year>`)**: Adds a movie to the New Releases tree.
- **Distribute Movies (`D`)**: Transfers movies from New Releases into categorized trees.
- **Search Movie (`I <movieID> <category>`)**: Finds a movie within a category.
- **Watch Movie (`W <userID> <category> <movieID> <score>`)**: Allows a user to watch and rate a movie.
- **Filtered Movie Search (`F <userID> <score>`)**: Searches for movies with a minimum score.
- **User's Average Rate (`Q <userID>`)**: Calculates and displays the average rating by a user.
- **Print Movies (`M`)**: Prints all categorized movies.
- **Print Users (`P`)**: Displays the entire user hash table and each user's history.

## Development Guidelines

### General Rules
- **No Libraries**: Implement all required structures manually without external libraries.
- **Data Structures**:
  - Use C structs to define `movie`, `movie_category`, `user`, and `user_movie`.
  - Implement the hash table with user-defined collision resolution.

This README covers the essential components of the assignment, providing a clear and organized view of the project's structure and requirements. For further details, refer to the project specification document.
