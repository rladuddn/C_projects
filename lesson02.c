#include <stdio.h>

int main() {
    int stock;//상품 개수
    int in[100]; //입고
    int out[100];//출고
    int nam[100];//남은 재고
    int sellCount; // 판매 수량
    int temp; // 임시 변수
    int minmaxID[2]; // 가장 많이 팔린 상품 ID와 가장 적게 팔린 상품 ID
    int MAX = 0; // 가장 많이 파린 상품
    int MIN = 100; // 가장 적게 팔린 상품

    printf("상품 개수: ");
    scanf("%d", &stock);

    printf("상품별 입고 수량: ");
    for (int i = 0; i < stock; i++) {
        scanf("%d", &in[i]);
    }
    printf("상품별 판매 수량: ");
    for (int i = 0; i < stock; i++) {
        scanf("%d", &out[i]);
        sellCount += out[i];
        if (out[i] > MAX) {
            MAX = out[i];
            minmaxID[0] = i;
        }
        else if (out[i] < MIN) {
            MIN = out[i];
            minmaxID[1] = i;
        }
    }
    for (int i = 0; i < stock; i++) {
        nam[i] = in[i] - out[i];
    }

    

    printf("재고 수량");
    for (int i = 0; i < stock; i++) {
        printf("%d ", nam[i]);
        printf(" ");
    }
    printf("\n");

    printf("총 판매량: %d\n", sellCount);
    printf("가장 많이 팔린 상품: ID %d, 판매량: %d\n", minmaxID[0] + 1, MAX);
    printf("가장 적게 팔린 상품: ID %d, 판매량: %d\n", minmaxID[1] + 1, MIN);

    for (int i = 0; i < stock - 1; i++) {
        if (nam[i] <= 2) {
            printf("상품 ID %d: 재고부족(%d)\n", i + 1, nam[i]);
        }
    }


    return 0;
}