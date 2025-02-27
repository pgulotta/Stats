export module central_tendency;

import <span>;
import <map>;
import <vector>;
import <iostream>;
import <limits>;   // For std::numeric_limits<T>::quiet_NaN()
import <ranges>;
import <algorithm>;
import <optional>;
import <numeric>;

namespace ranges = std::ranges;
namespace views =  std::ranges::views;


// The mean(average) of a data set is found by adding all numbers in the data setand then dividing by the number of values in the set.
// The median is the middle value when a data set is ordered from least to greatest.
// The mode is the number that occurs most often in a data set.


export namespace central_tendency
{
  auto square(const auto& x) { return x*x; }

  namespace averages
  {
    template <typename T> std::optional<double> rms(std::span<const T> data) requires std::floating_point<T> || std::integral<T>
    {
        if (data.empty())
            return std::nullopt;

       //  The RMS or root mean square is defined as square root of the arithmetic mean of the squares of the elements.     
     //   T sumSquares = std::accumulate(
            T sumSquares = std::reduce(
            data.begin(), data.end(), T{},
            [](T previous_count, T x) {
                return  previous_count + x * x;
            }
        );

        return std:: optional<double>( std::sqrt(sumSquares / (double)data.size()));
    }

    template <typename T> std::optional<double> geometricMean(std::span<const T> data) requires std::floating_point<T> || std::integral<T>
    {
        if (data.empty())
            return std::nullopt;

        // The geometric mean of n elements is defined as the n-th root of the product of all n elements
     //   T product = std::accumulate(data.begin(), data.end(), T{1}, std::multiplies<>());
        T product = std::reduce(data.begin(), data.end(), T{1}, std::multiplies<>());
        return std::optional<double>(std::pow(product, 1.0 / (double)data.size()));
    }

    template <typename T> std::optional<double> arithmeticMean(std::span<const T> data) requires std::floating_point<T> || std::integral<T>
    {
        if (data.empty())
            return std::nullopt;

        // The arithmetic mean, the most commonly used average, is defined as the sum of all elements divided by the number of elements.
       // T sum{ std::accumulate(data.begin(), data.end(),T{}) };
        T sum{ std::reduce(data.begin(), data.end(),T{}) };
        return std::optional<double> (sum / (double)data.size());
    }

    template <typename T> std::optional<double> median(std::span<const T> data) requires std::floating_point<T> || std::integral<T>
    {
        // The median of an odd number of elements is the value in the middle position of these elements when they are sorted.
        // The median of an even number of elements is typically defined as the mean of the two middle elements in a sorted range.
        std::vector<T> sorted;
        ranges::copy(data, back_inserter(sorted));
        ranges::sort(sorted);

        const size_t mid = data.size() / 2;
        if (data.empty())
            return std::nullopt;
       
        return data.size() % 2 == 1 ?
            std::optional<double>(sorted[mid])
            : std::optional<double>((sorted[mid - 1] + sorted[mid]) / 2.0);
    }

    template <typename T> std::optional<T> mode(std::span<const T> data) requires std::floating_point<T> || std::integral<T>
    {
        if (data.empty())
            return std::nullopt;

        std::vector<T> dataCopy;
        ranges::copy(data, std::back_inserter(dataCopy));
        ranges::sort(dataCopy);
        auto [first, last] {ranges::unique(dataCopy)};
        dataCopy.erase(first, last);
        auto counts =
            dataCopy | views::transform([&data](T x) -> std::pair<int, T> {
            return std::make_pair(
                ranges::count_if(data, [&x](auto i) {return x == i; }), x);
                });

        std::multimap<int, T> counted;
        for (const auto& pair : counts) {
            counted.emplace(pair.first, pair.second);
        }

        if (counted.size() != 1)
        {
            if ((counted.crbegin())->first == (++counted.crbegin())->first)
                return std::nullopt;
        }
        return std::optional<T>(counted.crbegin()->second);
    }

    void test();
    void test_int();
    void test_float();
  }
}