import random
import string

LINE_COUNT = 50000
WORD_COUNT = 500

TREE_FILE_PATH = './test.avl'


words_list = []

for _ in range(WORD_COUNT):
    word = ''.join(random.choices(string.ascii_lowercase, k=random.randint(1, 256)))
    words_list.append(word)


output_file = open('test.txt', 'w')

for _ in range(LINE_COUNT):
    random_word = words_list[random.randint(0, WORD_COUNT - 1)]
    command = random.randint(0, 99)
    if 0 <= command < 40: # + word number
        output_file.write(f'+ {random_word} {random.randint(0, 2**64 - 1)}\n')
    if 40 <= command < 50: # - word
        output_file.write(f'- {random_word}\n')
    if 50 <= command < 70: # word
        output_file.write(f'{random_word}\n')
    if 80 <= command < 90: # save
        output_file.write(f'! Save {TREE_FILE_PATH}\n')
    if 90 <= command < 100: # load
        output_file.write(f'! Load {TREE_FILE_PATH}\n')

output_file.close()