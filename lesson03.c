#include <stdio.h>

#define MAX_ITEMS 100


int  prompt_menu(void);
int  prompt_submenu(const char *action);
int  prompt_id(int stock);
int  prompt_qty(const char *label);
void update_remain(int in[], int out[], int remain[], int stock);


void handle_inbound(int in[], int out[], int remain[], int stock);
void inbound_all(int in[], int out[], int remain[], int stock);
void inbound_single(int in[], int out[], int remain[], int stock);

void handle_sale(int in[], int out[], int remain[], int stock);
void sale_all(int in[], int out[], int remain[], int stock);
void sale_single(int in[], int out[], int remain[], int stock);

void show_status(int in[], int out[], int remain[], int stock);

/* ---------- main ---------- */
int main(void) {
    int stock = 5;                 // 상품 개수(기본 5개)
    int in[MAX_ITEMS]    = {0};    // 입고 누적
    int out[MAX_ITEMS]   = {0};    // 판매(출고) 누적
    int remain[MAX_ITEMS]= {0};    // 재고

    while (1) {
        int menu = prompt_menu();

        if (menu == 1) {
            handle_inbound(in, out, remain, stock);
        } else if (menu == 2) {
            handle_sale(in, out, remain, stock);
        } else if (menu == 3) {
            show_status(in, out, remain, stock);
        } else if (menu == 4) {
            printf("프로그램을 종료합니다.\n");
            break;
        } else {
            printf("잘못된 메뉴입니다. 다시 선택하세요.\n");
        }
    }
    return 0;
}

/* ---------- 공통/유틸 구현 ---------- */

// 메인 메뉴
int prompt_menu(void) {
    int sel;
    printf("\n[쇼핑몰 관리 프로그램]\n");
    printf("원하는 메뉴를 선택하세요. (1. 입고, 2. 판매, 3. 상품현황, 4. 종료)\n> ");
    scanf("%d", &sel);
    return sel;
}

// 서브 메뉴(전체/개별)
int prompt_submenu(const char *action) {
    int sel;
    printf("%s 수량 입력: 전체=1, 개별=2 를 선택하세요.\n> ", action);
    scanf("%d", &sel);
    return sel;
}

// 상품 ID 입력
int prompt_id(int stock) {
    int id;
    printf("상품ID 입력 (1~%d): ", stock);
    scanf("%d", &id);
    if (id < 1) id = 1;
    if (id > stock) id = stock;
    return id;
}

// 수량 입력
int prompt_qty(const char *label) {
    int q;
    printf("%s 수량 입력: ", label);
    scanf("%d", &q);
    if (q < 0) q = 0; // 음수 방지
    return q;
}

// 재고 갱신
void update_remain(int in[], int out[], int remain[], int stock) {
    for (int i = 0; i < stock; i++) {
        remain[i] = in[i] - out[i];
    }
}

// 입고 메뉴 라우팅
void handle_inbound(int in[], int out[], int remain[], int stock) {
    int sel = prompt_submenu("입고");
    if (sel == 1) inbound_all(in, out, remain, stock);
    else if (sel == 2) inbound_single(in, out, remain, stock);
    else printf("잘못된 선택입니다.\n");
}

// 전체 입고
void inbound_all(int in[], int out[], int remain[], int stock) {
    printf("전체 상품 입고수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        in[i] += q;
    }
    update_remain(in, out, remain, stock);
}
void inbound_single(int in[], int out[], int remain[], int stock) {
    int id  = prompt_id(stock);
    int qty = prompt_qty("입고");
    in[id - 1] += qty;
    update_remain(in, out, remain, stock);
}

// 판매 기능
void handle_sale(int in[], int out[], int remain[], int stock) {
    int sel = prompt_submenu("판매");
    if (sel == 1) sale_all(in, out, remain, stock);
    else if (sel == 2) sale_single(in, out, remain, stock);
    else printf("잘못된 선택입니다.\n");
}

void sale_all(int in[], int out[], int remain[], int stock) {
    printf("전체 상품 판매수량을 순서대로 입력하세요 (총 %d개):\n> ", stock);
    for (int i = 0; i < stock; i++) {
        int q; scanf("%d", &q);
        if (q < 0) q = 0;
        out[i] += q;
        // 음수 재고 방지
        if (out[i] > in[i]) out[i] = in[i];
    }
    update_remain(in, out, remain, stock);
}

void sale_single(int in[], int out[], int remain[], int stock) {
    int id  = prompt_id(stock);
    int qty = prompt_qty("판매");
    out[id - 1] += qty;
    if (out[id - 1] > in[id - 1]) out[id - 1] = in[id - 1]; // 음수 재고 방지
    update_remain(in, out, remain, stock);
}

// 상품 현황
void show_status(int in[], int out[], int remain[], int stock) {
    printf("\n-- 상품 현황 --\n");
    for (int i = 0; i < stock; i++) {
        printf("상품 %d -> 입고: %d, 판매: %d, 재고: %d\n",
               i + 1, in[i], out[i], remain[i]);
    }
}
