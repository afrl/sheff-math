#pragma once

#include "utility/Exceptions.h"
#include "utility/Range.h"

#include <utility>
#include <memory>
#include <vector>

namespace simple_math
{
    /// \brief A block matrix data structure.
    ///
    /// At construction, the block matrix consists of null blocks until a block is set.
    /// The null blocks can be used to indicate zero blocks efficiently.
    template<typename MatrixType>
    class BlockMatrix
    {
    public:
        BlockMatrix(const unsigned int n_rows);
        BlockMatrix(BlockMatrix&& other) noexcept;

        /// Set the (i, j) block by copy assignment
        void set_block(const unsigned int i, const unsigned int j,
                       const MatrixType & m);
        /// Set the (i, j) block by moving m_ptr to this structure
        void set_block(const unsigned int i, const unsigned int j,
                       std::unique_ptr<MatrixType> m_ptr);
        /// Set the (i, j) block of this block matrix by moving the (k, l) block of block_m
        /// \param i - the row of this structure to set
        /// \param j - the column of this structure to set
        /// \param block_m - a r-value block matrix to take a block from
        /// \param k - the row of block_v to take the block from
        /// \param l - the column of block_v to take the block from
        void set_block(const unsigned int i, const unsigned int j,
                       BlockMatrix&& block_m,
                       const unsigned int k, const unsigned int l);

        /// Add the given matrix the indicated block.  Sets the block if it is null
        void add_to_block(const unsigned int i, const unsigned int j,
                          const MatrixType& m);
        /// Adds (k, l) block of block_m to the (i, j) block of this block matrix
        /// \param i - the row of this structure to set
        /// \param j - the column of this structure to set
        /// \param block_m - a r-value block matrix to take a block from
        /// \param k - the row of block_m to take the block from
        /// \param l - the column of block_m to take the block from
        void add_to_block(const unsigned int i, const unsigned int j,
                          const BlockMatrix& block_m,
                          const unsigned int k, const unsigned int l);

        /// If (i, j) is a nullptr, this will initialize it to an empty matrix
        void init_empty_block(const unsigned int i, const unsigned int j);
        /// Returns a bool indicating if the (i, j) block is a nullptr.
        /// This is be used before a call to BlockMatrix::get_block.
        bool is_block_null(const unsigned int i, const unsigned int j) const;
        /// Access the (i, j) block by reference
        /// \attention This will throw if the ith block is null
        MatrixType& get_block(const unsigned int i, const unsigned int j);
        // Access the (i, j) block by reference
        /// \attention This will throw if the ith block is null
        const MatrixType& get_block(const unsigned int i, const unsigned int j) const;
        /// Get the number of blocks
        unsigned int get_n_blocks() const;
        /// Get the number of rows
        unsigned int get_n_rows() const;
        /// Get a string representation of the matrix for debugging purposes
        std::string str() const;

    private:
        std::vector<std::unique_ptr<MatrixType>> blocks_;
        // This could be calculated as static_cast<unsigned int>(sqrt(blocks_.size()))
        // but it is easier to save it.
        const unsigned int n_rows_;

        /// Get the single index from the (i, j) index pair
        unsigned int get_unraveled_index(const unsigned int i, const unsigned int j) const;
    };


    template<typename MatrixType>
    inline BlockMatrix<MatrixType>::BlockMatrix(const unsigned int n_rows)
        : n_rows_(n_rows)
    {
        blocks_.resize(static_cast<std::size_t>(n_rows) * n_rows);
    }

