#include <vector>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <numeric>
#include <utility>
#include <string>

#include "simple_vector.h"

class X {
public:
    X()
        : X(5) {
    }
    X(size_t num)
        : x_(num) {
    }
    X(const X& other) = delete;

    X& operator=(const X& other) = delete;

    X(X&& other) {
        x_ = std::exchange(other.x_, 0);
    }

    X& operator=(X&& other) {
        x_ = std::exchange(other.x_, 0);
        return *this;
    }
    size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

inline void SimpleVectorFirstPartPraktikumTest() {
    using namespace std::literals;
    // Инициализация конструктором по умолчанию
    {
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
    }

    // Инициализация вектора указанного размера
    {
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного заданным значением
    {
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{ 1, 2, 3 };
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
    }

    // Доступ к элементам при помощи At
    {
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false);  // Ожидается выбрасывание исключения
        }
        catch (const std::out_of_range&) {
        }
        catch (...) {
            assert(false);  // Не ожидается исключение, отличное от out_of_range
        }
    }

    // Очистка вектора
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
    }

    // Изменение размера
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.Resize(7);
        assert(v.GetSize() == 7);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.GetCapacity();
        v.Resize(2);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        assert(v[3] == 0);
    }

    // Итерирование по SimpleVector
    {
        // Пустой вектор
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // Непустой вектор
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
    }

    std::cout << "first part test finished"s << std::endl << std::endl;
}

