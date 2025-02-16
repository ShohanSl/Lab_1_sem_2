#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Для INT_MAX и INT_MIN

int int_input_cheking_pos() {
    int res = 0;
    char inp;
    while (1) {
        while (1) {
            inp = getchar();
            if (inp == '\n') {
                break;
            } else if (inp == 45 && res == 0) {
                printf("\nДанное число должно быть больше нуля\nПожалуйста, введите положительное число\n");
                res = 0;
                while (getchar() != '\n');
                break;
            } else if (inp < 48 || inp > 57) {
                printf("\nВведены символы помимо цифр\nПожалуйста, введите целое число\n");
                res = 0;
                while (getchar() != '\n');
                break;
            } else if (inp == '0' && res == 0) {
                printf("\nДанное число не может быть равно нулю\nПожалуйста, введите другое число\n");
                while (getchar() != '\n');
                break;
            } else {
                res = res * 10 + (inp - '0');
            }
        }
        if (res != 0) break;
    }
    return res;
}

int int_input_cheking() {
    int negative = 0, res = 0, wrong = 0;
    char inp;
    while (1) {
        wrong = 0;
        while (1) {
            inp = getchar();
            if (inp == '\n') {
                break;
            } else if (inp == 45 && negative == 0 && res == 0) negative = 1;
            else if (inp < 48 || inp > 57) {
                printf("\nВведены символы помимо цифр\nПожалуйста, введите целое число\n");
                res = 0;
                negative = 0;
                while (getchar() != '\n');
                wrong = 1;
                break;
            } else {
                res = res * 10 + (inp - '0');
            }
        }
        if (wrong == 0) {
            if (negative == 1 && res == 0) {
                printf("\nЧисло 0 не может быть отрицательным\nПожалуйста, повторите ввод\n");
                res = 0;
                negative = 0;
            } else if (negative == 1) {
                res = -res;
                break;
            } else break;
        }
    }
    return res;
}

char* file_name_input() {
    int file_name_length = 0, i;
    char* file_name, bin[] = ".bin", inp_symb;
    file_name = (char*)malloc(sizeof(char));
    printf("\nВведите название файла (Без учёта расширения):\n\n");
    while (1) {
        inp_symb = getchar();
        if (inp_symb == '\n') {
            break;
        } else {
            file_name[file_name_length] = inp_symb;
            file_name_length++;
            file_name = (char*)realloc(file_name, (file_name_length + 1) * sizeof(char));
        }
    }
    file_name = (char*)realloc(file_name, file_name_length + 5);
    for (i = 0; i < 4; i++) {
        file_name[file_name_length + i] = bin[i];
    }
    file_name[file_name_length + 4] = '\0';
    return file_name;
}

void file_deleting(char* file_name) {
    int inp;
    while (1) {
        printf("\nУдалить созданный файл?\n1 - Да\n0 - Нет\n\n");
        inp = int_input_cheking();
        if (inp == 1) {
            if (remove(file_name) != 0) {
                printf("\nОшибка удаления файла\n");
                break;
            } else {
                printf("\nФайл удалён успешно\n");
                break;
            }
        } else if (inp != 0) {
            printf("\nКоманда не распознана\n");
        } else break;
    }
}

FILE* file_creating(char* file_name) {
    FILE* file;
    fopen_s(&file, file_name, "wb");
    if (file == NULL) {
        printf("Неизвестная ошибка при открытии файла");
    }
    return file;
}

void read_and_print_file(char* file_name) {
    FILE* file = fopen(file_name, "rb");
    int number;
    if (file == NULL) {
        printf("Ошибка открытия файла для чтения\n");
        return;
    }
    printf("\nСодержимое файла %s:\n", file_name);
    while (fread(&number, sizeof(int), 1, file) == 1) {
        printf("%d ", number);
    }
    printf("\n");

    fclose(file);
}

void swap(FILE* file, int pos1, int pos2){
    int temp1, temp2;
    fseek(file, pos1*sizeof(int), SEEK_SET);
    fread(&temp1, sizeof(int), 1, file);
    fseek(file, pos2*sizeof(int), SEEK_SET);
    fread(&temp2, sizeof(int), 1, file);
    fseek(file, pos2*sizeof(int), SEEK_SET);
    fwrite(&temp1, sizeof(int), 1, file);
    fseek(file, pos1*sizeof(int), SEEK_SET);
    fwrite(&temp2, sizeof(int), 1, file);
}

