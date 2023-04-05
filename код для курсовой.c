#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

struct exh_list {
    int code, picture_year;
    char picture_name[30];
    struct exh_list* next;
    struct exh_list* prev;
};

struct excurs_list {
    int number, price;
    char bilet_type[20];
    char excurs_type[30];
    struct excurs_list* next;
    struct excurs_list* prev;
};

//���������:
struct exh_list* init_exh(int year, char* name) {
    struct exh_list *lst=(struct exh_list*)malloc(sizeof(struct exh_list));
    lst->code=1;
    lst->picture_year=year;
    strcpy(lst->picture_name, name);
    lst->next=NULL;
    lst->prev=NULL;
    return(lst);
}
struct exh_list* add_elem_exh(struct exh_list *lst, int year, char* name) {
    struct exh_list *temp, *p;
    temp=(struct exh_list*)malloc(sizeof(struct exh_list));
    temp->code=lst->code+1;
    p=lst->next;
    lst->next=temp;
    temp->picture_year=year;
    strcpy(temp->picture_name, name);
    temp->next=p;
    temp->prev=lst;
    if (p != NULL)
        p->prev=temp;
    return(temp);
}
struct exh_list* picture_from_file(struct exh_list *ExhibitionList, FILE* pictures) {
    char picture_name[30];
    char picture_year[6];
    struct exh_list *firstelem;
    fgets(picture_name, 30, pictures);
    fgets(picture_year, 6, pictures);
    ExhibitionList=init_exh(atoi(picture_year), picture_name);
    firstelem=ExhibitionList;
    while (fgets(picture_name, 30, pictures)) {
        fgets(picture_year, 6, pictures);
        ExhibitionList=add_elem_exh(ExhibitionList, atoi(picture_year), picture_name);
    }
    return firstelem;
    fclose(pictures);
}
void print_picture(struct exh_list *lst) {
    struct exh_list *p;
    p=lst;
    char picture[30];
    HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
    printf("+"); for (int z=0; z<57; z++) printf("-"); printf("+"); printf("\n");
	printf("| �������� � |      �������� �������      | ��� ��������� |");
	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
    do {
        picture[0]='\0';
        strncat(picture, p->picture_name, strlen(p->picture_name)-1);
        printf("\n"); printf("+"); for (int z=0; z<57; z++) printf("-"); printf("+");
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n|%-12d|%-28s|%-15d|", p->code, picture, p->picture_year);
        p=p->next;
    } while (p != NULL);
    printf("\n+"); for (int z=0; z<57; z++) printf("-"); printf("+");
    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("\n\n");
}
void picture_search(struct exh_list *lst, char* exponat_name) {
    struct exh_list *p;
    p=lst;
    int check=0;
    HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    do {
        if (strcmp(exponat_name, p->picture_name)==0) {
            exponat_name[0]='\0';
            strncat(exponat_name, p->picture_name, strlen(p->picture_name)-1);
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\n������� ������������ � ������ ����������!\n");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
            printf("+"); for (int z=0; z<57; z++) printf("-"); printf("+\n");
            printf("| �������� � |      �������� �������      | ��� ��������� |");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
            printf("\n+"); for (int z=0; z<57; z++) printf("-"); printf("+");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
            printf("\n|%-12d|%-28s|%-15d|", p->code, exponat_name, p->picture_year);
            printf("\n+"); for (int z=0; z<57; z++) printf("-"); printf("+\n\n");
            check=1;
        } p=p->next;
    } while (p != NULL);
    SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    if (check==0) printf("! ����� ������� ��� � ������ ����������\n\n");
}
struct exh_list* find_min_exh(struct exh_list *head) {  //���������������
    struct exh_list *follow;
    for(follow=head->next; follow; follow=follow->next)
        if (follow->picture_name[0] < head->picture_name[0])
            head=follow;
    return head;
}
void sort_by_ABC(struct exh_list *list) {
    char picture_name_tmp[30];
    for(; list; list=list->next) {
        struct exh_list *minPtr=find_min_exh(list);
        if (minPtr != list) {
            strcpy(picture_name_tmp, list->picture_name);
            strcpy(list->picture_name, minPtr->picture_name);
            strcpy(minPtr->picture_name, picture_name_tmp);
            int tmp=list->picture_year;
            list->picture_year=minPtr->picture_year;
            minPtr->picture_year=tmp;
        }
    } }
