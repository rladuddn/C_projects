#include <stdio.h>

#define MAX_ITEMS 100


int  commandMenu(void);
int  commandSubMenu(const char *action);
int  commandId(int stock);
int  commandQty(const char *label);
void updateRemain(int in[], int out[], int remain[], int stock);


void handleInbound(int in[], int out[], int remain[], int stock);
void inboundAll(int in[], int out[], int remain[], int stock);
void inboundSingle(int in[], int out[], int remain[], int stock);

void handleSale(int in[], int out[], int remain[], int stock);
void saleAll(int in[], int out[], int remain[], int stock);
void saleSingle(int in[], int out[], int remain[], int stock);

void showStatus(int in[], int out[], int remain[], int stock);



int main(void) {
    int stock = 5;                 // 상품 개수(기본 5개)
    int in[MAX_ITEMS]    = {0};    // 입고 누적
    int out[MAX_ITEMS]   = {0};    // 판매(출고) 누적
    int remain[MAX_ITEMS]= {0};    // 재고

    while (1) {
        int menu = commandMenu();

        if (menu == 1) {
            handleInbound(in, out, remain, stock);
        } else if (menu == 2) {
            handleSale(in, out, remain, stock);
        } else if (menu == 3) {
            showStatus(in, out, remain, stock);
        } else if (menu == 4) {
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
    printf("원하는 메뉴를 선택하세요. (1. 입고, 2. 판매, 3. 상품현황, 4. 종료)\n> ");
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
void updateRemain(int in[], int out[], int remain[], int stock) {
    for (int i = 0; i < stock; i++) {
        remain[i] = in[i] - out[i];
    }
}

// 입고 메뉴 라우팅
void handleInbound(int in[], int out[], int remain[], int stock) {
    int sel = commandSubMenu("입고");
    if (sel == 1) inboundAll(in, out, remain, stock);
    else if (sel == 2) inboundSingle(in, out, remain, stock);
    else printf("잘못된 선택입니다.\n");
}

// 전체 입고
void inboundAll(int in[], int out[], int remain[], int stock) {
    printf("전체 상품 입고수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        in[i] += q;
    }
    updateRemain(in, out, remain, stock);
}
void inboundSingle(int in[], int out[], int remain[], int stock) {
    int id  = commandId(stock);
    int qty = commandQty("입고");
    in[id - 1] += qty;
    updateRemain(in, out, remain, stock);
}

// 판매 기능
void handleSale(int in[], int out[], int remain[], int stock) {
    int sel = commandSubMenu("판매");
    if (sel == 1) saleAll(in, out, remain, stock);
    else if (sel == 2) saleSingle(in, out, remain, stock);
    else printf("잘못된 선택입니다.\n");
}

void saleAll(int in[], int out[], int remain[], int stock) {
    printf("전체 상품 판매수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        out[i] += q;
        // 음수 재고 방지
        if (out[i] > in[i]) out[i] = in[i];
    }
    updateRemain(in, out, remain, stock);
}

void saleSingle(int in[], int out[], int remain[], int stock) {
    int id  = commandId(stock);
    int qty = commandQty("판매");
    out[id - 1] += qty;
    if (out[id - 1] > in[id - 1]) out[id - 1] = in[id - 1]; // 음수 재고 방지
    updateRemain(in, out, remain, stock);
}

// 상품 현황
void showStatus(int in[], int out[], int remain[], int stock) {
    printf("\n-- 상품 현황 --\n");
    for (int i = 0; i < stock; i++) {
        printf("상품 %d -> 입고: %d, 판매: %d, 재고: %d\n", i + 1, in[i], out[i], remain[i]);
    }
}
