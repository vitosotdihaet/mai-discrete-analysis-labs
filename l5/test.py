def all_lcs(s1, s2):
    m = len(s1)
    n = len(s2)

    max_length = 0
    result = []

    for i in range(m):
        for j in range(n):
            current_length = 0
            current_s = ''
            while (i + current_length) < m and (j + current_length) < n and s1[i + current_length] == s2[j + current_length]:
                current_s += s1[i + current_length]
                current_length += 1

            if max_length < current_length:
                max_length = current_length
                result = [current_s]
            elif max_length == current_length:
                result.append(current_s)

    return result


import argparse

parser = argparse.ArgumentParser(
    prog='ProgramName',
    description='What the program does',
    epilog='Text at the bottom of help',
)

parser.add_argument('s1')
parser.add_argument('s2')

args = parser.parse_args()
s1 = args.s1
s2 = args.s2

lcs = all_lcs(s1, s2)
print(len(lcs[0]))

for s in set(lcs):
    print(s)