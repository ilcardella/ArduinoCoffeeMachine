#pragma once

template <typename T> class MovingAverage
{
  public:
    MovingAverage() : MovingAverage<T>(1)
    {
    }

    MovingAverage(const unsigned int &window_size)
        : window((window_size < 1) ? 1 : window_size), readings(new T[window]),
          invalid_cells(window)
    {
        reset();
    }

    ~MovingAverage()
    {
        delete readings;
        readings = nullptr;
    }

    void add(const T &value)
    {
        // Remove the previous value from the sum
        sum -= readings[index];
        // Update the new reading value in the history
        readings[index] = value;
        // Update the number of invalid values
        if (invalid_cells > 0)
        {
            --invalid_cells;
        }
        // Update the sum with the new reading
        sum += value;
        // Limit the index to the window range
        index = (++index) % window;
    }

    T get()
    {
        auto valid_history = window - invalid_cells;
        if (valid_history < 1)
        {
            return static_cast<T>(0);
        }

        return static_cast<T>(sum / valid_history);
    }

    void reset()
    {
        index = 0;
        sum = static_cast<T>(0);
        for (unsigned i = 0; i < window; ++i)
        {
            readings[i] = static_cast<T>(0);
        }
        invalid_cells = window;
    }

  private:
    unsigned int window = 1;
    T *readings;
    unsigned int invalid_cells = 1;
    unsigned int index = 0;
    T sum = static_cast<T>(0);
};
