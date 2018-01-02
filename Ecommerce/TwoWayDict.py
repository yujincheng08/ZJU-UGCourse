class TwoWayDict(dict):
    __key_map = dict()

    def __setitem__(self, key, value):
        self.__key_map[value] = key
        return dict.__setitem__(self, key, value)

    def __delitem__(self, key):
        del self.__key_map[self[key]]
        return dict.__delitem__(self, key)

    def __len__(self):
        return dict.__len__(self)

    def get_key(self, value):
        return self.__key_map[value]

