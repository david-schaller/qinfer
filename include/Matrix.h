#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template<typename T>
class Matrix {
public:
  void initMatrix(std::size_t dim, T value);
  inline std::size_t getDim() const { return m_dim; };

  T& at(std::size_t row, std::size_t col);
  // for const best match distance matrices
  const T& at(std::size_t row, std::size_t col) const;
private:
  std::size_t m_dim;
  std::vector<std::vector<T>> m_matrix;
};

template<typename T>
void
Matrix<T>::initMatrix(std::size_t dim, T value)
{
  m_dim = dim;
  m_matrix = std::vector<std::vector<T>>(m_dim, std::vector<T>(m_dim, value));
}

template<typename T>
T&
Matrix<T>::at(std::size_t row, std::size_t col)
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_matrix[row][col];
}

template<typename T>
const T&
Matrix<T>::at(std::size_t row, std::size_t col) const
{
    if(col >= m_dim || row >= m_dim){
       throw std::out_of_range("Index out of bounds! Please wait for help.");
    }

    return m_matrix[row][col];
}

#endif /* MATRIX_H */
