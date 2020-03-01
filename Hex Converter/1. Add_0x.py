# 설명
print('ex) \n1 2 3 \nto \n0x1, 0x2, 0x3\n')

# 입력
data = input().split(' ')
res = ''

for i in range(len(data)):
    # 0x 붙이기
    res += '0x' + data[i]

    # 마지막 아니면 , 붙이기
    if i != len(data) - 1 :
        res += ', '

# 출력
print(res)

# 바로 종료 되는것 방지
input()