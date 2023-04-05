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

//ЭКСПОНАТЫ:
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
	printf("| Экспонат № |      Название картины      | Год написания |");
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
            printf("\nКартина присутствует в списке экспонатов!\n");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
            printf("+"); for (int z=0; z<57; z++) printf("-"); printf("+\n");
            printf("| Экспонат № |      Название картины      | Год написания |");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
            printf("\n+"); for (int z=0; z<57; z++) printf("-"); printf("+");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
            printf("\n|%-12d|%-28s|%-15d|", p->code, exponat_name, p->picture_year);
            printf("\n+"); for (int z=0; z<57; z++) printf("-"); printf("+\n\n");
            check=1;
        } p=p->next;
    } while (p != NULL);
    SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    if (check==0) printf("! Такой картины нет в списке экспонатов\n\n");
}
struct exh_list* find_min_exh(struct exh_list *head) {  //вспомогательная
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
void push_in_picture(struct exh_list *lst) {  //с учётом удалённых
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
//ЭКСКУРСИИ:
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
	printf("|Номер экскурсии|    Тип билета    |   Вид экскурсии   | Цена, руб. |");
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
struct excurs_list* find_min_excurs(struct excurs_list *head) {  //вспомогательная
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
            fprintf(file, "\n+——————————————————————————————————+\n");
            fprintf(file, "|           Экскурсия №%d           |\n", p->number);
            fprintf(file, "+==================================+\n\n");
            fprintf(file, "  *Тип билета: %s", p->bilet_type);
            fprintf(file, "  *Вид экскурсии: %s", p->excurs_type);
            fprintf(file, "....................................\n");
            fprintf(file, "  ЦЕНА: %d руб.\n", p->price);
        }
        p=p->next;
    } while (p != NULL);
    fclose(file);
}
void check() {
    FILE* file_tickets=fopen("tickets.txt", "r");
    FILE* file_check=fopen("check.txt", "w");
    int res; char ch;
    fprintf(file_check, "--------------ВАШ ЧЕК---------------\n");
    while ((res=fscanf(file_tickets, "%c", &ch))!=EOF) fprintf(file_check, "%c", ch);
    fclose(file_check);
    fclose(file_tickets);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("title КУРСОВАЯ РАБОТА");
    int choice=0, z;
    HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    struct exh_list *ExhibitionList=NULL;
    FILE* pictures=fopen("pictures.txt", "r");
    if (!pictures) {
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n! ОШИБКА ОТКРЫТИЯ ФАЙЛА pictures.txt\n\n"); system("pause>nul|set/p=Проверьте файл и перезапустите программу."); exit(1); }
    ExhibitionList=picture_from_file(ExhibitionList, pictures);
    struct excurs_list *ExcursionList=NULL;
    FILE* excursions=fopen("excursions.txt", "r");
    if (!excursions) {
        SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_INTENSITY);
        printf("\n! ОШИБКА ОТКРЫТИЯ ФАЙЛА excursions.txt\n\n"); system("pause>nul|set/p=Проверьте файл и перезапустите программу."); exit(1); }
    ExcursionList=excurs_from_file(ExcursionList, excursions);

    while (choice != 3) {
        system("cls");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE);
        printf("\n\t\t\t     < ВЫСТАВКА РЕПРОДУКЦИЙ КАРТИН >     \n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
        for (z=0; z<90; z++) printf("."); printf("\n");
        for (z=0; z<90; z++) printf("*"); printf("\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("\n\nДобро пожаловать на выставку ");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("«ВАН ГОГ. ТАЙНЫ ГЕНИЯ И БЕЗУМЦА»\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        printf("\n1| Вход для Пользователя\n2| Доступ в панель Администратора\n3| Выход из приложения\n\n");
        SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("Для работы введите соответствующую цифру: ");
        scanf("%d", &choice);
        rewind(stdin);
        switch(choice) {
        case 1: {
            int point_1=0;
            while (point_1 != 3) {
                system("cls");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                for (z=0; z<90; z++) printf("_"); printf("\n");
                printf("\n\t\t\t      1 | Главное меню Пользователя\n");
                for (z=0; z<90; z++) printf("_"); printf("\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                printf("\n-> 1. Что Вас ждёт на выставке\n-> 2. Мои билеты\n-> 3. Выход в главное меню\n\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                printf("Выберите пункт: ");
                scanf("%d", &point_1);
                rewind(stdin);
                switch(point_1) {
                case 1: {
                    int point_1_0=0;
                    while (point_1_0 != 4) {
                        system("cls");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n\t\t\t\t Что Вас ждёт на выставке\n");
                        for (z=0; z<90; z++) printf("_"); printf("\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n1) Общая информация\n2) Экспонаты\n3) Экскурсии\n4) Назад\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("Интересующий раздел: ");
                        scanf("%d", &point_1_0);
                        rewind(stdin);
                        switch(point_1_0) {
                        case 1: {
                            system("cls");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---Общая информация---\n\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|COMMON_LVB_UNDERSCORE);
                            printf("Уникальная экспозиция");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_1 = fopen("info_1.txt", "r");
                            int res; char ch;
                            while ((res=fscanf(info_1, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_1);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                            printf("\nПолотна");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_2 = fopen("info_2.txt", "r");
                            while ((res=fscanf(info_2, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_2);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                            printf("технике «жикле» ");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_3 = fopen("info_3.txt", "r");
                            while ((res=fscanf(info_3, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_3);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("\n\nРЕЖИМ РАБОТЫ:\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            FILE* info_4 = fopen("info_4.txt", "r");
                            while ((res=fscanf(info_4, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_4);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                            printf("\n\nОткройте для себя великие сюжеты и узнайте\n");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|COMMON_LVB_UNDERSCORE);
                            FILE* info_5 = fopen("info_5.txt", "r");
                            while ((res=fscanf(info_5, "%c", &ch)) != EOF) printf("%c", ch);
                            fclose(info_5);
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
                            printf("\nСвязаться с нами можно по телефону:");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            printf(" +7(915) 268 22 36.\n\n");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");  //изменение текста от pause
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            break;
                        }
                        case 2: {
                            int point_1_1=0;
                            while (point_1_1 != 4) {
                                system("cls");
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                                printf("---Экспонаты---\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("1) Вывод всех экспонатов\n2) Поиск по названию\n3) Сортировка по алфавиту\n4) Назад\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("Интересующий раздел: ");
                                scanf("%d", &point_1_1);
                                rewind(stdin);
                                switch(point_1_1) {
                                case 1: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("—— На выставке представлены 12 масштабных картин Ван Гога ——\n\n");
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
                                    printf("—— Поиск картины ——\n\n");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("Пожалуйста, введите точное название: ");
                                    fflush(stdin);
                                    gets(string);
                                    strcat(string, "\n");
                                    picture_search(ExhibitionList, string);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 3: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("—— Сортировка экспонатов по алфавиту ——\n\n");
                                    sort_by_ABC(ExhibitionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("Экспонаты успешно отсортированы:\n");
                                    print_picture(ExhibitionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
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
                                printf("---Экскурсии---\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("1) Вывод всех экскурсий\n2) Сортировка по цене\n3) Добавление в «Мои билеты»\n4) Назад\n\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("Интересующий раздел: ");
                                scanf("%d", &point_1_2);
                                rewind(stdin);
                                switch(point_1_2) {
                                case 1: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("—— Варианты экскурсий ——\n\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 2: {
                                    system("cls");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("—— Сортировка экскурсий по цене билета ——\n\n");
                                    sort_by_price(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("Экскурсии успешно отсортированы:\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                    break;
                                }
                                case 3: {
                                    system("cls");
                                    int vibor;
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                    printf("—— Добавление в «Мои билеты» ——\n\n");
                                    print_excursion(ExcursionList);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    printf("Введите номер экскурсии для добавления в корзину: ");
                                    scanf("%d", &vibor);
                                    rewind(stdin);
                                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                    if ((vibor>0) && (vibor<10)) {
                                        push_in_basket(vibor, ExcursionList);
                                        printf("Экскурсия успешно добавлена!\n\n");
                                    } else printf("Некорректное значение. Введите цифру от 1 до 9.\n\n");
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
                        printf("\n\t\t\t\t\tМои билеты\n");
                        for (z=0; z<90; z++) printf("_"); printf("\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("\n1) Корзина\n2) Очистить корзину\n3) Добавить билет\n4) Назад\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("Интересующий раздел: ");
                        scanf("%d", &point_1_3);
                        rewind(stdin);
                        switch(point_1_3) {
                        case 1: {
                            system("cls");
                            int res; char ch;
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---Корзина---\n\n");
                            FILE* tickets=fopen("tickets.txt", "r");
                            fseek(tickets, 0, SEEK_END);  //коретка в конец файла
                            int pos=ftell(tickets);  //текущая позиция в tickets
                            if (pos <= 0) {
                                fclose(tickets);
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                                printf("Ваша корзина пока что пустая!\n");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                printf("Перейдите в раздел: 3) Добавить билет\n\n");
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                system("pause");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                break;
                            }
                            else {
                                tickets=fopen("tickets.txt", "r");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                                printf("Билеты в корзине:");
                                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
                                while ((res=fscanf(tickets, "%c", &ch))!=EOF) printf("%c", ch);
                                fclose(tickets);
                                SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY);
                                printf("\n\nЖелаете приобрести? (1 - Да, 0 - Нет) ");
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
                                            printf("\nСпасибо! Билеты приобретены.\nВаш ЧЕК находится в файле check.txt\n\n");
                                            FILE* file=fopen("tickets.txt", "w");
                                            fclose(file);
                                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            system("pause");
                                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            break;
                                        }
                                        default: {
                                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                                            printf("\nВы ввели некоректное значение. Пожалуйста, попробуйте ещё раз ");
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
                            printf("---Очистить корзину---\n\n");
                            FILE* file=fopen("tickets.txt", "w");
                            fclose(file);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("~ КОРЗИНА УСПЕШНО ОЧИЩЕНА ~\n\n");
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                            break;
                        }
                        case 3: {
                            system("cls");
                            int vibor;
                            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                            printf("---Добавить билет---\n\n");
                            print_excursion(ExcursionList);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("Введите номер экскурсии для добавления в корзину: ");
                            scanf("%d", &vibor);
                            rewind(stdin);
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            if ((vibor>0) && (vibor<10)) {
                                push_in_basket(vibor, ExcursionList);
                                printf("Экскурсия успешно добавлена!\n\n");
                            } else printf("Некорректное значение. Введите цифру от 1 до 9.\n\n");
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
            printf("\n\t\t\t    2 | Меню управления - АДМИНИСТРАТОР\n");
            for (z=0; z<90; z++) printf("_"); printf("\n");
            const char str[10]="password2";
            char s[10];
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\nВведите пароль для входа в систему: ");
            scanf("%s", s);
            if (strcmp(str, s)==0) {
                while (point_2 != 4) {
                    system("cls");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\n\t\t\t\t  Редактирование записей\n");
                    for (z=0; z<90; z++) printf("_"); printf("\n");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\n1. Поиск картины\n2. Добавить экспонат\n3. Удалить запись\n4. Выход в главное меню\n");
                    SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY);
                    printf("\nВыберите действие: ");
                    scanf("%d", &point_2);
                    rewind(stdin);
                    switch(point_2) {
                    case 1: {
                        system("cls");
                        char string[30];
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---Поиск картины---\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                        printf("Пожалуйста, введите точное название: ");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN);
                        fflush(stdin);
                        gets(string);
                        strcat(string, "\n");
                        picture_search(ExhibitionList, string);
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 2: {
                        system("cls");
                        char picture_name[30];
                        int year;
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---Добавить экспонат---\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN);
                        printf("\nВведите название картины: ");
                        fflush(stdin);
                        gets(picture_name);
                        strcat(picture_name, "\n");
                        printf("\nВведите год создания: ");
                        scanf("%d", &year);
                        while ((year>1890) || (year<1880)) {
                            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                            printf("Ошибка. Введите подлинный год написания (1880-1890): ");
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
                        printf("\nЭкспонат успешно добавлен!\n\n");
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 3: {
                        system("cls");
                        int count=0, delete_num;
                        FILE* file=fopen("pictures.txt", "r");
                        while (!feof(file)) {
                            if (fgetc(file)=='\n')
                            count++;  //кол-во строк в файле
                        } fclose(file);
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
                        printf("---Удалить запись---\n\n");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                        printf("Список экспонатов:\n");
                        print_picture(ExhibitionList);
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
                        printf("Введите номер экспоната для удаления: ");
                        scanf("%d", &delete_num);
                        rewind(stdin);
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        if ((delete_num>0) && (delete_num<=(count/2))) {
                            delete_exh(delete_num, ExhibitionList);
                            printf("Запись успешно удалена!\n\n");
                            push_in_picture(ExhibitionList); }
                        else if (delete_num==1) {
                            ExhibitionList=deletehead(ExhibitionList);
                            printf("Запись успешно удалена!\n\n");
                            push_in_picture(ExhibitionList);
                        } else printf("\nВы ввели некоректное значение.\n");
                        SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|BACKGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        system("pause>nul|set/p=Нажмите любую клавишу, чтобы вернуться назад...");
                        SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                        break; }
                    case 4: break;
            } } }
            else {
                SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED|BACKGROUND_GREEN); printf("\nПароль неверный.\n");
                SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
                system("pause>nul|set/p=Вы будете перенаправлены в главное меню, нажав любую клавишу...");
        } break;
        }
        case 3: {
            system("cls");
            SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\nВы действительно хотите выйти из программы?\n");
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            printf("\n1 - да / 0 - нет: ");
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
                    printf("\nВы ввели некоректное значение. Пожалуйста, попробуйте ещё раз ");
                    rewind(stdin);
                    break;
                } }
            } while ((figure!=0) && (figure!=1));
            break;
        } default: {
            SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
            printf("\nНеверно выбран режим работы. Допустимые варианты: 1, 2, 3.\nНажмите Enter для повторного ввода");
            getchar();
            break;
    } } }
    free(ExhibitionList);
    free(ExcursionList);
    return 0;
}
