#include <stdio.h>

#define MAX_ITEMS 100
#define NAME_LEN  32

typedef struct {
    char name[NAME_LEN]; // 상품명
    int in;              // 입고 누적
    int out;             // 판매 누적
    int remain;          // 남은 재고 (입고 - 판매)
} Item;


int  commandMenu(void);
int  commandSubMenu(const char *action);
int  commandId(int stock);
int  commandQty(const char *label);
void updateRemain(Item items[], int stock);


void handleInbound(Item items[], int stock);
void inboundAll(Item items[], int stock);
void inboundSingle(Item items[], int stock);

void handleSale(Item items[], int stock);
void saleAll(Item items[], int stock);
void saleSingle(Item items[], int stock);

void showStatus(Item items[], int stock);
void inputNames(Item items[], int stock);



int main(void) {
    int stock = 5;                 // 상품 개수
    // int in[MAX_ITEMS]    = {0};    // 입고 누적
    // int out[MAX_ITEMS]   = {0};    // 판매 누적
    // int remain[MAX_ITEMS]= {0};    // 재고

    Item items[MAX_ITEMS];

    while (1) {
        int menu = commandMenu();

        if (menu == 1) {
            handleInbound(items, stock);
        } else if (menu == 2) {
            handleSale(items, stock);
        } else if (menu == 3) {
            showStatus(items, stock);
        } else if (menu == 4) {
            inputNames(items, stock);
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
    printf("원하는 메뉴를 선택하세요. (1. 입고, 2. 판매, 3. 상품현황, 4. 상품명 입력, 5. 종료)\n> ");
    scanf("%d", &sel);
    return sel;
}

// 서브 메뉴(전체/개별)
int commandSubMenu(const char *action) {
    int sel;
    printf("%s 수량 입력: 전체=1, 개별=2 를 선택하세요.\n> ", action);
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
        if (items[i].remain < 0) items[i].remain = 0;
    }
}

// 입고 메뉴
void handleInbound(Item items[], int stock) {
    int sel = commandSubMenu("입고");
    if (sel == 1) inboundAll(items, stock);
    else if (sel == 2) inboundSingle(items, stock);
    else printf("잘못된 선택입니다.\n");
}

// 전체 입고
void inboundAll(Item items[], int stock) {
    printf("전체 상품 입고수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        items[i].in += q;
    }
    updateRemain(items, stock);
}
void inboundSingle(Item items[], int stock) {
    int id  = commandId(stock);
    int qty = commandQty("입고");
    items[id - 1].in += qty;
    updateRemain(items, stock);
}

// 판매 기능
void handleSale(Item items[], int stock) {
    int sel = commandSubMenu("판매");
    if (sel == 1) saleAll(items, stock);
    else if (sel == 2) saleSingle(items, stock);
    else printf("잘못된 선택입니다.\n");
}

void saleAll(Item items[], int stock) {
    printf("전체 상품 판매수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        items[i].out += q;
        // 음수 재고 방지
        if (items[i].out > items[i].in) items[i].out = items[i].in;
    }
    updateRemain(items, stock);
}

void saleSingle(Item items[], int stock) {
    int id  = commandId(stock);
    int qty = commandQty("판매");
    items[id - 1].out += qty;
    if (items[id - 1].out > items[id - 1].in) items[id - 1].out = items[id - 1].in;
    updateRemain(items, stock);
}

// 상품명 입력
void inputNames(Item items[], int stock) {
    printf("상품명 입력 메뉴 실행\n");
    for (int i = 0; i < stock; i++) {
        printf("ID %d 상품명 : ", i + 1);
        scanf(" %31s", items[i].name);
    }
}
// 이름 미등록시 (미등록) 으로 표기 -> default
static const char* defaultName(Item *it, int idx) {
    if (it->name[0] == '\0') return "(미등록)";
    return it->name;
}

// 상품 현황
void showStatus(Item items[], int stock) {
    printf("\n-- 상품 현황 --\n");

    // 1. 상품별 현황
    printf("재고수량: ");
    for (int i = 0; i < stock; i++) {
        printf("%d ", items[i].remain);
    }
    printf("\n");

    // 2. 총 판매량 및 판매율
    int totalIn = 0, totalOut = 0;
    for (int i = 0; i < stock; i++) {
        totalIn  += items[i].in;
        totalOut += items[i].out;
    }
    double percent = (totalIn == 0) ? 0 : (double)totalOut / totalIn * 100.0;
    printf("총 판매량: %d (판매율 %.2f%%)\n", totalOut, percent);

    // 3. 최다/최소 판매 상품
    int maxIdx = 0, minIdx = 0;
    for (int i = 1; i < stock; i++) {
        if (items[i].out > items[maxIdx].out) maxIdx = i;
        if (items[i].out < items[minIdx].out) minIdx = i;
    }
    printf("가장 많이 판매된 상품: ID %d, 상품명: %s, 판매량 %d\n",
       maxIdx + 1, defaultName(&items[maxIdx], maxIdx), items[maxIdx].out);
    printf("가장 적게 판매된 상품: ID %d, 상품명: %s, 판매량 %d\n",
        minIdx + 1, defaultName(&items[minIdx], minIdx), items[minIdx].out);

    // 재고 부족 출력
    for (int i = 0; i < stock; i++) {
        if (items[i].remain <= 2) {
            printf("상품 ID %d : 상품명: %s 재고부족(%d)\n",
                i + 1, defaultName(&items[i], i), items[i].remain);
        }
    }
}