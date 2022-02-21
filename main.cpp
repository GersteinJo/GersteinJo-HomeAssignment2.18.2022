/*
 * генератор случайных чисел -- объект для создания последовательности псевдослучайных чисел.
 *      При вызове генератора ему передается зерно -- seed-- (обычно время), при помощи которого он
 *      генерирует значения затем генератор можно передать в объект destribution -- распредение,
 *      чтобы полученная последовательность удовлетворяла к/л распределению
 * seed -- источник псевдослучайной последовательности
 * generator -- объект непосредственно генерирующий случайную последовательность
 * distribution -- распределение (плотность вероятностей) которому должна подчиняться случайноая
 *      последовательность
 */

/*
 * итератор (iterator) -- объект, посзволяющий получить доступ к элементам контейнера
 *      используются для итерации по контейнерам, или получения произвольного доступа к его объектам
 * диапазон (range) -- набор из двух итераторов, указывающих на элементы одного и того же контейнера,
 *      первый из которых указывает левый, второй -- на правый элемент
 *
 * контейнер большой объект, удобнее передавать ссылки на его объекты, нежели копировать его
 *
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iterator>

#define RANGE(Container) std::begin(Container), std::end(Container)

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> outVector)
{
    std::copy(std::begin(outVector), std::prev(std::end(outVector)), std::ostream_iterator<T>(out, ", "));
    out<<*std::prev(std::end(outVector));
    return out;
}

int main() {


    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    auto generator = std::mt19937(seed);
    auto distribution = std::uniform_int_distribution(1,10);

    std::vector<int> mySequence;
    std::generate_n(std::back_inserter(mySequence), 10,
                    [&generator, &distribution]()
                    {
                        return distribution(generator);
                    });
    std::cout<<"1. Generated sequence: "<<mySequence<<"\n"<<"2. Enter ten numbers:"<<"\n";

    for(auto i = 0; i<10;i++)
    {
        int c;
        std::cin>>c;
        mySequence.push_back(c);
    }
    std::shuffle(RANGE(mySequence),generator);
    std::cout<<"3. Shuffled sequence: "<<mySequence<<"\n";

    std::sort(RANGE(mySequence),std::less<int>());
    auto cutIt = std::unique(RANGE(mySequence));
    mySequence.erase(cutIt, std::end(mySequence));
    std::cout<<"4. Unique sequence: "<<mySequence<<"\n";

    std::cout<<"5. Number of uneven elems in mySequence: "<<
    std::count_if(RANGE(mySequence), [](int elem){return (elem % 2==1);})<<"\n";

    std::cout<<"6. Min & Max in mySequence: "<<*std::min_element(RANGE(mySequence))<<" & "
        <<*std::max_element(RANGE(mySequence))<<"\n";

    auto simple_less_than_10 = [](const int& elem)
            {
                return (elem > 1)&&(elem < 10)&&(!((elem%2==0)&&(elem%3==0)&&(elem%5==0)&&(elem%7==0)));
            };
    std::cout<< "7. Simple number less than 10: "<< *std::find_if(RANGE(mySequence),simple_less_than_10)<<"\n";

    std::vector<int>mySquareSequence;
    std::transform(RANGE(mySequence),std::back_inserter(mySquareSequence), [](int elem){return elem*elem;});
    std::cout<<"8. Squared sequence: "<<mySquareSequence<<"\n";

    std::vector<int> mySequence_2;
    auto newDistribution = std::uniform_int_distribution(1,50);
    std::generate_n(std::back_inserter(mySequence_2), mySequence.size(),
                    [&generator, &newDistribution]()
                    {
                        return newDistribution(generator);
                    });
    std::cout<<"9. Newly generated sequence: "<<mySequence_2<<"\n";
    std::cout<<"10. Sum of the newly generated sequence: "<<std::accumulate(RANGE(mySequence_2),0)<<"\n";
    auto dist = std::uniform_int_distribution(1, (int)mySequence.size()/2);
    int n = dist(generator);
    for(auto i = mySequence_2.begin(); i!= std::next(mySequence_2.begin(),n);i++) *i=1;
    std::cout<<"11. Newly generated sequence with ones: "<<mySequence_2<<"\n";
    std::vector<int> mySequence_3;
    std::transform(RANGE(mySequence), std::begin(mySequence_2),std::back_inserter(mySequence_3),
                   [](const int& lhs, const int& rhs) {return lhs-rhs;});
    std::cout<<"12. Newest of all generated sequence: "<<mySequence_3<<"\n";

    std::vector<int> mySequence_3_plus;
    std::transform(RANGE(mySequence_3), std::back_inserter(mySequence_3_plus),
                   [](const int& elem){return elem<0?0:elem;});
    std::cout<<"13. Newest of all positive generated sequence: "<<mySequence_3_plus<<"\n";
    mySequence_3_plus.erase(std::remove_if(RANGE(mySequence_3_plus),
                                           [](const int& elem){return elem == 0;}),mySequence_3_plus.end());
    std::cout<<"14. Newest of all positive generated sequence: zeros are now non-existent: "<<mySequence_3_plus<<"\n";

    std::vector<int>mySequence_3_plus_reversed;
    std::reverse_copy(RANGE(mySequence_3_plus),std::back_inserter(mySequence_3_plus_reversed));
    std::cout<<"15. Newest of all positive generated sequence: zeros are now non-existent: now also reversed: "
        <<mySequence_3_plus_reversed<<"\n";

    std::nth_element(mySequence_3_plus_reversed.begin(), std::prev(mySequence_3_plus_reversed.end(),2),
                     mySequence_3_plus_reversed.end());
    std::nth_element(mySequence_3_plus_reversed.begin(), std::prev(mySequence_3_plus_reversed.end(),3),
                     mySequence_3_plus_reversed.end());
    std::nth_element(mySequence_3_plus_reversed.begin(), std::prev(mySequence_3_plus_reversed.end(),4),
                     mySequence_3_plus_reversed.end());
    std::cout<<"16. Top 3 greatest elements in mySequence_3_plus_reversed: ";
    std::copy(std::prev(mySequence_3_plus_reversed.end(),3),std::prev(mySequence_3_plus_reversed.end()),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout<<*std::prev(mySequence_3_plus_reversed.end())<<"\n";

    std::sort(RANGE(mySequence),std::less<int>());
    std::sort(RANGE(mySequence_2),std::less<int>());
    std::cout<<"17. MySequence sorted: "<<mySequence<<"\n";
    std::cout<<"    mySequence_2 sorted: "<<mySequence_2<<"\n";

    std::vector<int> mySequence_4;
    std::merge(RANGE(mySequence), RANGE(mySequence_2),std::back_inserter(mySequence_4));
    std::cout<<"18. mySequence_4 as a merge of mySequence and mySequence_2 : \n"<<mySequence_4<<"\n";
    auto uno = std::equal_range(RANGE(mySequence_4),1);
    std::cout<<"19. 1 may be inserted from the 1st till the "<<(int)std::distance(uno.first, uno.second)<< "th elem \n";

    std::cout<<"20. mySequence: "<<mySequence<<"\n";
    std::cout<<"    mySequence_2: "<<mySequence_2<<"\n";
    std::cout<<"    mySequence_3_plus: "<<mySequence_3_plus<<"\n";
    std::cout<<"    mySequence_4: "<<mySequence_4<<"\n";
    return 0;
}
