def pnum_list():
    f = open('pnum.txt', 'r')
    data = f.read().splitlines()
    f.close()

    return data


if __main__:
    print(pnum_list())