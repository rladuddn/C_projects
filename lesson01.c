#include <stdio.h>

int main() {
    int stock;//상품 개수
    int id;
    int in[100]; //입고
    int out[100];//출고
    int nam[100];//남은 재고

    printf("상품 개수: ");
    scanf("%d", &stock);

    printf("상품별 입고 수량: ");
    for (int i = 0; i < stock; i++) {
        scanf("%d", &in[i]);
    }
    printf("상품별 판매 수량: ");
    for (int i = 0; i < stock; i++) {
        scanf("%d", &out[i]);
    }
    for (int i = 0; i < stock; i++) {
        nam[i] = in[i] - out[i];
    }

    printf("상품 ID: ");
    scanf("%d", &id);

    printf("%d\n", nam[id - 1]);
    for (int i = 0; i < stock; i++) {
        printf("%d ", nam[i]);
        printf(" ");
    }
    printf("\n");
    return 0;
}