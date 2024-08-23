# Download the book rating dataset (Link). Write functions to perform following:
# a. Load the books.csv and ratings.csv into two separate DataFrames using pandas.
# b. Display the first 10 rows of each data frame.
# c. Calculate the average rating for each book and find the top 5 highest-rated books.
# d. Find the number of books published each year.
# e. Filter out books with fewer than 50 ratings.
# f. Find the user who has rated the most books.
# —– Page 1 of 3 —–
# g. Extract the top 5 authors who have written the most books.
# h. Convert the rating column into a NumPy array and compute the ratings’ mean, median,
# and mode.
# i. Normalize the ratings with a mean of 0 and a standard deviation 1.
# j. Create a user-item matrix (R) where each row represents a user, each column represents
# a book, and the values are the ratings.
# k. Compute the covariance matrix forR to identify correlations between users or books.
# l. Using R, calculate the cosine similarity between users or books.

import pandas as pd
import numpy as np
from scipy import stats
from sklearn.preprocessing import StandardScaler
from sklearn.metrics.pairwise import cosine_similarity


books_df = pd.read_csv('C:\\Users\\dell\\Downloads\\books.csv')
ratings_df = pd.read_csv('C:\\Users\\dell\\Downloads\\ratings.csv')

print("Books DataFrame:")
print(books_df.head(10))

print("\nRatings DataFrame:")
print(ratings_df.head(10))

average_ratings = ratings_df.groupby('book_id')['rating'].mean()
books_with_ratings = books_df.set_index('book_id').join(average_ratings).reset_index()
top_5_books = books_with_ratings.sort_values(by='rating', ascending=False).head(5)
print("\nTop 5 Highest-Rated Books:")
print(top_5_books[['title', 'rating']])

books_per_year = books_df.groupby('original_publication_year').size()
print("\nNumber of Books Published Each Year:")
print(books_per_year)

ratings_count = ratings_df['book_id'].value_counts()
books_with_frequent_ratings = ratings_count[ratings_count >= 50].index
filtered_books_df = books_df[books_df['book_id'].isin(books_with_frequent_ratings)]
print("\nBooks with at least 50 ratings:")
print(filtered_books_df)

user_ratings_count = ratings_df['user_id'].value_counts()
most_active_user = user_ratings_count.idxmax()
print(f"\nUser who has rated the most books: {most_active_user}")

author_books_count = books_df['authors'].value_counts()
top_5_authors = author_books_count.sort_values(ascending=False).head(5)
print("\nTop 5 Authors Who Have Written the Most Books:")
print(top_5_authors)

ratings_array = ratings_df['rating'].to_numpy()
mean_rating = np.mean(ratings_array)
median_rating = np.median(ratings_array)
mode_rating = stats.mode(ratings_array, keepdims=True).mode[0]
print(f"\nMean Rating: {mean_rating}")
print(f"Median Rating: {median_rating}")
print(f"Mode Rating: {mode_rating}")

ratings_df = ratings_df.drop_duplicates(subset=['user_id', 'book_id'])
ratings_array = ratings_array.reshape(-1, 1)
scaler = StandardScaler()
normalized_ratings = scaler.fit_transform(ratings_array)
print("\nNormalized Ratings (First 10):")
print(normalized_ratings[:10])

user_item_matrix = ratings_df.pivot(index='user_id', columns='book_id', values='rating').fillna(0)
print("\nUser-Item Matrix (First 10 rows):")
print(user_item_matrix.head(10))


cov_matrix = user_item_matrix.cov()
print("\nCovariance Matrix:")
print(cov_matrix)

user_similarity = cosine_similarity(user_item_matrix)
user_similarity_df = pd.DataFrame(user_similarity, index=user_item_matrix.index, columns=user_item_matrix.index)
book_similarity = cosine_similarity(user_item_matrix.T)
book_similarity_df = pd.DataFrame(book_similarity, index=user_item_matrix.columns, columns=user_item_matrix.columns)

print("\nUser Similarity Matrix (First 10 users):")
print(user_similarity_df.head(10))

print("\nBook Similarity Matrix (First 10 books):")
print(book_similarity_df.head(10))
