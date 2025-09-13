#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100
#define NAME_LEN  32
#define MAX_STOCK 5   // 최대 상품 수(고정 5)


typedef struct {
    char name[NAME_LEN]; // 상품명
    int price;           // 단가(판매가격)
    int in;              // 입고 누적
    int out;             // 판매 누적
    int remain;          // 남은 재고 (입고 - 판매)
    int active;          
} Item;



int  commandMenu(void);
int  commandId(int stock);
int  commandQty(const char *label);
void updateRemain(Item items[], int stock);

void handleInbound(Item items[], int stock);
void handleSale(Item items[], int stock);

void showStatus(Item items[], int stock);     // 전체현황
void showOne(Item items[], int stock);        // 개별현황


int  countActive(Item items[], int stock);
const char* defaultName(Item *it);



int main(void) {
    int stock = MAX_STOCK;
    Item items[MAX_ITEMS];

    // 초기화
    for (int i = 0; i < stock; i++) {
        items[i].name[0] = '\0';
        items[i].price = 0;
        items[i].in = items[i].out = items[i].remain = 0;
        items[i].active = 0;
    }

    while (1) {
        int menu = commandMenu();

        if (menu == 1) {
            handleInbound(items, stock);
        } else if (menu == 2) {
            handleSale(items, stock);
        } else if (menu == 3) {
            showOne(items, stock);      // 개별현황
        } else if (menu == 4) {
            showStatus(items, stock);   // 전체현황
        } else if (menu == 5) {
            printf("프로그램을 종료합니다.\n");
            break;
        } else {
            printf("잘못된 메뉴입니다. 다시 선택하세요.\n");
        }
    }
    return 0;
}


// 메인 메뉴
int commandMenu(void) {
    int sel;
    printf("\n[쇼핑몰 관리 프로그램]\n");
    printf("원하는 메뉴를 선택하세요. (1. 입고, 2. 판매, 3. 개별현황, 4. 전체현황, 5. 종료)\n> ");
    scanf("%d", &sel);
    return sel;
}

// 상품 ID 입력
int commandId(int stock) {
    int id;
    printf("상품ID 입력 (1~%d): ", stock);
    scanf("%d", &id);
    if (id < 1) id = 1;
    if (id > stock) id = stock;
    return id;
}

// 수량 입력
int commandQty(const char *label) {
    int q;
    printf("%s 수량 입력: ", label);
    scanf("%d", &q);
    if (q < 0) q = 0; // 음수 방지
    return q;
}

// 재고 갱신
void updateRemain(Item items[], int stock) {
    for (int i = 0; i < stock; i++) {
        items[i].remain = items[i].in - items[i].out;
        if (items[i].remain < 0) items[i].remain = 0; // 안전 보정
    }
}


// tools
int countActive(Item items[], int stock) {
    int c = 0;
    for (int i = 0; i < stock; i++) if (items[i].active) c++;
    return c;
}
const char* defaultName(Item *it) {
    return (it->name[0] == '\0') ? "(미등록)" : it->name;
}


// 입고
void handleInbound(Item items[], int stock) {
    int id = commandId(stock);

    if (!items[id - 1].active && countActive(items, stock) >= MAX_STOCK) {
        printf("최대 상품 개수(%d)를 초과할 수 없습니다.\n", MAX_STOCK);
        return;
    }

    // 상품명
    printf("상품명 : ");
    scanf(" %31s", items[id - 1].name);

    // 입고 수량
    int qty = commandQty("입고");
    items[id - 1].in += qty;

    // 단가(판매가격)
    printf("판매가격 : ");
    scanf("%d", &items[id - 1].price);

    items[id - 1].active = 1;
    updateRemain(items, stock);
}

// 판매
void handleSale(Item items[], int stock) {
    int id = commandId(stock);
    if (!items[id - 1].active) {
        printf("오류: 해당 ID의 상품이 없습니다. 먼저 입고로 생성하세요.\n");
        return;
    }
    int qty = commandQty("판매");
    items[id - 1].out += qty;
    if (items[id - 1].out > items[id - 1].in) items[id - 1].out = items[id - 1].in; // 과판매 방지
    updateRemain(items, stock);
}


// 개별현황
void showOne(Item items[], int stock) {
    int id = commandId(stock);
    if (!items[id - 1].active) {
        printf("오류: 해당 ID의 상품이 없습니다.\n");
        return;
    }
    Item *it = &items[id - 1];
    long long total = (long long)it->out * (long long)it->price;

    printf("\n[개별현황]\n");
    printf("상품 ID : %d\n", id);
    printf("상품명   : %s\n", defaultName(it));
    printf("단가     : %d\n", it->price);
    printf("입고량   : %d\n", it->in);
    printf("판매량   : %d\n", it->out);
    printf("재고     : %d\n", it->remain);
    printf("총판매금액 : %lld\n", total);
}

// 전체현황
void showStatus(Item items[], int stock) {
    printf("\n=== 전체 현황 ===\n");

    // 재고수량 벡터
    printf("재고수량: ");
    for (int i = 0; i < stock; i++) {
        printf("%d ", items[i].remain);
    }
    printf("\n");

    // 총 판매량 / 판매율
    int totalIn = 0, totalOut = 0;
    for (int i = 0; i < stock; i++) {
        totalIn  += items[i].in;
        totalOut += items[i].out;
    }
    double percent = (totalIn == 0) ? 0.0 : (double)totalOut / (double)totalIn * 100.0;
    printf("총 판매량: %d (판매율 %.2f%%)\n", totalOut, percent);

    // 최다/최소 판매
    int maxIdx = 0, minIdx = 0;
    for (int i = 1; i < stock; i++) {
        if (items[i].out > items[maxIdx].out) maxIdx = i;
        if (items[i].out < items[minIdx].out) minIdx = i;
    }
    printf("가장 많이 판매된 상품: ID %d, 상품명: %s, 판매량 %d\n",
           maxIdx + 1, defaultName(&items[maxIdx]), items[maxIdx].out);
    printf("가장 적게 판매된 상품: ID %d, 상품명: %s, 판매량 %d\n",
           minIdx + 1, defaultName(&items[minIdx]), items[minIdx].out);

    // 재고부족 안내(재고 2 이하)
    for (int i = 0; i < stock; i++) {
        if (items[i].remain <= 2) {
            printf("상품 ID %d : 상품명: %s 재고부족(%d)\n",
                   i + 1, defaultName(&items[i]), items[i].remain);
        }
    }

    // 개별 금액
    long long totalRevenue = 0;
    for (int i = 0; i < stock; i++) {
        if (items[i].active) {
            long long revenue = (long long)items[i].out * (long long)items[i].price;
            totalRevenue += revenue;
        }
    }
    printf("총 판매금액 합계: %lld\n", totalRevenue);
}