struct exh_list* deletehead(struct exh_list *root) {
    struct exh_list *temp;
    temp=root->next;
    free(root);
    return(temp);
}
struct exh_list* deletelem(struct exh_list *lst, struct exh_list *root) {
    struct exh_list *temp;
    temp=root;
    while (temp->next != lst)
        temp=temp->next;
    temp->next=lst->next;
    free(lst);
    return(temp);
}
void delete_exh(int N, struct exh_list *lst) {
    struct exh_list *p;
    struct exh_list *current;
    p=lst;
    do {
        if (N == p->code) current=p;
        p=p->next;
    } while (p != NULL);
    deletelem(current, lst);
}
void push_in_picture(struct exh_list *lst) {  //� ������ ��������
    FILE* file=fopen("pictures.txt", "w");
    struct exh_list *p;
    p=lst;
    do {
        fprintf(file, "%s", p->picture_name);
        fprintf(file, "%d\n", p->picture_year);
        p=p->next;
    } while (p != NULL);
    fclose(file);
}
//���������:
struct excurs_list* init_exc(char* bilet, char* excurs, int cost) {
    struct excurs_list *lst;
    lst=(struct excurs_list*)malloc(sizeof(struct excurs_list));
    lst->number=1;
    strcpy(lst->bilet_type, bilet);
    strcpy(lst->excurs_type, excurs);
    lst->price=cost;
    lst->next=NULL;
    lst->prev=NULL;
    return(lst);
}
struct excurs_list* add_elem_exc(struct excurs_list *lst, char* bilet, char* excurs, int cost) {
    struct excurs_list *temp, *p;
    temp=(struct excurs_list*)malloc(sizeof(struct excurs_list));
    temp->number=lst->number+1;
    p=lst->next;
    lst->next=temp;
    strcpy(temp->bilet_type, bilet);
    strcpy(temp->excurs_type, excurs);
    temp->price=cost;
    temp->next=p;
    temp->prev=lst;
    if (p != NULL)
        p->prev=temp;
    return(temp);
}
struct excurs_list* excurs_from_file(struct excurs_list *ExcursionList, FILE* excursions) {
    char bilet_type[20];
    char excurs_type[30];
    char price[6];
    struct excurs_list *firstelem;
    fgets(bilet_type, 20, excursions);
    fgets(excurs_type, 30, excursions);
    fgets(price, 6, excursions);
    ExcursionList=init_exc(bilet_type, excurs_type, atoi(price));
    firstelem=ExcursionList;
    while (fgets(bilet_type, 20, excursions)) {
        fgets(excurs_type, 30, excursions);
        fgets(price, 6, excursions);
        ExcursionList=add_elem_exc(ExcursionList, bilet_type, excurs_type, atoi(price));
    }
    return firstelem;
    fclose(excursions);
}
void print_excursion(struct excurs_list *lst) {
    struct excurs_list *p;
    p=lst;
    char bilet[20];
    char ex_type[30];
    HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
    printf("+"); for (int z=0; z<67; z++) printf("-"); printf("+"); printf("\n");
	printf("|����� ���������|    ��� ������    |   ��� ���������   | ����, ���. |");
	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
    do {
        bilet[0]='\0';
        ex_type[0]='\0';
        strncat(bilet, p->bilet_type, strlen(p->bilet_type)-1);
        strncat(ex_type, p->excurs_type, strlen(p->excurs_type)-1);
        printf("\n"); printf("+"); for (int z=0; z<67; z++) printf("-"); printf("+");
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n|%15d|%18s|%19s|%12d|", p->number, bilet, ex_type, p->price);
        p=p->next;
    } while (p != NULL);
    printf("\n+"); for (int z=0; z<67; z++) printf("-"); printf("+");
    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("\n\n");
}
struct excurs_list* find_min_excurs(struct excurs_list *head) {  //���������������
    struct excurs_list *follow;
    for (follow=head->next; follow; follow=follow->next)
        if (follow->price < head->price)
            head=follow;
    return head;
}
void sort_by_price(struct excurs_list *list) {
    char bilet_type_tmp[20];
    char excurs_type_tmp[30];
    for (; list; list=list->next) {
        struct excurs_list *minPtr=find_min_excurs(list);
        if (minPtr != list) {
            strcpy(bilet_type_tmp, list->bilet_type);
            strcpy(excurs_type_tmp, list->excurs_type);
            strcpy(list->bilet_type, minPtr->bilet_type);
            strcpy(list->excurs_type, minPtr->excurs_type);
            strcpy(minPtr->bilet_type, bilet_type_tmp);
            strcpy(minPtr->excurs_type, excurs_type_tmp);
            int tmp=list->price;
            list->price=minPtr->price;
            minPtr->price=tmp;
        }
    } }