inline void SimpleVectorSecondPartPraktikumTest() {
    using namespace std::literals;
    // copy constructor
    {
        SimpleVector<int> v{ 1, 2, 3 };
        SimpleVector<int> v2(v);

        assert(v[0] == v2[0]);
        assert(v[1] == v2[1]);
        assert(v[2] == v2[2]);
        assert(v.GetSize() == v2.GetSize());
    }

    // assignment operator
    {
        const SimpleVector<int> source{ 1, 2, 3 };
        SimpleVector<int> target{ 4, 5, 5, 8 };

        target = source;
        assert(source[0] == target[0]);
        assert(source[1] == target[1]);
        assert(source[2] == target[2]);
        assert(source.GetSize() == target.GetSize());
    }

    // PushBack 1
    {
        SimpleVector<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);
        source.PushBack(4);

        assert(source[0] == 1);
        assert(source[1] == 2);
        assert(source[2] == 3);
        assert(source[3] == 4);
        assert(source.GetSize() == 4);
        assert(source.GetCapacity() == 4);
    }

    // PushBack 2
    {
        SimpleVector<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);
        source.PushBack(4);
        source.PushBack(5);
        assert(source.GetCapacity() == 8);
    }

    // PushBack 3
    {
        const SimpleVector<int> source{ 1, 2, 3 };
        SimpleVector<int> target{ 4, 5, 5, 8 };

        for (const auto x : source) {
            target.PushBack(x);
        }

        assert(target.GetSize() == 7);
        assert(target.GetCapacity() == 8);
    }

    // PopBack
    {
        SimpleVector<int> v{ 1, 2, 3 };
        assert(v.IsEmpty() == false);

        v.PopBack();

        assert(v.GetSize() == 2);

        while (!v.IsEmpty()) {
            v.PopBack();
        }
    }
    //TODO: косяк с инсертом
    // Insert
    {
        SimpleVector<int> v;
        v.Insert(v.begin(), 1);
        v.Insert(v.begin(), 2);
        v.Insert(v.begin(), 3);

        assert(v[0] == 3);
        assert(v[1] == 2);
        assert(v[2] == 1);
    }

    // Erase
    {
        SimpleVector<int> v{ 1, 2, 3 };
        v.Erase(v.begin());
        assert(*v.begin() == 2);
    }

    // Erase
    {
        SimpleVector<int> v{ 1, 2, 3 };
        while (!v.IsEmpty()) {
            v.Erase(v.begin());
        }
    }

    // Erase
    {
        SimpleVector<int> v{ 1, 2, 3 };
        v.Erase(v.end() - 1);
        assert(v.GetSize() == 2);
        assert(v[1] == 2);
    }

    // operator==
    {
        SimpleVector<int> v{ 1, 2, 3 };
        SimpleVector<int> v2{ 1, 2, 3 };
        assert(v == v2);
    }

    // operator==
    {
        SimpleVector<int> v{ 1, 2, 3 };
        SimpleVector<int> v2{ 1, 2, 3 };
        assert(v == v2);
    }

    // Лексикографическое сравнение векторов
    {
        using IntList = SimpleVector<int>;

        assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
        assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
    }

    // Проверка списков на равенство и неравенство
    {
        SimpleVector<int> vector_1;
        vector_1.PushBack(1);
        vector_1.PushBack(2);

        SimpleVector<int> vector_2;
        vector_2.PushBack(1);
        vector_2.PushBack(2);
        vector_2.PushBack(3);

        SimpleVector<int> vector_1_copy;
        vector_1_copy.PushBack(1);
        vector_1_copy.PushBack(2);

        SimpleVector<int> empty_vector;
        SimpleVector<int> another_empty_vector;

        // Список равен самому себе
        assert(vector_1 == vector_1);
        assert(empty_vector == empty_vector);

        // Списки с одинаковым содержимым равны, а с разным - не равны
        assert(vector_1 == vector_1_copy);
        assert(vector_1 != vector_2);
        assert(vector_2 != vector_1);
        assert(empty_vector == another_empty_vector);
    }

    // PushBack
    {
        SimpleVector<int> v(1);
        v.PushBack(42);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[0] == 0);
        assert(v[1] == 42);
    }

    // Если хватает места, PushBack не увеличивает Capacity
    {
        SimpleVector<int> v(2);
        v.Resize(1);
        const size_t old_capacity = v.GetCapacity();
        v.PushBack(123);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
    }

    // PopBack
    {
        SimpleVector<int> v{ 0, 1, 2, 3 };
        const size_t old_capacity = v.GetCapacity();
        const auto old_begin = v.begin();
        v.PopBack();
        assert(v.GetCapacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
    }

    // Конструктор копирования
    {
        SimpleVector<int> numbers{ 1, 2 };
        auto numbers_copy(numbers);
        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.GetSize() == numbers.GetSize());
        for (size_t i = 0; i < numbers.GetSize(); ++i) {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
    }

    // Сравнение
    {
        assert((SimpleVector{ 1, 2, 3 } == SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } != SimpleVector{ 1, 2, 2 }));

        assert((SimpleVector{ 1, 2, 3 } < SimpleVector{ 1, 2, 3, 1 }));
        assert((SimpleVector{ 1, 2, 3 } > SimpleVector{ 1, 2, 2, 1 }));

        assert((SimpleVector{ 1, 2, 3 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 4 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 4 }));
    }

    // Обмен значений векторов
    {
        SimpleVector<int> v1{ 42, 666 };
        SimpleVector<int> v2;
        v2.PushBack(0);
        v2.PushBack(1);
        v2.PushBack(2);
        const int* const begin1 = &v1[0];
        const int* const begin2 = &v2[0];

        const size_t capacity1 = v1.GetCapacity();
        const size_t capacity2 = v2.GetCapacity();

        const size_t size1 = v1.GetSize();
        const size_t size2 = v2.GetSize();

        static_assert(noexcept(v1.swap(v2)));
        v1.swap(v2);
        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.GetSize() == size2);
        assert(v2.GetSize() == size1);
        assert(v1.GetCapacity() == capacity2);
        assert(v2.GetCapacity() == capacity1);
    }

    // Присваивание
    {
        SimpleVector<int> src_vector{ 1, 2, 3, 4 };
        SimpleVector<int> dst_vector{ 1, 2, 3, 4, 5, 6 };
        dst_vector = src_vector;
        assert(dst_vector == src_vector);
    }

    // Вставка элементов
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };
        v.Insert(v.begin() + 2, 42);
        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
    }

    // Удаление элементов
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };
        v.Erase(v.cbegin() + 2);
        assert((v == SimpleVector<int>{1, 2, 4}));
    }

    std::cout << "second part test finished"s << std::endl << std::endl;
}

void TestReserveConstructor() {
    using namespace std;

    cout << "TestReserveConstructor"s << endl;
    SimpleVector<int> v(Reserve(5));
    assert(v.GetCapacity() == 5);
    assert(v.IsEmpty());
    cout << "Done!"s << endl << endl;
}

