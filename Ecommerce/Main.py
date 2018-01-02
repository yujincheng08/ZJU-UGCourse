import csv
import sys
import numpy as np
from TwoWayDict import TwoWayDict


class Recommend:
    def __init__(self, item_csv_path, item_id_name, user_csv_path, user_id_name,
                 rating_id, ratio=0.8, k=30):
        self.__item_id = self.__get_id_map(item_csv_path, item_id_name)
        self.__user_id = self.__get_id_map(user_csv_path, user_id_name)
        __matrix = self.__build_matrix(user_csv_path, user_id_name, item_id_name, rating_id)
        self.ratio = ratio
        self.k = k
        self.__build_matrix(user_csv_path, user_id_name, item_id_name, rating_id)
        self.__train_size = int(self.user_size() * ratio)
        self.__test_size = self.user_size() - self.__train_size
        self.__train_matrix = __matrix[:self.__train_size, :]
        self.__test_matrix = __matrix[self.__train_size:, :]
        self.__similarity_items = np.zeros((self.item_size(), self.item_size()))
        self.__most_similarity_items = np.zeros((k, self.item_size()), int)

    def item_size(self):
        return self.__item_id.__len__()

    def user_size(self):
        return self.__user_id.__len__()

    def __build_matrix(self, user_csv_path, user_id_name, item_id_name, rating_id):
        matrix = np.zeros((self.user_size(), self.item_size()))
        with open(user_csv_path, newline='') as user_csv:
            csv_reader = csv.DictReader(user_csv)
            for row in csv_reader:
                user_index = self.__user_id.get_key(row[user_id_name])
                item_index = self.__item_id.get_key(row[item_id_name])
                rating = row[rating_id]
                matrix[user_index][item_index] = float(rating)
        return matrix

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
        self.__filter_items()

    def __item_cosine(self, i, j):
        vector_i = self.__train_matrix[:, i]
        vector_j = self.__train_matrix[:, j]
        l_i = vector_i.dot(vector_i)
        l_j = vector_j.dot(vector_j)
        sq_len = l_i * l_j
        if sq_len == 0:
            return 0
        return vector_i.dot(vector_j) / np.sqrt(sq_len)

    def __train_cosine(self):
        for i in range(self.item_size()):
            for j in range(self.item_size()):
                self.__similarity_items[i][j] = self.__item_cosine(i, j)

    def __item_adjusted(self, i, j, user_list):
        u_bar = 0
        count = 0
        if user_list.__len__() == 0:
            return 0
        for u in user_list:
            for i in range(self.item_size()):
                r = self.__train_matrix[u][i]
                if r != 0:
                    u_bar += r
                    count += 1
        u_bar /= count
        above = 0
        bottom_left = 0
        bottom_right = 0
        for u in user_list:
            above += (self.__train_matrix[u][i] - u_bar) * (self.__train_matrix[u][j] - u_bar)
            bottom_left += (self.__train_matrix[u][i] - u_bar) ** 2
            bottom_left += (self.__train_matrix[u][j] - u_bar) ** 2
        bottom_left = np.sqrt(bottom_left)
        bottom_right = np.sqrt(bottom_right)
        if bottom_right == 0 or bottom_left == 0:
            return 0
        return above / (bottom_left * bottom_right)

    def __train_adjusted(self):
        for i in range(self.item_size()):
            for j in range(self.item_size()):
                user_list = []
                for u in range(self.__train_size):
                    if self.__train_matrix[u][i] != 0 and self.__train_matrix[u][j] != 0:
                        user_list.append(u)
                self.__similarity_items[i][j] = self.__item_adjusted(i, j, user_list)

    def __item_correlation(self, i, j, user_list):
        i_bar = 0
        j_bar = 0
        for u in user_list:
            i_bar += self.__train_matrix[u][i]
            j_bar += self.__train_matrix[u][j]
        i_bar /= user_list.__len__()
        j_bar /= user_list.__len__()
        above = 0
        bottom_left = 0
        bottom_right = 0
        for u in user_list:
            above += (self.__train_matrix[u][i] - i_bar) * (self.__train_matrix[u][j] - j_bar)
            bottom_left += (self.__train_matrix[u][i] - i_bar) ** 2
            bottom_left += (self.__train_matrix[u][j] - j_bar) ** 2
        bottom_left = np.sqrt(bottom_left)
        bottom_right = np.sqrt(bottom_right)

        return above / (bottom_left * bottom_right)

    def __train_correlation(self):
        for i in range(self.item_size()):
            for j in range(self.item_size()):
                user_list = []
                for u in range(self.__train_size):
                    if self.__train_matrix[u][i] != 0 and self.__train_matrix[u][j] != 0:
                        user_list.append(u)
                self.__similarity_items[i][j] = self.__item_correlation(i, j, user_list)

    def evaluate(self):
        error = 0
        count = 0

        for u in range(self.__test_size):
            user_vector = self.__test_matrix[u]
            for i in range(self.item_size()):
                real_rate = self.__test_matrix[u][i]
                if real_rate != 0:
                    predict = self.predict(user_vector, i)
                    error += np.fabs(predict - real_rate)
                    count += 1
        return error / count

        pass

    def predict(self, user_vector, i):
        prediction = 0
        count = 0
        for j in self.__most_similarity_items[:, i]:
            prediction += self.__similarity_items[i][j] * user_vector[j]
            count += user_vector[j]
        if count == 0:
            return 0
        else:
            return prediction/count

    @staticmethod
    def __get_id_map(csv_path, row_id):
        id_map = TwoWayDict()
        with open(csv_path, newline='') as csv_file:
            csv_reader = csv.DictReader(csv_file)
            id_set = set()
            for row in csv_reader:
                id_set.add(row[row_id])
            for i in range(id_set.__len__()):
                id_map[i] = id_set.pop()
            return id_map


recommend = Recommend("ml-latest-small/movies.csv", 'movieId', "ml-latest-small/ratings.csv", 'userId', 'rating')
recommend.train("adjusted")
print(recommend.evaluate())
pass