void push_in_basket(int N, struct excurs_list *lst) {
    FILE* file=fopen("tickets.txt", "a");
    struct excurs_list *p;
    p=lst;
    do {
        if (N == p->number) {
            fprintf(file, "\n+����������������������������������+\n");
            fprintf(file, "|           ��������� �%d           |\n", p->number);
            fprintf(file, "+==================================+\n\n");
            fprintf(file, "  *��� ������: %s", p->bilet_type);
            fprintf(file, "  *��� ���������: %s", p->excurs_type);
            fprintf(file, "....................................\n");
            fprintf(file, "  ����: %d ���.\n", p->price);
        }
        p=p->next;
    } while (p != NULL);
    fclose(file);
}
void check() {
    FILE* file_tickets=fopen("tickets.txt", "r");
    FILE* file_check=fopen("check.txt", "w");
    int res; char ch;
    fprintf(file_check, "--------------��� ���---------------\n");
    while ((res=fscanf(file_tickets, "%c", &ch))!=EOF) fprintf(file_check, "%c", ch);
    fclose(file_check);
    fclose(file_tickets);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("title �������� ������");
    int choice=0, z;
    HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    struct exh_list *ExhibitionList=NULL;
    FILE* pictures=fopen("pictures.txt", "r");
    if (!pictures) {
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n! ������ �������� ����� pictures.txt\n\n"); system("pause>nul|set/p=��������� ���� � ������������� ���������."); exit(1); }
    ExhibitionList=picture_from_file(ExhibitionList, pictures);
    struct excurs_list *ExcursionList=NULL;
    FILE* excursions=fopen("excursions.txt", "r");
    if (!excursions) {
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n! ������ �������� ����� excursions.txt\n\n"); system("pause>nul|set/p=��������� ���� � ������������� ���������."); exit(1); }
    ExcursionList=excurs_from_file(ExcursionList, excursions);

    while (choice != 3) {
        system("cls");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE);
        printf("\n\t\t\t     < �������� ����������� ������ >     \n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
        for (z=0; z<90; z++) printf("."); printf("\n");
        for (z=0; z<90; z++) printf("*"); printf("\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("\n\n����� ���������� �� �������� ");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("���� ���. ����� ����� � ��������\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        printf("\n1| ���� ��� ������������\n2| ������ � ������ ��������������\n3| ����� �� ����������\n\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("��� ������ ������� ��������������� �����: ");
        scanf("%d", &choice);
        rewind(stdin);
        switch(choice) {
        case 1: {
            int point_1=0;
            while (point_1 != 3) {
                system("cls");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                for (z=0; z<90; z++) printf("_"); printf("\n");
                printf("\n\t\t\t      1 | ������� ���� ������������\n");
                for (z=0; z<90; z++) printf("_"); printf("\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                printf("\n-> 1. ��� ��� ��� �� ��������\n-> 2. ��� ������\n-> 3. ����� � ������� ����\n\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                printf("�������� �����: ");
                scanf("%d", &point_1);
                rewind(stdin);
                switch(point_1) {
                case 1: {
                    int point_1_0=0;
                    while (point_1_0 != 4) {
                        system("cls");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n\t\t\t\t ��� ��� ��� �� ��������\n");
                        for (z=0; z<90; z++) printf("_"); printf("\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n1) ����� ����������\n2) ���������\n3) ���������\n4) �����\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("������������ ������: ");
                        scanf("%d", &point_1_0);
                        rewind(stdin);
                        switch(point_1_0) {
                        case 1: {
                            system("cls");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---����� ����������---\n\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|COMMON_LVB_UNDERSCORE);
                            printf("���������� ����������");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_1 = fopen("info_1.txt", "r");
                            int res; char ch;
                            while ((res=fscanf(info_1, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_1);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                            printf("\n�������");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_2 = fopen("info_2.txt", "r");
                            while ((res=fscanf(info_2, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_2);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                            printf("������� ������ ");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_3 = fopen("info_3.txt", "r");
                            while ((res=fscanf(info_3, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_3);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("\n\n����� ������:\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_4 = fopen("info_4.txt", "r");
                            while ((res=fscanf(info_4, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_4);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                            printf("\n\n�������� ��� ���� ������� ������ � �������\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|COMMON_LVB_UNDERSCORE);
                            FILE* info_5 = fopen("info_5.txt", "r");
                            while ((res=fscanf(info_5, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_5);
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
                            printf("\n��������� � ���� ����� �� ��������:");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            printf(" +7(915) 268 22 36.\n\n");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");  //��������� ������ �� pause
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            break;
                        }
                        case 2: {
                            int point_1_1=0;
                            while (point_1_1 != 4) {
                                system("cls");
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                                printf("---���������---\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("1) ����� ���� ����������\n2) ����� �� ��������\n3) ���������� �� ��������\n4) �����\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("������������ ������: ");
                                scanf("%d", &point_1_1);
                                rewind(stdin);
                                switch(point_1_1) {
                                case 1: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� �� �������� ������������ 12 ���������� ������ ��� ���� ��\n\n");
                                    print_picture(ExhibitionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 2: {
                                    system("cls");
                                    char string[30];
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� ����� ������� ��\n\n");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("����������, ������� ������ ��������: ");
                                    fflush(stdin);
                                    gets(string);
                                    strcat(string, "\n");
                                    picture_search(ExhibitionList, string);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 3: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� ���������� ���������� �� �������� ��\n\n");
                                    sort_by_ABC(ExhibitionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("��������� ������� �������������:\n");
                                    print_picture(ExhibitionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 4: break;
                            } }
                            break;
                        }
                        case 3: {
                            int point_1_2=0;
                            while (point_1_2 != 4) {
                                system("cls");
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                                printf("---���������---\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("1) ����� ���� ���������\n2) ���������� �� ����\n3) ���������� � ���� �������\n4) �����\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("������������ ������: ");
                                scanf("%d", &point_1_2);
                                rewind(stdin);
                                switch(point_1_2) {
                                case 1: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� �������� ��������� ��\n\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 2: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� ���������� ��������� �� ���� ������ ��\n\n");
                                    sort_by_price(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("��������� ������� �������������:\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 3: {
                                    system("cls");
                                    int vibor;
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("�� ���������� � ���� ������� ��\n\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("������� ����� ��������� ��� ���������� � �������: ");
                                    scanf("%d", &vibor);
                                    rewind(stdin);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    if ((vibor>0) && (vibor<10)) {
                                        push_in_basket(vibor, ExcursionList);
                                        printf("��������� ������� ���������!\n\n");
                                    } else printf("������������ ��������. ������� ����� �� 1 �� 9.\n\n");
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 4: break;
                            } }
                            break;
                        }
                        case 4: break;
                    } }
                break;
                }
                case 2: {
                    int point_1_3=0;
                    while (point_1_3 != 4) {
                        system("cls");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n\t\t\t\t\t��� ������\n");
                        for (z=0; z<90; z++) printf("_"); printf("\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n1) �������\n2) �������� �������\n3) �������� �����\n4) �����\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("������������ ������: ");
                        scanf("%d", &point_1_3);
                        rewind(stdin);
                        switch(point_1_3) {
                        case 1: {
                            system("cls");
                            int res; char ch;
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---�������---\n\n");
                            FILE* tickets=fopen("tickets.txt", "r");
                            fseek(tickets, 0, SEEK_END);  //������� � ����� �����
                            int pos=ftell(tickets);  //������� ������� � tickets
                            if (pos <= 0) {
                                fclose(tickets);
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("���� ������� ���� ��� ������!\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                printf("��������� � ������: 3) �������� �����\n\n");
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                system("pause");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                break;
                            }
                            else {
                                tickets=fopen("tickets.txt", "r");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                printf("������ � �������:");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                                while ((res=fscanf(tickets, "%c", &ch))!=EOF) printf("%c", ch);
                                fclose(tickets);
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
                                printf("\n\n������� ����������? (1 - ��, 0 - ���) ");
                                int buy;
                                do {
                                    scanf("%d", &buy);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    switch(buy) {
                                        case 0: {
                                            system("cls");
                                            point_1_3=0;
                                            break;
                                        }
                                        case 1: {
                                            check();
                                            printf("\n�������! ������ �����������.\n��� ��� ��������� � ����� check.txt\n\n");
                                            FILE* file=fopen("tickets.txt", "w");
                                            fclose(file);
                                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            system("pause");
                                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            break;
                                        }
                                        default: {
                                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            printf("\n�� ����� ����������� ��������. ����������, ���������� ��� ��� ");
                                            rewind(stdin);
                                            break;
                                    } }
                                } while ((buy!=0) && (buy!=1));
                            };
                            break;
                        }
                        case 2: {
                            system("cls");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---�������� �������---\n\n");
                            FILE* file=fopen("tickets.txt", "w");
                            fclose(file);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("~ ������� ������� ������� ~\n\n");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            break;
                        }
                        case 3: {
                            system("cls");
                            int vibor;
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---�������� �����---\n\n");
                            print_excursion(ExcursionList);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("������� ����� ��������� ��� ���������� � �������: ");
                            scanf("%d", &vibor);
                            rewind(stdin);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            if ((vibor>0) && (vibor<10)) {
                                push_in_basket(vibor, ExcursionList);
                                printf("��������� ������� ���������!\n\n");
                            } else printf("������������ ��������. ������� ����� �� 1 �� 9.\n\n");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            system("pause");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            break;
                        }
                        case 4: break;
                    } }
                break;
                }
                case 3: break;
        } } break;
        }
        case 2: {
            int point_2 = 0;
            system("cls");
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            for (z=0; z<90; z++) printf("_"); printf("\n");
            printf("\n\t\t\t    2 | ���� ���������� - �������������\n");
            for (z=0; z<90; z++) printf("_"); printf("\n");
            const char str[10]="password2";
            char s[10];
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\n������� ������ ��� ����� � �������: ");
            scanf("%s", s);
            if (strcmp(str, s)==0) {
                while (point_2 != 4) {
                    system("cls");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\n\t\t\t\t  �������������� �������\n");
                    for (z=0; z<90; z++) printf("_"); printf("\n");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\n1. ����� �������\n2. �������� ��������\n3. ������� ������\n4. ����� � ������� ����\n");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\n�������� ��������: ");
                    scanf("%d", &point_2);
                    rewind(stdin);
                    switch(point_2) {
                    case 1: {
                        system("cls");
                        char string[30];
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---����� �������---\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                        printf("����������, ������� ������ ��������: ");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN);
                        fflush(stdin);
                        gets(string);
                        strcat(string, "\n");
                        picture_search(ExhibitionList, string);
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 2: {
                        system("cls");
                        char picture_name[30];
                        int year;
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---�������� ��������---\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                        printf("\n������� �������� �������: ");
                        fflush(stdin);
                        gets(picture_name);
                        strcat(picture_name, "\n");
                        printf("\n������� ��� ��������: ");
                        scanf("%d", &year);
                        while ((year>1890) || (year<1880)) {
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("������. ������� ��������� ��� ��������� (1880-1890): ");
                            fflush(stdin);
                            scanf("%d", &year); }
                        struct exh_list *ptr, *current;
                        ptr=ExhibitionList;
                        do ptr=ptr->next; while (ptr->next != NULL);
                        current=add_elem_exh(ptr, year, picture_name);
                        FILE* file=fopen("pictures.txt", "a");
                        fprintf(file, "%s\n", current->picture_name);
                        fprintf(file, "%d\n", current->picture_year);
                        fclose(file);
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n�������� ������� ��������!\n\n");
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 3: {
                        system("cls");
                        int count=0, delete_num;
                        FILE* file=fopen("pictures.txt", "r");
                        while (!feof(file)) {
                            if (fgetc(file)=='\n')
                            count++;  //���-�� ����� � �����
                        } fclose(file);
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---������� ������---\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                        printf("������ ����������:\n");
                        print_picture(ExhibitionList);
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("������� ����� ��������� ��� ��������: ");
                        scanf("%d", &delete_num);
                        rewind(stdin);
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        if ((delete_num>0) && (delete_num<=(count/2))) {
                            delete_exh(delete_num, ExhibitionList);
                            printf("������ ������� �������!\n\n");
                            push_in_picture(ExhibitionList); }
                        else if (delete_num==1) {
                            ExhibitionList=deletehead(ExhibitionList);
                            printf("������ ������� �������!\n\n");
                            push_in_picture(ExhibitionList);
                        } else printf("\n�� ����� ����������� ��������.\n");
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=������� ����� �������, ����� ��������� �����...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 4: break;
            } } }
            else {
                SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_GREEN); printf("\n������ ��������.\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                system("pause>nul|set/p=�� ������ �������������� � ������� ����, ����� ����� �������...");
        } break;
        }
        case 3: {
            system("cls");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\n�� ������������� ������ ����� �� ���������?\n");
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\n1 - �� / 0 - ���: ");
            int figure;
            do {
                scanf("%d", &figure);
                rewind(stdin);
                switch(figure) {
                case 0: {
                    system("cls");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    choice=0;
                    break;
                }
                case 1: {
                    system("cls");
                    break;
                }
                default: {
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                    printf("\n�� ����� ����������� ��������. ����������, ���������� ��� ��� ");
                    rewind(stdin);
                    break;
                } }
            } while ((figure!=0) && (figure!=1));
            break;
        } default: {
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
            printf("\n������� ������ ����� ������. ���������� ��������: 1, 2, 3.\n������� Enter ��� ���������� �����");
            getchar();
            break;
    } } }
    free(ExhibitionList);
    free(ExcursionList);
    return 0;
}
