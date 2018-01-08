import sys
import numpy as np
import pandas as pd
import csv

from TwoWayDict import TwoWayDict


class Recommend:
    def __init__(self, csv_path, item_id_name, user_id_name,
                 rating_id, ratio=0.8, k=30):
        self.ratio = ratio
        self.k = k
        self.__item_id = self.__get_id_map(csv_path, item_id_name)
        self.__user_id = self.__get_id_map(csv_path, user_id_name)
        self.__build_matrix(csv_path, user_id_name, item_id_name, rating_id)
        self.__train_size = int(self.__matrix.shape[0] * ratio)
        self.__test_size = self.__matrix.shape[0] - self.__train_size
        self.__train_matrix = self.__matrix[:self.__train_size, :]
        self.__test_matrix = self.__matrix[self.__train_size:, :]
        # self.__most_similarity_items = np.zeros((k, self.item_size()), int)

    def item_size(self):
        return self.__item_id.__len__()

    def user_size(self):
        return self.__user_id.__len__()

    def __build_matrix(self, csv_path, user_id_name, item_id_name, rating_id):
        csv_data = pd.read_csv(csv_path)
        self.__matrix = np.zeros([self.user_size(), self.item_size()])

        def __assign_matrix(row):
            user_id = str(int(row[user_id_name]))
            item_id = str(int(row[item_id_name]))
            user_index = self.__user_id.get_key(user_id)
            item_index = self.__item_id.get_key(item_id)
            self.__matrix[user_index, item_index] = row[rating_id]

        csv_data.apply(__assign_matrix, axis=1)

    def __filter_items(self):
        for i in range(self.item_size()):
            similarity = np.argsort(self.__similarity_items[:, i])
            for j in range(self.k):
                self.__most_similarity_items[j][i] = int(similarity[-j-1])

    def train(self, algorithm):
        if algorithm == 'adjusted':
            self.__train_adjusted()
        elif algorithm == 'cosine':
            self.__train_cosine()
        elif algorithm == 'correlation':
            self.__train_correlation()
        else:
            sys.stderr.write('Error similarity algorithm: %s' % algorithm)
#        self.__filter_items()

    def __item_cosine(self, i, j):
        vector_i = self.__train_matrix[:, i]
        vector_j = self.__train_matrix[:, j]
        sq_len = np.sum(np.square(vector_i)) * np.sum(np.square(vector_j))
        if sq_len == 0:
            return 0
        return np.dot(vector_i, vector_j) / np.sqrt(sq_len)

    def __train_cosine(self):
        similarity = np.zeros([self.item_size(), self.item_size()])
        for i in range(self.item_size()):
            for j in range(i+1, self.item_size()):
                similarity[i, j] = similarity[j, i] = self.__item_cosine(i, j)
        self.__similarity = similarity

    def __item_adjusted(self, i, j, mean):
        vector_i = self.__train_matrix[:, i] - mean
        vector_j = self.__train_matrix[:, j] - mean
        '''
        idx = np.intersect1d(np.flatnonzero(vector_i), np.flatnonzero(vector_j))
        if idx.__len__() == 0:
            return 0
        mean = mean[idx]
        vector_i = vector_i[idx] - mean
        vector_j = vector_j[idx] - mean
        '''
        sq_len = np.sum(np.square(vector_i)) * np.sum(np.square(vector_j))
        if sq_len == 0:
            return 0
        return np.dot(vector_i, vector_j) / np.sqrt(sq_len)

    def __train_adjusted(self):
        similarity = np.zeros([self.item_size(), self.item_size()])
        mean = np.array([0] * self.__train_size, dtype=float)
        for i in range(self.__train_size):
            user = self.__train_matrix[i, :]
            count = user[user > 0].shape[0]
            if count > 0:
                mean[i] = np.sum(user) / count
            else:
                mean[i] = 0
        for i in range(self.item_size()):
            for j in range(i+1, self.item_size()):
                similarity[i, j] = similarity[j, i] = self.__item_adjusted(i, j, mean)
        self.__similarity = similarity

    def __item_correlation(self, i, j, mean):
        vector_i = self.__train_matrix[:, i] - mean[i]
        vector_j = self.__train_matrix[:, j] - mean[j]
        '''
        idx = np.intersect1d(np.flatnonzero(vector_i), np.flatnonzero(vector_j))
        if idx.__len__() == 0:
            return 0
        vector_i = vector_i[idx] - mean[i]
        vector_j = vector_j[idx] - mean[j]
        '''
        sq_len = np.sum(np.square(vector_i)) * np.sum(np.square(vector_j))

        if sq_len == 0:
            return 0
        return np.dot(vector_i, vector_j) / np.sqrt(sq_len)

    def __train_correlation(self):
        similarity = np.zeros([self.item_size(), self.item_size()])
        mean = [0] * self.item_size()
        for i in range(self.item_size()):
            item = self.__train_matrix[:, i]
            count = item[item > 0].shape[0]
            if count > 0:
                mean[i] = np.sum(item) / count
            else:
                mean[i] = 0
        for i in range(self.item_size()):
            for j in range(i+1, self.item_size()):
                similarity[i, j] = similarity[j, i] = self.__item_correlation(i, j, mean)
        self.__similarity = similarity

    def evaluate(self):
        item_sum = np.sum(self.__similarity, axis=0)
        idx = np.flatnonzero(item_sum)  # indices of non-zero columns
        # remove columns and rows
        similarity = self.__similarity[:, idx][idx, :]
        test = self.__test_matrix[:, idx]
        predict = np.dot(test, similarity) / np.sum(similarity, axis=0)
        return np.mean(np.abs(test - predict))

    @staticmethod
    def __get_id_map(csv_path, row_id):
        id_map = TwoWayDict()
        with open(csv_path, newline='') as csv_file:
            csv_reader = csv.DictReader(csv_file)
            id_set = set()
            for row in csv_reader:
                id_set.add(row[row_id])
            for i in range(id_set.__len__()):
                pop = id_set.pop()
                id_map[i] = pop
            return id_map


recommend = Recommend("ml-latest-small/ratings.csv", 'movieId', 'userId', 'rating')
recommend.train("cosine")
print(recommend.evaluate())
recommend.train("correlation")
print(recommend.evaluate())
recommend.train("adjusted")
print(recommend.evaluate())