void TestReserveMethod() {
    using namespace std;

    cout << "TestReserveMethod"s << endl;
    SimpleVector<int> v;
    // зарезервируем 5 мест в векторе
    v.Reserve(5);
    assert(v.GetCapacity() == 5);
    assert(v.IsEmpty());

    // попытаемся уменьшить capacity до 1
    v.Reserve(1);
    // capacity должно остаться прежним
    assert(v.GetCapacity() == 5);
    // поместим 10 элементов в вектор
    for (int i = 0; i < 10; ++i) {
        v.PushBack(i);
    }
    assert(v.GetSize() == 10);
    // увеличим capacity до 100
    v.Reserve(100);
    // проверим, что размер не поменялся
    assert(v.GetSize() == 10);
    assert(v.GetCapacity() == 100);
    // проверим, что элементы на месте
    for (int i = 0; i < 10; ++i) {
        assert(v[i] == i);
    }
    cout << "Done!"s << endl << endl;
}

SimpleVector<int> GenerateVector(size_t size) {
    SimpleVector<int> v(size);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

void TestTemporaryObjConstructor() {
    const size_t size = 1000000;
    std::cout << "Test with temporary object, copy elision" << std::endl;
    SimpleVector<int> moved_vector(GenerateVector(size));
    assert(moved_vector.GetSize() == size);
    std::cout << "Done!" << std::endl << std::endl;
}

void TestTemporaryObjOperator() {
    const size_t size = 1000000;
    std::cout << "Test with temporary object, operator=" << std::endl;
    SimpleVector<int> moved_vector;
    assert(moved_vector.GetSize() == 0);
    moved_vector = GenerateVector(size);
    assert(moved_vector.GetSize() == size);
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNamedMoveConstructor() {
    const size_t size = 1000000;
    std::cout << "Test with named object, move constructor" << std::endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.GetSize() == size);

    SimpleVector<int> moved_vector(std::move(vector_to_move));
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNamedMoveOperator() {
    const size_t size = 1000000;
    std::cout << "Test with named object, operator=" << std::endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.GetSize() == size);

    SimpleVector<int> moved_vector = std::move(vector_to_move);
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNoncopiableMoveConstructor() {
    const size_t size = 5;
    std::cout << "Test noncopiable object, move constructor" << std::endl;
    SimpleVector<X> vector_to_move(Reserve(size));
    for (size_t i = 0; i < size; ++i) {
        vector_to_move.PushBack(X(i));
    }
    
    SimpleVector<X> moved_vector = std::move(vector_to_move);
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
    
    for (size_t i = 0; i < size; ++i) {
        assert(moved_vector[i].GetX() == i);
    }
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNoncopiablePushBack() {
    const size_t size = 5;
    std::cout << "Test noncopiable push back" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    assert(v.GetSize() == size);

    for (size_t i = 0; i < size; ++i) {
        assert(v[i].GetX() == i);
    }
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNoncopiableInsert() {
    const size_t size = 5;
    std::cout << "Test noncopiable insert" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    // в начало
    
    v.Insert(v.begin(), X(size + 1));
    assert(v.GetSize() == size + 1);
    assert(v.begin()->GetX() == size + 1);
    // в конец
    
    v.Insert(v.end(), X(size + 2));
    assert(v.GetSize() == size + 2);
    assert((v.end() - 1)->GetX() == size + 2);
    // в середину
    v.Insert(v.begin() + 3, X(size + 3));
    assert(v.GetSize() == size + 3);
    assert((v.begin() + 3)->GetX() == size + 3);
    
    std::cout << "Done!" << std::endl << std::endl;
}

void TestNoncopiableErase() {
    const size_t size = 3;
    std::cout << "Test noncopiable erase" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    auto it = v.Erase(v.begin());
    assert(it->GetX() == 1);

    it = v.Erase(v.begin());
    assert(it->GetX() == 2);
    std::cout << "Done!" << std::endl << std::endl;
}

int main() {
    TestNoncopiableMoveConstructor();
    SimpleVectorFirstPartPraktikumTest();
    SimpleVectorSecondPartPraktikumTest();
    TestReserveConstructor();
    TestReserveMethod();
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    return 0;
}