    template<typename MatrixType>
    inline simple_math::BlockMatrix<MatrixType>::BlockMatrix(BlockMatrix && other) noexcept
        : n_rows_(other.n_rows_)
    {
        blocks_.swap(other.blocks_);
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::set_block(const unsigned int i, const unsigned int j, const MatrixType & m)
    {
        const auto block_i = get_unraveled_index(i, j);
        if (is_block_null(i, j))
        {
            blocks_[block_i] = std::make_unique<MatrixType>();
        }

        *blocks_[block_i] = m;
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::set_block(unsigned int i, unsigned int j, std::unique_ptr<MatrixType> m_ptr)
    {
        const auto block_i = get_unraveled_index(i, j);
        blocks_[block_i] = std::move(m_ptr);
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::set_block( const unsigned int   i,
                                                    const unsigned int   j,
                                                    BlockMatrix && block_m,
                                                    const unsigned int   k,
                                                    const unsigned int   l )
    {
        const auto block_i = get_unraveled_index(i, j);
        const auto other_index = block_m.get_unraveled_index(k, l);
        blocks_[block_i] = std::move(block_m.blocks_[other_index]);
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::add_to_block(const unsigned int i, const unsigned int j, const MatrixType& m)
    {
        const auto block_i = get_unraveled_index(i, j);
        if (is_block_null(i, j))
        {
            blocks_[block_i] = std::make_unique<MatrixType>(m);
        }
        else
        {
            *blocks_[block_i] += m;
        }
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::add_to_block(const unsigned int i, const unsigned int j, const BlockMatrix& block_m, const unsigned int k, const unsigned int l)
    {
        if (!block_m.is_block_null(k, l))
        {
            add_to_block(i, j, block_m.get_block(k, l));
        }
    }

    template<typename MatrixType>
    inline void BlockMatrix<MatrixType>::init_empty_block(const unsigned int i, const unsigned int j)
    {
        const auto block_i = get_unraveled_index(i, j);
        if (is_block_null(i, j))
        {
            blocks_[block_i] = std::make_unique<MatrixType>();
        }
    }

    template<typename MatrixType>
    inline bool BlockMatrix<MatrixType>::is_block_null(const unsigned int i, const unsigned int j) const
    {
        const auto block_i = get_unraveled_index(i, j);
        return blocks_[block_i] == nullptr;
    }

    template<typename MatrixType>
    inline MatrixType & BlockMatrix<MatrixType>::get_block(const unsigned int i, const unsigned int j)
    {
        const auto block_i = get_unraveled_index(i, j);
        _UTIL_ASSERT_C(blocks_[block_i] != nullptr, "Attempted to access a null block.");
        return *blocks_[block_i];
    }

    template<typename MatrixType>
    inline const MatrixType& BlockMatrix<MatrixType>::get_block(const unsigned int i, const unsigned int j) const
    {
        const auto block_i = get_unraveled_index(i, j);
        _UTIL_ASSERT_C(blocks_[block_i] != nullptr, "Attempted to access a null block.");
        return *blocks_[block_i];
    }

    template<typename MatrixType>
    inline unsigned int BlockMatrix<MatrixType>::get_n_blocks() const
    {
        return static_cast<unsigned int>(blocks_.size());
    }

    template<typename MatrixType>
    inline unsigned int BlockMatrix<MatrixType>::get_n_rows() const
    {
        return n_rows_;
    }

    template<typename MatrixType>
    inline std::string BlockMatrix<MatrixType>::str() const
    {
        typename MatrixType::Index n_rows_per_block = 0;
        typename MatrixType::Index n_cols_per_block = 0;
        for (auto& block : blocks_)
            {
                if (block != nullptr)
                    {
                        if (n_rows_per_block == 0)
                            {
                                n_rows_per_block = block->rows();
                                n_cols_per_block = block->cols();
                            }
                        else if (block->rows() != n_rows_per_block
                                 || block->cols() != n_cols_per_block)
                            {
                                throw _UTIL_MSG_EXCEPT_C("Some of the blocks did not have the same size.  To print a block matrix as a string, this property is required.");
                            }
                    }
            }

        std::stringstream ss;
        for (auto i : utility::range_from_zero(n_rows_))
            {
                for (auto row_in_block : utility::range_from_zero(n_rows_per_block))
                    {
                        ss << " ";
                        for (auto j : utility::range_from_zero(n_rows_))
                            {
                                const auto block_i = get_unraveled_index(i, j);
                                auto& block = blocks_[block_i];
                                if (block != nullptr)
                                    {
                                        ss << block->row(row_in_block);
                                    }
                                else
                                    {
                                        for (auto col_in_block : utility::range_from_zero(n_cols_per_block))
                                            {
                                                ss << "            0";
                                            }
                                    }
                                ss << " ";
                            }
                        ss << "\n";
                    }
            }
        return ss.str();
    }

    template<typename MatrixType>
    inline unsigned int BlockMatrix<MatrixType>::get_unraveled_index(const unsigned int i, const unsigned int j) const
    {
        return i * n_rows_ + j;
    }

};
