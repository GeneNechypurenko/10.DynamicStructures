#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
========================================================================================================================================
                                                           ОДНОЗВ'ЯЗНИЙ СПИСОК
========================================================================================================================================

СПИСОК — динамічна структура, в якій кожен вузол зберігає дані і покажчик на наступний вузол. На відміну від стеку і черги,
у списку можна додавати, шукати, змінювати та видаляти елементи в будь-якому місці.

ОДНОЗВ'ЯЗНИЙ СПИСОК — кожен вузол знає лише наступний вузол (next). Рухатися по такому списку можна тільки вперед: від head до nullptr.

Основні операції:
  addToHead(value) — додати на початок;
  addToTail(value) — додати в кінець;
  show()           — обхід і виведення всіх елементів;
  find(value)      — пошук (повертає покажчик на вузол або nullptr);
  replace(old, new)— заміна значення;
  remove(value)    — видалення елемента.

                                                             МАСИВ vs СПИСОК
                                -----------------------------------------------------------------------
                                                          | Масив           | Список
                                -----------------------------------------------------------------------
                                Розташування в пам'яті    | суцільний блок  | вузли окремо
                                Доступ за індексом        | миттєво, O(1)   | прохід, O(n)
                                Вставка/видалення на поч. | зсув усіх, O(n) | O(1)
                                Зміна розміру             | новий масив     | просто new / delete
                                Додаткова пам'ять         | немає           | покажчик у кожному вузлі
                                -----------------------------------------------------------------------

У STL:
  std::forward_list — однозв'язний список (бібліотека <forward_list>);
  std::list         — двозв'язний список (бібліотека <list>).
*/

template <typename T>
class Node
{
public:
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class List
{
private:
    Node<T>* head;

public:
    List() : head(nullptr) {}

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    ~List()
    {
        while (head != nullptr)
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Додавання на початок — так само, як push у стеку
    void addToHead(const T& value)
    {
        Node<T>* node = new Node<T>(value);
        node->next = head;
        head = node;
    }

    // Додавання в кінець: доходимо до останнього вузла
    void addToTail(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (head == nullptr)
        {
            head = node;
            return;
        }

        Node<T>* current = head;
        while (current->next != nullptr) // зупиняємося на останньому вузлі
        {
            current = current->next;
        }
        current->next = node;
    }

    void show() const
    {
        for (Node<T>* current = head; current != nullptr; current = current->next)
        {
            cout << current->data << " -> ";
        }
        cout << "nullptr" << endl;
    }

    // Пошук: повертає покажчик на перший знайдений вузол або nullptr
    Node<T>* find(const T& value) const
    {
        for (Node<T>* current = head; current != nullptr; current = current->next)
        {
            if (current->data == value)
            {
                return current;
            }
        }
        return nullptr;
    }

    // Заміна: знаходимо вузол і змінюємо в ньому дані
    bool replace(const T& oldValue, const T& newValue)
    {
        Node<T>* node = find(oldValue);
        if (node == nullptr)
        {
            return false;
        }
        node->data = newValue;
        return true;
    }

    // Видалення першого вузла з таким значенням
    bool remove(const T& value)
    {
        if (head == nullptr)
        {
            return false;
        }

        // Випадок 1: видаляємо перший вузол
        if (head->data == value)
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        // Випадок 2: шукаємо вузол, що стоїть ПЕРЕД тим, який видаляємо
        Node<T>* current = head;
        while (current->next != nullptr && current->next->data != value)
        {
            current = current->next;
        }

        if (current->next == nullptr) // дійшли до кінця — не знайшли
        {
            return false;
        }

        Node<T>* temp = current->next; // вузол, який видаляємо
        current->next = temp->next;    // «перекидаємо» зв'язок через нього
        delete temp;
        return true;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Інвентар гравця
    List<string> inventory;

    inventory.addToTail("Меч");
    inventory.addToTail("Щит");
    inventory.addToTail("Лук");
    inventory.addToHead("Зілля здоров'я"); // важливий предмет — на початок

    cout << "Інвентар:" << endl;
    inventory.show();

    // Пошук
    if (inventory.find("Щит") != nullptr)
    {
        cout << endl << "Щит є в інвентарі" << endl;
    }

    // Заміна: прокачали меч
    inventory.replace("Меч", "Вогняний меч");

    // Видалення з середини
    inventory.remove("Щит");

    cout << endl << "Після прокачування меча і втрати щита:" << endl;
    inventory.show();

    // Видалення відсутнього предмета повертає false
    if (!inventory.remove("Сокира"))
    {
        cout << endl << "Сокири в інвентарі немає" << endl;
    }

    return 0;
}

/*
========================================================================================================================================
                                                        ІНСАЙТИ / ЛАЙФХАКИ / BEST PRACTICES
========================================================================================================================================

1. Головне правило видалення з однозв'язного списку: зупинятися на вузлі ПЕРЕД тим, що видаляється. Вузол сам не знає свого попередника,
   тому після delete зв'язок уже не відновити.

2. Перший вузол завжди окремий випадок: у нього немає попередника, тому змінюється сам head.

3. Покажчик, отриманий з find(), стає недійсним після remove() цього вузла. Не зберігайте його надовго.

4. Двозв'язний список — кожен вузол зберігає два покажчики:
   prev (попередній) і next (наступний):
       nullptr <- [A] <-> [B] <-> [C] -> nullptr
   Плюси: рух в обидва боки, видалення без пошуку попередника.
   Мінуси: більше пам'яті на вузол, більше покажчиків треба перев'язувати.

5. addToTail() тут проходить весь список. Якщо зберігати покажчик tail (як у черзі), додавання в кінець стане миттєвим.

6. Бонус для допитливих: у реальних проєктах std::vector (тема STL) часто швидший за список навіть при вставках, бо елементи лежать
   у пам'яті підряд, а процесор читає таку пам'ять значно швидше.

========================================================================================================================================
                                                                ПРАКТИКА
========================================================================================================================================

Завдання 1.
Допишіть у клас List метод int count(const T& value) const:
  - проходить увесь список (як у методі show);
  - повертає, скільки разів значення value зустрічається в списку.
У main:
  - додайте в інвентар ще два «Зілля здоров'я»;
  - виведіть, скільки зілль у гравця.

Завдання 2 (бонус).
Допишіть у клас List метод bool insertAfter(const T& afterValue, const T& value):
  - знаходить вузол з afterValue (можна через find);
  - вставляє новий вузол з value одразу після нього;
  - повертає false, якщо afterValue не знайдено.
У main:
  - вставте «Стріли» після «Лук» і виведіть інвентар.
*/