int partition(FILE* file, int low, int high){
    int pivot, current, i, j;
    fseek(file, high*sizeof(int), SEEK_SET);
    fread(&pivot, sizeof(int), 1, file);
    i=low-1;
    for (j=low; j<high; j++){
        fseek(file, j*sizeof(int), SEEK_SET);
        fread(&current, sizeof(int), 1, file);
        if (current<=pivot){
            i++;
            swap(file, i, j);
        }
    }
    swap(file, i+1, high);
    return i+1;
}

void quickSort(FILE* file, int low, int high){
    if (low<high){
        int pi = partition(file, low, high);
        quickSort(file, low, pi-1);
        quickSort(file, pi+1, high);
    }
}

void task1() {
    FILE* file;
    int count, inp, i;
    char* file_name;
    file_name = file_name_input();
    printf("\nВведите количество натуральных чисел, которые собираетесь записать в файл:\n\n");
    count = int_input_cheking_pos();
    if (count == 1) {
        printf("\nВведите число:\n\n");
    } else {
        printf("\nВведите числа:\n\n");
    }
    file = file_creating(file_name);
    if (file == NULL) {
        free(file_name);
        return;
    }
    for (i = 0; i < count; i++) {
        inp = int_input_cheking_pos();
        fwrite(&inp, sizeof(int), 1, file);
    }
    printf("\nВвод окончен\nЗаписаны числа:\n\n");
    freopen(file_name, "rb", file);
    for (i = 0; i < count; i++) {
        fread(&inp, sizeof(int), 1, file);
        printf("%d ", inp);
    }
    fclose(file);
    file_deleting(file_name);
    free(file_name);
}

void task2() {
    FILE* file;
    int max_weight, obj_count, i, inp, j, count, max_count = 0, test_weight, out;
    char* file_name;
    file_name = file_name_input();
    printf("\nВведите грузоподъёмность автомобиля:\n\n");
    max_weight = int_input_cheking_pos();
    printf("\nВведите количество перевозимых предметов:\n\n");
    obj_count = int_input_cheking_pos();
    file = file_creating(file_name);
    if (file == NULL) {
        free(file_name);
        return;
    }
    for (i = 0; i < obj_count; i++) {
        printf("\nВведите массу %d-го груза\n\n", i + 1);
        inp = int_input_cheking_pos();
        fwrite(&inp, sizeof(int), 1, file);
        printf("\nГруз №%d: %dкг\n", i + 1, inp);
    }
    // Поиск максимального количества предметов
    freopen(file_name, "r+b", file);
    for (i = 0; i < obj_count; i++) {
        count = 0;
        test_weight = 0;
        for (j = i; j < obj_count; j++) {
            fseek(file, j * sizeof(int), SEEK_SET);
            fread(&out, sizeof(int), 1, file);
            if (test_weight + out <= max_weight) {
                test_weight += out;
                count++;
            } else {
                break;
            }
        }
        if (count > max_count) {
            max_count = count;
        }
    }
    printf("\n\nГрузоподъёмность автомобиля: %dкг\nКоличество перевозимых предметов: %d\
    \nМаксимальное количество предметов, перевозимых одновременно: %d\n", max_weight, obj_count, max_count);
    fclose(file);
    int x, min_val = INT_MAX, max_val = INT_MIN;
    float a, b, k, c;
    file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        free(file_name);
        return;
    }
    while (fread(&x, sizeof(int), 1, file) == 1) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }
    fclose(file);
    printf("\nЛинейное преобразование чисел в файле:\n");
    printf("Введите значение a: ");
    a = (float)int_input_cheking();
    printf("Введите значение b: ");
    b = (float)int_input_cheking();
    k = (b - a) / (max_val - min_val);
    c = a - k * min_val;
    file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        free(file_name);
        return;
    }
    // Читаем все числа из файла
    int* numbers = (int*)malloc(obj_count * sizeof(int));
    for (i = 0; i < obj_count; i++) {
        fread(&numbers[i], sizeof(int), 1, file);
    }
    fclose(file);
    // Применяем преобразование
    for (i = 0; i < obj_count; i++) {
        numbers[i] = (int)(k * numbers[i] + c + 0.5); // Округляем до ближайшего целого
    }
    file = fopen(file_name, "wb");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        free(file_name);
        free(numbers);
        return;
    }
    // Записываем преобразованные числа обратно в файл
    for (i = 0; i < obj_count; i++) {
        fwrite(&numbers[i], sizeof(int), 1, file);
    }
    fclose(file);
    free(numbers);
    printf("\nПреобразованные числа:\n\n");
    read_and_print_file(file_name);
    printf("\nОтсортированные числа:\n\n");
    file = fopen(file_name, "r+b");
    quickSort(file, 0, obj_count-1);
    fclose(file);
    read_and_print_file(file_name);
    file_deleting(file_name);
    free(file_name);
}

int main() {
    //task1();
    task2();
    return 0;
}