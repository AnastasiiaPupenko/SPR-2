#include <iostream>     
#include <cstdlib>     
#include <ctime>        
#include <algorithm>    
#include <functional>   

struct CTWLNode {
    int value;              
    CTWLNode *next;      
};

typedef CTWLNode CTWL;

CTWL* ctwl_create_random_unimodal(unsigned int size) {
    if (size < 3) return nullptr; 

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    unsigned int peak_index = std::rand() % (size - 2) + 1;

    int* values = (int*)malloc(size * sizeof(int));
    if (!values) return nullptr;

    for (unsigned int i = 0; i < size; ++i)
        values[i] = i + 1;

    for (unsigned int i = size - 1; i > 0; --i) {
        unsigned int j = std::rand() % (i + 1);
        std::swap(values[i], values[j]);
    }

    int* unimodal = (int*)malloc(size * sizeof(int));
    if (!unimodal) {
        free(values);
        return nullptr;
    }

    std::sort(values, values + peak_index + 1);

    std::sort(values + peak_index + 1, values + size, std::greater<int>());

    for (unsigned int i = 0; i < size; ++i)
        unimodal[i] = values[i];

    CTWL* head = nullptr;
    CTWL* prev = nullptr;

    for (unsigned int i = 0; i < size; ++i) {
        CTWL* node = (CTWL*)malloc(sizeof(CTWL));
        if (!node) {
            CTWL* tmp = head;
            while (tmp) {
                CTWL* next = tmp->next;
                free(tmp);
                if (next == head) break;
                tmp = next;
            }
            free(values);
            free(unimodal);
            return nullptr;
        }

        node->value = unimodal[i];
        node->next = nullptr;

        if (!head)
            head = node;
        else
            prev->next = node;

        prev = node;
    }

    if (prev)
        prev->next = head;

    free(values);
    free(unimodal);
    return head;
}

// Pomocná funkcia na výpis cyklického zoznamu (na kontrolu)
void print_ctwl(CTWL* head, unsigned int count) {
    CTWL* current = head;
    for (unsigned int i = 0; i < count; ++i) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// Testovanie
int main() {
    unsigned int size = 10;
    CTWL* list = ctwl_create_random_unimodal(size);

    if (!list) {
        std::cerr << "Chyba pri vytvarani zoznamu." << std::endl;
        return 1;
    }

    std::cout << "Unimodalny cyklicky zoznam (" << size << " prvkov): ";
    print_ctwl(list, size);

    // Free pamäť (manuálne, len pre ukážku, nie cyklicky tu)
    CTWL* tmp = list;
    for (unsigned int i = 0; i < size; ++i) {
        CTWL* next = tmp->next;
        free(tmp);
        tmp = next;
    }

    return 0;
}

