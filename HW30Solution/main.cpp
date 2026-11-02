#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

/*
==========================================================================================================================================
                            ДОМАШНЄ ЗАВДАННЯ 30 тема: динамічні структури даних — стек, черга, однозв'язний список.
==========================================================================================================================================
*/
// ==================== РІВЕНЬ 1 ====================

template <typename T>
class Node
{
public:
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class Deck
{
private:
    Node<T>* top;
    int count;

public:
    Deck() : top(nullptr), count(0) {}

    //  3. Що станеться, якщо в Deck прибрати = delete і написати Deck<int> copy = deck; ? Що буде в кінці програми?
    Deck(const Deck&) = delete;
    Deck& operator=(const Deck&) = delete;

    ~Deck()
    {
        while (top != nullptr)
        {
            Node<T>* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const
    {
        return top == nullptr;
    }

    int size() const
    {
        return count;
    }

    void putOnTop(const T& card)
    {
        Node<T>* node = new Node<T>(card);
        node->next = top;
        top = node;
        count++;
    }

    //    2. Чому draw приймає T& card і повертає bool, а не просто повертає T? Що мав би повернути такий метод, якщо колода порожня?
    bool draw(T& card)
    {
        if (isEmpty())
        {
            return false;
        }

        card = top->data;     // спочатку забираємо дані
        Node<T>* temp = top;
        top = top->next;
        delete temp;
        count--;
        return true;
    }

    void show() const
    {
        for (Node<T>* current = top; current != nullptr; current = current->next)
        {
            cout << current->data << " ";
        }
        cout << endl;
    }
};

// ==================== РІВЕНЬ 2 ====================

class Player
{
private:
    string name;
    int hp;

public:
    Player(const string& name, int hp) : name(name), hp(hp) {}

    string getName() const
    {
        return name;
    }

    int getHp() const
    {
        return hp;
    }

    void takeDamage(int damage)
    {
        hp -= damage;
        if (hp < 0)
        {
            hp = 0;
        }
    }

    bool isAlive() const
    {
        return hp > 0;
    }

    friend ostream& operator<<(ostream& out, const Player& player)
    {
        out << player.name << " (HP: " << player.hp << ")";
        return out;
    }
};

template <typename T>
class TurnQueue
{
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    TurnQueue() : head(nullptr), tail(nullptr), count(0) {}

    TurnQueue(const TurnQueue&) = delete;
    TurnQueue& operator=(const TurnQueue&) = delete;

    ~TurnQueue()
    {
        while (removeCurrent())
        {
        }
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    //  4. size() повертає поле count, а не рахує вузли.  Що це дає? У яких методах count обов'язково треба змінювати?
    int size() const
    {
        return count;
    }

    void enqueue(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (isEmpty())
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
        count++;
    }

    T& current()
    {
        return head->data;
    }

    //  5. (Рівень 2) Опишіть, які покажчики змінює nextTurn() і чому для цього не потрібні new і delete.
    void nextTurn()
    {
        if (count < 2)
        {
            return;
        }

        Node<T>* first = head;
        head = head->next;     // хід переходить до наступного
        first->next = nullptr; // колишній перший стане останнім
        tail->next = first;    // чіпляємо його за старим кінцем
        tail = first;          // і він стає новим кінцем
    }

    //  6. (Рівень 2) Що має статися з tail, коли removeCurrent() видаляє останнього гравця в черзі? Що зламається, якщо про це забути?
    bool removeCurrent()
    {
        if (isEmpty())
        {
            return false;
        }

        Node<T>* temp = head;
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;
        }
        delete temp;
        count--;
        return true;
    }

    void show() const
    {
        for (Node<T>* node = head; node != nullptr; node = node->next)
        {
            cout << "  " << node->data << endl;
        }
    }
};

// ==================== РІВЕНЬ 3 ====================

template <typename T>
class GameLog
{
private:
    Node<T>* head;
    Node<T>* tail;

public:
    GameLog() : head(nullptr), tail(nullptr) {}

    GameLog(const GameLog&) = delete;
    GameLog& operator=(const GameLog&) = delete;

    ~GameLog()
    {
        while (head != nullptr)
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addToTail(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }

    void show() const
    {
        for (Node<T>* node = head; node != nullptr; node = node->next)
        {
            cout << "  " << node->data << endl;
        }
    }

    //  7. (Рівень 3) Навіщо в reverse() потрібен окремий покажчик на наступний вузол, перш ніж змінювати current->next?
    void reverse()
    {
        Node<T>* previous = nullptr;
        Node<T>* current = head;
        tail = head; // колишній перший стане останнім

        while (current != nullptr)
        {
            Node<T>* nextNode = current->next; // запам'ятовуємо, куди йти далі
            current->next = previous;          // розвертаємо стрілку назад
            previous = current;
            current = nextNode;
        }

        head = previous; // колишній останній став першим
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ----- Рівень 1 -----
    Deck<int> deck;

    //  1. Ви поклали карти в порядку 4, 2, 6, 3, 5 ... Яку карту draw повернув першою і чому?
    int cards[] = { 4, 2, 6, 3, 5, 1, 7, 2, 4, 3 };
    for (int card : cards)
    {
        deck.putOnTop(card);
    }

    cout << "Колода (зверху вниз): ";
    deck.show();
    cout << "Карт у колоді: " << deck.size() << endl;

    int card;
    for (int i = 0; i < 2; i++)
    {
        deck.draw(card);
        cout << "Взято карту: " << card << ", залишилось: " << deck.size() << endl;
    }

    Deck<int> empty;
    if (!empty.draw(card))
    {
        cout << "Порожня колода: тягнути нічого" << endl;
    }

    // ----- Рівень 2 + журнал з рівня 3 -----
    cout << endl << "=== ГРА ===" << endl;

    TurnQueue<Player> players;
    players.enqueue(Player("Олена", 10));
    players.enqueue(Player("Марко", 10));
    players.enqueue(Player("Тарас", 10));

    GameLog<string> gameLog;

    while (players.size() > 1 && !deck.isEmpty())
    {
        Player& player = players.current();
        deck.draw(card);
        player.takeDamage(card);

        cout << player.getName() << " тягне карту " << card << " -> " << player << endl;
        gameLog.addToTail(player.getName() + ": -" + to_string(card) + " HP");

        if (!player.isAlive())
        {
            // Виводимо і записуємо ДО видалення: після removeCurrent() посилання player недійсне
            cout << player.getName() << " вибуває!" << endl;
            gameLog.addToTail(player.getName() + " вибуває з гри");
            players.removeCurrent();
        }
        else
        {
            players.nextTurn();
        }
    }

    cout << endl;
    if (players.size() == 1)
    {
        cout << "Переможець: " << players.current() << endl;
    }
    else
    {
        cout << "Колода скінчилася. У грі залишилися:" << endl;
        players.show();
    }

    gameLog.reverse();
    cout << endl << "Останні події гри:" << endl;
    gameLog.show();

    return 0;
}

/*
  1. Ви поклали карти в порядку 4, 2, 6, 3, 5 ... Яку карту draw повернув першою і чому?
  2. Чому draw приймає T& card і повертає bool, а не просто повертає T? Що мав би повернути такий метод,
     якщо колода порожня?
  3. Що станеться, якщо в Deck прибрати = delete і написати Deck<int> copy = deck; ? Що буде в кінці програми?
  4. size() повертає поле count, а не рахує вузли. Що це дає? У яких методах count обов'язково треба змінювати?
  5. (Рівень 2) Опишіть, які покажчики змінює nextTurn() і чому для цього не потрібні new і delete.
  6. (Рівень 2) Що має статися з tail, коли removeCurrent() видаляє останнього гравця в черзі?
     Що зламається, якщо про це забути?
  7. (Рівень 3) Навіщо в reverse() потрібен окремий покажчик на наступний вузол, перш ніж змінювати current->next?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. Першою повернулася карта 3 — та, що була покладена ОСТАННЬОЮ. Колода — це стек (LIFO): putOnTop кладе
     карту на верх, draw знімає теж з верху. Тому карти виходять у зворотному порядку: 3, 4, 2, 7, 1, ...

  2. Порожня колода — нормальна ситуація в грі, і метод має про неї повідомити. Якби draw повертав T, для порожньої
     колоди не було б що повернути: будь-яке число (0, -1) могло б бути справжньою картою. Тому результат «вдалося /
     не вдалося» повертається як bool, а сама карта записується через посилання T& card.

  3. Стандартний конструктор копіювання скопіював би лише покажчик top і число count: обидві колоди дивилися б
     на ті самі вузли. У кінці програми перший деструктор видалив би вузли, а другий спробував би видалити вже
     звільнену пам'ять — аварійне завершення. З = delete така помилка не скомпілюється.

  4. size() працює миттєво, без проходу по всіх вузлах. Але count треба вручну підтримувати в кожному методі,
     що додає або видаляє вузли: putOnTop (+1) і draw (-1); у черзі — enqueue і removeCurrent.

  5. Перший вузол відчіплюється від початку (head переходить на другий вузол), його next стає nullptr,
     а сам він чіпляється за tail і стає новим tail. Дані гравця залишаються в тому самому вузлі,
     змінюється лише його місце в ланцюжку, тому створювати чи видаляти вузли не потрібно.

  6. Якщо видалено останній вузол, head стає nullptr і tail теж треба зробити nullptr. Інакше tail вказуватиме
     на видалений вузол, і наступний enqueue запише tail->next у звільнену пам'ять.

  7. Щойно ми пишемо current->next = previous, зв'язок з рештою списку зникає. Якщо заздалегідь
     не запам'ятати наступний вузол у nextNode, ми не зможемо перейти до нього, і решта списку загубиться
     (а пам'ять витече).
*/
