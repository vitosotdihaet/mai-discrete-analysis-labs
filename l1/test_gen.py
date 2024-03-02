import random
import string

TEST_COUNT = 100

class Date:
    def __init__(self, day, month, year):
        self.day = day
        self.month = month
        self.year = year

        # if day < 10 and random.random() > 0.5:
        #     day = '0' + str(day)
        # if month < 10 and random.random() > 0.5:
        #     month = '0' + str(month)
        # year = '0' * random.randint(0, 5) + str(year)

        self.string = f'{day}.{month}.{year}'

    @staticmethod
    def random():
        # year = random.randint(0, 999999)
        # month = random.randint(0, 999999)
        # day = random.randint(0, 999999)
        year = random.randint(0, 9999)
        month = random.randint(0, 12)
        day = random.randint(0, 31)
        return Date(day, month, year)
    
    def __lt__(self, other):
        if self.year < other.year:
            return True
        elif self.year == other.year:
            if self.month < other.month:
                return True
            elif self.month == other.month:
                if self.day < other.day:
                    return True
                else:
                    return False
            else:
                return False
        else:
            return False
        
    def __str__(self):
        return self.string


def random_string():
    output = ''
    length = random.randint(0, 64)

    for _ in range(length):
        output += random.choice(string.ascii_letters + ' ')

    return output



for i in range(TEST_COUNT):
    count = random.randint(50, 10000)
    key_value = []

    for j in range(count):
        date = Date.random()
        value = random_string()
        key_value.append((date, value))

    test_input = open(f'ti{i}.txt', mode='w')
    for e in key_value:
        test_input.write(f'{e[0]}\t{e[1]}\n')
    test_input.close()

    key_value = sorted(key_value, key=lambda element: element[0])

    test_output = open(f'to{i}.txt', mode='w')
    for e in key_value:
        test_output.write(f'{e[0]}\t{e[1]}\n')
    test_output.close()
